/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 canvas.hpp

 2018-09-01 13:10:43

*/
#ifndef CANVAS_HPP_20180901131043
#define CANVAS_HPP_20180901131043

#include <vcl.h>

#include <cgl/public.h>
#include <cgl/math.hpp>
#include <cgl/vec_utils.hpp>
#include <cgl/graphics/gdi/gdi.hpp>

namespace cgl{
namespace vcl{

class TAutoCanvas
{
    alone_ptr<TControlCanvas> m_cc;

public:
    TAutoCanvas() : m_cc(new TControlCanvas) { }
    TAutoCanvas(TControl* obj) : m_cc(new TControlCanvas)
    {
        m_cc->Control = obj;
    }

    TCanvas* operator->()
    {
        return m_cc.get();
    }

    __property TControl* Control={read=getControl};
    __property TCanvas* Handle={read=getHandle};

private:
    TControl* getControl()
    {
        return m_cc->Control;
    }
    TCanvas* getHandle()
    {
        return m_cc.get();
    }
};

int offset_canvas(TCanvas* canvas, int x, int y)
{
    int backup_graphics_mode = SetGraphicsMode(canvas->Handle, GM_ADVANCED);
    /*
    graphics::gdimat mat;
    mat.identity();
    mat.translate(x, y);
    mat.use(canvas->Handle);
    /*/

    XFORM mat;
    mat.eM11 = 1.0f;
    mat.eM12 = 0.0f;
    mat.eM21 = 0.0f;
    mat.eM22 = 1.0f;
    mat.eDx  = x;
    mat.eDy  = y;
    SetWorldTransform(canvas->Handle, &mat);
    //*/

    return backup_graphics_mode;
}

void restore_canvas(TCanvas* canvas, int backup_graphics_mode)
{
    XFORM mat;
    mat.eM11 = 1.0f;
    mat.eM12 = 0.0f;
    mat.eM21 = 0.0f;
    mat.eM22 = 1.0f;
    mat.eDx  = 0;
    mat.eDy  = 0;
    SetWorldTransform(canvas->Handle, &mat);

    SetGraphicsMode(canvas->Handle, backup_graphics_mode);
}

void canvas_clear(TCanvas* canvas, TColor color)
{
    canvas->Brush->Style = bsSolid;
    canvas->Brush->Color = color;
    canvas->FillRect(canvas->ClipRect);
}

void canvas_clear(TControl* obj, TColor color)
{
    TAutoCanvas canvas(obj);
    canvas->Brush->Style = bsSolid;
    canvas->Brush->Color = color;
    canvas->FillRect(obj->ClientRect);
}


void draw_point(TCanvas* dc, int x, int y, int size = 4)
{
    dc->Ellipse(x - size, y - size, x + size, y + size);
}

//void copy_screen(TCanvas* canvas,

//绘制十字坐标系
void draw_coordsystem(TCanvas* dc, int left, int top, int right, int bottom)
{
    //graphics::draw_coordsystem(dc->Handle, left, top, right, bottom);
    dc->Pen->Color = clBlack;
    dc->Pen->Style = psDot;
    TPenMode pen_mode = dc->Pen->Mode;
    dc->Pen->Mode = pmNot;
    dc->Pen->Width = 2;

    dc->MoveTo(left, top + (bottom - top) / 2);
    dc->LineTo(right, top + (bottom - top) / 2);

    dc->MoveTo(left + (right - left) / 2, top);
    dc->LineTo(left + (right - left) / 2, bottom);

    dc->Pen->Mode = pen_mode;
}

void draw_coordsystem(TCanvas* dc, TRect rect)
{
    graphics::draw_coordsystem(dc->Handle, rect.left, rect.top, rect.right, rect.bottom);
}

void draw_coordsystem(TControl* obj)
{
    TAutoCanvas canvas(obj);
    draw_coordsystem(canvas.Handle, canvas.Control->ClientRect);
}

void draw_hotspot(TCanvas* dc, int x, int y, int size = 32)
{
    graphics::draw_hotspot(dc->Handle, x, y, size);
}

void draw_line(TCanvas* dc, int x1, int y1, int x2, int y2)
{
    dc->MoveTo(x1, y1);
    dc->LineTo(x2, y2);
}

void draw_line(TCanvas* dc, int x1, int y1, int x2, int y2, TColor color)
{
    dc->Pen->Color = color;
    dc->MoveTo(x1, y1);
    dc->LineTo(x2, y2);
}

void draw_line(TCanvas* dc, const vec2i& p1, const vec2i& p2)
{
    return draw_line(dc, p1.x, p1.y, p2.x, p2.y);
}

void draw_line(TCanvas* dc, const vec2i& p1, const vec2i& p2, TColor color)
{
    return draw_line(dc, p1.x, p1.y, p2.x, p2.y, color);
}

void draw_rect(TCanvas* canvas, const vec4i& v, TColor color)
{
    canvas->Pen->Color = color;
    canvas->Brush->Style = bsClear;
    canvas->Rectangle(v.x, v.y, v.x + v.width, v.y + v.height);
}

//绘制旋转的矩形
void draw_rect(TCanvas* canvas, int x, int y, int size_x, int size_y, float angle, TColor color)
{
    using namespace std;
    angle *= M_RD;
    float sinval = sin(angle);
    float cosval = cos(angle);
    float cosx = size_x * cosval;
    float sinx = size_x * sinval;
    float cosy = size_y * cosval;
    float siny = size_y * sinval;

    int x1 = x + cosx - siny;
    int y1 = y + cosy + sinx;

    canvas->Pen->Color = color;
    canvas->MoveTo(x1, y1);
    canvas->LineTo(x - cosx - siny, y + cosy - sinx);
    canvas->LineTo(x - cosx - -siny, y + -cosy + -sinx);
    canvas->LineTo(x + cosx - -siny, y + -cosy + sinx);
    canvas->LineTo(x1, y1);
}

void draw_rect(TCanvas* canvas, const vec4i& rect, float angle, TColor color)
{
    int w = rect.width / 2;
    int h = rect.height / 2;
    return draw_rect(canvas, rect.x + w, rect.y + h, w, h, angle, color);
}

template<typename T>
void draw_circle(TCanvas* dc, const vec2<T>& c, T r, TColor color)
{
    dc->Pen->Color = color;
    dc->Brush->Style = bsClear;
    dc->Ellipse(c.x - r, c.y - r, c.x + r, c.y + r);
}

void draw_triangle(TCanvas* dc, vec2f a, vec2f b, vec2f c)
{
    dc->MoveTo(a.x, a.y);
    dc->LineTo(b.x, b.y);
    dc->LineTo(c.x, c.y);
    dc->LineTo(a.x, a.y);
}

template<typename T>
void draw_polyline(TCanvas* dc, const std::vector<T>& ls)
{
    graphics::draw_polyline<T>(dc->Handle, ls);
}

template<typename T>
void draw_polygon(TCanvas* dc, const std::vector<T>& ls)
{
    graphics::draw_polygon<T>(dc->Handle, ls);
}

void draw_curve(TCanvas* dc, int x1, int y1, int x2, int y2, int x3, int y3, int steps)
{
    std::vector<vec2f> ls;
    math::conic_bezier(ls, x1, y1, x2, y2, x3, y3, steps);
    draw_polygon(dc, ls);
}

void draw_curve(TCanvas* dc, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int steps)
{
    std::vector<vec2f> ls;
    math::cubic_bezier(ls, x1, y1, x2, y2, x3, y3, x4, y4, steps);
    draw_polygon(dc, ls);
}

//T as TPoint or vec234
template<typename T>
void draw_triangle(TCanvas* g, const T& v1, const T& v2, const T& v3)
{
    g->MoveTo(v1.x, v1.y);
    g->LineTo(v2.x, v2.y);
    g->LineTo(v3.x, v3.y);
    g->LineTo(v1.x, v1.y);
}

void draw_unit(TCanvas* dc, int x, int y, int size, int angle, TColor color, int vision = 0, TColor vision_color = clGray)
{
    TRect rect(x - size, y - size, x + size, y + size);

    TPenStyle ps = dc->Pen->Style;
    dc->Pen->Style = psSolid;
    dc->Pen->Color = color;
    dc->Brush->Style = bsClear;
    dc->Ellipse(rect);

    float vx = cos(M_RD * angle) * size;
    float vy = sin(M_RD * angle) * size;
    dc->MoveTo(x, y);
    dc->LineTo(x + vx, y + vy);

    //绘制视野
    if(vision){
        dc->Pen->Width = 1;
        dc->Pen->Style = psDot;
        dc->Pen->Color = vision_color;
        rect = TRect(x - vision, y - vision, x + vision, y + vision);
        dc->Ellipse(rect);
    }

    dc->Pen->Style = ps;
}

/*
void draw_unit2(int x, int y, int cx, float angle)
{
    int first_x = cos((angle + 5) * M_RD) * cx;
    int first_y = -sin((angle + 5) * M_RD) * cx;

    int x1 = first_x;
    int y1 = first_y;
    int x2, y2;
    for(int i=angle + 5; i < angle + 355; i+=5){
        x2 = cos(i * M_RD) * cx;
        y2 = -sin(i * M_RD) * cx;
        draw_line(x+x1, y+y1, x+x2, y+y2);
        x1 = x2;
        y1 = y2         ;
    }

    x2 = cos((angle + 355) * M_RD) * cx;
    y2 = -sin((angle + 355) * M_RD) * cx;
    draw_line(x+x1, y+y1, x+x2, y+y2);
    x1 = x2; y1 = y2;

    x2 = cos((angle) * M_RD) * cx * 1.1;
    y2 = -sin((angle) * M_RD) * cx * 1.1;
    draw_line(x+x1, y+y1, x+x2, y+y2);
    x1 = x2; y1 = y2;
    draw_line(x+x1, y+y1, x+first_x, y+first_y);
}

void draw_unit3(float x, float y, float r, float angle)
{
    float begin_x = cos(angle * M_RD) * r;
    float begin_y = -sin(angle * M_RD) * r;

    float x1 = r;
    float y1 = 0;
    float x2, y2;
    for(int i=0; i < 90; i += 6){
        x2 = cos(i * M_RD) * r;
        y2 = -sin(i * M_RD) * r;
        draw_line(x + x1, y + y1, x + x2, y + y2);
        draw_line(x - x1, y + y1, x - x2, y + y2);
        draw_line(x - x1, y - y1, x - x2, y - y2);
        draw_line(x + x1, y - y1, x + x2, y - y2);

        x1 = x2;
        y1 = y2;
    }
    draw_line(x + x2, y + y2, x + 0, y - r);
    draw_line(x - x2, y + y2, x + 0, y - r);
    draw_line(x - x2, y - y2, x + 0, y + r);
    draw_line(x + x2, y - y2, x + 0, y + r);

    begin_x *= 1.075;
    begin_y *= 1.075;
    x1 = cos((angle + 3) * M_RD) * r * 1.025;
    y1 = -sin((angle + 3) * M_RD) * r * 1.025;
    x2 = cos((angle - 3) * M_RD) * r * 1.025;
    y2 = -sin((angle - 3)* M_RD) * r * 1.025;
    draw_line(x + begin_x, y + begin_y, x + x1, y + y1);
    draw_line(x + begin_x, y + begin_y, x + x2, y + y2);
    draw_line(x+x1, y+y1, x+x2, y+y2);
}
*/

//绘制刻度
//计算显示刻度单位
void calc_rule(int &min, int &max, float scale)
{
    if(scale < 0.25f){//50
        min = 50;
        max = 500;
    }
    else if(scale < 0.5f){//25
        min = 25;
        max = 200;
    }
    else if(scale < 1.0f){//10
        min = 10;
        max = 100;
    }
    else if(scale < 2.0f){//10像素一个刻度
        min = 5;
        max = 50;
    }
    else if(scale < 4.0f){//5
        min = 2;
        max = 20;
    }
    else{// if(scale < 8.0f){//2.5
        min = 1;
        max = 10;
    }
    //else{//1
    //    min = 2;
    //    max = 20;
    //}
}

//绘制横向标尺
//range     bounds.left => viewport.right
//offset    bounds.left
//scale     缩放量
void draw_rule_x(TCanvas* dc, int range, int offset, float scale)
{
    int min;    //每个刻度代表多少像素
    int max;    //大刻度分割线

    calc_rule(min, max, scale);

    int org = offset / scale;
    int i = -org;
    int x;
    range /= scale;
    i -= min - (org % min); //对齐到min倍数
    //CGL_LOG("横 min %i, max %i, %i   %f", min, max, i, scale);
    for(; i<range; i+=min){
        x = offset + int(i * scale) / 1;
        if(i % max == 0){
            draw_line(dc, x, 0, x, 8);
            dc->TextOut(x + 2, 4, i);
        }
        else{
            draw_line(dc, x, 0, x, 4);
        }
    }
}

void draw_rule_y(TCanvas* dc, int range, int offset, float scale)
{
    int min;    //每个刻度代表多少像素
    int max;    //大刻度分割线

    calc_rule(min, max, scale);

    int org = offset / scale;
    int i = -org;// / min;
    int y;
    range /= scale;
    i -= min - (org % min);
    //CGL_LOG("横 min %i, max %i, %i   %f", min, max, i, scale);
    for(; i<range; i+=min){
        y = offset + int(i * scale) / 1;
        if(i % max == 0){
            draw_line(dc, 0, y, 8, y);
            dc->TextOut(4, y + 0, i);
        }
        else{
            draw_line(dc, 0, y, 4, y);
        }
    }
}

//字体旋转
void rotate_font(TFont* font, int angle)
{
    LOGFONT lf;
    GetObject(font->Handle, sizeof(LOGFONT), &lf);
    lf.lfEscapement = angle * 10;//1/10度
    HFONT f = CreateFontIndirect(&lf);
    font->Handle = f;
}

}//end namespace vcl
}//end namespace cgl

#endif //CANVAS_HPP_20180901131043
