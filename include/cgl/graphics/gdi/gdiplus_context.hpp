/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdiplus_context.hpp

 2020-03-20 19:21:23

*/
#ifndef GDIPLUS_CONTEXT_HPP_20200320192123
#define GDIPLUS_CONTEXT_HPP_20200320192123

#include <cgl/public.h>
#include <cgl/graphics/context.hpp>
#include <cgl/graphics/path.hpp>
#include <cgl/math/matrix/mat2.hpp>
#include <cgl/memory.hpp>
#include <cgl/stack.hpp>
#include <cgl/vec.hpp>
#include "gdiplus.hpp"

namespace cgl{

#ifdef CGL_PLATFORM_VCL
namespace vcl{
extern HDC GetHDC(TComponent* obj);
extern HWND GetHWND(TComponent* obj);
}
#endif

namespace graphics{
namespace gdiplus{

//
// gdipen
//

class gdipen
{
public:
    Gdiplus::Pen * handle;

public:
    gdipen() : handle(new Gdiplus::Pen(Gdiplus::Color::Black))
    {

    }

    ~gdipen()
    {
        safe_delete(handle);
    }

    uint32_t getColor()
    {
        Gdiplus::Color c;
        handle->GetColor(&c);
        return c.GetValue();
    }

    void setColor(vec4ub color)
    {
        handle->SetColor(Gdiplus::Color(color.a, color.r, color.g, color.b));
    }

    void setColor(uint32_t color)
    {
        handle->SetColor(Gdiplus::Color(color));
    }

    float getWidth()
    {
        return handle->GetWidth();
    }

    void setWidth(float width)
    {
        handle->SetWidth(width);
    }

    void setDash(float *p, size_t size)
    {
        if(p){
            handle->SetDashStyle(Gdiplus::DashStyleCustom);
            handle->SetDashPattern(p, size);
        }
        else{
            handle->SetDashStyle(Gdiplus::DashStyleSolid);
        }
    }

    void setDash(std::vector<float>& dash)
    {
        setDash(&dash[0], dash.size());
    }

    __cgl_property(gdipen, uint32_t, color, getColor, setColor);
    __cgl_property(gdipen, float, width, getColor, setColor);
};

//
// gdibrush
//

class gdibrush
{
public:
    Gdiplus::SolidBrush* handle;

public:
    gdibrush() : handle(new Gdiplus::SolidBrush(0))
    {
    }

    ~gdibrush()
    {
        safe_delete(handle);
    }

    uint32_t getColor()
    {
        Gdiplus::Color c;
        handle->GetColor(&c);
        return c.GetValue();
    }

    void setColor(uint32_t color)
    {
        handle->SetColor(Gdiplus::Color(color));
    }

    void setColor(const Gdiplus::Color& color)
    {
        handle->SetColor(color);
    }

    void setColor(vec4ub color)
    {
        handle->SetColor(Gdiplus::Color(color.a, color.r, color.g, color.b));
    }

    __cgl_property(gdibrush, uint32_t, color, getColor, setColor);
};

//
// gdifont
//

//    FontStyleRegular    = 0,
//    FontStyleBold       = 1,
//    FontStyleItalic     = 2,
//    FontStyleUnderline  = 4,
//    FontStyleStrikeout  = 8

class gdifont
{
public:
    Gdiplus::Font* handle;

    gdifont() : handle() { }
    gdifont(HDC hdc) : handle(new Gdiplus::Font(hdc)) { }

    int create(const unistring& name, int size, int style = Gdiplus::FontStyleRegular)
    {
        this->dispose();
        handle = new Gdiplus::Font(name.c_str(), size, style, Gdiplus::UnitPoint, NULL);
        return 0;
    }

    int bind(HDC hdc)
    {
        this->dispose();
        handle = new Gdiplus::Font(hdc);
        return 0;
    }

    void dispose()
    {
        safe_delete(handle);
    }

