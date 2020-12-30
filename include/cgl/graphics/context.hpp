/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 context.hpp

 2018-10-29 15:10:48

context.font
context.image
context.matrix



template<typename T, typename _char_t>
int draw_text(T dc, int x, int y, const _char_t* str, size_t size)
{
    return 0;
}

template<typename T>
int char_width(T dc, int ch)
{
    return 0;
}

template<typename T>
int char_height(T dc, int ch)
{
    return 0;
}

template<typename T, typename _char_t>
int text_width(T dc, const _char_t* str, size_t size)
{
}

*/
#ifndef GRAPHICS_CONTEXT_HPP_20181029151048
#define GRAPHICS_CONTEXT_HPP_20181029151048

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include <cgl/vertex.hpp>
#include <cgl/graphics/teximage.hpp>
#include <cgl/graphics/font.hpp>

namespace cgl{
namespace graphics{

//---------------------------------------------------------------------------
//
// device_context unicontext
//
//---------------------------------------------------------------------------

class unicontext
{
public:
    typedef vtx3f vertex;

public:
    vec4ub color;
    font_t font;

public:
    unicontext() : color(color::white) { /*void*/ }
    ~unicontext() { this->dispose(); }

    virtual void dispose() { /*void*/ }

    virtual bool ready()const { return false; }

    virtual void viewport(int x, int y, int width, int height){ /*void*/ }


//---------------------------------------------------------------------------
//paint
    virtual void pen_width(float value) { }

    virtual int draw_line(float x1, float y1, float x2, float y2) { return CGL_ERROR; }
    int draw_line(const vec2f& v1, const vec2f& v2) { return draw_line(v1.x, v1.y, v2.x, v2.y); }
    int draw_line(const vec3f& v1, const vec3f& v2) { return draw_line(v1.x, v1.y, v2.x, v2.y); }

    virtual int draw_rect(float x, float y, float width, float height) { return CGL_ERROR; }
    virtual int fill_rect(float x, float y, float width, float height) { return CGL_ERROR; }

    virtual void draw_ellipse(float x, float y, float cx, float cy) { /*void*/ }
    virtual void fill_ellipse(float x, float y, float cx, float cy) { /*void*/ }

    void draw_circle(float x, float y, float r)
    {
        draw_ellipse(x, y, r, r);
    }

    void fill_circle(float x, float y, float r)
    {
        fill_ellipse(x, y, r, r);
    }

    virtual void draw_polyline(vec2f* vs, size_t size);

    virtual void draw_polygon(vec2f* vs, size_t size) { /*void*/ }
    virtual void fill_polygon(vec2f* vs, size_t size) { /*void*/ }

    virtual int draw_image(intptr_t image, float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        return CGL_ERROR;
    }


    virtual int draw_arrays(int shape, const vec2f* vs, size_t size) { return CGL_ERROR; }
    virtual int draw_elements(int shape, const vec2f* vs,  const int* ids, size_t size) { return CGL_ERROR; }

    virtual int draw_arrays(int shape, const vtx3f* vp, size_t start, size_t size) { return CGL_ERROR; }
    virtual int draw_arrays(int shape, const vtx3fx* vp, size_t start, size_t size) { return CGL_ERROR; }


    /*
    int draw_arrays(int shape, const std::vector<vtx3f>& vs)
    {
        this->draw_arrays(shape, &vs[0], 0u, vs.size());
    }

    int draw_arrays(int shape, const std::vector<vtx3fx>& vs)
    {
        this->draw_arrays(shape, &vs[0], 0u, vs.size());
    }
    */
    template<typename E, typename T>
    int draw_arrays(E shape, const std::vector<T>& vs)
    {
        this->draw_arrays(shape, &vs[0], 0u, vs.size());
    }

//---------------------------------------------------------------------------
//print

public:
    int print(float x, float y, const char* text, size_t length = TEXT_MAX);
    int print(float x, float y, const wchar_t* text, size_t length = TEXT_MAX);
    int print(float x, float y, const char8_t* text, size_t length = TEXT_MAX);
    int print(float x, float y, const char16_t* text, size_t length = TEXT_MAX);
    int print(float x, float y, const char32_t* text, size_t length = TEXT_MAX);
    int print(float x, float y, const unistring& text);

protected:
    virtual int vprint(float x, float y, const unichar_t* text, size_t length)
    {
        return CGL_ERROR;
    }

//---------------------------------------------------------------------------
//matrix

public:
    virtual void identity() { /*void*/ }
    virtual void scale(float x, float y, float z) { /*void*/ }
    virtual void translate(float x, float y, float z) { /*void*/ }
    virtual void rotate(float angle, float x, float y, float z) { /*void*/ }
    virtual void push_matrix() { /*void*/ }
    virtual void pop_matrix() { /*void*/ }

//---------------------------------------------------------------------------
//texture

    virtual int create_texture(
        int width, int height, int format,
        int source_format, int source_type, const byte_t* pixels = NULL,
        int mode = 0)//expand attribute
    {
        return 0;
    }

    virtual int load_texture(const wchar_t* filename, int format, int mode = 0) { return 0; }
    virtual int default_texture() { return 0; }
    virtual void delete_texture(int tex) { /*void*/ }
    virtual void bind_texture(int tex) { /*void*/ }

