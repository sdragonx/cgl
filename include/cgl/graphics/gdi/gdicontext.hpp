/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdicontext.hpp

 2020-03-20 19:20:42

*/
#ifndef GDICONTEXT_HPP_20200320192042
#define GDICONTEXT_HPP_20200320192042

#include "gdi.hpp"
#include "gdiplus.hpp"
#include "bitmap.hpp"
#include <cgl/stack.hpp>
#include <cgl/matrix.hpp>
#include <cgl/graphics/context.hpp>

namespace cgl{

#ifdef CGL_PLATFORM_VCL
namespace vcl{
extern HDC GetHDC(TComponent* obj);
extern HWND GetHWND(TComponent* obj);
}
#endif

namespace graphics{
namespace gdi{

//
// gdibase
//
// 主要用途是释放GDI对象
//

template<typename T>
class gdibase
{
public:
    typedef T handle_type;

protected:
    int mode;    //handle句柄信息。CGL_NULL|CGL_ALLOCATE|CGL_BIND

public:
    handle_type const handle;

    gdibase() : mode(), handle(null)
    {

    }

    ~gdibase()
    {
        this->dispose();
    }

    //返回句柄类型
    /*
    #define OBJ_PEN             1
    #define OBJ_BRUSH           2
    #define OBJ_DC              3
    #define OBJ_METADC          4
    #define OBJ_PAL             5
    #define OBJ_FONT            6
    #define OBJ_BITMAP          7
    #define OBJ_REGION          8
    #define OBJ_METAFILE        9
    #define OBJ_MEMDC           10
    #define OBJ_EXTPEN          11
    #define OBJ_ENHMETADC       12
    #define OBJ_ENHMETAFILE     13
    #define OBJ_COLORSPACE      14
    */
    int type()const
    {
        return objectType(handle);
    }

    //释放绑定的句柄，但不删除
    handle_type release()
    {
        handle_type temp = handle;
        const_write(handle) = null;
        mode = CGL_NULL;
        return handle;
    }

    //删除句柄
    virtual void dispose()
    {
        if(mode == CGL_ALLOCATE && handle){
            delete_object(handle);
        }
        const_write(handle) = null;
        mode = CGL_NULL;
    }
};

//
// gdiobj
//

template<typename T, DWORD TYPE, typename INFO>
class gdiobj : public gdibase<T>
{
public:
    typedef gdibase<T> base_type;
    typedef T handle_type;
    typedef INFO info_type;
    const static DWORD object_type = TYPE;

public:
    gdiobj() : gdibase<T>()
    {
    }

    //赋值创建的对象
    int assign(handle_type obj)
    {
        //CGL_ASSERT(object_type == object_type(obj));

        if(base_type::handle){
            this->dispose();
        }

        const_write(base_type::handle) = obj;
        if(base_type::handle){
            base_type::mode = CGL_ALLOCATE;
            return CGL_OK;
        }
        return CGL_ERROR;
    }

    //绑定HDC的对象
    int bind(HDC dc)
    {
        return this->bind((handle_type)GetCurrentObject(dc, object_type));
    }

    //绑定一个对象
    int bind(handle_type obj)
    {
        if(!this->assign(obj)){
            base_type::mode = CGL_BIND;
            return CGL_OK;
        }
        return CGL_ERROR;
    }

    //获得对象信息
    int getInfo(info_type& info)const
    {
        return GetObject(base_type::handle, sizeof(info), &info);
    }
};


//
// gdipen
//

class gdipen : public gdiobj<HPEN, OBJ_PEN, LOGPEN>
{
public:
    gdipen() : gdiobj<HPEN, OBJ_PEN, LOGPEN>()
    {
    }

    gdipen(COLORREF color, int width = 1, int style = PS_SOLID) : gdiobj<HPEN, OBJ_PEN, LOGPEN>()
    {
        this->create(color, width, style);
    }