    unistring name()
    {
        Gdiplus::FontFamily family;
        wchar_t buf[64] = {0};

        handle->GetFamily(&family);
        family.GetFamilyName(buf, 64);
        return buf;
    }

    int size()
    {
        //handle->GetLogFont(g, LOGFONT)
        return handle->GetSize();
    }

    void set_size(int size)
    {

    }

    int style()const
    {
        return handle ? handle->GetStyle() : 0;
    }

};

//
// gdibitmap
//

Gdiplus::PixelFormat gdiplus_pixelformat(int format)
{
    switch(format){
    case CGL_INDEX:
        return PixelFormat8bppIndexed;
    case CGL_MONO:
        return PixelFormat1bppIndexed;
    case CGL_RGB:
    case CGL_BGR:
        return PixelFormat24bppRGB;
    case CGL_RGBA:
    case CGL_BGRA:
        return PixelFormat32bppARGB;
    case CGL_RGB565:
        return PixelFormat16bppRGB565;
    default:
        break;
    }
    return PixelFormatUndefined;
}

Gdiplus::PixelFormat pixelformat_from_pixelbits(int bits)
{
    switch(bits){
    case 1:
        return PixelFormat1bppIndexed;
    case 8:
        return PixelFormat8bppIndexed;
    case 16:
        return PixelFormat16bppRGB565;
    case 24:
        return PixelFormat24bppRGB;
    case 32:
        return PixelFormat32bppARGB;
    default:
        break;
    }
    return PixelFormatUndefined;
}

class gdibitmap
{
public:
    Gdiplus::Bitmap* handle;

public:
    gdibitmap() : handle() { }

    gdibitmap(const unistring& filename) : handle()
    {
        this->open(filename.c_str());
    }

    gdibitmap(HBITMAP bmp, HPALETTE pal) : handle()
    {
        this->assign(bmp, pal);
    }

    gdibitmap(HICON ico) : handle()
    {
        this->assign(ico);
    }

    ~gdibitmap()
    {
        this->dispose();
    }

    int create(int width, int height, int format = CGL_RGBA)
    {
        this->dispose();
        handle = new Gdiplus::Bitmap(width, height, gdiplus_pixelformat(format));
        return 0;
    }

    int open(const wchar_t* filename)
    {
        this->dispose();
        handle = Gdiplus::Bitmap::FromFile(filename);
        return 0;
    }

    int assign(HBITMAP bmp, HPALETTE pal)
    {
        this->dispose();
        handle = new Gdiplus::Bitmap(bmp, pal);
        return 0;
    }

    int assign(HICON ico)
    {
        this->dispose();
        handle = new Gdiplus::Bitmap(ico);
        return 0;
    }

    int map(const void* data, int width, int height, int format, int stride)
    {
        this->dispose();
        handle = new Gdiplus::Bitmap(width, height, stride, gdiplus_pixelformat(format), (byte_t*)data);
        return 0;
    }

    int map(HBITMAP hbmp)
    {
        this->dispose();
        BITMAP bmp;
        GetObject(hbmp, sizeof(BITMAP), &bmp);
        byte_t *data = (byte_t*)bmp.bmBits;
        data += (bmp.bmHeight - 1) * bmp.bmWidthBytes;
        handle = new Gdiplus::Bitmap(
            bmp.bmWidth,
            bmp.bmHeight,
            -bmp.bmWidthBytes,
            pixelformat_from_pixelbits(bmp.bmBitsPixel),
            data);
        return 0;
    }

    void dispose()
    {
        safe_delete(handle);
    }

    int width()const
    {
        return handle->GetWidth();
    }

    int height()const
    {
        return handle->GetHeight();
    }

