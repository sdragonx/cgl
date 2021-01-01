/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sciedit.hpp

 2020-01-09 15:31:39

void __fastcall TfrmMain::WndProc(Messages::TMessage &Message)
{
    TForm::WndProc(Message);
    if(Message.Msg == WM_NOTIFY){
        editor->on_notify(Message.LParam);
    }
}

*/
#ifndef SCIEDIT_HPP_20200109153139
#define SCIEDIT_HPP_20200109153139

#include <cgl/public.h>
#include <cgl/vcl.hpp>

#include <cgl/windows/message/spymsg.hpp>


#define INCLUDE_DEPRECATED_FEATURES

#include <Scintilla/Scintilla.h>
#include <Scintilla/SciLexer.h>

namespace cgl{

HWND sciedit_create(HWND parent, int x, int y, int width, int height)
{
    HWND editor = ::CreateWindow(
        TEXT("Scintilla"),
        NULL,
        WS_CHILD|WS_TABSTOP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,
        x,
        y,
        width,
        height,
        parent,
        0,
        HInstance, NULL);

    ::SetParent(editor, parent);
    return editor;
}

class keywords_query
{
public:
    std::vector<string_t> keywords;
    std::vector<string_t> functions;

    string_t find(const string_t& str)
    {
        string_t ls;
        if(!str.empty()){
            find_in(ls, keywords, str);
            find_in(ls, functions, str);
        }
        return ls;
    }

    string_t ls_keywords()
    {
        return ls_in(keywords);
    }

    string_t ls_functions()
    {
        return ls_in(functions);
    }

private:
    void find_in(string_t& ls, const std::vector<string_t>& items, const string_t& str)
    {
        for(size_t i=0; i<items.size(); ++i){
            if(!strnicmp(items[i].c_str(), str.c_str(), str.length())){
                ls.append(items[i]);
                ls.push_back(' ');
            }
        }
    }

    string_t ls_in(const std::vector<string_t>& items)
    {
        string_t ls;
        for(size_t i=0; i<items.size(); ++i){
            ls.append(items[i]);
            if(i != items.size() - 1)ls.push_back(' ');
        }
        return ls;
    }
};

class TEditPlus : public TWinControl
{
public:
    typedef sptr_t (*SCI_DIRECT)(sptr_t ptr, unsigned int iMessage, uptr_t wParam, sptr_t lParam);

public:
    SCI_DIRECT    sci_send;
    sptr_t        sci_ptr;

    int LastProcessedChar;//处理自动缩进
    int LastPos;
    int LastWordWidth;

    keywords_query m_keywords;


private://自动加载dll
    static HANDLE sci_handle;

public:
    static HANDLE Load_SCILib()
    {
        return LoadLibrary(TEXT("SciLexer.dll"));
    }

protected:
    virtual void __fastcall CreateParams(Controls::TCreateParams &Params)
    {
        TWinControl::CreateParams(Params);
        CreateSubClass(Params, L"Scintilla");
    }

public:
    __fastcall virtual TEditPlus(Classes::TComponent* AOwner) : TWinControl(AOwner)
    {
        this->Parent = (TWinControl*)AOwner;
//        if(this->Handle){
//            ShowMessage("yes");
//        }
        sci_send = (SCI_DIRECT)SendMessage(Handle, SCI_GETDIRECTFUNCTION, 0, 0);
        sci_ptr = (sptr_t)SendMessage(Handle, SCI_GETDIRECTPOINTER, 0, 0);
    }

    __fastcall virtual ~TEditPlus(void)
    {
    }

    virtual void __fastcall WndProc(Messages::TMessage &Message)
    {
        TWinControl::WndProc(Message);
        if(Message.Msg == WM_CREATE){
            sci_send = (SCI_DIRECT)SendMessage(Handle, SCI_GETDIRECTFUNCTION, 0, 0);
            sci_ptr = (sptr_t)SendMessage(Handle, SCI_GETDIRECTPOINTER, 0, 0);
        }

        if(Message.Msg == WM_NCCREATE){
            sci_send = (SCI_DIRECT)SendMessage(Handle, SCI_GETDIRECTFUNCTION, 0, 0);
            sci_ptr = (sptr_t)SendMessage(Handle, SCI_GETDIRECTPOINTER, 0, 0);
        }

        if(Message.Msg == WM_GETDLGCODE){//让窗体接受方向键和TAB键
            Message.Result = DLGC_WANTALLKEYS|DLGC_WANTARROWS|DLGC_WANTTAB;
        }
        if(Message.Msg == WM_NOTIFY){
            this->on_notify(Message.LParam);
        }
    }

