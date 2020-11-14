/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 windows_surface.hpp

 2017-02-06 13:43:44

 2016-04-20‏‎12:14:48
*/
#ifndef WINDOWS_SURFACE_HPP_20170206134344
#define WINDOWS_SURFACE_HPP_20170206134344

#include <iostream>
#include <cgl/vec.hpp>
#include <cgl/graphics/color.hpp>
#include "bitmap.hpp"

namespace cgl{
namespace graphics{

class surface
{
private:
    HDC     m_device;
    HBITMAP m_hbmp;
    BITMAP  m_bmp;

public:
    surface();
    surface(const surface& s);
    surface(int w, int h);
    ~surface();

    HDC handle()const;
    HBITMAP bitmap()const;

    int assign(const BITMAP& bmp);
    int assign(HDC hdc);
    int assign(const surface& s);
    bool create(int width, int height, int pf = 32);
    int open(const wchar_t* fname);
    int save(const wchar_t* fname);
    void close();
    bool empty()const;

    //图像尺寸操作
    int width()const;
    int height()const;
    int resize(int w, int h, bool copy = true);//重设图像大小，
    surface scale(int w, int h);//返回一个缩放的图像

    //赋值操作
    surface& operator=(const surface& s);
    surface clone()const;//制作一个复制品
    void swap(surface& s);//交换图像，可用于释放图像

    //数据操作
    void* data();
    const void* data()const;
    void* scanline(int y);
    int PRGBA();

public://绘图函数（向目标绘制）
    void clear(DWORD color = 0x00FFFFFF);
    void clear(const color32& color);
    bool fastblt(HDC hDC, int x, int y, int w = -1, int h = -1);
    int paint(HDC hDC, int dx, int dy, int dw = 0, int dh = 0,
            RECT* cliprect = NULL, int alpha = 0xFF);
    int paint(surface& s, int dx, int dy, int dw, int dh,
            RECT* cliprect = NULL, int alpha = 0xFF);