    //DIB位图得到的是原始地址
    //PNG图片，只能lock unlock
    void* scanline(int y)
    {
        //handle->LockBits(0, )
        /*
        Gdiplus::BitmapData bm;
        Gdiplus::Rect rc(20, 20, 40, 40);
        Gdiplus::Status n = b.LockBits(0, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &bm);
        print(n);
        void *p = bm.Scan0;

        memset(p, 128, 20 * 4 * 200);

        b.UnlockBits(&bm);
        */
        return 0;
    }

};

//
// gdimat
//
/*
class gdimat
{
public:
    Gdiplus::Matrix* m;

public:
    gdimat() : m(new Gdiplus::Matrix)
    {
    }

    gdimat(const mat2x3& mat) : m(mat.m->Clone())
    {
    }

    ~gdimat()
    {
        safe_delete(m);
    }

    gdimat& operator=(const gdimat& mat)
    {
        safe_delete(m);
        m = mat.m->Clone();
        return *this;
    }

    void identity()
    {
        m->Reset();
    }

    void translate(float x, float y)
    {
        m->Translate(x, y, Gdiplus::MatrixOrderAppend);
    }

    void scale(float x, float y)
    {
        m->Scale(x, y, Gdiplus::MatrixOrderAppend);
    }

    void rotate(float a)
    {
        m->Rotate(a, Gdiplus::MatrixOrderAppend);
    }
};
*/

//
//gdipath
//

class gdipath
{
public:
    Gdiplus::GraphicsPath *handle;
    vec2f lastpos;

public:
    gdipath() : handle(new Gdiplus::GraphicsPath) { }
    ~gdipath()
    {
        this->dispose();
    }

    void dispose()
    {
        safe_delete(handle);
    }

    /*
    void move_to(float x, float y)
    {
        lastpos = vec2f(x, y);
    }

    void line_to(float x, float y)
    {
        //vec2f p(x, y);
        //this->draw_line(lastpos, p);
        //lastpos = p;
        lastpos = vec2f(x, y);
        handle->AddPolygon(reinterpret_cast<const Gdiplus::PointF*>(&lastpos), 1);
    }
    */

    template<typename T>
    void draw_line(T x1, T y1, T x2, T y2)
    {
        handle->AddLine(x1, y1, x2, y2);
    }

    template<typename T>
    void draw_line(const vec2<T>& a, const vec2<T>& b)
    {
        this->draw_line(a.x, a.y, b.x, b.y);
    }

    void draw_line(const std::vector<vec2f>& points)
    {
        handle->AddLines(reinterpret_cast<const Gdiplus::PointF*>(&points[0]), points.size());
    }

    void draw_polygon(const std::vector<vec2f>& points)
    {
        handle->AddPolygon(reinterpret_cast<const Gdiplus::PointF*>(&points[0]), points.size());
    }

    void curve(const std::vector<vec2f>& points)
    {
        handle->AddCurve(reinterpret_cast<const Gdiplus::PointF*>(&points[0]), points.size());
    }

    void closed_curve(const std::vector<vec2f>& points)
    {
        handle->AddClosedCurve(reinterpret_cast<const Gdiplus::PointF*>(&points[0]), points.size());
    }
};

//
// gdicontext
//

class gdicontext : public device_context
{
public:
    Gdiplus::Graphics* handle;
    gdipen pen;
    gdibrush brush;
    gdifont font;
    gdibitmap bitmap;
    value_stack<mat2f> matix;

public:
    gdicontext() : handle()
    {
    }

    gdicontext(HDC hdc) : handle(), pen(), brush(), matix()
    {
        this->assign(hdc);
    }

    ~gdicontext()
    {
        this->dispose();
    }

    void assign(HDC hdc)
    {
        this->dispose();
        handle = Gdiplus::Graphics::FromHDC(hdc);
        font.bind(hdc);
        this->effect_level(CGL_QUALITY);
        this->identity();
    }

    void assign(HWND hwnd)
    {
        this->dispose();
        handle = Gdiplus::Graphics::FromHWND(hwnd);
        this->effect_level(CGL_QUALITY);
        this->identity();
    }