    //style : PS_SOLID PS_DASH PS_DOT PS_DASHDOT PS_DASHDOTDOT PS_NULL PS_INSIDEFRAME
    int create(COLORREF color, int width = 1, int style = PS_SOLID)
    {
        LOGPEN pen = {0};
        pen.lopnStyle = style;
        pen.lopnWidth.x = width;//y is not used
        pen.lopnColor = color;
        if(handle)this->dispose();
        const_write(handle) = CreatePenIndirect(&pen);
        if(handle){
            mode = CGL_ALLOCATE;
            return CGL_OK;
        }
        return CGL_ERROR;
    }
};

//
// gdibrush
//

class gdibrush : public gdiobj<HBRUSH, OBJ_BRUSH, LOGBRUSH>
{
public:
    gdibrush() : gdiobj<HBRUSH, OBJ_BRUSH, LOGBRUSH>()
    {
    }

    gdibrush(COLORREF color) : gdiobj<HBRUSH, OBJ_BRUSH, LOGBRUSH>()
    {
        this->create(color);
    }

    int create(COLORREF color)
    {
        this->dispose();
        HBRUSH brush = CreateSolidBrush(color);
        return this->assign(brush);
    }

    int create(HBITMAP bmp)
    {
        this->dispose();
        return this->assign(CreatePatternBrush(bmp));
    }
};

//
// gdipath
//

//FillRgn
class gdipath : public gdiobj<HRGN, OBJ_REGION, int>
{
public:
    typedef gdipath this_type;

public:
    gdipath() : gdiobj<HRGN, OBJ_REGION, int>()
    {

    }

    int set_rect(int x, int y, int width, int height)
    {
        this->dispose();
        const_write(handle) = CreateRectRgn(x, y, x + width, y + height);
        mode = CGL_ALLOCATE;
        //SetRectRgn(
        return CGL_OK;
    }

    int round_rect(int x, int y, int width, int height, int rx, int ry)
    {
        this->dispose();
        const_write(handle) = CreateRoundRectRgn(x, y, x + width, y + height, rx, ry);
        mode = CGL_ALLOCATE;
        return CGL_OK;
    }

    this_type& operator=(const this_type& other)
    {
        if(!other.handle || handle == other.handle){
            return *this;
        }
        if(!handle){
            set_rect(0, 0, 0, 0);
        }
        CombineRgn(handle, other.handle, NULL, RGN_COPY);
        return *this;
    }

    this_type& operator&=(const this_type& other)
    {
        CombineRgn(handle, handle, other.handle, RGN_AND);
        return *this;
    }

    this_type& operator|=(const this_type& other)
    {
        CombineRgn(handle, handle, other.handle, RGN_OR);
        return *this;
    }

    this_type& operator^=(const this_type& other)
    {
        CombineRgn(handle, handle, other.handle, RGN_XOR);
        return *this;
    }

    this_type& offset(int x, int y)
    {
        OffsetRgn(handle, x, y);
        return *this;
    }

    void SetWindowRgn(HWND hwnd)
    {
        //SetWindowRgn(hwnd, handle, true);
    }

};

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

//
// gdipelette
//

template<typename T>
HPALETTE create_rgb_palette(const T* color, size_t size)
{
    union {
        LOGPALETTE pal;
        byte_t data[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * PALETTE_MAX];
    };

    if(size >= PALETTE_MAX)size = PALETTE_MAX;
    pal.palVersion = 0x300;
    pal.palNumEntries = size;
    for(size_t i=0; i < size; ++i){
        pal.palPalEntry[i].peRed   = color[i].r;
        pal.palPalEntry[i].peGreen = color[i].g;
        pal.palPalEntry[i].peBlue  = color[i].b;
        pal.palPalEntry[i].peFlags = CGL_ALPHA_OPAQUE;
    }
    HPALETTE hPalette = ::CreatePalette(&pal);
    return hPalette;
}

template<typename T>
HPALETTE create_rgba_palette(const T* color, size_t size)
{
    union {
        LOGPALETTE pal;
        byte_t data[sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * PALETTE_MAX];
    };

    if(size >= PALETTE_MAX)size = PALETTE_MAX;
    pal.palVersion = 0x300;
    pal.palNumEntries = size;
    for(size_t i=0; i < size; ++i){
        pal.palPalEntry[i].peRed   = color[i].r;
        pal.palPalEntry[i].peGreen = color[i].g;
        pal.palPalEntry[i].peBlue  = color[i].b;
        pal.palPalEntry[i].peFlags = color[i].a;
    }
    HPALETTE hPalette = ::CreatePalette(&pal);
    return hPalette;
}

class gdipelette : public gdiobj<HPALETTE, OBJ_PAL, LOGPALETTE>
{
public:
    gdipelette() : gdiobj<HPALETTE, OBJ_PAL, LOGPALETTE>()
    {
    }

};

//
// gdibitmap
//

class gdibitmap : public gdiobj<HBITMAP, OBJ_BITMAP, BITMAP>
{
public:
    gdibitmap() : gdiobj<HBITMAP, OBJ_BITMAP, BITMAP>()
    {

    }

