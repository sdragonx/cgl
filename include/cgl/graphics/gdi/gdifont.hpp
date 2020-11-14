/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdifont

 2020-02-13 19:46:09

*/
#ifndef GDIFONT_20200213194609
#define GDIFONT_20200213194609

#include <cgl/graphics/font/truetype.hpp>
#include "gdiobj.hpp"

namespace cgl{
namespace graphics{


//获取字体轮廓
int draw_font_outline(HDC dc, HDC source, int x, int y, wchar_t ch, int max_step = 8)
{
    GLYPHMETRICS gm = {0};
    char buffer[1024*8];

    MAT2 mat2;
    mat2.eM11.value = 1;mat2.eM11.fract = 0;
    mat2.eM12.value = 0;mat2.eM12.fract = 0;
    mat2.eM21.value = 0;mat2.eM21.fract = 0;
    mat2.eM22.value = 1;mat2.eM22.fract = 0;

    POINT lastpos;

//    std::vector<vec2f> ps;
//    std::vector<vec2f> spline;
    //math::spline spline;

    POINT curve[32];//这个数量不一定够

    DWORD returnBytes = GetGlyphOutlineW(source, ch, GGO_BEZIER, &gm, sizeof(buffer), buffer, &mat2);
    if(returnBytes != GDI_ERROR){
        TTPOLYGONHEADER* pTTPH = (TTPOLYGONHEADER*)buffer;
        CGL_ASSERT(pTTPH->dwType == TT_POLYGON_TYPE);
        //对闭合路径进行循环
        while(returnBytes > 0){
            MoveToEx(dc, x + pTTPH->pfxStart.x.value, y + pTTPH->pfxStart.y.value, NULL);
            TTPOLYCURVE* pCurrentCurve = (TTPOLYCURVE*)(pTTPH+1);
            int remainBytes = pTTPH->cb - sizeof(TTPOLYGONHEADER);
            while(remainBytes > 0){
                switch(pCurrentCurve->wType){
                case TT_PRIM_LINE:
                    //Canvas->Pen->Color = clRed;
                    for(int i=0; i<pCurrentCurve->cpfx; ++i){
                        LineTo(dc, x + pCurrentCurve->apfx[i].x.value, y + pCurrentCurve->apfx[i].y.value);
                    }
                    break;
                case TT_PRIM_QSPLINE:
                    //Canvas->Pen->Color = clGreen;
                    for(int i=0; i<pCurrentCurve->cpfx; ++i){
                        LineTo(dc, x + pCurrentCurve->apfx[i].x.value, y + pCurrentCurve->apfx[i].y.value);
                    }
                    break;
                case TT_PRIM_CSPLINE:
                    //Canvas->Pen->Color = clBlue;
                    #if 0
                    GetCurrentPositionEx(dc, &lastpos);
                    //error
                    ps.clear();
                    for(int i=0; i<pCurrentCurve->cpfx; ++i){
                        ps.push_back(vec2f(
                            pCurrentCurve->apfx[i].x.value,
                            pCurrentCurve->apfx[i].y.value));
                    }
                    #else
                    for(int i=0; i<pCurrentCurve->cpfx; ++i){
                        curve[i].x = pCurrentCurve->apfx[i].x.value;
                        curve[i].y = pCurrentCurve->apfx[i].y.value;
                    }
                    PolyBezierTo(dc, curve, pCurrentCurve->cpfx);
                    #endif
                    break;
                }
                int count = sizeof(TTPOLYCURVE) + (pCurrentCurve->cpfx-1)*sizeof(POINTFX);
                pCurrentCurve = (TTPOLYCURVE*)((char*)pCurrentCurve + count);
                remainBytes -= count;
            }
            LineTo(dc, x + pTTPH->pfxStart.x.value, y + pTTPH->pfxStart.y.value);
            returnBytes -= pTTPH->cb;
            pTTPH = (TTPOLYGONHEADER*)((char*)pTTPH + pTTPH->cb);
        }
    }
    return 0;
}
/*
//
// gdifont
//

class gdifont : public gdiobj<HFONT, OBJ_FONT, LOGFONT>
{
public:
    gdifont() : gdiobj<HFONT, OBJ_FONT, LOGFONT>()
    {

    }

    int create(const char* name, int size, int charset = GB2312_CHARSET)
    {
        LOGFONTA lf = {0};
        lf.lfHeight = size;
        //lf.lfWidth
        //lf.lfEscapement
        //lf.lfOrientation
        //lf.lfWeight
        //lf.lfItalic
        //lf.lfUnderline
        //lf.lfStrikeOut
        lf.lfCharSet = charset;//GB2312_CHARSET;
        //lf.lfOutPrecision
        //lf.lfClipPrecision
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
        cscpy(lf.lfFaceName, name, sizeof(lf.lfFaceName));
        if(handle)this->dispose();
        const_write(handle) = CreateFontIndirectA(&lf);
        if(handle){
            mode = CGL_ALLOCATE;
            return 0;
        }
        return CGL_ERROR;
    }

    int create(const wchar_t* name, int size, int charset = GB2312_CHARSET)
    {
        //m_dc = CreateCompatibleDC(0);
        HFONT hFont = CreateFontW(
            size, // logical height of font height
            0, // logical average character width
            0, // angle of escapement
            0, // base-line orientation angle
            0, // font weight
            0, // italic attribute flag
            0, // underline attribute flag
            0, // strikeout attribute flag
            charset, // character set identifier
            0, // output precision
            0, // clipping precision
            DEFAULT_QUALITY, // output quality
            DEFAULT_PITCH | FF_SWISS, // pitch and family
            name // pointer to typeface name string
        );
        //SelectObject(m_dc, m_font);

        const_write(handle) = hFont;
        if(handle){
            mode = CGL_ALLOCATE;
            return 0;
        }
        return CGL_ERROR;
    }
};
*/

//
// gdifont_outline
//
// 获得字体点阵信息。最大支持64x64的点阵图
//

class gdifont_outline
{
public:
    GLYPHMETRICS gm;
    MAT2 mat2;//转置矩阵，不用变换
    char buf[64*64];//max 64*64
public:
    gdifont_outline(){
        mat2.eM11.value = 1;mat2.eM11.fract = 0;
        mat2.eM12.value = 0;mat2.eM12.fract = 0;
        mat2.eM21.value = 0;mat2.eM21.fract = 0;
        mat2.eM22.value = 1;mat2.eM22.fract = 0;
    }