    #ifdef CGL_PLATFORM_VCL
    gdicontext(TComponent* obj) : handle()
    {
        this->assign(obj);
    }

    gdicontext(TCanvas* canvas) : handle()
    {
        this->assign(canvas->Handle);
    }

    void assign(TComponent* obj)
    {
        this->dispose();
        HDC hdc = vcl::GetHDC(obj);
        if(hdc){
            return this->assign(hdc);
        }
        HWND hwnd = vcl::GetHWND(obj);
        if(hwnd){
            return this->assign(hwnd);
        }
    }

    void assign(TCanvas* canvas)
    {
        this->assign(canvas->Handle);
    }

    #endif//CGL_PLATFORM_VCL

    void dispose()
    {
//        pen.dispose();
//        brush.dispose();
        safe_delete(handle);
    }

    int effect_level(int level)
    {
        /* InterpolationMode:
         * InterpolationModeInvalid          = QualityModeInvalid,    //插值无效
         * InterpolationModeDefault          = QualityModeDefault,    //指定默认模式
         * InterpolationModeLowQuality       = QualityModeLow,        //指定低质量插值法
         * InterpolationModeHighQuality      = QualityModeHigh,       //指定高质量插值法
         * InterpolationModeBilinear,                                 //指定双线性插值法
         * InterpolationModeBicubic,                                  //指定双三次插值法
         * InterpolationModeNearestNeighbor,                          //指定最临近插值法
         * InterpolationModeHighQualityBilinear,                      //指定高质量双线性插值法
         * InterpolationModeHighQualityBicubic                        //指定高质量双三次插值法
         */

        switch(level){
        case CGL_SPEED:
            handle->SetCompositingMode   ( Gdiplus::CompositingModeSourceOver );        //混合模式
            handle->SetCompositingQuality( Gdiplus::CompositingQualityHighSpeed );      //混合质量
            handle->SetSmoothingMode     ( Gdiplus::SmoothingModeHighSpeed );           //反锯齿
            handle->SetPixelOffsetMode   ( Gdiplus::PixelOffsetModeNone );              //像素偏移模式
            handle->SetInterpolationMode ( Gdiplus::InterpolationModeNearestNeighbor ); //图形缩放质量
            break;
        case CGL_MEDIUM:
            handle->SetCompositingMode   ( Gdiplus::CompositingModeSourceOver );        //混合模式
            handle->SetCompositingQuality( Gdiplus::CompositingQualityHighSpeed );      //混合质量
            handle->SetSmoothingMode     ( Gdiplus::SmoothingModeAntiAlias );           //反锯齿
            handle->SetPixelOffsetMode   ( Gdiplus::PixelOffsetModeNone );              //像素偏移模式
            handle->SetInterpolationMode ( Gdiplus::InterpolationModeBilinear );        //图形缩放质量
            break;
        case CGL_QUALITY:
            handle->SetCompositingMode   ( Gdiplus::CompositingModeSourceOver );        //混合模式
            handle->SetCompositingQuality( Gdiplus::CompositingQualityHighQuality );    //混合质量
            handle->SetSmoothingMode     ( Gdiplus::SmoothingModeAntiAlias );           //反锯齿
            handle->SetPixelOffsetMode   ( Gdiplus::PixelOffsetModeHighQuality );       //像素偏移模式
            handle->SetInterpolationMode ( Gdiplus::InterpolationModeBicubic );         //图形缩放质量
            break;
        }
        return 0;
    }

public://draw
    void clear(int r, int g, int b, int a = 255)
    {
        handle->Clear(Gdiplus::Color(a, r, g, b));
    }

    void clear(uint32_t c = 0xFFFFFFFF)
    {
        handle->Clear(Gdiplus::Color(c));
    }

    void clear(Gdiplus::Color c = Gdiplus::Color::White)
    {
        handle->Clear(c);
    }

    void clear(vec4ub color)
    {
        this->clear(color.r, color.g, color.b);
    }

