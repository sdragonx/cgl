/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 font_manager.hpp

 2018-12-08 07:00:24

 字体管理器

*/
#ifndef FONT_MANAGER_HPP_20181208070024
#define FONT_MANAGER_HPP_20181208070024

#include <cgl/public.h>
#include <cgl/resource/fonts/monaco_ttf.h>

#ifdef CGL_USING_FREETYPE
    #include <cgl/graphics/font/freetype.hpp>
#else
    #include <cgl/graphics/font/stb_font.hpp>
#endif

namespace cgl{
namespace graphics{

#ifdef CGL_USING_FREETYPE
    typedef ftfont ttfont;
#else
    typedef stb_font ttfont;
#endif

ttfont* ttfont_load(const string_t& name);

class font_manager
{
public:
    typedef ttfont font_type;
    typedef font_type* pointer;
    typedef std::map<string_t, pointer> font_map;
    typedef TYPENAME font_map::iterator iterator;

private:
    font_type m_basefont;
    font_map  m_fonts;

public:
    font_manager() : m_basefont(), m_fonts()
    {
    }

    ~font_manager()
    {
        this->dispose();
    }

    void dispose()
    {
        m_basefont.dispose();
        delete_all(m_fonts);
        m_fonts.clear();
    }

    pointer basefont()
    {
        if(!m_basefont.is_open()){
            m_basefont.open_memory(
                resource::fonts::monaco_ttf,
                sizeof(resource::fonts::monaco_ttf));
        }
        return &m_basefont;
    }

    //加载字体文件
    //加载失败，返回basefont
    pointer load(const string_t& filename)
    {
        if(filename.empty()){
            //return null;
            return this->basefont();
        }

        iterator itr = m_fonts.find(filename);
        if(itr != m_fonts.end()){
            return itr->second;
        }

        font_type* font = new font_type;
        if(font->open(filename.c_str()) < 0){
            delete font;
            //return null;
            return this->basefont();
        }

        m_fonts[filename] = font;
        return font;
    }

    //从内存中加载字体文件
    pointer load_memory(const string_t& name, const void* data, size_t size)
    {
        if(name.empty()){
            //return null;
            return this->basefont();
        }

        iterator itr = m_fonts.find(name);
        if(itr != m_fonts.end()){
            return itr->second;
        }

        font_type* font = new font_type;
        if(font->open_memory(data, size) < 0){
            delete font;
            //return null;
            return this->basefont();
        }

        m_fonts[name] = font;
        return font;
    }

    void erase(const string_t& name)
    {
        font_type* font = this->find(name);
        if(font){
            delete font;
            m_fonts.erase(name);
        }
    }

    bool exists(const string_t& name)const
    {
        return is_exists(m_fonts, name);
    }

    pointer find(const string_t& name)
    {
        return *find_obj(m_fonts, name);
    }

    //instance()
    static font_manager& instance()
    {
        return single<font_manager>::value;
    }
};

ttfont* ttfont_load(const string_t& name)
{
    return font_manager::instance().load(name);
}

//这个函数没有缓存数据效率高
int font_char_width(const font_t& font, wchar_t ch)
{
    ttfont* f = ttfont_load(font.name);
    if(f){
        return f->char_width(ch);
    }
    return 0;
}

int font_char_height(const font_t& font)
{
    ttfont* f = ttfont_load(font.name);
    if(f){
        return f->char_height();
    }
    return 0;
}

int font_text_width(const font_t& font, const wchar_t* text, size_t length)
{
    ttfont* f = ttfont_load(font.name);
    int width = 0;
    for(size_t i=0; i<length; ++i){
        width += f->char_width(text[i]);
    }
    return width;
}


}//end namespace graphics
}//end namespace cgl

#endif //FONT_MANAGER_HPP_20181208070024
