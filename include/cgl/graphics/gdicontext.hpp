/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdicontext.hpp

 2019-06-06 11:30:33

 print(sizeof(dynamic_array<gdimat>));
 print(sizeof(value_stack<gdimat, dynamic_array>));
 print("tinycontext", sizeof(tinycontext));
 print("gdibitmap", sizeof(gdibitmap));
 print("gdicontext", sizeof(gdicontext));    //116
 print(sizeof(softgl));

 print(sizeof(pixelimage));
 print(sizeof(pixellines));

 gdicanvas => gdicontext
 softcontext => soft3d

*/
#ifndef GDICONTEXT_HPP_20190606113033
#define GDICONTEXT_HPP_20190606113033

#include <cgl/public.h>
#include <cgl/vertex.hpp>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/draw_context.hpp>
#include <cgl/std/dynamic_array.hpp>
#include <cgl/graphics/gdi/bitmap.hpp>
#include <cgl/graphics/gdi/alphablend.hpp>
#include <cgl/graphics/gdi/gdi.hpp>
#include <cgl/graphics/gdi/gdiplus.hpp>
#include <cgl/math/matrix/mat4.hpp>

#include "gdi/gdicontext.hpp"
#include "gdi/gdiplus_context.hpp"

namespace cgl{
namespace graphics {

//绘制三角形轮廓
void draw_arrays(HDC dc, const vtx3f* vs, size_t size, int shape, const mat4f& mat)
{
    HPEN oldPen = (HPEN)SelectObject(dc, (HPEN)GetStockObject(DC_PEN));
    HBRUSH oldBrush = (HBRUSH)SelectObject(dc, (HBRUSH)GetStockObject(NULL_BRUSH));
    size_t begin = 0;
    size_t end;
    SetDCPenColor(dc, 0xFF);

    switch(shape){
    case CGL_POINTS:
        for(size_t i=0; i<size; ++i){
            SetPixelV(dc, vs[i].x, vs[i].y, RGB(vs[i].color.r, vs[i].color.g, vs[i].color.b));
        }
        break;
    case CGL_LINES:
    case CGL_LINE_LOOP:
    case CGL_LINE_STRIP:
    case CGL_TRIANGLE_STRIP:
        //print("draw", c.command);
        break;
    case CGL_TRIANGLES:
        end = size;
        for(begin = 0; begin < end; begin += 3){
            MoveToEx(dc, vs[begin].x, vs[begin].y, null);
            LineTo(dc, vs[begin+1].x, vs[begin+1].y);
            LineTo(dc, vs[begin+2].x, vs[begin+2].y);
            LineTo(dc, vs[begin].x, vs[begin].y);
        }
        break;
    case CGL_TRIANGLE_FAN:
        end = size - 1;
        for(begin = 1; begin < end; ++begin){
            MoveToEx(dc, vs[0].x, vs[0].y, null);
            LineTo(dc, vs[begin].x, vs[begin].y);
            LineTo(dc, vs[begin+1].x, vs[begin+1].y);
            LineTo(dc, vs[0].x, vs[0].y);
        }
        break;
    default:
        break;
    }

    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
}

//绘制三角形轮廓
template<typename T>
void draw_arrays(HDC dc, int shape, const vec2<T>* vs, size_t size)
{
    HPEN oldPen = (HPEN)SelectObject(dc, (HPEN)GetStockObject(DC_PEN));
    HBRUSH oldBrush = (HBRUSH)SelectObject(dc, (HBRUSH)GetStockObject(NULL_BRUSH));
    size_t begin = 0;
    size_t end;
    SetDCPenColor(dc, 0xFF);

    switch(shape){
    case CGL_POINTS:
        for(size_t i=0; i<size; ++i){
            //SetPixelV(dc, vs[i].x, vs[i].y, RGB(vs[i].color.r, vs[i].color.g, vs[i].color.b));
        }
        break;
    case CGL_LINES:
    case CGL_LINE_LOOP:
    case CGL_LINE_STRIP:
    case CGL_TRIANGLE_STRIP:
        //print("draw", c.command);
        break;
    case CGL_TRIANGLES:
        end = size;
        for(begin = 0; begin < end; begin += 3){
            MoveToEx(dc, vs[begin].x, vs[begin].y, null);
            LineTo(dc, vs[begin+1].x, vs[begin+1].y);
            LineTo(dc, vs[begin+2].x, vs[begin+2].y);
            LineTo(dc, vs[begin].x, vs[begin].y);
        }
        break;
    case CGL_TRIANGLE_FAN:
        end = size - 1;
        for(begin = 1; begin < end; ++begin){
            MoveToEx(dc, vs[0].x, vs[0].y, null);
            LineTo(dc, vs[begin].x, vs[begin].y);
            LineTo(dc, vs[begin+1].x, vs[begin+1].y);
            LineTo(dc, vs[0].x, vs[0].y);
        }
        break;
    default:
        break;
    }

    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
}

template<typename T>
void draw_arrays(HDC dc, int shape, std::vector< vec2<T> >& vs)
{
    return draw_arrays(dc, shape, &vs[0], vs.size());
}

void draw_elements(HDC dc, int shape, const std::vector<vec2f> vs, const std::vector<int>& ids)
{
    HPEN oldPen = (HPEN)SelectObject(dc, (HPEN)GetStockObject(DC_PEN));
    HBRUSH oldBrush = (HBRUSH)SelectObject(dc, (HBRUSH)GetStockObject(NULL_BRUSH));
    size_t begin = 0;
    size_t end;
    int id;
    SetDCPenColor(dc, 0xFF);

    switch(shape){
    case CGL_POINTS:
        for(size_t i=0; i<ids.size(); ++i){
            id = ids[i];
            SetPixelV(dc, vs[id].x, vs[id].y, 0xFF);
        }
        break;
    case CGL_LINES:
    case CGL_LINE_LOOP:
    case CGL_LINE_STRIP:
    case CGL_TRIANGLE_STRIP:
        //print("draw", c.command);
        break;
    case CGL_TRIANGLES:
        end = ids.size();
        for(begin = 0; begin < end; begin += 3){
            id = ids[begin];     MoveToEx(dc, vs[id].x, vs[id].y, null);
            id = ids[begin + 1]; LineTo(dc, vs[id].x, vs[id].y);
            id = ids[begin + 2]; LineTo(dc, vs[id].x, vs[id].y);
            id = ids[begin];     LineTo(dc, vs[id].x, vs[id].y);
        }
        break;
    case CGL_TRIANGLE_FAN:
//        end = size - 1;
//        for(begin = 1; begin < end; ++begin){
//            MoveToEx(dc, vs[0].x, vs[0].y, null);
//            LineTo(dc, vs[begin].x, vs[begin].y);
//            LineTo(dc, vs[begin+1].x, vs[begin+1].y);
//            LineTo(dc, vs[0].x, vs[0].y);
//        }
        break;
    default:
        break;
    }

    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
}


}//end namespace graphics
}//end namespace cgl



#endif //GDICONTEXT_HPP_20190606113033