    int draw_line(float x1, float y1, float x2, float y2)
    {
        return handle->DrawLine(pen.handle, x1, y1, x2, y2);
    }

    int draw_rect(float x, float y, float width, float height)
    {
        return handle->DrawRectangle(pen.handle, x, y, width, height);
    }

    int fill_rect(float x, float y, float width, float height)
    {
        return handle->FillRectangle(brush.handle, x, y, width, height);
    }

    void draw_ellipse(float cx, float cy, float cw, float ch)
    {
        handle->DrawEllipse(pen.handle, cx - cw, cy - ch, cw * 2, ch * 2);
    }

    void fill_ellipse(float cx, float cy, float cw, float ch)
    {
        handle->FillEllipse(brush.handle, cx - cw, cy - ch, cw * 2, ch * 2);
    }

    void draw_circle(float cx, float cy, float r)
    {
        return this->draw_ellipse(cx, cy, r, r);
    }

    void fill_circle(float cx, float cy, float r)
    {
        return this->fill_ellipse(cx, cy, r, r);
    }

    void draw_polyline(const vec2f* p, size_t size)
    {
        handle->DrawLines(pen.handle, (Gdiplus::PointF*)(p), size);
    }

    void draw_polyline(const std::vector<vec2f>& vs)
    {
        this->draw_polyline(&vs[0], vs.size());
    }

    void draw_polygon(const vec2f* p, size_t size)
    {
        handle->DrawPolygon(pen.handle, (Gdiplus::PointF*)(p), size);
    }

    void draw_polygon(const std::vector<vec2f>& vs)
    {
        this->draw_polygon(&vs[0], vs.size());
    }

    void fill_polygon(const vec2f* p, size_t size)
    {
        handle->FillPolygon(brush.handle, (Gdiplus::PointF*)(p), size);
    }

    void fill_polygon(const std::vector<vec2f>& vs)
    {
        this->fill_polygon(&vs[0], vs.size());
    }

    void draw_path(Gdiplus::GraphicsPath& path)
    {
        handle->DrawPath(pen.handle, &path);
    }

    void fill_path(Gdiplus::GraphicsPath& path)
    {
        handle->FillPath(brush.handle, &path);
    }

    int draw_image(Gdiplus::Image* image, int x, int y, int width, int height)
    {
        return handle->DrawImage(image, x, y, width, height);
    }

    int draw_image(Gdiplus::Image* image, float x, float y, float width, float height)
    {
        return handle->DrawImage(image, x, y, width, height);
    }

    int draw_image(gdibitmap& bmp, float x, float y, float width, float height)
    {
        return handle->DrawImage(bmp.handle, x, y, width, height);
    }

    //在xy位置绘制图片，缩放，并旋转一个角度
    void point_image(Gdiplus::Image* image, float x, float y, float width, float height, float angle)
    {
        float cx = width / 2;
        float cy = height / 2;
        Gdiplus::Matrix m;
        handle->GetTransform(&m);
        handle->TranslateTransform(x, y);   //移动到当前位置
        handle->RotateTransform(angle);     //旋转
        handle->TranslateTransform(-cx, -cy);//移动到旋转中心
        handle->DrawImage(image, 0.0f, 0.0f, width, height);
        handle->SetTransform(&m);
    }

    void point_image(gdibitmap& bmp, float x, float y, float width, float height, float angle)
    {
        return point_image(bmp.handle, x, y, width, height, angle);
    }

    void point_image(gdibitmap& bmp, float x, float y, float angle)
    {
        return point_image(bmp.handle, x, y, bmp.width(), bmp.height(), angle);
    }