    gdibitmap(int width, int height, int pixelbits, RGBQUAD* palette = null) :
        gdiobj<HBITMAP, OBJ_BITMAP, BITMAP>()
    {
        this->create(width, height, pixelbits, palette);
    }

    HBITMAP defbitmap()
    {
        //CreateCompatibleBitmap(GetDC(NULL), 0, 0);//GetLastError()确定
        //CreateCompatibleBitmap(CreateCompatibleDC(NULL), 0, 0)//这两个返回的句柄一样
        HDC hdc = GetDC(NULL);
        HBITMAP hbmp = CreateCompatibleBitmap(hdc, 0, 0);
        ReleaseDC(NULL, hdc);
        return hbmp;
    }

    int create(int width, int height, int pixelbits, RGBQUAD* palette = null)
    {
        handle_type hbmp;
        if(!width || !height){//return default bitmap handle
            hbmp = defbitmap();
            return this->bind(hbmp);
        }

        union{
            byte_t buf[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];
            BITMAPINFO bmpinfo;
        };
        size_t    size  = sizeof(BITMAPINFOHEADER);
        size_t    palsize = 0;

        switch(pixelbits)
        {
        case 1:
        case 4:
        case 8:
            palsize = sizeof(RGBQUAD)*(1<<pixelbits);
            break;
        //case 15://not suppert
        case 16:
        case 24:
        case 32:
            break;
        default:
            return CGL_ERROR;
        }

        memset(&bmpinfo, 0, size);
        bmpinfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
        bmpinfo.bmiHeader.biWidth    = width;
        bmpinfo.bmiHeader.biHeight   = height;
        bmpinfo.bmiHeader.biPlanes   = 1;
        bmpinfo.bmiHeader.biBitCount = pixelbits;
        bmpinfo.bmiHeader.biClrUsed  = palsize / sizeof(RGBQUAD);

        if(palsize && palette){
            memcpy(bmpinfo.bmiColors, palette, palsize);
        }

        hbmp = CreateDIBSection(NULL, &bmpinfo, DIB_RGB_COLORS, NULL/*bmpData*/, NULL, 0);
        return this->assign(hbmp);
    }

    int open(const wchar_t* filename)
    {
        this->dispose();
        const_write(handle) = gdiplus_LoadHBitmap(filename);
        return 0;
    }

    int width()const
    {
        info_type info;
        this->getInfo(info);
        return info.bmWidth;
    }

    int height()const
    {
        info_type info;
        this->getInfo(info);
        return info.bmHeight;
    }
};


//
// gdimat
//
// GDI矩阵运算
//

class gdimat : public XFORM
{
public:
    gdimat()
    {
        identity();
    }

    void identity()
    {
        eM11 = 1.0f; eM12 = 0.0f;
        eM21 = 0.0f; eM22 = 1.0f;
        eDx  = 0.0f; eDy  = 0.0f;
    }

