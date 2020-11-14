/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdiplus.h

 2015-02-18 04:23:07

enum InterpolationMode
{
    InterpolationModeInvalid          = QualityModeInvalid,    //插值无效
    InterpolationModeDefault          = QualityModeDefault,    //指定默认模式
    InterpolationModeLowQuality       = QualityModeLow,        //指定低质量插值法
    InterpolationModeHighQuality      = QualityModeHigh,       //指定高质量插值法
    InterpolationModeBilinear,                                 //指定双线性插值法
    InterpolationModeBicubic,                                  //指定双三次插值法
    InterpolationModeNearestNeighbor,                          //指定最临近插值法
    InterpolationModeHighQualityBilinear,                      //指定高质量双线性插值法
    InterpolationModeHighQualityBicubic                        //指定高质量双三次插值法
};
*/
#ifndef GDIPLUS_H_20150218042307
#define GDIPLUS_H_20150218042307

#include <algorithm>
#include <windows.h>
#include <shlwapi.h>    //SHCreateMemStream
#include <vector>
#include <cgl/public.h>

using std::min;
using std::max;

#ifdef CGL_PLATFORM_CBUILDER
#pragma option -w-8022        //hides virtual function
#endif

//#define GDIPVER 0x0110
#include <gdiplus.h>

#ifdef CGL_PLATFORM_CBUILDER
#pragma pop
#endif

#if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)
    #pragma comment(lib, "gdiplus.lib")

    #ifndef CGL_PLATFORM_CB6
        #pragma link "shlwapi.lib"
    #endif
#endif

namespace cgl{
namespace graphics{

//图片格式
static const wchar_t* GDIPLUS_IMAGE_BMP  = L"image/bmp";
static const wchar_t* GDIPLUS_IMAGE_JPG  = L"image/jpeg";
static const wchar_t* GDIPLUS_IMAGE_GIF  = L"image/gif";
static const wchar_t* GDIPLUS_IMAGE_TIFF = L"image/tiff";
static const wchar_t* GDIPLUS_IMAGE_PNG  = L"image/png";

//auto init gdiplus context
class gdiplus_autoinit
{
private:
    ULONG_PTR token;
    Gdiplus::GdiplusStartupInput input;

public:
    gdiplus_autoinit():token(0), input(0)
    {
        Gdiplus::GdiplusStartup(&token, &input, null);
    }
    ~gdiplus_autoinit()
    {
        Gdiplus::GdiplusShutdown(token);
    }
};

CGL_PUBLIC_DECLARE gdiplus_autoinit gdiplus_init = gdiplus_autoinit();

//绘制旋转图片
void gdiplus_drawimage(Gdiplus::Graphics& g, Gdiplus::Image* image,
    int x, int y, int angle)
{
    POINT center;
    center.x = image->GetWidth() / 2;
    center.y = image->GetHeight() / 2;
    int state = g.Save();//保存当前状态
    g.TranslateTransform(x, y);//移动到当前位置
    g.RotateTransform(angle); //旋转
    g.TranslateTransform(-center.x, -center.y);//移动到旋转中心
    g.DrawImage(image, 0, 0);//绘制图像
    g.Restore(state);//恢复绘图状态
}

//加载缩放的图片
Gdiplus::Bitmap* gdiplus_ImageResize(Gdiplus::Image* image, int width, int height)
{
    Gdiplus::Bitmap *pbmp = null;
    if(image && image->GetLastStatus() == Gdiplus::Ok)
    {
        pbmp = new Gdiplus::Bitmap(width, height, image->GetPixelFormat());
        Gdiplus::Graphics g(pbmp);
        g.SetCompositingMode( Gdiplus::CompositingModeSourceCopy );
        g.SetCompositingQuality( Gdiplus::CompositingQualityHighSpeed );
        g.SetPixelOffsetMode( Gdiplus::PixelOffsetModeHighSpeed );
        g.SetSmoothingMode( Gdiplus::SmoothingModeHighSpeed );
        g.SetInterpolationMode( Gdiplus::InterpolationModeBilinear );
            //InterpolationModeHighQuality
            //InterpolationModeInvalid 最快的是这个
        g.DrawImage(image, 0, 0, width, height);
    }
    return pbmp;
}

//加载缩放的图片
Gdiplus::Bitmap* gdiplus_LoadImage(const wchar_t* fname, int width, int height)
{
    Gdiplus::Image image(fname);
    return gdiplus_ImageResize(&image, width, height);
}

//加载内存图片
Gdiplus::Bitmap* gdiplus_LoadFromMemory(const void* data, size_t size)
{
    #ifndef CGL_PLATFORM_CB6
    Gdiplus::Bitmap* bmp = null;
    IStream* pstm = SHCreateMemStream((const byte_t*)data, size);
    if(pstm){
        bmp = Gdiplus::Bitmap::FromStream(pstm);
        pstm->Release();
    }
    return bmp;
    #endif
}

HBITMAP gdiplus_LoadHBitmap(const wchar_t* fname)
{
    HBITMAP hbmp = null;
    Gdiplus::Bitmap bmp(fname);
    if(bmp.GetLastStatus() == Gdiplus::Ok)    {
        bmp.GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hbmp);
    }
    return hbmp;
}

HBITMAP gdiplus_LoadHBitmap(const wchar_t* fname, int width, int height)
{
    HBITMAP hbmp = null;
    Gdiplus::Bitmap *bmp = gdiplus_LoadImage(fname, width, height);
    if(bmp){
        bmp->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hbmp);
        delete bmp, bmp = null;
    }
    return hbmp;
}