    int get_bitmap(HDC dc, wchar_t ch)
    {
        //int n = GetGlyphOutlineW(dc, ch, GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat2);
        GetGlyphOutlineW(dc, ch, GGO_GRAY8_BITMAP, &gm, 64*64, buf, &mat2);
        return 0;
    }

    void paint(HDC dc, int px, int py)
    {
        char* p;
        int c;
        for(size_t y=0; y<gm.gmBlackBoxY; ++y)
        {
            p = buf + y * gm.gmBlackBoxX;
            for(size_t x = 0; x < gm.gmBlackBoxX; ++x){
                c = p[x];
                //if(!c)continue;
                c *= 4;
                if(c > 255)c = 255;
                //SetDCPenColor(dc, RGB(c, c, c));
                //Rectangle(dc, x * 5, y * 5, x * 5 + 5, y * 5 + 5);
                SetPixelV(dc, px + x, py + y, RGB(c, c, c));
            }
        }
    }
};

class gdifont_ttf : public gdiobj<HFONT, OBJ_FONT, LOGFONT>
{
public:
    typedef gdiobj<HFONT, OBJ_FONT, LOGFONT> base_type;
    unistring name;     //字体名称
    unistring filename; //本地字体文件

    HANDLE mem_font;    //内存字体句柄
    memblock<byte_t> fontData; //内存字体数据

public:
    gdifont_ttf() : gdiobj<HFONT, OBJ_FONT, LOGFONT>()
    {
        mem_font = null;
    }

    int create(const char* name, int size, int charset = DEFAULT_CHARSET)
    {
        if(handle){
            base_type::dispose();
        }

        LOGFONTA lf = {0};
        lf.lfHeight = size;
        //lf.lfWidth
        //lf.lfEscapement
        //lf.lfOrientation
        //lf.lfWeight
        //lf.lfItalic
        //lf.lfUnderline
        //lf.lfStrikeOut
        lf.lfCharSet = charset;
        //lf.lfOutPrecision
        //lf.lfClipPrecision
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;
        cscpy(lf.lfFaceName, name, sizeof(lf.lfFaceName));
        const_write(handle) = CreateFontIndirectA(&lf);
        if(handle){
            mode = CGL_ALLOCATE;
            return 0;
        }
        return CGL_ERROR;
    }

