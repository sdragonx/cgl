/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 console.hpp

 2016-06-10 10:40:04

 cmd_console.parse(string);
 cmd_console.on_command(stringlist);

 consolebox
 consoleline

*/
#ifndef CONSOLE_HPP_20160610104004
#define CONSOLE_HPP_20160610104004

#include <deque>
#include <cgl/string.hpp>
#include <cgl/graphics/context.hpp>
#include <cgl/system/time.hpp>

#ifdef CGL_PLATFORM_WINDOWS
#include <cgl/graphics/gdi/gdi.hpp>
#endif

namespace cgl{

template<typename T>
class consoleline
{
public:
    typedef cstring<T> string_type;
public:
    std::deque<string_type> lines;
    string_type cmd;
    graphics::font_t font;
    vec4ub text_color;
    vec4ub back_color;
    float  alpha;
    vec2i  position;
private:
    size_t   m_maxline;
    bool     m_show;
    uint32_t m_showtime;
    uint32_t m_hidetime;
public:
    consoleline() : lines(), cmd()
    {
        m_show     = true;
        m_showtime = system::time::clock();
        m_hidetime = 3000;//3Ãë
        alpha = 1.0f;
        this->set_maxline(20);//³õÊ¼ÏÔÊ¾20ÐÐ
        text_color = vec4ub(255, 255, 255, 255);
        back_color = vec4ub(0, 128, 255, 128);
    }
    ~consoleline()
    {
        this->clear();
    }

    void clear()
    {
        lines.clear();
        cmd.clear();
    }

    void show(bool bshow)
    {
        if(bshow)
        {
            m_showtime = system::time::clock();
            //alpha = 1.0f;//½¥Òþ
        }
        m_show = bshow;
    }

    bool visible()
    {
        if(m_hidetime < clock()-m_showtime)
        {
            m_show = false;
        }
        return m_show;
    }

    void set_maxline(size_t size)//??
    {
        m_maxline = size;
    }

    size_t maxline()
    {
        return m_maxline;
    }

    void print(const cstring<T>& s)
    {
        lines.push_back(s);
        if(this->maxline() < lines.size()){
            lines.pop_front();
        }
        this->show(true);
    }

    //void shell();

    template<typename Context>
    void paint(Context* dc)
    {
        graphics::font_t f = dc->font;
        dc->font = font;
        int text_height = dc->char_height();
        dc->color = text_color;
        for(size_t i=0; i<lines.size(); ++i)
        {
            dc->print(position.x, position.y + i * text_height, lines[i].c_str(), lines[i].length());
        }
        dc->font = f;
    }

    #ifdef CGL_PLATFORM_WINDOWS
    void paint(HDC dc)
    {
        int x = position.x;
        int y = position.y;
        //dc->Brush->Style = bsClear;
        //dc->Brush->Color = TColor(RGB(back_color.r, back_color.g, back_color.b));
        //dc->Font->Color = TColor(RGB(text_color.r, text_color.g, text_color.b));
        for(size_t i=0; i<lines.size(); ++i){
            tvalue<T>(TextOutA, TextOutW)(dc, x, y, lines[i].c_str(), lines[i].length());
            y += graphics::char_height(dc);
        }
    }
    #endif
};

template<typename T>
class color_console
{
public:
    typedef color_console this_type;
    struct COLOR_WORD{
        vec4ub color;
        cstring<T> text;
    };
    typedef std::vector<COLOR_WORD> line_type;
    typedef std::deque<line_type> memo_type;
private:
    vec4ub color;
    memo_type memo;
public:
    size_t maxline;
    color_console() : color(graphics::color::white), memo(), maxline(20)
    {

    }

    void clear()
    {
        color = graphics::color::white;
        memo.clear();
    }

    this_type& operator<<(const cstring<T>& text)
    {
        if(memo.empty()){
            memo.push_back(line_type());
        }
        COLOR_WORD w;
        w.text = text;
        w.color = color;
        memo.back().push_back(w);
        return *this;
    }
    this_type& operator<<(const vec4ub& clr)
    {
        color = clr;
        return *this;
    }

    this_type& operator<<(const endline_t&)//endline
    {
        memo.push_back(line_type());
        if(memo.size() >= maxline){
            memo.pop_front();
        }
        return *this;
    }

    void paint(graphics::device_context* dc, int x, int y)
    {
        int px = 0;
        int py = 0;
        int ch = dc->char_height();
        for(size_t i=0; i<memo.size(); ++i)
        {
            for(size_t j=0; j<memo[i].size(); ++j){
                const COLOR_WORD& word = memo[i][j];
                dc->color = memo[i][j].color;
                dc->print(x + px, y + py, word.text.c_str(), word.text.length());
                px += dc->text_width(word.text.c_str(), word.text.length());
            }
            py += ch;
            px = 0;
        }
    }
};

}//end namespace cgl

#endif //CONSOLE_HPP_20160610104004