int GetImageCLSID(const WCHAR* format, CLSID* pCLSID)
{
    //得到格式为format的图像文件的编码值，访问该格式图像的COM组件的GUID值保存在pCLSID中
    UINT num  = 0;
    UINT size = 0;

    Gdiplus::ImageCodecInfo* pImageCodecInfo = null;
    Gdiplus::GetImageEncodersSize(&num, &size);

    if(size == 0)return -1;    //编码信息不可用

    //分配内存
    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if(!pImageCodecInfo)return -2;    //分配失败

    //获得系统中可用的编码方式的所有信息
    GetImageEncoders(num, size, pImageCodecInfo);

    //在可用编码信息中查找format格式是否被支持
    for(UINT i = 0; i < num; ++i){
        //MimeType：编码方式的具体描述
        if(wcscmp(pImageCodecInfo[i].MimeType, format) == 0){
            *pCLSID = pImageCodecInfo[i].Clsid;
            free(pImageCodecInfo);
            return 0;    //成功
        }
    }

    free(pImageCodecInfo);
    return -3;
}

int gdiplus_SaveImage(Gdiplus::Image* image, const wchar_t* filename, const wchar_t* type)
{
    CLSID id;
    if(!GetImageCLSID(type, &id)){
        return image->Save(filename, &id, null);
    }
    return -1;
}

//图片格式转换
int gdiplus_ImageConvert(const wchar_t* dest, const wchar_t* src, const wchar_t* type)
{
    Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromFile(src);
    if(bmp){
        gdiplus_SaveImage(bmp, dest, type);
        delete bmp, bmp = null;
        return 0;
    }
    return -1;
}

//图片格式大小转换
int gdiplus_ImageResize(const wchar_t* dest, int width, int height, const wchar_t* src, const wchar_t* type)
{
    Gdiplus::Bitmap *bmp = gdiplus_LoadImage(src, width, height);
    if(bmp){
        gdiplus_SaveImage(bmp, dest, type);
        delete bmp, bmp = null;
        return 0;
    }
    return -1;
}

//绘制九宫格
void gdiplus_DrawNineBlock(Gdiplus::Graphics& g, Gdiplus::Image* image,
    Gdiplus::Rect rect, int x1, int x2, int y1, int y2)
{
    int w1 = x1;
    int src_w2 = x2 - x1;
    int w3 = image->GetWidth() - src_w2 - w1;
    int dst_w2 = rect.Width - w3 - w1;

    int h1 = y1;
    int src_h2 = y2 - y1;
    int h3 = image->GetHeight() - src_h2 - h1;
    int dst_h2 = rect.Height - h3 - h1;

    Gdiplus::Rect rc(rect.X + w1, rect.Y, dst_w2, h1);

    //顶部
    g.DrawImage(image, rect.GetLeft(), rect.GetTop(), 0, 0, w1, h1, Gdiplus::UnitPixel);
    g.DrawImage(image, rc, x1, 0, src_w2, h1, Gdiplus::UnitPixel);
    g.DrawImage(image, rect.GetLeft() + w1 + dst_w2, rect.GetTop(), x2, 0, w3, h1, Gdiplus::UnitPixel);

    //中间
    rc.X = rect.X;
    rc.Y = rect.Y + h1;
    rc.Width = w1;
    rc.Height = dst_h2;
    g.DrawImage(image, rc, 0, y1, w1, src_h2, Gdiplus::UnitPixel);
    rc.X = rect.X + w1;
    rc.Width = dst_w2;
    g.DrawImage(image, rc, x1, y1, src_w2, src_h2, Gdiplus::UnitPixel);
    rc.X = rect.X + w1 + dst_w2;
    rc.Width = w3;
    g.DrawImage(image, rc, x2, y1, w3, src_h2, Gdiplus::UnitPixel);

    //底部
    rc.X = rect.X;
    rc.Y = rect.Y + h1 + dst_h2;
    rc.Width = w1;
    rc.Height = h3;
    g.DrawImage(image, rc, 0, y2, x1, h3, Gdiplus::UnitPixel);
    rc.X = rect.X + x1;
    rc.Width = dst_w2;
    g.DrawImage(image, rc, x1, y2, src_w2, h3, Gdiplus::UnitPixel);
    rc.X = rect.X + x1 + dst_w2;
    rc.Width = w3;
    g.DrawImage(image, rc, x2, y2, w3, h3, Gdiplus::UnitPixel);
}