    void Init(String config)
    {
        //utf8编码
        send_message(SCI_SETCODEPAGE, SC_CP_UTF8);

        //STYLE_DEFAULT（默认）、STYLE_LINENUMBER（行号）、
        //STYLE_BRACELIGHT（括号匹 配）、STYLE_BRACEBAD（括号失配）、
        //STYLE_CONTROLCHAR（控制字符）、STYLE_INDENTGUIDE（缩进线）、 STYLE_CALLTIP（调用提示）。
        //send_message( SCI_SETSTYLEBITS, 7 );

        //C++语法解析
        send_message(SCI_SETLEXER, SCLEX_CPP);

        this->LoadConfig(config);

        //行高亮
        this->highlight_line(true);

        //括号匹配
//        send_message( SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB( 0x00, 0xFF, 0x00 ) );
//        send_message( SCI_STYLESETBACK, STYLE_BRACEBAD, RGB( 0xFF, 0x00, 0x00 ) );

        // 选择背景
        this->select_color(0x00FF7F00);

        // 设置margin 0
        //send_message( SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER );

        //set_margin(0, SC_MARGIN_SYMBOL, 20, 0x0000FF );

        //显示行号
        set_margin(2, SC_MARGIN_NUMBER, 32, 0x808080);

        //行长度？
        //send_message(SCI_TEXTWIDTH, STYLE_LINENUMBER, (sptr_t)"_99");

        //send_message( SCI_SETMOUSEDWELLTIME, 500 );

        LastProcessedChar = 0;

        //this->ImeMode = imAlpha;
    }

private:
    void LoadConfig(String filename)
    {
        AnsiString inifile = appPath + filename;
        char text[1024] = {0};
        char *p;
        int n;
        DWORD size;
        DWORD color;

        //设置全局风格
        GetPrivateProfileStringA("main", "font", "微软雅黑", text, 1024, inifile.c_str());
        send_message(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)text);
        n = GetPrivateProfileIntA("main", "font.size", 12, inifile.c_str());
        send_message(SCI_STYLESETSIZE, STYLE_DEFAULT, n);

        send_message(SCI_STYLECLEARALL);

        //制表符宽度
        n = GetPrivateProfileIntA("main", "table", 4, inifile.c_str());
        this->tabwidth(n);

        //语法高亮设置
        color = GetPrivateProfileIntA("main", "pragma", 0x00FF00FF, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, color);//预编译开关
        color = GetPrivateProfileIntA("main", "char", 0x001515A3, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_CHARACTER, color);//字符
        color = GetPrivateProfileIntA("main", "string", 0x001515A3, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_STRING, color);//字符串
        color = GetPrivateProfileIntA("main", "operator", 0x007F0000, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_OPERATOR, color);//操作符
        color = GetPrivateProfileIntA("main", "comment", 0x00008000, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_COMMENT, color);//块注释
        send_message(SCI_STYLESETFORE, SCE_C_COMMENTLINE, color);//行注释
        send_message(SCI_STYLESETFORE, SCE_C_COMMENTDOC, color);//文档注释（/**开头）

        //读取关键字
        memset(text, 0, 1024);
        size = GetPrivateProfileSectionA("keywords", text, 1024, inifile.c_str());
        p = text;
        while(p[0]){
            m_keywords.keywords.push_back(p);
            p += strlen(p) + 1;
        }

        memset(text, 0, 1024);
        size = GetPrivateProfileSectionA("functions", text, 1024, inifile.c_str());
        p = text;
        while(p[0]){
            m_keywords.functions.push_back(p);
            p += strlen(p) + 1;
        }

        //设置关键字
        string_t str = m_keywords.ls_keywords();
        send_message(SCI_SETKEYWORDS, 0, (sptr_t)str.c_str());
        color = GetPrivateProfileIntA("main", "keyword.color", 0x00FF0000, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_WORD, color);
        n = GetPrivateProfileIntA("main", "keyword.bold", TRUE, inifile.c_str());
        send_message(SCI_STYLESETBOLD, SCE_C_WORD, n);