    void draw_text(float x, float y, float width, float height, const unistring& text, int align)
    {
        Gdiplus::StringFormat format;

        Gdiplus::StringAlignment hAlign = Gdiplus::StringAlignmentNear;
        Gdiplus::StringAlignment vAlign = Gdiplus::StringAlignmentNear;
        if((align & CGL_HORIZONTAL) == CGL_HORIZONTAL){
            hAlign = Gdiplus::StringAlignmentCenter;
        }
        else if(align & CGL_RIGHT){
            hAlign = Gdiplus::StringAlignmentFar;
        }

        if((align & CGL_VERTICAL) == CGL_VERTICAL){
            vAlign = Gdiplus::StringAlignmentCenter;
        }
        else if(align & CGL_DOWN){
            vAlign = Gdiplus::StringAlignmentFar;
        }

        format.SetAlignment(hAlign);    //水平对齐
        format.SetLineAlignment(vAlign);//垂直对齐
        Gdiplus::RectF rect(x, y, width, height);
        handle->DrawString(text.c_str(), text.length(),
            font.handle,
            rect,
            &format,
            brush.handle);

    }

    int text_width(const unichar_t* text)
    {
        Gdiplus::SizeF layoutSize(FLT_MAX, FLT_MAX);
        Gdiplus::SizeF size;
        handle->MeasureString(text, -1, font.handle, layoutSize, NULL, &size);

        return size.Width;
    }

    int text_height(const unichar_t* text)
    {
        Gdiplus::SizeF layoutSize(FLT_MAX, FLT_MAX);
        Gdiplus::SizeF size;
        handle->MeasureString(text, -1, font.handle, layoutSize, NULL, &size);

        return size.Height;
    }

protected:
    virtual int vprint(float x, float y, const unichar_t* text, size_t length)
    {
        Gdiplus::StringFormat fmt;
        if(length == TEXT_MAX)length = wcslen(text);
        return handle->DrawString(text, length, font.handle, Gdiplus::PointF(x, y), &fmt, brush.handle);
    }

public:
    void identity()
    {
        matix.value.identity();
        load_matrix(matix.value);
    }

    void translate(float x, float y)
    {
        matix.value.translate(x, y);
        load_matrix(matix.value);
    }

    void scale(float x, float y)
    {
        matix.value.scale(x, y);
        load_matrix(matix.value);
    }

    void rotate(float a)
    {
        matix.value.rotate(a);
        load_matrix(matix.value);
    }

    virtual void push_matrix()
    {
        matix.push();
    }

    virtual void pop_matrix()
    {
        matix.pop();
        load_matrix(matix.value);
    }

    void load_matrix(const mat2f& mat)
    {
        if(handle){
            Gdiplus::Matrix m(mat[0], mat[1], mat[2], mat[3], mat[4], mat[5]);
            handle->SetTransform(&m);
        }
    }

private:

};

//path_command 转 Gdiplus::GraphicsPath
void transform_path(Gdiplus::GraphicsPath& path, path_command& p)
{
    int j = 0;
    vec2f last;
    path.Reset();
    for(size_t i=0; i<p.commands.size(); ++i){
        switch(p.commands[i]){
        case CGL_MOVE_TO:
            last = p.points[j];
            ++j;
            break;
        case CGL_LINE_TO:
            path.AddLine(last.x, last.y, p.points[j].x, p.points[j].y);
            last = p.points[j];
            ++j;
            break;
        case CGL_CONIC_CURVE_TO:
            path.AddBezier(
                last.x, last.y,
                p.points[j].x, p.points[j].y,
                p.points[j+1].x, p.points[j+1].y,
                p.points[j+1].x, p.points[j+1].y);
            last = p.points[j + 1];
            j += 2;
            break;
        case CGL_CUBIC_CURVE_TO:
            path.AddBezier(
                last.x, last.y,
                p.points[j].x, p.points[j].y,
                p.points[j+1].x, p.points[j+1].y,
                p.points[j+2].x, p.points[j+2].y);
            last = p.points[j + 2];
            j += 3;
            break;
        }
    }
}

}//end namespace gdiplus
}//end namespace graphics
}//end namespace cgl

#endif //GDIPLUS_CONTEXT_HPP_20200320192123
