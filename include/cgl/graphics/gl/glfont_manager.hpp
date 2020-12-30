/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 font_manager.hpp

 2018-07-17 17:57:48

*/
#ifndef FONT_MANAGER_HPP_20180717175748
#define FONT_MANAGER_HPP_20180717175748

#include <cgl/graphics/font.hpp>
#include <cgl/resource/fonts/monaco_ttf.h>
#include <cgl/graphics/gl/glfont.hpp>

namespace cgl{
namespace graphics{
namespace gl{

class glfont_manager
{
public:
    typedef font_t font_type;
    typedef std::map<font_type, glfont*> fontlist_type;
    typedef TYPENAME fontlist_type::iterator iterator;
    //typedef glatlas<font_char, char_info> imagelist_type;

    device_context* m_gl;
    glfont m_basefont;
    fontlist_type m_fonts;
//    imagelist_type m_base_imagelist;
//    std::map<string_t, imagelist_type> m_imagelists;
public:
    glfont_manager(device_context* context):m_gl(context), m_fonts()
    {
        //基础字体，默认贴图256大小，字体大小不能超过256
        m_basefont.init(context, CGL_TEXT_DEFAULT, CGL_DEFAULT_FONT_SIZE, 256);
        //m_base_imagelist.init(m_gl, 256, glfont::TEX_FORMAT, true, false, glfont::DEFAULT_PACK_METHOD);
    }

    ~glfont_manager()
    {
        //this->dispose();
    }

    /*
    imagelist_type* get_imagelist(const string_t& name)
    {
        if(name.empty()){
            return &m_base_imagelist;
        }
        else{
            imagelist_type* imagelist = &m_imagelists[name];
            if(!imagelist->is_init()){
                imagelist->init(m_gl, glfont::DEFAULT_TEXTURE_SIZE, glfont::TEX_FORMAT, true, false, glfont::DEFAULT_PACK_METHOD);
            }
            return imagelist;
        }
    }


    glfont* load(const string_t& name, int size, int page = 1024)
    {
        glfont f;
        f.init(m_gl, size);
        return f;
    }

    glfont* load(const font_t& font)
    {
        return this->font(font.name, font.size);
    }

    void dispose()
    {
        delete_all(m_fonts);
        m_fonts.clear();

        dispose_all(m_imagelists);
        m_imagelists.clear();
    }
    */

    //*
    glfont* load(const cstring<char>& name, int size, int page)
    {
        font_t font(name, size);
        return this->load(font);
    }

    //default font, only ascii charactor
    //default font = font_t(nullstr, 0, int)

    glfont* load(const font_t& font)
    {
        iterator itr;
        //default font
        if(font.name.empty()){
            font_t temp;
            itr = m_fonts.find(temp);
            if(itr == m_fonts.end()){
                glfont* f = new glfont();
                f->init(m_gl, string_t(), CGL_DEFAULT_FONT_SIZE, 256);
                m_fonts[temp] = f;
                return f;
            }
            return itr->second;
        }

        //other font
        itr = m_fonts.find(font);
        if(itr != m_fonts.end()){
            return itr->second;
        }
        else{
            glfont* f = new graphics::glfont;
            //如果打开失败，返回默认的字体
            /*
            if(f->init(m_gl, font.name, font.size) < 0){
                delete f;
                f = this->get_font(font_t());
                return f;
            }
            */
            f->init(m_gl, font.name, font.size);
            m_fonts[font] = f;
            return f;
        }
    }


    glfont* open_memory(const font_t& font, const void* data, size_t size)
    {
        if(font.name.empty()){//return default font
            return this->load(font);
        }

        iterator itr = m_fonts.find(font);
        if(itr != m_fonts.end()){
            return itr->second;
        }
        else{
            //如果打开失败，返回默认的字体
            if(!font_manager::instance().load_memory(font.name, data, size)){
                return &m_basefont;
            }
            else{
                glfont* f = new graphics::glfont();
                f->init(m_gl, font.name, font.size);
                m_fonts[font] = f;
                return f;
            }
        }
    }

    void dispose()
    {
        delete_all(m_fonts);
        m_fonts.clear();
    }

    //*/
};

}//end namespace gl
}//end namespace graphics
}//end namespace cgl

#endif //FONT_MANAGER_HPP_20180717175748