        str = m_keywords.ls_functions();
        send_message(SCI_SETKEYWORDS, 1, (sptr_t)str.c_str());
        color = GetPrivateProfileIntA("main", "function.color", 0x00007FFF, inifile.c_str());
        send_message(SCI_STYLESETFORE, SCE_C_WORD2, color);
        n = GetPrivateProfileIntA("main", "function.bold", TRUE, inifile.c_str());
        send_message(SCI_STYLESETBOLD, SCE_C_WORD2, n);
    }

public:
    sptr_t send_message(unsigned int iMessage, uptr_t wParam, sptr_t lParam)
    {
        return sci_send(sci_ptr, iMessage, wParam, lParam);
    }

    sptr_t send_message(unsigned int iMessage)
    {
        return sci_send(sci_ptr, iMessage, 0, 0);
    }

    sptr_t send_message(unsigned int iMessage, uptr_t wParam)
    {
        return sci_send(sci_ptr, iMessage, wParam, 0);
    }

public://setup
    void tabwidth(int n)
    {
        send_message(SCI_SETTABWIDTH, n);
    }

    void highlight_line(bool show)
    {
        send_message(SCI_SETCARETLINEVISIBLE, show ? TRUE : FALSE);
        send_message(SCI_SETCARETLINEBACK, 0xC0FFFF);
    }

    void select_color(uint32_t color)
    {
        send_message( SCI_SETSELBACK, true, color );
    }

    void set_margin(int id, int type, int width, uint32_t color)
    {
        send_message( SCI_SETMARGINTYPEN, id, type );
        //send_message( SCI_SETMARGINMASKN, id, 3 );    //掩码
        send_message( SCI_SETMARGINWIDTHN, id, width );
        send_message( SCI_MARKERSETFORE, id, color );
        send_message( SCI_SETMARGINSENSITIVEN, id, true );//鼠标点击
    }

    void set_focus()
    {
        send_message(SCI_SETFOCUS, TRUE);
    }

    void readonly(bool n)
    {
        send_message(SCI_SETREADONLY, n);
    }

public://edit
    void clear()
    {
        send_message(SCI_CLEARALL, 0, 0);
    }

    void set_text(const char* text)
    {
        send_message(SCI_SETTEXT, 0, (sptr_t)text);
    }

    int get_text(char* text, size_t size)
    {
        return send_message(SCI_GETTEXT, size, (sptr_t)text);
    }

    int length()
    {
        return get_text(0, 0);
    }

    void select_all()
    {
        send_message(SCI_SELECTALL);
    }

    void undo()
    {
        send_message(SCI_UNDO);
    }

    bool can_redo()
    {
        return send_message(SCI_CANREDO);
    }

    void redo()
    {
        send_message(SCI_REDO);
    }

    int selpos()
    {
        return send_message(SCI_GETCURRENTPOS);
        //int line_num = s( SCI_LINEFROMPOSITION, pos_x );        // 当前行
        //int line_length = s( SCI_LINELENGTH, line_num );        // 当前行长度
        //int pos0 = s( SCI_FINDCOLUMN, line_num, 0 );
    }

    int current_line()
    {
        return 0;
    }

    void on_notify(LPARAM param)
    {
        SCNotification* notify = (SCNotification*)param;

        switch(notify->nmhdr.code){
        case SCN_CHARADDED:
            //在CharAdded事件中记录最后输入的字符
            LastProcessedChar = notify->ch;
            this->auto_complete();
            break;
        case SCN_SAVEPOINTLEFT:
            break;

        case SCN_UPDATEUI:
            this->auto_indent();
            break;
        case SCN_MODIFIED:
            break;
        case SCN_PAINTED:
            break;
        case SCN_AUTOCSELECTION:
            break;
        case SCN_FOCUSIN:
            break;
        case SCN_FOCUSOUT:
            break;
        case SCN_AUTOCCOMPLETED:
            this->on_completed();
            //print(notify->text);
            break;
        case SCN_AUTOCSELECTIONCHANGE://自动完成选单改变

            break;
        default:
            //print(notify->nmhdr.code);
            break;
        }
    }

