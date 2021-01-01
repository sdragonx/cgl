/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 textbox.hpp

 2019-06-04 10:50:25

*/
#ifndef TEXTBOX_HPP_20190604105025
#define TEXTBOX_HPP_20190604105025

#include <cgl/public.h>
#include <cgl/string.hpp>
#include <cgl/system/clipboard.hpp>
#include <vector>
#include <deque>
#include <set>
#include "custom_ui.hpp"

namespace cgl{

/*
 * �ؼ�����
 *
 */
struct keywords_group
{
public:
    uint32_t color;
    std::set<unistring> items;
};

typedef std::vector<keywords_group> keywords_list;

//�ж��Ƿ�Ϊ��
inline bool char_isWord(int ch)
{
    return isalpha(ch) || isdigit(ch) || ch == '_';
}

//����һ������
unistring parsingWord(const unistring& s, size_t& pos)
{
    unistring word;
    for(size_t i = pos; i < s.length(); ++i){
        if(isalpha(s[i]) || isdigit(s[i]) || s[i] == '_'){
            word += s[i];
        }
        else{
            pos = i;
            return word;
        }
    }
    pos = s.length();
    return word;
}

//����һ������
unistring parsingNumeric(const unistring& s, size_t& pos)
{
    unistring word;
    for(size_t i = pos; i < s.length(); ++i){
        if(isdigit(s[i])){
            word += s[i];
        }
        else{
            pos = i;
            return word;
        }
    }
    pos = s.length();
    return word;
}

//const wchar_t* keywords[] = {
//    L"attribute", L"varying", L"void", L"int", L"float", L"vec2", L"vec3", L"vec4", L"uniform", L"struct"
//};

class textbox : public custom_canvas
{
public:
    typedef unistring string_type;
    typedef std::deque<string_type> textbox_lines;

public:
    textbox_lines m_lines;

    vec2i offset;    //scroll offset

    vec2ui selstart;    //select start
    vec2ui selend;    //select end

    int lineLabel;    //line label width

    bool button;
    bool is_double_click;

public:
    keywords_list keywords;
    uint32_t text_color;
    uint32_t numeric_color;
    uint32_t symbol_color;
    uint32_t select_color;
    uint32_t comment_color;

public:
    textbox() :
        m_lines(),
        offset(0, 0),
        lineLabel(40)
    {
        m_lines.push_back(unistring());
        button = false;
        is_double_click = false;

        text_color      = 0x00000000;
        numeric_color   = 0x007F0000;
        symbol_color    = 0x000000FF;
        select_color    = 0x00FF7F00;
        comment_color   = 0x00007F00;
    }

    ~textbox()
    {
        this->dispose();
    }

    void dispose()
    {
        custom_canvas::reset_functions();
        this->clear();
        keywords.clear();
    }

    void clear()
    {
        m_lines.clear();
        offset = vec2i();
        selstart = selend = vec2ui();
        button = false;
    }

    void load(const char* file)
    {
        FILE* f = fopen(file, "rb");
        fseek(f, 0, SEEK_END);
        int n = ftell(f);
        char *buf = new char[n + 1];
        fseek(f, 0, SEEK_SET);
        fread(buf, n, 1, f);
        set_text(unistring(buf, n));
        delete buf;
        fclose(f);
    }

    void set_text(const unistring& text)
    {
        int begin = 0;
        m_lines.clear();
        for(size_t i=0; i<text.length(); ++i){
            if(text[i] == '\r'){
                m_lines.push_back(text.substr(begin, i-begin));
                ++i;
                begin = i + 1;
            }
        }
        calc_label_width();
        selpos(INT_MAX, INT_MAX);
        //this->repaint();
    }

    unistring text()const
    {
        unistring s;
        s.reserve(length());
        for(size_t i=0; i<m_lines.size(); ++i){
            s += m_lines[i];
            if(i < m_lines.size() - 1){
                s += L"\r\n";
            }
        }
        return s;
    }

