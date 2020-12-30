/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pixelbuffer.hpp

 2020-04-04 18:34:26

*/
#ifndef PIXELBUFFER_HPP_20200404183426
#define PIXELBUFFER_HPP_20200404183426

#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/memory/block.hpp>

namespace cgl{
namespace graphics{

//
// pixelbuffer<T>
//
template<typename T>
class pixelbuffer
{
public:
    typedef pixelbuffer this_type;
    typedef T value_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;

protected:
    memblock<pointer> m_lines;
    int m_width;
    int m_height;
    int m_stride;

public:
    pixelbuffer() : m_lines() { }

    int assign(void* data, int width, int height, int stride)
    {
        this->dispose();
        m_width = width;
        m_height = height;
        m_stride = stride;
        m_lines.resize(height);
        byte_t *p = static_cast<byte_t*>(data);
        for(int i=0; i<height; ++i){
            m_lines[i] = reinterpret_cast<pointer>(p);
            p += stride;
        }
        return 0;
    }

    int assign(this_type& other)
    {
        this->dispose();
        m_width = other.m_width;
        m_height = other.m_height;
        m_stride = other.m_stride;
        m_lines.resize(m_height);
        for(int i=0; i<m_height; ++i){
            m_lines[i] = other.m_lines[i];
        }
        return 0;
    }

    int assign(pixelimage& image)
    {
        return this->assign(image.data(), image.width(), image.height(), image.linebytes());
    }

    #ifdef CGL_PLATFORM_WINDOWS
    int assign(HBITMAP bmp)
    {
        this->dispose();
        BITMAP bm;
        GetObject(bmp, sizeof(bm), &bm);
        if(!bm.bmBits){
            return CGL_ERROR;
        }

        byte_t *data = static_cast<byte_t*>(bm.bmBits);
        data += (bm.bmHeight - 1) * bm.bmWidthBytes;//first scanline
        return this->assign(data, bm.bmWidth, bm.bmHeight, -int(bm.bmWidthBytes));

        //return this->assign(bm.bmBits, bm.bmWidth, bm.bmHeight, -int(bm.bmWidthBytes));
    }

    int assign(HBITMAP bmp, int x, int y, int width, int height)
    {
        this->dispose();
        BITMAP bm;
        GetObject(bmp, sizeof(bm), &bm);
        if(!bm.bmBits){
            return CGL_ERROR;
        }

        //cliprect
        vec4i rect(0, 0, bm.bmWidth, bm.bmHeight);
        rect = rect & vec4i(x, y, width, height);
        if(rect.reversed()){
            return CGL_ERROR;
        }

        byte_t *data = static_cast<byte_t*>(bm.bmBits);
        data += (bm.bmHeight - 1 - rect.y) * bm.bmWidthBytes;
        data += rect.x * bm.bmBitsPixel / 8;
        return this->assign(data, rect.width, rect.height, -int(bm.bmWidthBytes));
    }

    #ifdef CGL_PLATFORM_VCL
    int assign(Graphics::TBitmap* bmp)
    {
        return this->assign(bmp->Handle);
    }
    int assign(Graphics::TBitmap* bmp, int x, int y, int width, int height)
    {
        return this->assign(bmp->Handle, x, y, width, height);
    }

    int assign(TImage* img)
    {
        return this->assign(img->Picture->Bitmap->Handle);
    }
    int assign(TImage* img, int x, int y, int width, int height)
    {
        return this->assign(img->Picture->Bitmap, x, y, width, height);
    }
    #endif//CGL_PLATFORM_VCL
    #endif//CGL_PLATFORM_WINDOWS

    #ifdef ILIMAGE_HPP_20160314145804
    //32bits
    int assign(ilimage& image)
    {
        size_t linebytes = image.width() * 4;
        if(image.origin() == IL_ORIGIN_UPPER_LEFT){
            this->assign(image.data(), image.width(), image.height(), linebytes);
        }
        else{
            this->assign(image.data() + (image.height() - 1) * linebytes, image.width(), image.height(), -linebytes);
        }

        return 0;
    }
    #endif

    void dispose()
    {
        m_lines.dispose();
        m_width = m_height = 0;
    }

    /*
    int channels()const
    {
        return sizeof(value_type);
    }
    */

    void* data()
    {
        return m_lines[0];
    }

    const void* data()const
    {
        return m_lines[0];
    }

    int width()const
    {
        return m_width;
    }

    int height()const
    {
        return m_height;
    }

    int stride()const
    {
        return m_stride;
    }

    reference pixels(int x, int y)
    {
        return m_lines[y][x];
    }

    const_reference pixels(int x, int y)const
    {
        return m_lines[y][x];
    }

    pointer operator[](int y)
    {
        return m_lines[y];
    }

