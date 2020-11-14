/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 windows_bitmap.hpp

 2017-02-06 13:52:38

 2014-7-26 10:01:39

*/
#ifndef WINDOWS_BITMAP_HPP_20170206135238
#define WINDOWS_BITMAP_HPP_20170206135238

#include <windows.h>
#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/pixel/bilinear_blt.hpp>
#include <cgl/graphics/effect/gaussblur.hpp>
//#include <cgl/graphics/rotate.hpp>
#include <cgl/io.hpp>
#include <cgl/vec.hpp>

#include "alphablend.hpp"

#ifdef CGL_USING_ILIMAGE
#include <cgl/graphics/ilimage.hpp>
#endif

namespace cgl{
namespace graphics{

const uint32_t GDI_ALPHA_MARK = 0x01FFFFFF;

//---------------------------------------------------------------------------
//返回一个1x1单色位图句柄，系统默认不需要删除
HBITMAP bm_default()
{
    //CreateCompatibleBitmap(GetDC(NULL), 0, 0);//GetLastError()确定
    //CreateCompatibleBitmap(CreateCompatibleDC(NULL), 0, 0)//这两个返回的句柄一样
    HDC hdc = GetDC(NULL);
    HBITMAP hbmp = CreateCompatibleBitmap(hdc, 0, 0);
    ReleaseDC(NULL, hdc);
    return hbmp;
}

//---------------------------------------------------------------------------
//创建HBITMAP，height默认正值，如果为负数的话，图像数据是上下颠倒的
HBITMAP bm_create(int width, int height, int pixelbits, void* data = null, RGBQUAD* palette = null)
{
    if(!width || !height){//return default bitmap handle
        return CreateCompatibleBitmap(GetDC(NULL), 0, 0);
    }

    union{
        byte_t buf[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256];
        BITMAPINFO bmpinfo;
    };

    HBITMAP hbmp;
    size_t    size  = sizeof(BITMAPINFOHEADER);
    size_t    palsize = 0;
    LPVOID    bmpData = null;

    switch(pixelbits)
    {
    case 1:
    case 4:
    case 8:
        palsize = sizeof(RGBQUAD)*(1<<pixelbits);
        break;
    //case 15://不支持
    case 16:
    case 24:
    case 32:
        break;
    default:
        return null;
    }

    memset(&bmpinfo, 0, size);
    bmpinfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth    = width;
    bmpinfo.bmiHeader.biHeight   = height;
    bmpinfo.bmiHeader.biPlanes   = 1;
    bmpinfo.bmiHeader.biBitCount = pixelbits;
    bmpinfo.bmiHeader.biClrUsed  = palsize / 4;

    if(palsize && palette){
        memcpy(bmpinfo.bmiColors, palette, palsize);
    }

    hbmp = CreateDIBSection(NULL, &bmpinfo, DIB_RGB_COLORS, &bmpData, NULL, 0);

    if(data && bmpData){
        int bmpDataSize = image_linebytes(width, pixelbits, 4) * height;
        memcpy(bmpData, data, bmpDataSize);
    }

    return hbmp;
}

//创建32位带alpha通道位图
HBITMAP bm_create32(int width, int height, void* data)
{
    HBITMAP hBitmap;
    BITMAPV5HEADER bi;
    void *lpBits = null;

    ZeroMemory(&bi,sizeof(BITMAPV5HEADER));
    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    // The following mask specification specifies a supported 32 BPP
    // alpha format for Windows XP.
    bi.bV5RedMask   = 0x00FF0000;
    bi.bV5GreenMask = 0x0000FF00;
    bi.bV5BlueMask  = 0x000000FF;
    bi.bV5AlphaMask = 0xFF000000;

    // Create the DIB section with an alpha channel.
    hBitmap = CreateDIBSection(GetDC(NULL), (BITMAPINFO *)&bi, DIB_RGB_COLORS, (void **)&lpBits, NULL, (DWORD)0);

    if(lpBits && data){
        memcpy(lpBits, data, width * height * sizeof(RGBQUAD));
    }

    return hBitmap;
}

//---------------------------------------------------------------------------
//打开图片文件
HBITMAP bm_open(const wchar_t* fname)
{
    HBITMAP hbmp = NULL;
    #ifdef CGL_USING_ILIMAGE
    ilimage image;

    image.open(fname);
    if(!image.is_open())
    {//文件打开失败
        image.close();
        return NULL;
    }

    //转换为BGRA像素格式
    image.convert(IL_BGRA);
    if(image.origin() == IL_ORIGIN_UPPER_LEFT){
        image.flip(false, true);
    }

    hbmp = bm_create32(image.width(), image.height(), image.data());
    image.close();
    #else

    //*
    int w, h, channels;
    //加载图片转换成RGBA格式
    memblock<byte_t> buf;
    io::load_file(buf, fname);
    byte_t *img = stbi_load_from_memory(buf.data(), buf.size(), &w, &h, &channels, 4);
    if(img){
        memswap(img, img + w * h * 4, w * 4);
        hbmp = bm_create(w, h, 32, img);
        stbi_image_free(img);
    }

    /*/
    hbmp = (HBITMAP)::LoadImageW(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //*/
    #endif
    return hbmp;
}

//打开缩放的图片
//HBITMAP bm_open(const wchar_t* fname, size_t width, size_t height);

//---------------------------------------------------------------------------
//删除HBITMAP
int bm_free(HBITMAP& hbmp)
{
    DeleteObject(hbmp); hbmp = NULL;
    return GetLastError();
}

//---------------------------------------------------------------------------
//位图保存
//hDC是绑定hbmp的DC
//hDC可以是NULL，但hbmp是未被绑定的位图，不然得不到正确的调色板数据
int bm_save(HDC hDC, HBITMAP hbmp, io::unifile* f)
{
    if(!f){
        return -1;
    }

    HDC memDC = null;
    HBITMAP hDIB = null;

    BITMAP bm;
    GetObject(hbmp, sizeof(bm), &bm);

    if(!hDC){
        memDC = CreateCompatibleDC(0);
        hDC = memDC;
    }
    SelectObject(hDC, hbmp);

    if(!bm.bmBits)//is DDB
    {//DDB to DIB
        hDIB = bm_create(bm.bmWidth, bm.bmHeight, bm.bmBitsPixel, null);
        HDC dibDC = CreateCompatibleDC(0);
        SelectObject(dibDC, hDIB);
        BitBlt(dibDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
        if(memDC)DeleteDC(memDC);
        hDC = memDC = dibDC;
        hbmp = hDIB;
        GetObject(hbmp, sizeof(bm), &bm);
    }

    BITMAPFILEHEADER bmFile;
    BITMAPINFOHEADER bmInfo;
    RGBQUAD pal[256] = {0};
    int palsize = 0;

    //填充bmpinfo结构
    bmInfo.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo.biWidth = bm.bmWidth;
    bmInfo.biHeight = bm.bmHeight;
    bmInfo.biPlanes = bm.bmPlanes;
    bmInfo.biBitCount = bm.bmBitsPixel;
    bmInfo.biCompression = 0;
    bmInfo.biSizeImage = bm.bmWidthBytes*bm.bmHeight;
    bmInfo.biXPelsPerMeter = 0;
    bmInfo.biYPelsPerMeter = 0;
    bmInfo.biClrUsed = bm.bmBitsPixel <= 8 ? 1<<bm.bmBitsPixel : 0;
    bmInfo.biClrImportant = 0;

    //获得调色板数据
    if(bm.bmBitsPixel <= 8){
        palsize = GetDIBColorTable(hDC, 0, 256, pal);
    }

    //填充文件头
    bmFile.bfType = 0x4D42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
    bmFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmInfo.biSizeImage;
    bmFile.bfReserved1 = 0;
    bmFile.bfReserved2 = 0;
    bmFile.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
        + palsize*sizeof(RGBQUAD);

    //写入数据
    f->seek(0, SEEK_SET);
    f->write(&bmFile, sizeof(BITMAPFILEHEADER));
    f->write(&bmInfo, sizeof(BITMAPINFOHEADER));

    //写入调色板数据
    f->write(pal, sizeof(RGBQUAD) * palsize);

    //写入图像数据
    f->write(bm.bmBits, bmInfo.biSizeImage);

    if(hDIB){
        DeleteObject(hDIB), hDIB = null;
    }
    if(memDC){
        DeleteDC(hDC), hDC = null;
    }
    return 0;
}

int bm_save(HDC hDC, HBITMAP hbmp, const char* filename)
{
    io::binfile f;
    f.open(filename, io_create|io_write);
    bm_save(hDC, hbmp, &f);
    f.close();
    return 0;
}

int bm_save(HDC hDC, HBITMAP hbmp, const wchar_t* filename)
{
    io::binfile f;
    f.open(filename, io_create|io_write);
    bm_save(hDC, hbmp, &f);
    f.close();
    return 0;
}

//---------------------------------------------------------------------------
//获取位图数据
int bm_info(HBITMAP hbmp, BITMAP& bmp)
{
    return GetObject(hbmp, sizeof(BITMAP), &bmp);
}

//返回HBITMAP一行的数据指针
void* bm_scan(BITMAP& bm, int y)
{
    return cgl::graphics::image_scanline(bm.bmBits, bm.bmHeight, bm.bmWidthBytes, y);
}

void* bm_scan(HBITMAP hbmp, int y)
{
    BITMAP bmp = {0};
    bm_info(hbmp, bmp);
    return bm_scan(bmp, y);
}

//---------------------------------------------------------------------------
//初始化、清空位图
int bm_clear(HBITMAP hbmp)
{
    BITMAP bm = {0};
    GetObject(hbmp, sizeof(BITMAP), &bm);
    if(bm.bmBits){
        byte_t* pdata;
        for(int y = 0; y<bm.bmHeight; ++y){
            pdata = (byte_t*)bm_scan(bm, y);
            memset(pdata, 0, bm.bmWidthBytes);
        }
    }
    return 0;
}

//用颜色填充HBITMAP
int bm_clear(HBITMAP hbmp, uint32_t color)
{
    BITMAP bm = {0};
    HDC dc = CreateCompatibleDC(0);
    HBRUSH brush = CreateSolidBrush(color);
    RECT rc;

    SelectObject(dc, hbmp);
    bm_info(hbmp, bm);
    rc.left = 0;
    rc.top = 0;
    rc.right = bm.bmWidth;
    rc.bottom = bm.bmHeight;
    FillRect(dc, &rc, brush);
    DeleteObject(brush), brush = 0;
    DeleteDC(dc), dc = null;
    return 0;
}
//---------------------------------------------------------------------------
//bm_copy
HBITMAP bm_clone(HBITMAP hbmp)
{
    BITMAP bmp;
    bm_info(hbmp, bmp);

    HBITMAP hbmp_clone = bm_create(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel, null, null);
    if(!hbmp_clone){
        return null;
    }

    BITMAP bmp_clone;
    bm_info(hbmp_clone, bmp_clone);

    if(bmp.bmBits && bmp_clone.bmBits){
        memcpy(bmp_clone.bmBits, bmp.bmBits, image_linebytes(bmp.bmWidth, bmp.bmBitsPixel, 4) * bmp.bmHeight);
    }

    return hbmp_clone;
}

//---------------------------------------------------------------------------
//bm_convert
HBITMAP bm_convert(HBITMAP hbmp, int pixelformat)
{
    BITMAP bmp;
    bm_info(hbmp, bmp);

    HBITMAP hbmp_clone = bm_create(bmp.bmWidth, bmp.bmHeight, pixelformat, null, null);
    if(!hbmp_clone){
        return null;
    }

    HDC dc = CreateCompatibleDC(0);
    SelectObject(dc, hbmp_clone);

    //...

    return hbmp_clone;
}

//---------------------------------------------------------------------------
//旋转位图，返回一个旋转后的位图句柄
/*
    bm_rotate()
    旋转位图，返回一个旋转后的位图句柄
    这个函数有待优化
    by sdragonx 2:00 2015/2/5
*/

HBITMAP bm_rotate(HBITMAP hbmp, int angle)
{
    const static int bit = 16;
    HBITMAP hdstbmp = NULL;
    BITMAP bmsrc = {0}, bmdst = {0};
    int cosine, sine;
    int src_width, src_height;
    int dst_width, dst_height;
    int px, py;
    int vx, vy;
    byte_t *pdst = NULL, *psrc = NULL;
    int channels;

    GetObject(hbmp, sizeof(BITMAP), &bmsrc);
    if(bmsrc.bmBitsPixel < 8){//不支持低于8位的图像
        return NULL;
    }
    else{//计算通道数量
        channels = bmsrc.bmBitsPixel/8;
    }

    src_width  = bmsrc.bmWidth;
    src_height = bmsrc.bmHeight;

    //计算角度，和sin、cos值
    angle = angle%360;
    cosine = int(cos(M_RD*angle)*65536.0f);
    sine = int(sin(M_RD*angle)*65536.0f);

    /*
    int p1x = int(src_width*cosine)>>bit;
    int p1y = int(src_width*sine)>>bit;
    int p2x = int(src_height*sine)>>bit;
    int p2y = int(src_height*cosine)>>bit;
    */
    //计算目标矩形大小
    dst_width = abs((src_width*cosine)>>bit)+ abs((src_height*sine)>>bit);
    dst_height = abs((src_width*sine)>>bit)+ abs((src_height*cosine)>>bit);

    hdstbmp = bm_create(dst_width, dst_height, bmsrc.bmBitsPixel, null);
    GetObject(hdstbmp, sizeof(BITMAP), &bmdst);

    //计算源像素偏移量
    vx = (-dst_width*cosine - dst_height*sine + (src_width<<bit))/2;
    vy = (dst_width*sine - dst_height*cosine + (src_height<<bit))/2;

    //反向映射
    for(int y=0;y<dst_height;y++)
    {
        pdst = (byte_t*)bm_scan(bmdst, y);
        for(int x=0;x<dst_width;x++)
        {
            px = (x*cosine + y*sine + vx)>>bit;
            py = (y*cosine - x*sine + vy)>>bit;
            if(px>=0 && px<src_width && py>=0 && py<src_height)
            {
                psrc = (byte_t*)bm_scan(bmsrc, py);
                memcpy(pdst, psrc+px*channels, channels);
            }
            else
            {
                memset(pdst, 0, channels);
            }
            pdst += channels;
        }
    }
    return hdstbmp;
}

//---------------------------------------------------------------------------
//双线性取样缩放
int bilinear_blt(HBITMAP hbmp_dst, vec4i rc_dst, HBITMAP hbmp_src, vec4i rc_src)
{
    cgl::graphics::pixelimage img_dst, img_src;
    BITMAP bm;
    GetObject(hbmp_dst, sizeof(BITMAP), &bm);
    img_dst.map(bm);
    GetObject(hbmp_src, sizeof(BITMAP), &bm);
    img_src.map(bm);
    return cgl::graphics::bilinear_blt(img_dst, rc_dst.x, rc_dst.y,
        rc_dst.width, rc_dst.height,
        img_src, rc_src.x, rc_src.y,
        rc_src.width, rc_src.height);
}

int bilinear_blt(HBITMAP hbmp_dst, HBITMAP hbmp_src)
{
    cgl::graphics::pixelimage img_dst, img_src;
    BITMAP bm;
    GetObject(hbmp_dst, sizeof(BITMAP), &bm);
    img_dst.map(bm);
    GetObject(hbmp_src, sizeof(BITMAP), &bm);
    img_src.map(bm);
    return cgl::graphics::bilinear_blt(img_dst, 0, 0, img_dst.width(), img_dst.height(),
        img_src, 0, 0, img_src.width(), img_src.height());
}

//---------------------------------------------------------------------------
//位图缩放, 0 快速  1 双线插值
//返回缩放后的位图句柄
HBITMAP bm_scale(HBITMAP hbmp, int width, int height, int filter = CGL_LINEAR)
{
    HBITMAP hbmpscale = NULL;
    BITMAP bm = {0};
    if(!hbmp)return NULL;
    bm_info(hbmp, bm);
    hbmpscale = bm_create(width, height, bm.bmBitsPixel, null);
    if(hbmpscale)bilinear_blt(hbmpscale, hbmp);
    return hbmpscale;
}

//---------------------------------------------------------------------------

//DrawIconEx绘制的图标不带alpha通道，这个函数可以创建alpha通道
//先使用GDI_ALPHA_MARK(0x01FFFFFF)填充bmp，再使用DrawIconEx绘制图标
//DrawIconEx绘制过的像素alpha为0，然后经过这个函数处理，图像获得正确的alpha通道
int bm_mkalpha(BITMAP &bmp)
{
    color32* pcolor = (color32*)bmp.bmBits;
    for(int i=0; i<bmp.bmWidth*bmp.bmHeight; ++i)
    {
        pcolor[i].alpha -= 1;
        pcolor[i].red   = int(pcolor[i].red)*pcolor[i].alpha/256;
        pcolor[i].green = int(pcolor[i].green)*pcolor[i].alpha/256;
        pcolor[i].blue  = int(pcolor[i].blue)*pcolor[i].alpha/256;
    }
    return 0;
}

inline int bm_mkalpha(HBITMAP hbmp)
{
    BITMAP bmp = {0};
    GetObject(hbmp, sizeof(BITMAP), &bmp);
    return bm_mkalpha(bmp);
}

//---------------------------------------------------------------------------
//drawDIBitmap
int draw_bitmap(HDC dc, int dx, int dy,
    int width,      //图像信息
    int height,
    int pixelformat,
    int stride,
    void* data)
{
    BITMAPINFO bminfo;
    bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bminfo.bmiHeader.biWidth = width;
    bminfo.bmiHeader.biHeight = height;
    bminfo.bmiHeader.biPlanes = 1;
    bminfo.bmiHeader.biBitCount = pixelformat;
    bminfo.bmiHeader.biCompression = BI_RGB;
    bminfo.bmiHeader.biSizeImage = stride * height;
    bminfo.bmiHeader.biClrUsed = 0;
    bminfo.bmiHeader.biClrImportant = 0;

    return SetDIBitsToDevice(dc, dx, dy, width, height, 
        0, 0, 0, height, data, &bminfo, DIB_RGB_COLORS);
}

//调色板模式
int draw_bitmap(HDC dc, int dx, int dy, int width, int height, int pixelformat, int stride, void* data, void* palette = 0)
{
    #pragma pack(push, 1)
    struct{//BMP图像头+256调色板
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD pal[256];
    }bminfo;
    #pragma pack(pop)

    memset(&bminfo, 0, sizeof(bminfo));

    int palsize = 0;
    if(pixelformat <= 8){
        palsize = 1 << pixelformat;
        if(palette){
            memcpy(bminfo.pal, palette, palsize * sizeof(RGBQUAD));
        }
    }

    bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bminfo.bmiHeader.biWidth = width;
    bminfo.bmiHeader.biHeight = height;
    bminfo.bmiHeader.biPlanes = 1;
    bminfo.bmiHeader.biBitCount = pixelformat;
    bminfo.bmiHeader.biCompression = BI_RGB;
    bminfo.bmiHeader.biSizeImage = stride * height;
    bminfo.bmiHeader.biClrUsed = palsize;
    bminfo.bmiHeader.biClrImportant = 0;

    return SetDIBitsToDevice(dc, dx, dy, width, height,
        0, 0, 0,
        height, data,
        (BITMAPINFO*)&bminfo, DIB_RGB_COLORS);
}
//---------------------------------------------------------------------------
//gauss_blur

int gauss_blur(HBITMAP hbmp, float sigma)
{
    BITMAP bm;
    bm_info(hbmp, bm);
    if(bm.bmBits){
        gauss_blur((byte_t*)bm.bmBits, bm.bmWidth, bm.bmHeight, 4, bm.bmWidthBytes, 2.0f);
        return 0;
    }
    return -1;
}
//---------------------------------------------------------------------------
//bm_mask32
//光标或者图标掩码图片，适用于带alpha通道的图片
//

HBITMAP bm_mask32(HBITMAP hbmp)
{
    BITMAP bmp;
    graphics::bm_info(hbmp, bmp);

    HDC dc = GetDC(0);
    HBITMAP hbmp_mask = CreateCompatibleBitmap(dc, bmp.bmWidth, bmp.bmHeight);
    graphics::bm_clear(hbmp_mask, 0x00FFFFFF);
    ReleaseDC(0, dc);

    return hbmp_mask;
}

//创建光标
HCURSOR cs_create(HBITMAP hbmp, int xhotspot, int yhotspot)
{
    BITMAP bmp;
    GetObject(hbmp, sizeof(BITMAP), &bmp);
    if(!bmp.bmBits){
        return NULL;
    }
    if(bmp.bmBitsPixel != 32){
        return NULL;
    }

    HBITMAP hbmp_mask = graphics::bm_mask32(hbmp);

    ICONINFO info = {0};
    info.fIcon = IMAGE_CURSOR;
    info.xHotspot = xhotspot;
    info.yHotspot = yhotspot;
    info.hbmMask  = hbmp_mask;
    info.hbmColor = hbmp;
    HCURSOR cursor = CreateIconIndirect(&info);

    graphics::bm_free(hbmp_mask);
    return cursor;
}

}//end namespace graphics
}//end namespace cgl

#endif //WINDOWS_BITMAP_HPP_20170206135238