//截屏
void PrintScreen(
    Gdiplus::Graphics& g,
    int x,                  //绘制位置
    int y, 
    int left,               //屏幕矩形
    int top,
    int width,
    int height)
{
	HDC dcScreen = GetDC(GetDesktopWindow());
	HDC dcClient  = CreateCompatibleDC(dcScreen);
	HBITMAP bmpClient = CreateCompatibleBitmap(dcScreen, width, height);
	SelectObject(dcClient, bmpClient);
	BitBlt(dcClient, 0, 0, width, height, GetDC(0), left, top, SRCCOPY);
	Gdiplus::Bitmap *bmp = Gdiplus::Bitmap::FromHBITMAP(bmpClient, NULL);
	g.DrawImage(bmp, x, y);
	delete bmp; bmp = NULL;
	DeleteObject(bmpClient); bmpClient = NULL;
	DeleteDC(dcClient);
	ReleaseDC(GetDesktopWindow(), dcScreen);
}

//#ifndef CGL_PLATFORM_GNUC
#if GDIPVER >= 0x0110

//模糊效果
void effect_blur(Gdiplus::Bitmap* bmp, int x, int y, int width, int height, int radius)
{
    Gdiplus::Blur effect;
    Gdiplus::BlurParams param;
    param.radius = radius;
    param.expandEdge = radius;
    effect.SetParameters(&param);
    RECT rect = {x, y, x + width, y + height};
    bmp->ApplyEffect(&effect, &rect);
}

void effect_invert(Gdiplus::Bitmap* bmp)
{
    Gdiplus::ColorMatrix mat = {
        -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };
    Gdiplus::ColorMatrixEffect effect;
    RECT rect = {0, 0, bmp->GetWidth(), bmp->GetHeight()};
    effect.SetParameters(&mat);
    bmp->ApplyEffect(&effect, &rect);
}
#endif


}//end namespace graphics
}//end namespace cgl

#endif //GDIPLUS_H_20150218042307
/*

bool ImageFromIDResource(UINT nID, LPCTSTR szType, Gdiplus::Bitmap* &pImg)
{
	HRSRC hResource = ::FindResource(0, MAKEINTRESOURCE(nID), szType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(0, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(0, hResource));
	if (!pResourceData)
		return false;

	void* m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				pImg = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (pImg)
				{
					if (pImg->GetLastStatus() != Gdiplus::Ok)
					{
						delete pImg;
						pImg = NULL;
                    }
				}
			}
			::GlobalUnlock(m_hBuffer);
        }
        ::GlobalFree(m_hBuffer);
        m_hBuffer = NULL;
	}
	FreeResource(hResource);
	hResource = NULL;
    return false;
}

//平铺图片
void TComicBox::TiledImage(gdiplus::gdicontext& dc, gdiplus::gdibitmap& bmp)
{
    vec4i bounds = FBounds;
    //移动过到起始位置
    while(0 < bounds.x){
        bounds.x -= bounds.width;
    }
    while(0 < bounds.y){
        bounds.y -= bounds.height;
    }

    //平铺
    //dc.brush.setColor(color::black);
    for(int y=bounds.y; y<FViewPort.height; y+=bounds.height){
        for(int x=bounds.x; x<FViewPort.width; x+=bounds.width){
            dc.draw_image(bmp, x, y, FBounds.width, FBounds.height);
            //dc.print(x, y, (String(x) + ", " + String(y)).c_str());
        }
    }
}


Gdiplus::Bitmap* TBitmapToBitmap(Graphics::TBitmap* bmp)
{
    int stride;
    int pf;

    switch(bmp->PixelFormat){
    case pf24bit:
        stride = -bmp->Width * 3;
        pf = PixelFormat24bppRGB;
        break;
    case pf32bit:
        stride = -bmp->Width * 4;
        pf = PixelFormat32bppRGB;
        break;
    }
    return new Gdiplus::Bitmap(bmp->Width, bmp->Height, stride, pf, (BYTE*)bmp->ScanLine[0]);
}

*/