    const_pointer operator[](int y)const
    {
        return m_lines[y];
    }

    void clear()
    {
        for_each_pixel(*this, pixel_reset<value_type>);
    }

    void cliprect(int& x1, int& y1, int& x2, int& y2)
    {
        if(x2 < x1)std::swap(x1, x2);
        if(y2 < y1)std::swap(y1, y2);
        clip_x(x1);
        clip_x(x2);
        clip_y(y1);
        clip_y(y2);
    }

    void fill(value_type color)
    {
        for(size_t y = 0; y < this->height(); ++y){
            pointer p = m_lines[y];
            std::fill<pointer>(p, p + m_width, color);
        }
    }

    bool is_invalid(int x, int y)const
    {
        return x < 0 || y < 0 || x >= this->width() || y >= this->height();
    }

    void set_pixel(int x, int y, value_type color)
    {
        if(is_invalid(x, y)){
            return ;
        }
        pixels(x, y) = color;
    }

    value_type get_pixel(int x, int y)const
    {
        if(is_invalid(x, y)){
            return 0;
        }
        return pixels(x, y);
    }

    void blend(int x, int y, value_type color)
    {
        if(is_invalid(x, y)){
            return ;
        }
        pixels(x, y) = ::blend<T>(pixels(x, y), color);
    }

    void blend(int x, int y, value_type color, T alpha)
    {
        if(is_invalid(x, y)){
            return ;
        }
        pixels(x, y) = ::blend<T, T>(pixels(x, y), color, alpha);
    }

    void hline(int x1, int x2, int y, value_type c)
    {
        if(is_invalid_y(y))return ;
        clip_x(x1);
        clip_x(x2);
        pointer p = m_lines[y];
        std::fill<pointer>(p + x1, p + x2, c);
    }

    void vline(int x, int y1, int y2, value_type c)
    {
        if(is_invalid_x(x))return ;
        clip_y(y1);
        clip_y(y2);
        for(int y=y1; y < y2; ++y){
            pixels(x, y) = c;
        }
    }

    void draw_rect(int x, int y, int width, int height, value_type color)
    {
        hline(x, x + width, y, color);
        hline(x, x + width, y + height, color);
        vline(x, y + 1, y + height, color);
        vline(x + width, y, y + height + 1, color);
    }

    void fill_rect(int x, int y, int width, int height, value_type color)
    {
        int x1 = x + 1;
        int x2 = x + width;
        int y1 = y + 1;
        int y2 = y + height;
        cliprect(x1, y1, x2, y2);
        for(y = y1; y < y2; ++y){
            pointer p = m_lines[y];
            std::fill<pointer>(p + x1, p + x2, color);
        }
    }

    void blend_hline(int x1, int x2, int y, value_type c)
    {
        if(is_invalid_y(y))return ;
        if(x1 < 0)x1 = 0;
        if(x2 >= this->width())x2 = this->width();
        for(int x=x1; x < x2; ++x){
            pixels(x, y) = ::blend<T>(pixels(x, y), c);
        }
    }

    void blend_vline(int x, int y1, int y2, value_type c)
    {
        if(is_invalid_x(x))return ;
        if(y1 < 0)y1 = 0;
        if(y2 >= this->height())y2 = this->height();
        for(int y=y1; y < y2; ++y){
            pixels(x, y) = ::blend<T>(pixels(x, y), c);
        }
    }

    void blend_draw_rect(int x, int y, int width, int height, value_type color)
    {
        blend_hline(x, x + width, y, color);
        blend_hline(x, x + width, y + height, color);
        blend_vline(x, y + 1, y + height, color);
        blend_vline(x + width, y, y + height + 1, color);
    }

    void blend_fill_rect(int x, int y, int width, int height, value_type color)
    {
        int x1 = x + 1;
        int x2 = x + width;
        int y1 = y + 1;
        int y2 = y + height;
        cliprect(x1, y1, x2, y2);
        for(y = y1; y < y2; ++y){
            pointer p = m_lines[y];
            for(x = x1; x < x2; ++x){
                pixels(x, y) = ::blend<T>(pixels(x, y), color);
            }
        }
    }

private:
    bool is_valid_x(int x)
    {
        return x >= 0 && x <= this->width();
    }

    bool is_valid_y(int y)
    {
        return y >= 0 || y <= this->height();
    }

    bool is_invalid_x(int x)
    {
        return x < 0 || x >= this->width();
    }

    bool is_invalid_y(int y)
    {
        return y < 0 || y >= this->height();
    }

    void clip_x(int &x)
    {
        x = math::clamp(x, 0, width());
    }

    void clip_y(int &y)
    {
        y = math::clamp(y, 0, height());
    }
};


}//end namespace graphics
}//end namespace cgl

#endif //PIXELBUFFER_HPP_20200404183426
