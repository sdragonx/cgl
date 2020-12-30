/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glfont.hpp

 2015-04-05 01:47:10

 abc.ttf-32-1024

*/
#ifndef GLFONT_HPP_20150405014710
#define GLFONT_HPP_20150405014710

#include <cgl/graphics/context.hpp>
#include <cgl/graphics/font.hpp>
#include <cgl/graphics/font_char.hpp>
#include <cgl/graphics/font_manager.hpp>
#include <cgl/graphics/effect/gaussblur.hpp>
#include <cgl/graphics/gl/gl.hpp>
#include <cgl/graphics/gl/atlas.hpp>

namespace cgl{
namespace graphics{

class glfont : public unifont
{
public:
    typedef glatlas<font_char, char_info> imagelist_type;

    typedef TYPENAME imagelist_type::value_type* char_image;

    //const static int SHADOW_SIZE = 2;    //阴影大小
    const static int DEFAULT_TEXTURE_SIZE = 1024;
    const static int DEFAULT_PACK_METHOD  = math::maxrects<int>::BOTTOM_LEFT;

    #ifdef CGL_PLATFORM_GLES
    //GL_ALPHA;//GL_LUMINANCE_ALPHA;
    const static int TEX_FORMAT = GL_RGBA;;
    const static int SRC_FORMAT = GL_LUMINANCE_ALPHA;//GL_LUMINANCE_ALPHA;
    #else
    const static int TEX_FORMAT = CGL_ALPHA8;
    const static int SRC_FORMAT = CGL_ALPHA;
    #endif

private:
    device_context* m_gl;
    string_t m_name;
    int m_size;            //字体大小
    imagelist_type m_imagelist;

public:
    glfont() : m_gl(null), m_name(), m_size(), m_imagelist()
    {

    }

    void init(device_context* gl, const string_t& name, int size = CGL_DEFAULT_FONT_SIZE, int texture_size = DEFAULT_TEXTURE_SIZE)
    {
        m_gl = gl;
        m_name = name;
        m_size = size;
        //m_imagelist = imagelist;
        m_imagelist.init(m_gl, texture_size, TEX_FORMAT, true, false, math::maxrects<int>::BOTTOM_LEFT);
    }

    void clear()
    {
        m_imagelist.dispose();
    }

    virtual void dispose()
    {
        if(m_gl){
            m_gl = null;
            m_imagelist.dispose();
            //m_imagelist = null;
            m_size = 0;
        }
    }

    void set_size(int size)
    {
        if(size < 8)size = 8;
        if(m_size != size){
            m_size = size;
        }
    }

    //获得字符宽度
    int char_width(wchar_t ch);

    //获取字符高度
    int char_height() { return m_size; }

    //获取字符串宽度
    //int text_width(const wchar_t* text, size_t length);

    char_image char_item(wchar_t ch)
    {
        font_char id(ch, m_size);
        if(!m_imagelist.exist(id)){
            make_char(id);
        }
        return m_imagelist.value(id);
    }

    //paint a char
    int put_char(unicontext* dc, int x, int y, unichar_t ch, int flag = 0)
    {
        font_char id(ch, m_size, flag);
        char_image icon = m_imagelist.value(id);
        if(!icon){
            make_char(id);
            icon = m_imagelist.value(id);
        }
        dc->draw_image(icon->image.id,
            x + icon->data.x,
            y + m_size - icon->data.y,
            icon->image.width, icon->image.height,
            icon->image.u1, icon->image.v1, icon->image.u2, icon->image.v2);

        return icon->data.next_x;
    }

    //outline
    int text_outline(device_context* dc, int x, int y, const wchar_t* text, size_t length = TEXT_MAX)
    {
        //阴影坐标默认右下移动一个像素
        if(length == TEXT_MAX)length = wcslen(text);
        for(size_t i=0; i<length; ++i){
            x += put_char(dc, x, y, text[i], font_char::TT_OUTLINE);
        }
        return 0;
    }

    //绘制阴影
    int text_shadow(device_context* dc, int x, int y, const wchar_t* text, size_t length = TEXT_MAX)
    {
        //阴影坐标默认右下移动一个像素
        if(length == TEXT_MAX)length = wcslen(text);
        for(size_t i=0; i<length; ++i){
            x += put_char(dc, x, y, text[i], font_char::TT_SHADOW);
        }
        return 0;
    }