    void translate(float x, float y)
    {
        XFORM mat;
        mat.eM11 = 1.0f; mat.eM12 = 0.0f;
        mat.eM21 = 0.0f; mat.eM22 = 1.0f;
        mat.eDx  = x;
        mat.eDy  = y;
        CombineTransform(this, this, &mat);
    }

    void scale(float x, float y)
    {
        XFORM mat;
        mat.eM11 = x;    mat.eM12 = 0.0f;
        mat.eM21 = 0.0f; mat.eM22 = y;
        mat.eDx  = 0.0f;
        mat.eDy  = 0.0f;
        CombineTransform(this, this, &mat);
    }

    void rotate(float angle)
    {
        using namespace std;
        XFORM mat;
        angle *= M_RD;
        float cosin = cos(angle);
        float sine  = sin(angle);
        mat.eM11 = cosin; mat.eM12 = sine;
        mat.eM21 = -sine; mat.eM22 = cosin;
        mat.eDx  =  0.0f;
        mat.eDy  =  0.0f;
        CombineTransform(this, this, &mat);
    }

    void to_mat4f(mat4f& mat)
    {
        to_mat4f(mat, *this);
    }

    //应用到目标HDC
    void use(HDC dc)
    {
        SetWorldTransform(dc, this);
    }

private:
    void to_mat4f(mat4f& mat, const XFORM& xform)
    {
        mat.assign(
            xform.eM11, xform.eM12, 0, 0,
            xform.eM21, xform.eM22, 0, 0,
            0, 0, 1.0f, 0,
            xform.eDx, xform.eDy, 0, 1.0f);
    }
};

//
// gdiDC
//

class gdicontext : public gdibase<HDC>, public device_context
{
public:
    //vec4ub color;

protected:
    int m_saveDC;                //存储HDC状态
    int backup_graphics_mode;    //储存HDC图像模式
    value_stack<mat2f> m_mat;    //矩阵堆栈

public:
    gdicontext() : gdibase<HDC>(), device_context(), m_saveDC(), backup_graphics_mode(), m_mat()
    {
    }

    gdicontext(HDC dc) : gdibase<HDC>(), device_context(), m_saveDC(), backup_graphics_mode(), m_mat()
    {
        this->bind(dc);
    }

    gdicontext(HWND hwnd) : gdibase<HDC>(), device_context(), m_saveDC(), backup_graphics_mode(), m_mat()
    {
        this->bind(hwnd);
    }

    gdicontext(HBITMAP hbmp) : gdibase<HDC>(), device_context(), m_saveDC(), backup_graphics_mode(), m_mat()
    {
        this->bind(hbmp);
    }

    ~gdicontext()
    {
        this->dispose();
    }

    virtual void dispose()
    {
        if(m_saveDC){
            RestoreDC(handle, m_saveDC);
        }
        this->identity();
        this->end_paint();
        gdibase<HDC>::dispose();
    }

    int assign(HDC dc)
    {
        if(handle)this->dispose();
        const_write(handle) = dc;
        if(handle){
            m_saveDC = SaveDC(handle);
            SetBkMode(handle, TRANSPARENT);
            SelectObject(handle, GetStockObject(DC_PEN));
            SelectObject(handle, GetStockObject(DC_BRUSH));
            mode = CGL_ALLOCATE;
            return 0;
        }
        return CGL_ERROR;
    }

    int bind(HDC dc)
    {
        if(!this->assign(dc)){
            mode = CGL_BIND;
            this->begin_paint();
            return 0;
        }
        return CGL_ERROR;
    }

    int bind(HWND hwnd)
    {
        return this->assign(GetDC(hwnd));
    }

    //这里是创建独立的DC，有问题？
    int bind(HBITMAP hbmp)
    {
        if(handle)this->dispose();
        this->create();
        SelectObject(handle, hbmp);
        on_bind_bitmap(hbmp);
        return 0;
    }

    int bind(const gdibitmap& bmp)
    {
        return this->bind(bmp.handle);
    }

    int create(HDC dc = null)
    {
        if(handle)this->dispose();
        this->assign(::CreateCompatibleDC(dc));
        return 0;
    }

