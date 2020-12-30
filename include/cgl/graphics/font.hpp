/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 graphics_font.hpp

 2018-07-18 23:47:39

*/
#ifndef CORE_FONT_HPP_20180718234739
#define CORE_FONT_HPP_20180718234739

#include <cgl/public.h>
#include <cgl/string.hpp>
//#include <cgl/graphics/context.hpp>
#include <cgl/graphics/image.hpp>

#ifndef CGL_DEFAULT_FONT_SIZE
#define CGL_DEFAULT_FONT_SIZE 16
#endif

namespace cgl{
namespace graphics{

//预设
const int FONT_UNDERLINE = 1;    //下划线
const int FONT_BOLD      = 2;    //粗体
const int FONT_OUTLINE   = 4;    //描边
const int FONT_SHADOW    = 8;    //阴影

//draw_text字符串绘制参数
#define PT_LEFT      0x00000000
#define PT_RIGHT     0x00000001
#define PT_CENTER    0x00000002
#define PT_SHADOW    0x00010000
#define PT_CALCRECT  0x80000000

class unicontext;

#pragma pack(push, 1)
struct char_info
{
    int16_t x;    //offset
    int16_t y;
    int16_t next_x;//next position offset (char width)
    int16_t next_y;
};
#pragma pack(pop)

/*
typedef uint8_t font_sizebuf[65535 - 256];    //0xFEFF

int gdifont_fontsize(font_sizebuf buf, HDC dc)
{
    INT sizes[0xFEFF] = {0};
    GetCharWidth32(dc, 0, 0xFEFF, sizes);
    for(int i=0; i<0xFEFF; ++i){
        buf[i] = sizes[i];
    }
    return 0;
}

在xdoc中保存的对象格式：
font{
    name="fonts/font.ttf";
    size=16;
    bold=true;
    underline=true;
}
*/

class font_t
{
public:
    string_t name;    //font name
    int size;        //font size
    int attribute;
    //vec4ub color;

    font_t() : name(), size(CGL_DEFAULT_FONT_SIZE) { /*void*/ }
    font_t(const font_t& f):name(f.name), size(f.size) { /*void*/ }
    font_t(const cstring<char>& a_name, int a_size = CGL_DEFAULT_FONT_SIZE)
        : name(a_name), size(a_size) { /*void*/ }

    font_t& operator=(const font_t& f)
    {
        name = f.name;
        size = f.size;
        return *this;
    }

    bool operator<(const font_t& f)const
    {
        int n = name.icompare(f.name);
        return n < 0 || (n == 0 && size < f.size);
    }

    bool operator==(const font_t& other)const
    {
        return name == other.name && size == other.size && attribute == other.attribute;
    }
};

/*
 unifont

 virtual int char_width(int ch);
 virtual int char_height();
 virtual int text_width(const unichar_t* text, size_t size);

 virtual int putchar(void* dc, int ch);
 virtual int textout(void* dc, int x, int y, const unichar_t* text, size_t size);

 */

class unifont
{
public:
    vec4ub color;
    vec4ub shadow_color;
    int tab_width;
public:
    unifont();
    virtual ~unifont() { this->dispose(); }

    virtual void dispose() { /*void*/ }

    virtual int char_width(unichar_t ch) { return 0; }
    virtual int char_height() { return 0; }
    int text_width(const unichar_t* text, size_t length);

    virtual int put_char(unicontext* dc, int x, int y, unichar_t ch, int param = 0) { return 0; }

    //绘制一行字体，不支持制表符
    int print(unicontext* dc, int x, int y, const unichar_t* text, size_t length = TEXT_MAX);

    //绘制一行字体，支持制表符
    int tab_print(unicontext* dc, int x, int y, const unichar_t* text, size_t length = TEXT_MAX);