    //virtual int teximage(void* data, int width, int height, int format
    //virtual int loadimage();
    //virtual void closeimage(int id);
    //virtual void close_all_image();
    //virtual void images.open(...)
    //virtual void fonts.open(...)

//---------------------------------------------------------------------------
//font
    virtual unifont* load_font(const font_t& font) { return null; }
    virtual int char_width(wchar_t ch) { return 0; }
    virtual int char_height() { return 0; }
    virtual int text_width(const wchar_t* text, size_t length) { return 0; }

    //设置字体
    virtual int set_font(const char* name, int size, int style) { return CGL_ERROR; }
    virtual int set_font(const char* name, int size, bool bold, bool italic, bool underline) { return CGL_ERROR; }

//cliprect
    virtual void cliprect(int x, int y, int width, int height) { /*void*/ }
    template<typename T>
    void cliprect(const vec4<T>& rect)
    {
        return this->cliprect(rect.x, rect.y, rect.width, rect.height);
    }
    virtual void push_cliprect() { /*void*/ }
    virtual void pop_cliprect() { /*void*/ }
};

//
// source
//

void unicontext::draw_polyline(vec2f* vs, size_t size)
{
    if(size < 2){
        return ;
    }
    for(size_t i=1; i<size; ++i){
        draw_line(vs[i - 1], vs[i]);
    }
}

int unicontext::print(float x, float y, const char* text, size_t length)
{
    unistring str(text, length);
    return this->vprint(x, y, str.c_str(), str.length());
}

int unicontext::print(float x, float y, const wchar_t* text, size_t length)
{
    #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
    return this->vprint(x, y, text, length);
    #else
    unistring str(text, length);
    return this->vprint(x, y, str.c_str(), length);
    #endif
}

int unicontext::print(float x, float y, const char8_t* text, size_t length)
{
    unistring str(text, length);
    return this->vprint(x, y, str.c_str(), str.length());
}

int unicontext::print(float x, float y, const char16_t* text, size_t length)
{
    return this->vprint(x, y, reinterpret_cast<const unichar_t*>(text), length);
}

int unicontext::print(float x, float y, const char32_t* text, size_t length)
{
    unistring str(text, length);
    return this->vprint(x, y, str.c_str(), str.length());
}

int unicontext::print(float x, float y, const unistring& text)
{
    return this->vprint(x, y, text.c_str(), text.length());
}



//---------------------------------------------------------------------------
//
// device_context unicontext
//
//---------------------------------------------------------------------------

class device_context : public unicontext
{
public:
    device_context() : unicontext()
    {

    }
};


//
//
//


template<typename Context, typename T>
void draw_line(Context& dc, T x1, T y1, T x2, T y2)
{
    dc.draw_line(x1, y1, x2, y2);
}

#ifdef CGL_PLATFORM_WINDOWS

template<>
void draw_line<HDC, int>(HDC& dc, int x1, int y1, int x2, int y2)
{
    MoveToEx(dc, x1, y1, NULL);
    LineTo(dc, x2, y2);
}

template<>
void draw_line<HDC, float>(HDC& dc, float x1, float y1, float x2, float y2)
{
    MoveToEx(dc, x1, y1, NULL);
    LineTo(dc, x2, y2);
}

#endif

template<typename Context>
int dash_line(Context& dc, int x1, int y1, int x2, int y2, int solid, int space, vec2i& offset)
{
    using namespace std;

    const static int bits = 16;
    const static int one = 1 << bits;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << bits) / k;
        y_step = y1 < y2 ? one : -one;
    }
    else{
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? one : -one;
        y_step = (dy << bits) / k;
    }

    x1 <<= bits;
    y1 <<= bits;
    x1 += one >> 1;//四舍五入
    y1 += one >> 1;

    int i = offset.x;
    int j = offset.y;

    vec2i p;
    bool is_line_to = false;

    //恢复绘制模式
    if(offset.y && (offset.y & 1)){//solid
        p.x = x1 >> bits;
        p.y = y1 >> bits;
        is_line_to = true;
    }
    //恢复绘制进度
    x1 += x_step * offset.x;
    y1 += y_step * offset.x;

    for( ; i < k; ++j){
        if(is_line_to){
            draw_line<Context>(dc, p.x, p.y, x1 >> bits, y1 >> bits);
            is_line_to = false;
        }
        else{
            p.x = x1 >> bits;
            p.y = y1 >> bits;
            is_line_to = true;
        }

        if(j & 1){//space mode
            x1 += x_step * space;
            y1 += y_step * space;
            i += space;
        }
        else{//solid mode
            x1 += x_step * solid;
            y1 += y_step * solid;
            i += solid;
        }
    }
    //space不用画，solid要结尾，保证顶点数量为2的倍数
    if(j & 1){
//        if(is_line_to){
            draw_line<Context>(dc, p.x, p.y, x2, y2);
//        }
    }

    //保存绘制模式、进度
    offset.x = i - k;
    offset.y = j;

    return k;
}

}//end namespace graphics
}//end namespace cgl

#endif //GRAPHICS_CONTEXT_HPP_20181029151048