    #ifdef CGL_PLATFORM_VCL
    gdicontext(TComponent* obj) : gdibase<HDC>(), device_context(), m_saveDC(), backup_graphics_mode(), m_mat()
    {
        this->bind(obj);
    }

    int bind(TCanvas* ACanvas)
    {
        return this->bind(ACanvas->Handle);
    }

    int bind(TComponent* obj)
    {
        this->dispose();
        HDC hdc = vcl::GetHDC(obj);
        if(hdc){
            return this->bind(hdc);
        }
        HWND hwnd = vcl::GetHWND(obj);
        if(hwnd){
            return this->bind(hwnd);
        }

        return 0;
    }
    #endif

public://attribute
    static RECT getbounds(HDC dc)
    {
        RECT rc = {0};
        HWND hwnd = WindowFromDC(dc);
        if(hwnd){
            GetClientRect(hwnd, &rc);
        }
        else{
            HBITMAP bmp;
            BITMAP bm;
            bmp = (HBITMAP)GetCurrentObject(dc, OBJ_BITMAP);
            GetObject(bmp, sizeof(BITMAP), &bm);
            rc.right = bm.bmWidth;
            rc.bottom = bm.bmHeight;
        }
        return rc;
    }

    RECT bounds()const
    {
        return getbounds(handle);
    }

    void cliprect(int x, int y, int width, int height)
    {
        HRGN rgn = CreateRectRgn(x, y, x + width, y + height);
        SelectClipRgn(handle, rgn);
        DeleteObject(rgn);
    }

public://drawing
    void fill_mode(int mode)
    {
        if(mode == CGL_LINE || mode == CGL_POINT){
            SelectObject(handle, GetStockObject(DC_PEN));
            SelectObject(handle, GetStockObject(NULL_BRUSH));
        }
        else if(mode == CGL_FILL){
            SelectObject(handle, GetStockObject(NULL_PEN));
            SelectObject(handle, GetStockObject(DC_BRUSH));
        }
    }

    void set_brush_color(COLORREF color)
    {
        SetDCBrushColor(handle, color);
    }

    void clear(COLORREF color)
    {
        fill_mode(CGL_FILL);
        SetDCBrushColor(handle, color);
        RECT rc = this->bounds();
        FillRect(handle, &rc, (HBRUSH)GetCurrentObject(handle, OBJ_BRUSH));
    }

    void draw_point(int x, int y)
    {
        fill_mode(CGL_POINT);
        SetPixelV(handle, x, y, getColor());
    }

    virtual int draw_line(float x1, float y1, float x2, float y2)
    {
        fill_mode(CGL_LINE);
        SetDCPenColor(handle, getColor());
        MoveToEx(handle, round(x1), round(y1), null);
        LineTo(handle, round(x2), round(y2));
        return 0;
    }

    virtual int draw_rect(float x, float y, float width, float height)
    {
        fill_mode(CGL_LINE);
        SetDCPenColor(handle, getColor());
        Rectangle(handle, round(x), round(y), round(x + width), round(y + height));
        return 0;
    }

    virtual int fill_rect(float x, float y, float width, float height)
    {
        RECT rc = {round(x), round(y), round(x + width), round(y + height)};
        SetDCBrushColor(handle, RGB(color.red, color.green, color.blue));
        FillRect(handle, &rc, (HBRUSH)GetCurrentObject(handle, OBJ_BRUSH));
        return 0;
    }

    int draw_rect(const vec4f& rc)
    {
        return this->draw_rect(rc.x, rc.y, rc.width, rc.height);
    }

    int fill_rect(const vec4f& rc)
    {
        return this->draw_rect(rc.x, rc.y, rc.width, rc.height);
    }

    int textout(int x, int y, const char* text, int length)
    {
        SetTextColor(handle, getColor());
        return TextOutA(handle, x, y, text, length);
    }

    int textout(int x, int y, const wchar_t* text, int length)
    {
        SetTextColor(handle, getColor());
        return TextOutW(handle, x, y, text, length);
    }