    //在矩形内部输出多行字体，返回值是绘制高度
    int draw_text(unicontext* dc, int left, int top, int width, int height, const unichar_t* text, size_t length, int style);
private:
    struct PT_WORD
    {
        const unichar_t* begin;
        const unichar_t* end;
        size_t width;
    };
    int get_tabled_line(const unichar_t* &begin, const unichar_t* end, int max_width);
    int align_next(int n, int align);
};

unifont::unifont()
{
    tab_width = 4;
    color = vec4ub(255, 255, 255, 255);
    shadow_color = vec4ub(0, 0, 0, 255);
}

//获取字符串宽度
int unifont::text_width(const unichar_t* text, size_t length)
{
    int width = 0;
    for(size_t i=0; i<length; ++i){
        width += char_width(text[i]);
    }
    return width;
}

//根据当前位置n，计算下一个tab的对齐位置 next_tab_position
int unifont::align_next(int n, int align)
{
    /*n += align-1;
    n /= align;
    n *= align;
    */
    n = n - n % align + align;
    return n;
}

//绘制一行字体，不支持制表符
int unifont::print(unicontext* dc, int x, int y, const unichar_t* text, size_t length)
{
    if(length == TEXT_MAX)length = cslen(text);
    for(size_t i=0; i<length; ++i){
        x += put_char(dc, x, y, text[i]);
    }
    return 0;
}

//绘制一行字体，支持制表符
int unifont::tab_print(unicontext* dc, int x, int y, const unichar_t* text, size_t length)
{
    int tab = tab_width * (this->char_height() >> 1);
    if(length == TEXT_MAX)length = cslen(text);
    for(size_t i=0; i<length; ++i)
    {
        if(text[i] == '\t'){
            x = align_next(x, tab);
        }
//        else if(text[i] =='\n'){
//            x += put_char(dc, x, y, '\\');
//            x += put_char(dc, x, y, 'n');
//        }
        else{
            x += put_char(dc, x, y, text[i]);
        }
    }
    return 0;
}

//返回下一个字符的绘制位置-1表示换行
int unifont::get_tabled_line(const unichar_t* &l_end, const unichar_t* end, int width)
{
    //int x = 0;
    //int y = 0;

    int tab = tab_width * (this->char_height() >> 1);
    int n;// = 0;
    int l_width = 0;

    for(; l_end < end; ++l_end){
        if(*l_end == '\r'){
            continue;
        }
        else if(*l_end == '\n'){//next line
            break;
        }
        //else if(*l_end == ' ')//add word
        else if(*l_end == '\t'){
            n = align_next(l_width, tab);
            if(n < width){
                l_width = n;
            }
            else{
                //break;
                return l_width;
            }
            /*
            //chwidth = tab - l_width % tab; //align_next(l_width, tab) - l_width;
            if(tab <= width - l_width)
                l_width = align_next(l_width, tab);
            else{
                l_width = width;
                //l_width += tab;
                //++l_end;
                break;
            }
            */
            //c = vec4ub(0, 255, 255, 255);
        }
        else{
            n = this->char_width(*l_end);
            if(l_width + n < width){
                l_width += n;
            }
            else{//next line
                //break;
                return l_width;
            }
        }
    }
    return l_width;
}

/*
    while(l_end < end)
    line += next_word(..., max_width);
    draw_line

    阴影绘制没有实现

*/
int unifont::draw_text(unicontext* dc, int left, int top, int width, int height, const unichar_t* text, size_t length, int style)
{
    int px;
    int py = top;    //字符绘制位置
    //int chwidth = 0;        //字符宽度
    int ch_size = this->char_height();
    if(length == TEXT_MAX)length = cslen(text);
    int tab = tab_width * (ch_size >> 1);

    //vec4ub c = dc->color;

    const unichar_t* end = text + length;
    const unichar_t* l_begin;
    const unichar_t* l_end = text;
    int l_width;
    //const unichar_t *word_begin;
    std::vector<PT_WORD> words;

    int x;

    while(l_end < end)
    {
        //l_width = 0;
        l_begin = l_end;
        //word_begin = l_end;
        //get line
        l_width = get_tabled_line(l_end, end, width);


        px = left;
        if(style & PT_RIGHT){
            px += width - l_width;
        }
        else if(style & PT_CENTER){
            px += (width - l_width) / 2;
        }
        if(l_begin != l_end && !(style & PT_CALCRECT)){
            if(style & PT_SHADOW){
                //dc->color = shadow_color;
                //draw_shadow(dc, px, py, text+begin, end-begin);
            }
            //dc->color = c;

            x = 0;
            for(; l_begin < l_end; ++l_begin)
            {
                if(*l_begin == '\t'){
                    x = align_next(x, tab);
                    continue;
                }
                else if(*l_begin == '\r' || *l_begin == '\n'){
                    continue;
                }
                x += put_char(dc, px + x, py, *l_begin);
            }
        }

        if(*l_end == '\n'){//next line
            ++l_end;
        }

        py += ch_size + (ch_size / 8);// 1/8 line space
        if(int(top + height) < py + ch_size){
            break;
        }
    }

    return py - top;
}

//---------------------------------------------------------------------------
//
// 通用字体函数
//
//---------------------------------------------------------------------------

/*
unifont* font_get_font(const font_t& font);
int font_char_width(const font_t& font, unichar_t ch);
int font_char_height(const font_t& font);
int font_text_width(const font_t& font, const unichar_t* text, size_t length);
*/

/*
int font_char_width(const font_t& font, unichar_t ch)
{
    unifont* f = font_get_font(font);
    if(f){
        return f->char_width(ch);
    }
    return 0;
}

int font_char_height(const font_t& font)
{
    unifont* f = font_get_font(font);
    if(f){
        return f->char_height();
    }
    return 0;
}

int font_text_width(const font_t& font, const unichar_t* text, size_t length)
{
    unifont* f = font_get_font(font);
    if(f){
        return f->text_width(text, length);
    }
    return 0;
}
*/

}//end namespace graphics
}//end namespace cgl

#endif //CORE_FONT_HPP_20180718234739