    size_t length()const
    {
        size_t n = 0;
        for(size_t i=0; i<m_lines.size(); ++i){
            n += m_lines[i].length() + 2;
        }
        return n - 2;
    }

public://select
    unistring seltext()const
    {
        unistring s;

        if(!has_selected()){
            return s;
        }

        vec2ui begin, end;
        select_range(begin, end);

        if(begin.y == end.y){//ѡ��ֻ�е���
            s = m_lines[begin.y].substr(begin.x, end.x - begin.x);
            return s;
        }

        //��������
        s.append(m_lines[begin.y].substr(begin.x, INT_MAX));
        s.append(endline.w_str());

        //����ѡ����
        for(size_t i=begin.y + 1; i < end.y; ++i){
            s.append(m_lines[i]);
            s.append("\r\n");
        }

        //����β��
        s.append(m_lines[end.y].substr(0, end.x));

        return s;
    }

    void set_seltext(const unistring& s)
    {
        if(this->has_selected()){
            erase();
        }
        insert(s);
        //this->repaint();
    }

    void selpos(int x, int y)
    {
        set_position(selstart, x, y);
        selend = selstart;
        this->repaint();
    }

    void selpos(size_t pos)
    {
        size_t p = 0;
        for(size_t i=0; i<m_lines.size(); ++i){
            if(p + m_lines[i].length() >= pos){
                set_position(selstart, pos - p, i);
//                set_y(selstart, i);
//                set_x(selstart, , selstart.y);
                selend = selstart;//�м�λ��
            }
            else{
                p += m_lines[i].length();
            }
        }
        set_position(selstart, INT_MAX, INT_MAX);
        selend = selstart;
        this->repaint();
    }

    int selpos()const
    {
        return 0;
    }

    void select_all()
    {
        set_position(selstart, 0, 0);
        set_position(selend, INT_MAX, INT_MAX);
        this->repaint();
    }

private:
    void insert(const string_type& str)
    {
        if(str.empty()){
            return ;
        }

        size_t begin = 0;
        while(true){
            size_t p = str.find('\r', begin);
            if(p != string_type::npos){
                //��ֵ�ǰ��ĩβ������
                m_lines.insert(m_lines.begin() + selstart.y + 1, m_lines[selstart.y].substr(selstart.x, SIZE_MAX));
                //ɾ����ǰ��β
                m_lines[selstart.y].erase(selstart.x, SIZE_MAX);
                //���������ַ�����
                m_lines[selstart.y].append(str.substr(begin, p - begin));
                //�ƶ�����һ������
                set_position(selstart, 0, selstart.y + 1);
            }
            else{
                m_lines[selstart.y].insert(selstart.x, str.substr(begin, SIZE_MAX));
                //�ƶ���굽�����ַ�����ĩβ
                selstart.x += str.length() - begin;
                selend = selstart;
                break;
            }
            begin = p + 2;
        }
        calc_label_width();
        this->repaint();
    }

public:

    //ɾ��ѡ�������
    void erase()
    {
        if(!has_selected()){
            return ;
        }

        vec2ui begin, end;
        select_range(begin, end);

        if(begin.y == end.y){//����
            m_lines[begin.y].erase(begin.x, end.x - begin.x);
        }
        else{//����
            m_lines[begin.y].erase(begin.x, SIZE_MAX);//ɾ������
            m_lines[end.y].erase(0, end.x);//ɾ��ĩ��
            m_lines[begin.y].append(m_lines[end.y]);//�ϲ����к�ĩ��

            for(size_t i=end.y; begin.y < i; --i){//ɾ���м���
                m_lines.erase(m_lines.begin() + i);
            }
        }

        selstart = begin;
        selend = begin;
        calc_label_width();
        this->repaint();
    }

public://clipboard
    void copy();
    void cut();
    void paste();

public://input
    void mousedown(int x, int y);
    void mousemove(int x, int y);
    void mouseup(int x, int y);
    //ѡ���
    void double_click();
    void keypress(wchar_t key);
    void keydown(int key);

public://scroll
    void scroll(int x, int y)
    {
        offset = vec2i(x, -y);
        this->repaint();
    }