    //绘制阴影
    int text_fluore(device_context* dc, int x, int y, const wchar_t* text, size_t length = TEXT_MAX)
    {
        //阴影坐标默认右下移动一个像素
        if(length == TEXT_MAX)length = wcslen(text);
        for(size_t i=0; i<length; ++i){
            x += put_char(dc, x, y, text[i], font_char::TT_FLUORE);
        }
        return 0;
    }
private:
    int make_char(const font_char& ch);
};

//获得字符宽度
int glfont::char_width(wchar_t ch)
{
    char_image icon;
    font_char id(ch, m_size);
    if(!m_imagelist.exist(id)){
        make_char(id);
    }
    icon = m_imagelist.value(id);
    return icon ? icon->data.next_x : 0;
}

int shadow_text(pixelimage& dest, const pixelimage& source, float sigma = 2.0f)
{
    int size = (sigma * 3.0f);// + 0.5f;
    dest.create(source.width() + size * 2, source.height() + size * 2, 8, false);
    dest.clear();
    bitblt(dest, size, size, source.width(), source.height(), source, 0, 0, CGL_COPY);
    gauss_blur(dest.data(), dest.linebytes(), dest.width(), dest.height(), source.pixelbytes(), sigma);
    byte_t* pdata = dest.data();
    int n;
    for(size_t y = 0; y < dest.height(); ++y){//加深一点阴影
        for(size_t x = 0; x < dest.width(); ++x){
            n = pdata[x];
            if(n){
                n = n >= 128 ? 255 : n * 2;
                //pdata[x] = 255-pdata[x];
                //pdata[x] = pdata[x] > 223 ? 255 : pdata[x] + 32;
                pdata[x] = n;
            }
        }
        pdata += dest.linebytes();
    }
    return size;
}

//荧光字 FluoreStyle
int fluore_text(pixelimage& image, const pixelimage& source, float sigma = 2.0f)
{
    int result = shadow_text(image, source, sigma);
    byte_t* dest;
    int n ;
    for (size_t i=0; i < image.height(); ++i){
        dest = image.scanline(i);
        for(size_t x=0; x < image.width(); ++x){
            n = dest[x];
            if(n){
                if(n > 196){
                    n *= 32;
                    if(n >= 255)n = 255;
                    dest[x] = n;
                }
                else{
                    //dest[x] = 255;
                }
            }
        }
    }
    return result;
}

int glfont::make_char(const font_char& item)
{
    if(!m_gl->ready()){
        return 0;
    }

    pixelimage image;
    char_info info;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    ttfont* font = ttfont_load(m_name);
    font->set_size(m_size);

    if(item.value() == 0x20){//空格
        //空格用字符'A'来代替，渲染出来，清空图像数据
        if(font->render_font(image, info, 'A') == CGL_ERROR){
        }
        image.clear();
        m_imagelist.create(item, image.width(), image.height(), SRC_FORMAT, image.data(), info);
        image.dispose();
        return 0;
    }

    if(item.outline()){//outline
        int size = m_size / 16.0f;
        if(size < 1.0f)size = 1.0f;
        int n = font->render_outline(image, info, item.value(), size);
        if(n){
            CGL_LOG_ERROR("glfont : outline renderer faild.");
            return -1;
        }
    }
    else if(item.shadow()){//shadow
        pixelimage graph;
        if(font->render_font(graph, info, item.value())){
            CGL_LOG_ERROR("glfont : shadow renderer faild.");
            return -1;
        }

        const float sigma = m_size / 16.0f;
        int size = shadow_text(image, graph, sigma);
        info.x -= size;
        info.y += size;
    }
    else if(item.fluore()){//fluore
        const float sigma = m_size / 24.0f;
        pixelimage graph;
        if(font->render_font(graph, info, item.value())){
            CGL_LOG_ERROR("glfont : fluore renderer faild.");
            return -1;
        }
        int size = fluore_text(image, graph, sigma);
        info.x -= size;
        info.y += size;
    }
    else{
        if(font->render_font(image, info, item.value())){
            CGL_LOG_ERROR("glfont : char render faild.");
            return -1;
        }
    }

    #ifndef CGL_PLATFORM_GLES
    m_imagelist.create(item, image.width(), image.height(), SRC_FORMAT, image.data(), info);
    #else
    /*
    m_imagelist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
    image.dispose();
    */
    /*
    //GL_LUMINANCE_ALPHA
    pixelimage la;
    la.create(image.width(), image.height(), 16);
    byte_t* pdata = image.data();
    byte_t* pi = la.data();
    for(size_t y=0; y<image.height(); ++y)
    {
        for(size_t x=0; x<image.width(); ++x)
        {
            pi[x*2] = 255;
            pi[x*2+1] = pdata[x];
        }
        pdata += image.linebytes();
        pi += la.linebytes();
    }

    m_imagelist.insert(ch, image.width(), image.height(), la.data(), SRC_FORMAT, info);
    image.dispose();
    la.dispose();
    /*/
    //GL_RGBA
    pixelimage source;
    source.create(image.width(), image.height(), 32, false);
    byte_t* pdata = image.data();
    for(size_t y=0; y<image.height(); ++y){
        vec4ub* pi = (vec4ub*)source.scanline(y);
        for(size_t x=0; x<image.width(); ++x){
            pi[x].red = pi[x].green = pi[x].blue = 255;
            pi[x].alpha = pdata[x];
        }
        pdata += image.linebytes();
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    m_imagelist.create(item, image.width(), image.height(), CGL_RGBA, source.data(), info);
    source.dispose();
        //*/
    #endif//CGL_PLATFORM_GLES

    image.dispose();
    return 0;
}

}//end namespace graphics
}//end namespace cgl

//#include <glfont.cpp>

#endif //GLFONT_HPP_20150405014710