    int create(const wchar_t* name, int size, int charset = DEFAULT_CHARSET)
    {
        if(handle){
            base_type::dispose();
        }
        //m_dc = CreateCompatibleDC(0);
        HFONT hFont = CreateFontW(
            size, // logical height of font height
            0, // logical average character width
            0, // angle of escapement
            0, // base-line orientation angle
            0, // font weight
            0, // italic attribute flag
            0, // underline attribute flag
            0, // strikeout attribute flag
            charset, // character set identifier
            0, // output precision
            0, // clipping precision
            DEFAULT_QUALITY, // output quality
            DEFAULT_PITCH | FF_SWISS, // pitch and family
            name // pointer to typeface name string
        );
        const_write(handle) = hFont;
        if(handle){
            mode = CGL_ALLOCATE;
            return 0;
        }
        return CGL_ERROR;
    }

    /*
    //加载字体文件
    int load(const wchar_t* filename, const wchar_t* fontname, int size, int charset = 0)
    {
        using namespace std;

//        if(this->handle()){
//            this->dispose();
//        }


        m_ttfile = filename;
        HANDLE h = 0;// AddFontResourceExW(m_ttfile.c_str(), FR_PRIVATE, 0);

        if(h){
            m_dc = CreateCompatibleDC(0);
            SelectObject(m_dc, h);
            TCHAR text[1024];
            GetTextFace(m_dc, 1024, text);
            //Caption = (text);
        }

        //this->create(fontname, size, charset);

        return 0;
    }
    */

    int load(const wchar_t* filename, int size, int charset = DEFAULT_CHARSET)
    {
        this->dispose();

        io::load_file(fontData, filename);

        DWORD dwFonts = 0;
        //return "SYSTEM"
        mem_font = (HFONT)AddFontMemResourceEx(fontData.data(), fontData.size(), 0, &dwFonts);
        if(mem_font){
            name = GetFontNameFromData(fontData.data(), fontData.size());
            this->create(name.c_str(), size, charset);
        }
        return 0;
    }

    void dispose()
    {
         if(!filename.empty()){
            RemoveFontResourceExW(filename.c_str(), FR_PRIVATE, 0);
            filename.clear();
        }

        if(mem_font){
            RemoveFontMemResourceEx(mem_font);
            mem_font = null;
            fontData.dispose();
        }

        gdiobj<HFONT, OBJ_FONT, LOGFONT>::dispose();
    }

    int width()const
    {
        //return gm.gmBlackBoxX;
        return 0;
    }

    int height()const
    {
        //return gm.gmBlackBoxY;
        return 0;
    }

    int load_glyph(pixelimage& image, wchar_t ch, char_info& info)
    {
        memblock<byte_t> buf;
        GLYPHMETRICS gm;
        MAT2 mat2;
        HDC dc = CreateCompatibleDC(0);
        SelectObject(dc, handle);

        //转置矩阵，不用变换
        mat2.eM11.value = 1;mat2.eM11.fract = 0;
        mat2.eM12.value = 0;mat2.eM12.fract = 0;
        mat2.eM21.value = 0;mat2.eM21.fract = 0;
        mat2.eM22.value = 1;mat2.eM22.fract = 0;

        int size = GetGlyphOutlineW(dc, ch, GGO_GRAY8_BITMAP, &gm, 0, 0, &mat2);
        if(size < 0){//err
            return -1;
        }
        buf.resize(size);
        GetGlyphOutlineW(dc, ch, GGO_GRAY8_BITMAP, &gm, size, buf.data(), &mat2);

        gray256(buf.data(), gm);

        image.create(gm.gmBlackBoxX, gm.gmBlackBoxY, CGL_ALPHA, 1, buf.data());
        image.flip(0, 1);

        info.x = gm.gmptGlyphOrigin.x;
        info.y = gm.gmptGlyphOrigin.y;
        info.next_x = gm.gmCellIncX;
        info.next_y = gm.gmBlackBoxY;
        DeleteDC(dc);
        return 0;
    }

    //64阶灰度转256阶灰度
    void gray256(byte_t *buf, const GLYPHMETRICS& gm)
    {
        byte_t* p = buf;
        size_t stride = (gm.gmBlackBoxX + 3) & ~3;
        for(size_t y=0; y<gm.gmBlackBoxY; ++y){
            for(size_t x = 0; x < gm.gmBlackBoxX; ++x){
                p[x] = byte_clamp(int(p[x]) * 4);
            }
            p += stride;
        }
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //GDIFONT_20200213194609
