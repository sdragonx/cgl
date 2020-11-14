/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 alphablend.hpp

 2017-02-06 14:38:28

bindfun.BlendOp = AC_SRC_OVER;
bindfun.BlendFlags = 0;//0需要PRGB预乘。1值完美的混合，但是速度很慢
bindfun.SourceConstantAlpha = alpha;
bindfun.AlphaFormat = AC_SRC_ALPHA;

 AlphaBlend支持的最大坐标32767

*/
#ifndef ALPHABLEND_HPP_20170206143828
#define ALPHABLEND_HPP_20170206143828

#include <windows.h>
#include <cgl/graphics/color.hpp>
//#include <cgl/windows/graphics/bitmap.hpp>

//AlphaBlend
#ifdef __BORLANDC__
#pragma comment (lib, "psdk/MSIMG32.lib")
#endif

namespace cgl{
namespace graphics{

extern HBITMAP bm_create(int , int , int , void*, RGBQUAD*);
extern void* bm_scan(HBITMAP, int);
extern void* bm_scan(BITMAP&, int);

//Alpha预乘
int bm_PRGBA(HBITMAP hbmp)
{
    BITMAP bm = {0};
    cgl::graphics::color32 *pcolor;
    GetObject(hbmp, sizeof(BITMAP), &bm);
    if(bm.bmBitsPixel !=32)
    {
        return -1;
    }

    for(int y=0; y<bm.bmHeight; ++y)
    {
        pcolor = static_cast<cgl::graphics::color32*>(bm_scan(bm, y));
        for(int x=0; x<bm.bmWidth; ++x)
        {
            pcolor[x].red   = int(pcolor[x].red)*pcolor[x].alpha/256;
            pcolor[x].green = int(pcolor[x].green)*pcolor[x].alpha/256;
            pcolor[x].blue  = int(pcolor[x].blue)*pcolor[x].alpha/256;
        }
    }
    return 0;
}

//如果目标图像是32位色，绘制过去之后，目标图像也就具有alpha通道
int alphablend(HDC dc_dst, int dx, int dy, int dw, int dh,
        HDC dc_src, int x, int y, int w, int h, int alpha)
{
    BLENDFUNCTION bindfun = {AC_SRC_OVER, 0, (byte_t)(alpha), AC_SRC_ALPHA};
    return ::AlphaBlend(dc_dst, dx, dy, dw, dh, dc_src, x, y, w, h, bindfun);
}

int bm_paint(HDC hDC, int dx, int dy, int dw, int dh,
        HBITMAP bmImage, int x, int y, int w, int h, int alpha)
{
    HDC dcImage = CreateCompatibleDC(hDC);
    HBITMAP bmDefault;
    int result;

    if(!dcImage)
    {
        return -1;
    }

    bmDefault = (HBITMAP)SelectObject(dcImage, bmImage);
    result = alphablend(hDC, dx, dy, dw, dh, dcImage, x, y, w, h, alpha);
    SelectObject(dcImage, bmDefault);
    DeleteDC(dcImage);
    dcImage = NULL;
    return result;
}

int bm_paint(HDC hDC, int dx, int dy, HBITMAP bmImage, int alpha)
{
    BITMAP bm = {0};
    GetObject(bmImage, sizeof(BITMAP), &bm);
    return bm_paint(hDC, dx, dy, bm.bmWidth, bm.bmHeight,
        bmImage, 0, 0, bm.bmWidth, bm.bmHeight, alpha);
}

//绘制半透明矩形
int alpha_rect(HDC dc, int x, int y, int w, int h, uint32_t color, int alpha)
{
    HBITMAP hbmp = NULL;
    HDC dcbmp = NULL;
    int result;
    hbmp = bm_create(1, 1, 32, &color, null);
    if(!hbmp){
        return -1;
    }

    dcbmp = CreateCompatibleDC(NULL);
    if(!dcbmp){
        DeleteObject(hbmp); hbmp = NULL;
        return -1;
    }
    SelectObject(dcbmp, hbmp);
    //reinterpret_cast<cgl::graphics::color32*>(bm_scan(hbmp, 0))[0].color = color;
    result = alphablend (dc, x, y, w, h, dcbmp, 0, 0, 1, 1, alpha);
    DeleteObject(hbmp); hbmp = NULL;
    DeleteDC(dcbmp); dcbmp = NULL;
    return result;
}

inline int alpha_rect(HDC dc, RECT rc, uint32_t color, int alpha)
{
    return alpha_rect(dc, rc.left, rc.top, rc.right, rc.bottom, color, alpha);
}

//渐变色填充 v true = 竖向填充 false = 横向填充
int bm_fill(HDC dc, int x, int y, int w, int h, DWORD color1, DWORD color2, bool v = true)
{
    TRIVERTEX vtx[2];
    GRADIENT_RECT rc = {0, 1};
    vtx[0].Alpha = (color1>>16)&0xFF00;
    vtx[0].Blue  = GetBValue(color1)<<8;
    vtx[0].Green = GetGValue(color1)<<8;
    vtx[0].Red   = GetRValue(color1)<<8;
    vtx[0].x = x;
    vtx[0].y = y;

    vtx[1].Alpha = (color2>>16)&0xFF00;
    vtx[1].Blue  = GetBValue(color2)<<8;
    vtx[1].Green = GetGValue(color2)<<8;
    vtx[1].Red   = GetRValue(color2)<<8;
    vtx[1].x = x+w;
    vtx[1].y = y+h;

    return GradientFill(dc, vtx, 2, &rc, 1, v ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
}

}//end namespace graphics
}//end namespace cgl

#endif //ALPHABLEND_HPP_20170206143828