    int scroll_max(int height)
    {
        int max_size = m_lines.size() * char_height();
        max_size -= height;
        if(max_size < 0)max_size = 0;
        return max_size;
    }

    void calc_label_width()
    {
        string_type str((int)m_lines.size());
        lineLabel = text_width(str.c_str(), str.length()) + 20;
    }

public://paint
    void paint(int width, int height);

    vec2i caret_pos()
    {
        int x = lineLabel + char_position(m_lines[selstart.y], selstart.x);
        int y = selstart.y * char_height();
        //return vec2i(x - offset.x, y - offset.y);
        return vec2i(x, y);
    }

private:
    void set_x(vec2ui& p, uint32_t x, uint32_t y)
    {
        p.x = x;
        //if(p.x < 0)p.x = 0;
        if(m_lines[y].length() < p.x) p.x = m_lines[y].length();
    }

    void set_y(vec2ui& p, int y)
    {
        p.y = y;
        //if(p.y < 0)p.y = 0;
        if(p.y >= m_lines.size())p.y = m_lines.size() - 1;
    }

    //move jump
    void set_position(vec2ui& p, int x, int y)
    {
        set_y(p, y);
        set_x(p, x, p.y);
    }

    //�ж��Ƿ���ѡ������
    bool has_selected()const
    {
        return selstart != selend;
    }

    void select_range(vec2ui& begin, vec2ui& end)const
    {
        if(selstart.y < selend.y){
            begin = selstart;
            end = selend;
        }
        else if(selstart.y == selend.y){
            if(selstart.x < selend.x){
                begin = selstart;
                end = selend;
            }
            else{
                begin = selend;
                end = selstart;
            }
        }
        else{
            begin = selend;
            end = selstart;
        }
    }

    //��ת��ǰһ����
    vec2ui jump_previous_word()
    {
        int flag = 0;
        unistring& str = m_lines[selstart.y];
        vec2ui pos = selstart;
        for(int i=selstart.x; i > 0; ){
            if(flag == 0){//��ǰ��괦�Ǵʣ����ҷǴ��ַ�
                if(char_isWord(str[i])){
                    --i;
                }
                else{
                    flag = 1;
                }
            }
            else if(flag == 1){//�����Ǵ��ַ�
                if(!char_isWord(str[i])){
                    --i;
                }
                else{
                    flag = 2;
                }
            }
            else{
                if(char_isWord(str[i])){
                    --i;
                }
                else{
                    pos.x = i + 1;
                    return pos;
                }
            }
        }
        pos.x = 0;
        return pos;
    }

    //��ת����һ����
    vec2ui jump_next_word()
    {
        int flag = 0;
        unistring& str = m_lines[selstart.y];
        vec2ui pos = selstart;
        for(int i=selstart.x; i < int(str.length()); ){
            if(!flag){//���ҷǴ��ַ�
                if(char_isWord(str[i])){
                    ++i;
                }
                else{
                    flag = 1;
                }
            }
            else{
                if(!char_isWord(str[i])){
                    ++i;
                }
                else{//���ҽ���
                    pos.x = i;
                    return pos;
                }
            }
        }
        //����ĩβ
        pos.x = str.length();
        return pos;
    }

