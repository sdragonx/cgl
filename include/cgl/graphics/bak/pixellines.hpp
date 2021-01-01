/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pixellines.hpp

 2018-10-26 14:55:03

 缓存图片扫描线，以加快访问速度

*/
#ifndef PIXELLINES_HPP_20181026145503
#define PIXELLINES_HPP_20181026145503

#include <cgl/public.h>
#include <cgl/graphics/image.hpp>
#include <cgl/memory/block.hpp>

namespace cgl{
namespace graphics{


/*
class scanline
{
private:
    //graphics::pixelimage* m_image;
    std::vector<byte_t*> m_lines;
    vec2i m_size;    //m_bounds;

public:
    void bind(graphics::pixelimage& image);
    int width();
    int height();
    byte_t* lines(int index)
    {
        return m_lines[index];
    }

    byte_t* operator[](int index)
    {
        return this->lines(index);
    }
};

class pixelbuffer
{
private:
    scanline* m_scanline;

public:
    void bind(scanline& linebuffer);
    void set_pixel(int x, int y, uint32_t color);
    uint32_t get_pixel(int x, int y);
};

*/

//template<typename T>
class pixellines
{
public:
    typedef uint32_t color_type;
    int id;

private:
    int16_t m_width;
    int16_t m_height;
    int16_t m_pixelbits;
    int16_t m_linebytes;
    memblock<byte_t*> m_lines;

public:
    pixellines() : id(), m_width(), m_height(), m_pixelbits(), m_linebytes(), m_lines()
    {
    }

    void dispose()
    {
        m_lines.dispose();
        id = m_width = m_height = m_pixelbits = m_linebytes = 0;
    }

    int width()const  { return m_width; }
    int height()const { return m_height; }
    int pixelbits()const { return m_pixelbits; }
    int linebytes()const { return m_linebytes; }

    byte_t* operator[](int y) { return m_lines[y]; }
    const byte_t* operator[](int y)const { return m_lines[y]; }

    #ifdef CGL_PLATFORM_WINDOWS

    #ifndef STRICT
        #error pixellines : must define STRICT.
    #endif

    int bind(HDC dc)
    {
        int t = GetObjectType(dc);
        if(t == OBJ_DC){
            id = (int)dc;
            m_pixelbits = -1;
            HWND hwnd = WindowFromDC(dc);
            RECT rc;
            GetClientRect(hwnd, &rc);
            m_width = rc.right - rc.left;
            m_height = rc.bottom - rc.top;
        }
        else if(t == OBJ_MEMDC){
            HBITMAP hbmp = (HBITMAP)GetCurrentObject(dc, OBJ_BITMAP);
            return this->bind(hbmp);
        }
        return -1;
    }

    int bind(HBITMAP hbmp)
    {
        if(hbmp){
            BITMAP bm = {0};
            GetObject(hbmp, sizeof(bm), &bm);
            if(!this->bind(bm)){
                id = (int)hbmp;
                return 0;
            }
        }
        return -1;
    }

    int bind(const BITMAP& bm)
    {
        this->dispose();
        //if(bm.bmBitsPixel < 24 || !bm.bmBits)return -1;
        if(!bm.bmBits)return -1;
        m_width = bm.bmWidth;
        m_height = bm.bmHeight;
        m_pixelbits = bm.bmBitsPixel;
        m_linebytes = bm.bmWidthBytes;
        m_lines.resize(bm.bmHeight);
        for(int y=0; y<int(bm.bmHeight); ++y)
        {
            byte_t* p = (byte_t*)(((char*)bm.bmBits) + bm.bmWidthBytes * (bm.bmHeight - y - 1));
            m_lines[y] = p;
        }

        id = (int)bm.bmBits;

        return 0;
    }
    #endif

    int bind(pixelimage& image, bool reverse_scan = false)
    {
        this->dispose();
        if(image.is_null() || image.pixelbits() < 24){
            return -1;
        }
        m_width = image.width();
        m_height = image.height();
        m_pixelbits = image.pixelbits();
        m_linebytes = image.linebytes();
        m_lines.resize(image.height());
        for(size_t y=0; y<image.height(); ++y){
            m_lines[y] = (byte_t*)image.scanline(y, reverse_scan);
        }
        id = (int)&image;
        return 0;
    }