    string_t getWord()
    {
        char word[256];        //保存当前光标下的单词
        Sci_TextRange range;//用于SCI_GETTEXTRANGE命令
        int pos = send_message(SCI_GETCURRENTPOS); //取得当前位置
        int startpos = send_message(SCI_WORDSTARTPOSITION, pos - 1);//当前单词起始位置
        int endpos = send_message(SCI_WORDENDPOSITION, pos - 1);//当前单词终止位置
        range.chrg.cpMin = startpos;//设定单词区间，取出单词
        range.chrg.cpMax = endpos;
        range.lpstrText = word;
        send_message(SCI_GETTEXTRANGE, 0, sptr_t(&range));
        LastPos = pos;
        LastWordWidth = strlen(word);
        return word;
    }

    void auto_complete()
    {
        string_t word = getWord();
        if(LastProcessedChar == '.'){
            /*
            if(strcmp(word,"file.") == 0){//输入file.后提示file对象的几个方法
                const char* str = "close eof good open rdbuf size";
                send_message(SCI_AUTOCSHOW, 0, sptr_t(str));
            }
            */
        }
        else{
            //自动完成
            string_t complete = m_keywords.find(word);
            if(!complete.empty()){
                send_message(SCI_AUTOCSHOW, word.size(), sptr_t(complete.c_str()));
            }
        }
    }

    void on_completed()
    {
        //string_t word = getWord();
//        int pos = send_message(SCI_GETCURRENTPOS); //取得当前位置
//        send_message(SCI_SETSELECTIONSTART, LastPos - LastWordWidth);
//        send_message(SCI_SETSELECTIONEND, LastPos);
//        send_message(SCI_REPLACESEL, 0, (sptr_t) "");
//        send_message(SCI_SETCURRENTPOS, pos);
//        print("完成");
    }

    void auto_indent()
    {
        if(LastProcessedChar == 0){
            return ;
        }

        char space[1024];

        //在UpdateUI事件中处理缩进
        int pos = send_message(SCI_GETCURRENTPOS); //取得当前位置
        int line = send_message(SCI_LINEFROMPOSITION, pos); //取得当前行

        if(LastProcessedChar == 10){// KEY_ENTER
            // 如果输入的是回车，则保持与上一行缩进一致
            // 如果上一行最后有效字符为左括号，就多缩进四个空格
            if(line > 0){
                // 得到上一行缩进设置
                int nIndent = send_message(SCI_GETLINEINDENTATION, line - 1);
                // 查找上一行最后一个有效字符（非空白字符）
                int nPrevLinePos = send_message(SCI_POSITIONFROMLINE, line - 1);
                int c = ' ';
                for(int p = pos - 2; p >= nPrevLinePos && isspace(c); ){
                    --p;
                    c = send_message(SCI_GETCHARAT, p);
                }
                // 如果是左括号，就多缩进四格
                if(c && strchr("{([<",c)) nIndent+=4;
                // 缩进...
                memset(space, ' ', nIndent);
                space[nIndent] = 0;
                send_message(SCI_REPLACESEL, 0, (sptr_t)space);
            }
        }
        //如果最后输入的字符是右括号的话就自动让当前行缩进和它匹配的左括号所在行一致
        //要求右括号左边是空白字符
        else if( strchr("})>]", LastProcessedChar) && isspace(send_message(SCI_GETCHARAT,pos-2)) ){
            //找前一个单词起始位置，这里用它来确定右括号左边是否全是空白字符
            int startpos = send_message(SCI_WORDSTARTPOSITION, pos-1, false);
            int linepos = send_message(SCI_POSITIONFROMLINE,line); //当前行起始位置
            if(startpos == linepos){ //这样相当于判断右括号左边是否全是空白字符
                int othpos = send_message(SCI_BRACEMATCH,pos-1); //得到对应的左括号所在的位置
                int othline = send_message(SCI_LINEFROMPOSITION,othpos);  //左括号所在行
                int nIndent = send_message(SCI_GETLINEINDENTATION,othline);//左括号所在行的缩进值
                // 替换右括号前面的空白字符，使之与左括号缩进一致
                memset(space, ' ', 1024);
                send_message(SCI_SETTARGETSTART, startpos);
                send_message(SCI_SETTARGETEND, pos-1);
                send_message(SCI_REPLACETARGET, nIndent, (sptr_t)space);
            }
        }
        LastProcessedChar = 0;
    }
};

HANDLE TEditPlus::sci_handle = TEditPlus::Load_SCILib();

}//end namespace cgl

#endif //SCIEDIT_HPP_20200109153139