    //���ҹؼ���
    int is_keywords(const unistring& s, uint32_t& color);
    void tab_print(int offset, int x, int y, const unistring& text);
    //��ù��λ����Ļ����
    int char_position(const unistring& text, int pos);
    //ͨ����Ļ���꣬��ù��λ��
    int caret_position(const unistring& text, int pos);
};

int textbox::is_keywords(const unistring& s, uint32_t& color)
{
    for(size_t i=0; i<keywords.size(); ++i){
        if(keywords[i].items.find(s) != keywords[i].items.end()){
            color = keywords[i].color;
            return true;
        }
    }

    //��ͨ������ɫ
    color = text_color;
    return false;
}

void textbox::tab_print(int offset, int x, int y, const unistring& text)
{
    int cw = char_width('a');
    unistring word;
    uint32_t color;
    for(size_t i=0; i<text.length(); ++i){
        switch(text[i]){
        case '\t':
            putchar(offset + x, y, '>', 0x00DFDFDF);
            x = math::align_next(x, cw * 4);
            break;
        case '\n':
            //y += char_height();
            //textout(x, y, "->", 0x00DFDFDF);
            break;
        case '\v':
        case '\f':
        case '\r':
            break;
        case ' ':
        //case '\s':
            x += cw;
            break;
        case '!':
        case '\"':
        case '#':
        case '$':
        case '%':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case '-':
        case '.':
            putchar(offset + x, y, text[i], symbol_color);
            x += cw;
            break;
        case '/':
            if(text[i + 1] == '/'){
                //textout(offset + x, y, text.substr(i, text.length() - i  + 0), comment_color);
                textout(offset + x, y, &text[i], text.length() - i, comment_color);
                return ;
            }
            else{
                putchar(offset + x, y, text[i], symbol_color);
                x += cw;
            }
            break;
        /*
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        */
        case ':':
        case ';':
        case '<':
        case '=':
        case '>':
        case '?':
        case '@':
        /*
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        */
        case '[':
        case '\\':
        case ']':
        case '^':
        case '_':
        case '`':
        /*
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        */
        case '{':
        case '|':
        case '}':
        case '~':
            putchar(offset + x, y, text[i], symbol_color);
            x += cw;
            break;
        default:
            if(isalpha(text[i])){//��ĸ��ƥ�䵥��
                word = parsingWord(text, i);
                if(is_keywords(word, color)){
                    textout(offset + x, y, word.c_str(), word.length(), color);
                }
                else{
                    textout(offset + x, y, word.c_str(), word.length(), color);
                }
                x += word.length() * cw;
                --i;//for ++i
            }
            else if(isdigit(text[i])){//ƥ������
                word = parsingNumeric(text, i);
                textout(offset + x, y, word.c_str(), word.length(), numeric_color);
                x += word.length() * cw;
                --i;//for ++i
            }
            else{
                //textout(offset + x, y, text[i].c_str(), text[i].length(), text_color);
                putchar(offset + x, y, text[i], text_color);
                x += cw;
            }
            break;
        }
    }
    //textout(x, y, "->", 0x00DFDFDF);
}


//��ù��λ����Ļ����
int textbox::char_position(const unistring& text, int pos)
{
    if(text.empty()){
        return 0;
    }

    int cw = char_width('a');
    int x = 0;
    if(pos > int(text.length()))pos = text.length();
    for(int i=0; i<pos; ++i){
        if(text[i] == '\t'){
            x = math::align_next(x, cw * 4);
        }
        else{
            x += cw;
        }
    }
    return x;
}

//ͨ����Ļ���꣬��ù��λ��
int textbox::caret_position(const unistring& text, int pos)
{
    if(text.empty()){
        return 0;
    }

    int cw = char_width('a');
    int x = 0;
    for(size_t i=0; i<text.size(); ++i){
        if(text[i] == '\t'){
            x = math::align_next(x, cw * 4);
        }
        else{
            x += cw;
        }
        if(pos < x){
            return i;
        }
    }
    return text.length();
}

void textbox::paint(int width, int height)
{
    int x = 0, y = 0;
    int ch_height = char_height();
    unistring line_name;
    int ln_width;

    vec2ui begin, end;
    select_range(begin, end);

    int x1, x2;

    //cls
    fillrect(0, 0, width, height, 0xFFFFFF);

    //�����
    fillrect(0, 0, lineLabel, 1024, 0xEFEFEF);

    size_t begin_line = -offset.y / ch_height;
    int off_y = offset.y % ch_height;

    for(size_t i=begin_line; i<m_lines.size(); ++i){

        //---------------------------------------------------------------------------
        if(i == selstart.y){//������ǰ��
            fillrect(lineLabel + offset.x, off_y + y, 1024, ch_height, 0xE0FFE0);
        }

        //---------------------------------------------------------------------------
        if(has_selected()){
            if(i == begin.y){//ǰ�����
                x1 = char_position(m_lines[i], begin.x);
                x2 = begin.y == end.y ? char_position(m_lines[i], end.x) : 1024;
                fillrect(lineLabel + offset.x + x1, off_y + y, x2 - x1, ch_height, select_color);
            }
            else if(i == end.y){//�������
                fillrect(lineLabel + offset.x, off_y + y, char_position(m_lines[i], end.x), ch_height, select_color);
            }
            else if(begin.y < i && i < end.y){//����ѡ���ı�
                fillrect(lineLabel + offset.x, off_y + y, 1024, ch_height, select_color);
            }
        }
        //---------------------------------------------------------------------------
        tab_print(lineLabel + offset.x, x, off_y + y, m_lines[i]);

        //�к�
        line_name = unistring(int(i));
        ln_width = text_width(line_name.c_str(), line_name.length()) + 8;
        textout(lineLabel - ln_width, off_y + y, line_name.c_str(), line_name.length(), 0x7F7F7F);

        y += ch_height;

        if(y > height){
            break;
        }
    }

    /*
    if((GetTickCount() & 511) < 256){//���ƹ��
        x = char_position(m_lines[selstart.y], selstart.x);
        y = selstart.y * ch_height;
        fillrect(lineLabel + offset.x + x, offset.y + y, 2, ch_height, 0x000000);
    }
    */

    if((GetTickCount() & 511) < 256){//����ѡ�����
        x = char_position(m_lines[selend.y], selend.x);
        y = selend.y * ch_height;
        //fillrect(lineLabel + offset.x + x, off_y + y, 2, ch_height, 0xFF0000);
    }
}

//---------------------------------------------------------------------------
//
// input
//

void textbox::mousedown(int x, int y)
{
    if(is_double_click){//�����˫��������
        is_double_click = false;
        return ;
    }

    x -= lineLabel + offset.x;
    y -= offset.y;

    if(key_shift_down()){
        set_y(selend, y / char_height());
        set_x(selend, caret_position(m_lines[selstart.y], x), selstart.y);
    }
    else{
        set_y(selstart, y / char_height());
        set_x(selstart, caret_position(m_lines[selstart.y], x), selstart.y);
        selend = selstart;
    }
    //frmMain->Caption = String(caret.x) + ", " + caret.y + " -- " + (int)m_lines[caret.y].length();
    button = true;
    this->repaint();
}

void textbox::mousemove(int x, int y)
{
    x -= lineLabel + offset.x;
    y -= offset.y;

    if(button){
        set_y(selend, y / char_height());
        set_x(selend, caret_position(m_lines[selend.y], x), selend.y);
        this->repaint();
    }
}

void textbox::mouseup(int x, int y)
{
    button = false;
}

//ѡ���
void textbox::double_click()
{
    unistring& str = m_lines[selstart.y];
    if(selstart.x < str.length() && char_isWord(str[selstart.x])){
        int begin, end;
        begin = end = selstart.x;
        for(; begin > 0; ){
            if(char_isWord(str[begin - 1])){
                --begin;
            }
            else{
                break;
            }
        }
        for(; end + 1 < int(str.length()); ){
            if(char_isWord(str[end])){
                ++end;
            }
            else{
                break;
            }
        }
        selend = selstart;
        selstart.x = begin;
        selend.x = end;
        is_double_click = true;//����˫�����
        this->repaint();
    }
}

void textbox::keypress(wchar_t key)
{
    if(isgraph(key) || key == ' ' || key == '\t'){
        if(has_selected())this->erase();
        m_lines[selstart.y].insert(m_lines[selstart.y].begin() + selstart.x, &key, (&key)+1);
        selpos(selstart.x + 1, selstart.y);
        this->repaint();
    }
}

void textbox::keydown(int key)
{
    unistring str;
    switch(key){
    case VK_RETURN:
        if(has_selected()){
            this->erase();
        }

        str = m_lines[selstart.y].substr(selstart.x, m_lines[selstart.y].size() - selstart.x);
        m_lines.insert(m_lines.begin() + selstart.y + 1, str);
        m_lines[selstart.y].erase(selstart.x, SIZE_MAX);
        //setCaret(0, caret.y + 1);
        selpos(0, selstart.y + 1);
        break;
    case VK_BACK:
        if(has_selected()){
            this->erase();
        }
        else{
            if(key_ctrl_down()){//delete word
                //��覴�
                selend = this->jump_previous_word();
                this->erase();
            }
            else{
                if(selstart.x > 0){//�˸�
                    m_lines[selstart.y].erase(m_lines[selstart.y].begin() + selstart.x - 1);
                    selpos(selstart.x - 1, selstart.y);
                }
                else{
                    if(selstart.y > 0){//��ǰ�в���ǰһ��
                        //�����ƶ���굽ǰһ��ĩβ
                        selpos(m_lines[selstart.y - 1].length(), selstart.y - 1);
                        //ԭ�в��뵱ǰ��
                        m_lines[selstart.y].append(m_lines[selstart.y + 1]);
                        m_lines.erase(m_lines.begin() + selstart.y + 1);
                    }
                }
            }
        }
        break;
    case VK_DELETE:
        if(has_selected()){
            this->erase();
        }
        else{
            if(key_ctrl_down()){//delete word
                //��覴�
                selend = this->jump_next_word();
                this->erase();
            }
            else{
                if(selstart.x + 1 < m_lines[selstart.y].length()){//ɾ��
                    m_lines[selstart.y].erase(m_lines[selstart.y].begin() + selstart.x);
                    //selpos(selstart.x - 1, selstart.y);
                }
                else{
                    if(selstart.y + 1 < m_lines.size()){//��һ�в��뵱ǰ��
                        m_lines[selstart.y].append(m_lines[selstart.y + 1]);
                        m_lines.erase(m_lines.begin() + selstart.y + 1);
                        //selpos(m_lines[selstart.y - 1].length(), selstart.y - 1);
                    }
                }
            }
        }
        break;
    case VK_LEFT:
        set_x(selstart, selstart.x - 1, selstart.y);
        if(!key_shift_down()){
            selend = selstart;
        }

        if(key_ctrl_down()){//next word
            selstart = this->jump_previous_word();
            selend = selstart;
        }
        break;
    case VK_RIGHT:
        set_x(selstart, selstart.x + 1, selstart.y);
        if(!key_shift_down()){
            selend = selstart;
        }

        if(key_ctrl_down()){//next word
            selstart = this->jump_next_word();
            selend = selstart;
        }
        break;

    case VK_UP:
        set_position(selstart, selstart.x, selstart.y - 1);
        if(!key_shift_down()){
            selend = selstart;
        }
        break;
    case VK_DOWN:
        set_position(selstart, selstart.x, selstart.y + 1);
        if(!key_shift_down()){
            selend = selstart;
        }
        break;

    case VK_HOME:
        set_position(selstart, 0, selstart.y);
        if(!key_shift_down()){
            selend = selstart;
        }
        break;
    case VK_END:
        set_position(selstart, INT_MAX, selstart.y);
        if(!key_shift_down()){
            selend = selstart;
        }
        break;
    /*
    case 'A':
        if(key_ctrl_down()){
            this->select_all();
        }
        break;
    case 'C':
        if(key_ctrl_down()){
            this->copy();
        }
        break;
    case 'V':
        if(key_ctrl_down()){
            this->paste();
        }
        break;
    case 'X':
        if(key_ctrl_down()){
            this->cut();
        }
        break;
    */

    default:
        break;
    }
    this->repaint();
}


//---------------------------------------------------------------------------
//
// clipboard
//

void textbox::copy()
{
    if(this->has_selected()){
        system::clipboard cb;
        string_type str = this->seltext();
        cb.write(str.c_str(), str.length() * sizeof(wchar_t), CGL_UNICODETEXT);
    }
}

void textbox::cut()
{
    this->copy();
    this->erase();
}

void textbox::paste()
{
    system::clipboard cb;
    const wchar_t* p = static_cast<const wchar_t*>(cb.lock(CGL_UNICODETEXT));
    if(p){
        this->set_seltext(string_type(p, cb.size() / sizeof(wchar_t)));
    }
    cb.unlock();
}

}//end namespace cgl

#endif //TEXTBOX_HPP_20190604105025