    void clear()
    {
        for(size_t y = 0; y < m_lines.size(); ++y){
            memset(m_lines[y], 0, m_linebytes);
        }
    }

    void clear(int n)
    {
        for(size_t y = 0; y < m_lines.size(); ++y){
            memset(m_lines[y], n, m_linebytes);
        }
    }

    void fill(color_type color)
    {
        if(pixelbits() == 32){
            for(size_t y = 0; y < m_lines.size(); ++y){
                for(int x = 0; x < width(); ++x){
                    ((uint32_t*)m_lines[y])[x] = color;
                }
            }
        }
        else if(pixelbits() == 24){
            for(size_t y = 0; y < m_lines.size(); ++y){
                for(int x = 0; x < width(); ++x){
                    ((color24*)m_lines[y])[x].set(color);
                }
            }
        }
    }

    void setPixel(int x, int y, color_type color)
    {
        if(pixelbits() == 32){
            ((uint32_t*)m_lines[y])[x] = color;
        }
        else if(pixelbits() == 24){
            ((color24*)m_lines[y])[x].set(color);
        }
        else{

        }
    }

    color_type getPixel(int x, int y)const
    {
        if(x < 0 || y < 0 || x >= width() || y >= height())return 0;
        if(pixelbits() == 32){
            return ((uint32_t*)m_lines[y])[x];
        }
        else if(pixelbits() == 24){
            return ((color24*)m_lines[y])[x].color();
        }
        else{
            return 0;
        }
    }

    void setPixel32(int x, int y, color_type color)
    {
        //if(x < 0 || y < 0 || x >= width || y >= height)return ;
        ((uint32_t*)m_lines[y])[x] = color;
    }

    color_type getPixel32(int x, int y)
    {
        //if(x < 0 || y < 0 || x >= width || y >= height)return 0;
        return ((uint32_t*)m_lines[y])[x];
    }

    void setPixel24(int x, int y, color_type color)
    {
        if(x < 0 || y < 0 || x >= width() || y >= height())return ;
        ((color24*)m_lines[y])[x].set(color);
    }

    color_type getPixel24(int x, int y)
    {
        if(x < 0 || y < 0 || x >= width() || y >= height())return 0;
        return ((color24*)m_lines[y])[x].color();
    }
};

//获取双线插值像素
//x y 坐标 <<=16的定点数
uint32_t bilinear_color(pixellines& lines, uint32_t x, uint32_t y)
{
    byte_t *ps0, *ps1;
    int channels = lines.pixelbits() / 8;
    uint32_t width = lines.width();
    uint32_t height = lines.height();
    uint32_t u, v;
    uint32_t pm00, pm10;
    uint32_t pm01, pm11;
    union{
        uint32_t color;
        byte_t   data[4];
    };

    u = x >> 16;
    v = y >> 16;

    if(u >= width || v >= height)return 0;

    if(u == width - 1){
        if(v < height){
            return lines[v][u];
        }
        return 0;
    }

    if(v == height - 1){
        if(u < width){
            return lines[v][u];
        }
        return 0;
    }

    if(u + 1 == width)--u;//暂定，越界

    ps0 = (byte_t*)lines[v];//源图像第一行像素，连续获得两行像素
    ps1 = (byte_t*)lines[v + 1];//源图像第二行像素
    u *= channels;

    ps0 += u;
    ps1 += u;

    //计算4个像素各占的分量

    u = x & 0xFFFF;
    v = y & 0xFFFF;
    pm11 = (u * v) >> 16;
    pm10 = u - pm11;
    pm01 = v - pm11;
    pm00 = 65536 - pm10 - pm01 - pm11;

    //计算源像素x坐标
    for(int z=0; z < channels; ++z){
        data[z] =((
             pm00 * ps0[z]
            +pm10 * ps0[z + channels]
            +pm01 * ps1[z]
            +pm11 * ps1[z + channels])>>16);
    }

    return color;
}

}//end namespace graphics
}//end namespace cgl

#endif //PIXELLINES_HPP_20181026145503