    #ifndef CGL_PLATFORM_GNUC
    int paint_sse2(HBITMAP hbmp_dst, int dx, int dy, int dw = INT16_MAX, int dh = INT16_MAX,
            vec4i* cliprect = NULL, int alpha = 0xFF);
    #endif

private:
    void rebind();
    void close_bitmap();
};

surface::surface():m_device(NULL), m_hbmp(NULL)
{
    memset(&m_bmp, 0, sizeof(BITMAP));
}
surface::surface(const surface& s):m_device(NULL), m_hbmp(NULL)
{
    memset(&m_bmp, 0, sizeof(BITMAP));
    this->assign(s);
}
surface::surface(int w, int h):m_device(NULL), m_hbmp(NULL)
{
    memset(&m_bmp, 0, sizeof(BITMAP));
    this->create(w, h);
}

surface::~surface()
{
    this->close();
}

HDC surface::handle()const
{
    return m_device;
}

HBITMAP surface::bitmap()const
{
    return m_hbmp;
}

int surface::assign(const BITMAP& bmp)
{
    if(m_bmp.bmWidth != bmp.bmWidth || m_bmp.bmHeight!=bmp.bmHeight || m_bmp.bmBitsPixel!=bmp.bmBitsPixel){
        if(!this->create(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel)){
            return -1;
        }
    }
    SetBitmapBits(m_hbmp, bmp.bmWidthBytes*bmp.bmHeight, bmp.bmBits);
    return -1;
}

int surface::assign(HDC hdc)
{
    HBITMAP hbmp = NULL;
    BITMAP  bmp = {0};
    hbmp = (HBITMAP)GetCurrentObject(hdc, OBJ_BITMAP);
    if(hbmp){
        bm_info(hbmp, bmp);
        if(bmp.bmBits){
            this->assign(bmp);
        }
        else{//通过BitBlt复制
            CGL_ASSERT(bmp.bmBits);
        }
        return 0;
    }
    return -1;
}

int surface::assign(const surface& s)
{
    return assign(s.m_bmp);
}

bool surface::create(int width, int height, int pf)
{
    this->close_bitmap();
    m_hbmp = bm_create(width, height, pf, null);
    if(!m_hbmp)
    {
        return false;
    }
    bm_info(m_hbmp, m_bmp);
    this->rebind();
    return true;
}

int surface::open(const wchar_t* fname)
{
    this->close_bitmap();
    m_hbmp = bm_open(fname);
    if(m_hbmp){
        bm_info(m_hbmp, m_bmp);
        rebind();
        return 0;
    }
    else{
        return -1;
    }
}

int surface::save(const wchar_t* fname)
{
    return bm_save(m_device, m_hbmp, fname);
}

void surface::rebind()
{
    if(!m_device)m_device = CreateCompatibleDC(NULL);
    SelectObject(m_device, m_hbmp);
}

void surface::close_bitmap()
{
    if(m_hbmp){
        //选择空HBITMAP
        if(m_device)SelectObject(m_device, CreateCompatibleBitmap(m_device, 0, 0));
        bm_free(m_hbmp);
    }
}

void surface::close()
{
    close_bitmap();
    if(m_device){
        DeleteDC(m_device); m_device = NULL;
    }
}

int surface::width()const
{
    return m_bmp.bmWidth;
}

int surface::height()const
{
    return m_bmp.bmHeight;
}

int surface::resize(int w, int h, bool copy)
{
    if(w && h){
        if(this->width()!=w || this->height()!=h){
            surface s;
            s.create(w, h);
            if(s.empty()){
                return -1;
            }
            if(m_hbmp && copy){//原图像复制到新图像上面
                this->fastblt(s.handle(), 0, 0, w, h);
            }
            this->swap(s);
            return 0;
        }
    }
    return -1;
}

bool surface::empty()const
{
    return !m_hbmp;
}

void* surface::data()
{
    return m_bmp.bmBits;
}

const void* surface::data()const
{
    return m_bmp.bmBits;
}

void* surface::scanline(int y)
{
    return bm_scan(m_bmp, y);
}

surface& surface::operator=(const surface& s)
{
    this->assign(s);
    return *this;
}

//制作一个复制品
surface surface::clone()const
{
    return surface(*this);
}

void surface::swap(surface& s)
{
    std::swap(m_device, s.m_device);
    std::swap(m_hbmp, s.m_hbmp);
    std::swap(m_bmp, s.m_bmp);
}

surface surface::scale(int w, int h)
{
    surface bmp;
    if(bmp.create(w, h)){
        //bilinear_blt(bmp.bmsurface, NULL, bmsurface, NULL);
        //bilinear_blt(bmp.bmsurface, bmsurface);
    }
    return bmp;
}

int surface::PRGBA()
{
    return bm_PRGBA(m_hbmp);
}


//---------------------------------------------------------------------------
//绘图函数
//
void surface::clear(DWORD color)
{
    HBRUSH brush = CreateSolidBrush(color & 0x00FFFFFF);
    RECT rc = {0, 0, m_bmp.bmWidth, m_bmp.bmHeight};
    FillRect(m_device, &rc, brush);
    DeleteObject(brush);
}

void surface::clear(const color32& color)
{
    bm_clear(m_hbmp, color.value);
}

bool surface::fastblt(HDC hDC, int x, int y, int w, int h)
{
    if(w<0)w = width();
    if(h<0)h = height();
    return BitBlt(hDC, x, y, w, h, handle(), 0, 0, SRCCOPY);
}

int surface::paint(HDC hDC, int dx, int dy, int dw, int dh,
            RECT* cliprect, int alpha)
{
    RECT bmRect = {0, 0, width(), height()};

    if(!dw && !dh)
    {
        dw = width();
        dh = height();
    }
    if(!cliprect)cliprect = &bmRect;

    alphablend(hDC, dx, dy, dw, dh,
        handle(),
        cliprect->left, cliprect->top, cliprect->right, cliprect->bottom,
        alpha);
    return 0;
}

int surface::paint(surface& s, int dx, int dy, int dw, int dh,
        RECT* cliprect, int alpha)
{
    return this->paint(s.handle(), dx, dy, dw, dh, cliprect, alpha);
}

#ifndef CGL_PLATFORM_GNUC
int surface::paint_sse2(HBITMAP hbmp_dst, int dx, int dy, int dw, int dh,
            vec4i* cliprect, int alpha)
{
    BITMAP bm_dst;
    vec4i rc_dst;
    vec4i rc_src(0, 0, width(), height());
    color32* pdst;
    color32* psrc;
    __x128*  p128;

    GetObject(hbmp_dst, sizeof(BITMAP), &bm_dst);
    CGL_ASSERT(bm_dst.bmBitsPixel == 32);
    dw = std::min<int>(dw, bm_dst.bmWidth-dx);
    dh = std::min<int>(dh, bm_dst.bmHeight-dy);
    rc_dst = vec4i(dx, dy, dw, dh);
    if(!cliprect)cliprect = &rc_src;
    rc_dst = rc_dst&rc_src;
    if(rc_dst.reversed())return -1;

    dw = rc_dst.width/4;
    dh = rc_dst.height;

    for(int y=0; y<dh; ++y)
    {
        psrc = (color32*)bm_scan(m_bmp, y);
        pdst = ((color32*)bm_scan(bm_dst, y+dy))+dx;
        for(int x=0; x<dw; ++x)
        {
            //pdst[dx+x].color = blend32(pdst[dx+x].color, psrc[x].color);//24  alphablend119
            //pdst[dx+x].color = blend32_asm(pdst[dx+x].color, psrc[x].color);//42
            //pdst[dx+x].color = alphablend_mmx(pdst[dx+x].color, psrc[x].color);//27
            //pdst[dx+x].color = blend32_mmx(pdst[dx+x].color, psrc[x].color);//25
            cgl::graphics::blend_sse128((__x128*)(pdst)+x, (__x128*)(psrc)+x);// x+=3;//110
        }
        for(int x=dx+dw*4; x<rc_dst.width; ++x)
        {
            pdst[x].value = cgl::graphics::blend32(pdst[x].value, psrc[x-dx].value);
        }
    }
    return 0;
}
#endif

}//end namespace graphics
}//end namespace cgl

//---------------------------------------------------------------------------
#endif //WINDOWS_SURFACE_HPP_20170206134344