    int draw(int x, int y, int width, int height,
        HDC dc, int sx = 0, int sy = 0, int mode = SRCCOPY)
    {
        return BitBlt(handle, x, y, width, height, dc, sx, sy, SRCCOPY);
    }

    int draw(int x, int y, int width, int height,
        HBITMAP hbmp, int sx = 0, int sy = 0, int mode = SRCCOPY)
    {
        HDC dc = CreateCompatibleDC(NULL);
        SelectObject(dc, hbmp);
        BitBlt(handle, x, y, width, height, dc, sx, sy, SRCCOPY);
        DeleteDC(dc);
        return 0;
    }

    int alphablend(int x, int y, int width, int height,
        HDC dc, int sx = 0, int sy = 0, int sw = -1, int sh = -1, int alpha = 255)
    {
        RECT rc = getbounds(dc);
        if(sw < 0){
            sw = rc.right - rc.left;
        }
        if(sh < 0){
            sh = rc.bottom - rc.top;
        }
        return graphics::alphablend(handle, x, y, width, height, dc, sx, sy, sw, sh, alpha);
    }

    int alphablend(int x, int y, int width, int height,
        HBITMAP hbmp, int sx = 0, int sy = 0, int sw = -1, int sh = -1, int alpha = 255)
    {
        HDC dc = CreateCompatibleDC(NULL);
        BITMAP bm;

        GetObject(hbmp, sizeof(BITMAP), &bm);

        if(sw < 0)sw = bm.bmWidth;
        if(sh < 0)sh = bm.bmHeight;
        graphics::alphablend(handle, x, y, width, height, dc, sx, sy, sw, sh, alpha);
        DeleteDC(dc);
        return 0;
    }

    int paint(HDC dc, int x, int y, int width, int height)
    {
        return BitBlt(dc, x, y, width, height, handle, 0, 0, SRCCOPY);
    }

public://matrix
    virtual void identity()
    {
        m_mat.value.identity();
        update_matrix();
    }

    virtual void rotate(float angle, float x = 0.0f, float y = 0.0f, float z = 0.0f)
    {
        m_mat.value.rotate(angle);
        update_matrix();
    }

    virtual void translate(float x, float y, float z = 0.0f)
    {
        m_mat.value.translate(x, y);
        update_matrix();
    }

    virtual void scale(float x, float y, float z = 0.0f)
    {
        m_mat.value.scale(x, y);
        update_matrix();
    }

    virtual void push_matrix()
    {
        m_mat.push();
    }

    virtual void pop_matrix()
    {
        m_mat.pop();
        update_matrix();
    }

    static void load_matrix(HDC dc, const XFORM& mat)
    {
        SetWorldTransform(dc, &mat);
    }

    void load_matrix(const XFORM& mat)
    {
        SetWorldTransform(handle, &mat);
    }

    void load_matrix(const mat2f& mat)
    {
        XFORM m = { mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]};
        SetWorldTransform(handle, &m);
    }

protected:
    virtual int vprint(float x, float y, const unichar_t* text, size_t length)
    {
        return this->textout(round(x), round(y), text, length);
    }

    template<typename T, DWORD Type, typename Info>
    HGDIOBJ select(gdiobj<T, Type, Info>& obj)
    {
        return SelectObject(handle, obj.handle);
    }

    COLORREF getColor()
    {
        return RGB(color.red, color.green, color.blue);
    }

    void begin_paint()
    {
        //set and backup graphics mode
        backup_graphics_mode = SetGraphicsMode(handle, GM_ADVANCED);
    }

    void end_paint()
    {
        //恢复矩阵变换
        load_matrix(mat2f());

        //restore graphics mode
        SetGraphicsMode(handle, backup_graphics_mode);
    }

    void update_matrix()
    {
        load_matrix(m_mat.value);
    }

    virtual void on_bind_bitmap(HBITMAP)
    {

    }
};

}//end namespace gdi
}//end namespace graphics
}//end namespace cgl

#endif //GDICONTEXT_HPP_20200320192042
