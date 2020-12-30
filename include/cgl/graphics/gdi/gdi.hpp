/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gdi.hpp

 2018-10-10 13:01:40

 2006-7-6 14:24:47	DC中的默认对象
 CreateCompatibleDC(NULL)创建的dc的默认对象
 Pen	= BLACK_PEN
 Brush	= WHITE_BRUSH
 Font	= SYSTEM_FIXED_FONT

win98中不支持 DC_BRUSH、DC_PEN 对象，支持 DEVICE_DEFAULT_FONT、DEFAULT_GUI_FONT 对象

 这两个一样
HDC dc = CreateDC("DISPLAY", NULL, NULL, NULL);
HDC dc = GetDC(GetDesktopWindow());

*/
#ifndef GDI_HPP_20181010130140
#define GDI_HPP_20181010130140

#include <cgl/public.h>
#include <windows.h>

namespace cgl{
namespace graphics{

//
// GDI 通用函数
//

/*
 Stock Logical Objects
#define WHITE_BRUSH         0
#define LTGRAY_BRUSH        1
#define GRAY_BRUSH          2
#define DKGRAY_BRUSH        3
#define BLACK_BRUSH         4
#define NULL_BRUSH          5
#define HOLLOW_BRUSH        NULL_BRUSH
#define WHITE_PEN           6
#define BLACK_PEN           7
#define NULL_PEN            8
#define OEM_FIXED_FONT      10
#define ANSI_FIXED_FONT     11
#define ANSI_VAR_FONT       12
#define SYSTEM_FONT         13
#define DEVICE_DEFAULT_FONT 14
#define DEFAULT_PALETTE     15
#define SYSTEM_FIXED_FONT   16
*/

int object_type(handle_t obj)
{
    return GetObjectType(obj);
}

//删除GDI对象
void delete_object(handle_t obj)
{
    HWND hwnd;
    switch(object_type(obj)){
    case OBJ_PEN:
    case OBJ_BRUSH:
        DeleteObject(obj);
        break;
    case OBJ_DC:
        hwnd = WindowFromDC((HDC)obj);
        ReleaseDC(hwnd, (HDC)obj);
        break;
    //case OBJ_METADC:
    case OBJ_PAL:
    case OBJ_FONT:
    case OBJ_BITMAP:
    case OBJ_REGION:
    case OBJ_METAFILE:
        DeleteObject(obj);
        break;
    case OBJ_MEMDC:
        DeleteDC((HDC)obj);
        break;
    case OBJ_EXTPEN:
        DeleteObject(obj);
        break;
    default:
        CGL_LOG_ERROR("gdi::delete_object : unknown object type.");
        break;
    }
}

//返回内置GDI对象(不需要释放)
handle_t default_object(int type)
{
    switch(type){
    case OBJ_PEN:
        return GetStockObject(BLACK_PEN);
    case OBJ_BRUSH:
        return GetStockObject(WHITE_BRUSH);
    case OBJ_DC:
        break;
    //case OBJ_METADC:
    case OBJ_PAL:
        break;
    case OBJ_FONT:
        return GetStockObject(SYSTEM_FIXED_FONT);
    case OBJ_BITMAP:{
            HDC hdc = GetDC(NULL);
            HBITMAP hbmp = CreateCompatibleBitmap(hdc, 0, 0);
            ReleaseDC(NULL, hdc);
            return hbmp;
        }
    case OBJ_REGION:
    case OBJ_METAFILE:
        break;
    case OBJ_MEMDC:
        break;
    case OBJ_EXTPEN:
        return GetStockObject(BLACK_PEN);
    default:
        break;
    }
    return null;
}

/*
 函数名：IsStockObject

 功能：判断一个HGDIOBJ对象是否是内部GDI对象

 用法：PCTSTR IsStockObject(HGDIOBJ obj)

 参数：obj：GDI对象句柄

 返回：Windows内部GDI对象的名称

  2006-8-14 8:18:04 by sdragon
*/
template<typename T>
PCTSTR IsStockObject(T obj)
{
    if(obj == GetStockObject(WHITE_BRUSH))	return TEXT("WHITE_BRUSH");
    if(obj == GetStockObject(LTGRAY_BRUSH))	return TEXT("LTGRAY_BRUSH");
    if(obj == GetStockObject(GRAY_BRUSH))	return TEXT("GRAY_BRUSH");
    if(obj == GetStockObject(DKGRAY_BRUSH))	return TEXT("DKGRAY_BRUSH");
    if(obj == GetStockObject(BLACK_BRUSH))	return TEXT("BLACK_BRUSH");
    if(obj == GetStockObject(NULL_BRUSH))	return TEXT("NULL_BRUSH");
    if(obj == GetStockObject(WHITE_PEN))	return TEXT("WHITE_PEN");
    if(obj == GetStockObject(BLACK_PEN))	return TEXT("BLACK_PEN");
    if(obj == GetStockObject(NULL_PEN))		return TEXT("NULL_PEN");
    if(obj == GetStockObject(OEM_FIXED_FONT))	return TEXT("OEM_FIXED_FONT");
    if(obj == GetStockObject(ANSI_FIXED_FONT))	return TEXT("ANSI_FIXED_FONT");
    if(obj == GetStockObject(ANSI_VAR_FONT))	return TEXT("ANSI_VAR_FONT");
    if(obj == GetStockObject(SYSTEM_FONT))		return TEXT("SYSTEM_FONT");
    if(obj == GetStockObject(DEVICE_DEFAULT_FONT))	return TEXT("DEVICE_DEFAULT_FONT");
    if(obj == GetStockObject(DEFAULT_PALETTE))		return TEXT("DEFAULT_PALETTE");
    if(obj == GetStockObject(SYSTEM_FIXED_FONT))	return TEXT("SYSTEM_FIXED_FONT");
    #if(WINVER >= 0x0400)
    if(obj == GetStockObject(DEFAULT_GUI_FONT))	return TEXT("DEFAULT_GUI_FONT");
    #endif
    #if (_WIN32_WINNT >= 0x0500)
    if(obj == GetStockObject(DC_BRUSH))	return TEXT("DC_BRUSH");
    if(obj == GetStockObject(DC_PEN))	return TEXT("DC_PEN");
    #endif
    return NULL;
}

/*
 函数名：GetStockObjectIndex

 功能：判断并获得内部GDI对象名称的索引

 用法：int GetStockObjectIndex(HGDIOBJ obj)

 参数：obj：GDI对象句柄

 返回：如果是内部GDI对象，返回内部GDI对象名称的索引
       如果不是内部GDI对象，返回-1

 2006-8-14 8:21:04 by sdragon
*/
template<typename T>
int GetStockObjectIndex(T obj)
{
	if(obj == GetStockObject(WHITE_BRUSH))	return 0;
	if(obj == GetStockObject(LTGRAY_BRUSH))	return 1;
	if(obj == GetStockObject(GRAY_BRUSH))	return 2;
	if(obj == GetStockObject(DKGRAY_BRUSH))	return 3;
	if(obj == GetStockObject(BLACK_BRUSH))	return 4;
	if(obj == GetStockObject(NULL_BRUSH))	return 5;
	if(obj == GetStockObject(WHITE_PEN))	return 6;
	if(obj == GetStockObject(BLACK_PEN))	return 7;
	if(obj == GetStockObject(NULL_PEN))		return 8;
	if(obj == GetStockObject(OEM_FIXED_FONT))	return 10;
	if(obj == GetStockObject(ANSI_FIXED_FONT))	return 11;
	if(obj == GetStockObject(ANSI_VAR_FONT))	return 12;
	if(obj == GetStockObject(SYSTEM_FONT))		return 13;
	if(obj == GetStockObject(DEVICE_DEFAULT_FONT))	return 14;
	if(obj == GetStockObject(DEFAULT_PALETTE))		return 15;
	if(obj == GetStockObject(SYSTEM_FIXED_FONT))	return 16;
	#if(WINVER >= 0x0400)
	if(obj == GetStockObject(DEFAULT_GUI_FONT))	return 17;
	#endif
	#if (_WIN32_WINNT >= 0x0500)
	if(obj == GetStockObject(DC_BRUSH))	return 18;
	if(obj == GetStockObject(DC_PEN))	return 19;
	#endif
	return -1;
}

//
// font
//

int char_width(HDC dc, int ch)
{
    INT n;
    if(GetCharWidth32W(dc, ch, ch, &n)){
        return n;
    }
    return 0;
}

int char_height(HDC dc)
{
    /*
    TEXTMETRICA tm;
    if(GetTextMetricsA(dc, &tm)){
        return tm.tmHeight;
    }
    */
    SIZE size;
    if(GetTextExtentPoint32W(dc, L"", 1, &size)){
        return size.cy;
    }
    return 0;
}

int text_width(HDC dc, const char* str, int length)
{
    SIZE size;
    if(GetTextExtentPoint32A(dc, str, length, &size)){
        return size.cx;
    }
    return 0;
}

int text_width(HDC dc, const wchar_t* str, int length)
{
    SIZE size = {0};
    if(GetTextExtentPoint32W(dc, str, length, &size)){
        return size.cx;
    }
    return 0;
}

RECT dc_bounds(HDC dc)
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

void dc_clear(HDC dc, HBRUSH brush)
{
    RECT rc = dc_bounds(dc);
    FillRect(dc, &rc, brush);
}

void dc_clear(HDC dc, DWORD color)
{
    HBRUSH brush = CreateSolidBrush(color & 0x00FFFFFF);
    RECT rc = dc_bounds(dc);
    FillRect(dc, &rc, brush);
    DeleteObject(brush);
}

//form.scale 用于canvas绘图缩放
void dc_scale(HDC dc, int viewport_width, int viewport_height,
    int x, int y, int width, int height)
{
    SetMapMode(dc, MM_ANISOTROPIC) ;
    SetViewportExtEx (dc, viewport_width, viewport_height, NULL);
    SetWindowOrgEx(dc, x, y, NULL);
    SetWindowExtEx(dc, width, height, NULL);
}

void moveto(HDC dc, int x, int y)
{
    MoveToEx(dc, x, y, NULL);
}

void lineto(HDC dc, int x, int y)
{
    LineTo(dc, x, y);
}

void draw_line(HDC hDC, int x1, int y1, int x2, int y2, DWORD color)
{
    HPEN hPen = (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 1, color));
    MoveToEx(hDC, x1, y1, NULL);
    LineTo(hDC, x2, y2);
    DeleteObject(SelectObject(hDC, hPen));
}

void draw_rect(HDC hDC, int x1, int y1, int x2, int y2, DWORD color, DWORD brush)
{
    HPEN hPen = (HPEN)SelectObject(hDC, CreatePen(PS_SOLID, 1, color));
    HBRUSH hBrush = (HBRUSH)SelectObject(hDC, CreateSolidBrush((DWORD)brush));
    Rectangle(hDC, x1, y1, x2, y2);
    DeleteObject(SelectObject(hDC, hPen));
    DeleteObject(SelectObject(hDC, hBrush));
}

void draw_triangle(HDC dc, vec2f a, vec2f b, vec2f c)
{
    moveto(dc, a.x, a.y);
    lineto(dc, b.x, b.y);
    lineto(dc, c.x, c.y);
    lineto(dc, a.x, a.y);
}

int line_stipple(HDC dc, int x1, int y1, int x2, int y2, int solid, int space, vec2i& offset)
{
    using namespace std;

    const static int bits = 16;
    const static int one = 1 << bits;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << bits) / k;
        y_step = y1 < y2 ? one : -one;
    }
    else{
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? one : -one;
        y_step = (dy << bits) / k;
    }

    x1 <<= bits;
    y1 <<= bits;
    x1 += one >> 1;//四舍五入
    y1 += one >> 1;

    int i = offset.x;
    int j = offset.y;

    int x, y;

    //恢复绘制模式
    if(offset.y && (offset.y & 1)){//solid
        x = x1 >> bits;
        y = y1 >> bits;
        MoveToEx(dc, x, y, 0);
    }
    //恢复绘制进度
    x1 += x_step * offset.x;
    y1 += y_step * offset.x;

    for( ; i < k; ++j){
        x = x1 >> bits;
        y = y1 >> bits;

        if(j & 1){//space mode
            LineTo(dc, x, y);
            x1 += x_step * space;
            y1 += y_step * space;
            i += space;
        }
        else{//solid mode
            MoveToEx(dc, x, y, 0);
            x1 += x_step * solid;
            y1 += y_step * solid;
            i += solid;
        }
    }
    //space不用画，solid要结尾，保证顶点数量为2的倍数
    if(j & 1){
        x = x2;
        y = y2;
        LineTo(dc, x, y);
    }

    //保存绘制模式、进度
    offset.x = i - k;
    offset.y = j;

    return k;
}

void draw_dotline(HDC dc, int x1, int y1, int x2, int y2, int width, int style, DWORD color, int rop2)
{
    HPEN pen_bak = (HPEN)SelectObject(dc, CreatePen(style, width, color));
    int rop2_bak = SetROP2(dc, rop2);
    vec2i offset;
    line_stipple(dc, x1, y1, x2, y2, 4 * width, 4 * width, offset);
    SetROP2(dc, rop2_bak);
    DeleteObject(SelectObject(dc, pen_bak));
}

void draw_coordsystem(HDC dc, int left, int top, int right, int bottom)
{
    HPEN pen_bak = (HPEN)SelectObject(dc, CreatePen(PS_SOLID, 0, 0x0));
    int rop2_bak = SetROP2(dc, R2_NOT);
    vec2i offset;

    int cx = left + (right - left) / 2;
    int cy = top + (bottom - top) / 2;

    line_stipple(dc, left, cy, right, cy, 4, 4, offset);
    line_stipple(dc, cx, top, cx, bottom, 4, 4, offset);

    SetROP2(dc, rop2_bak);
    DeleteObject(SelectObject(dc, pen_bak));
}

void draw_focus_frame(HDC dc, int x, int y, int size)
{
    int n = size / 3;
    MoveToEx(dc, x - size, y - n, null);
    LineTo(dc, x - size, y - n - n);
    LineTo(dc, x - size + n, y - size);
    LineTo(dc, x - n, y - size);

    MoveToEx(dc, x + size, y - n, null);
    LineTo(dc, x + size, y - n - n);
    LineTo(dc, x + size - n, y - size);
    LineTo(dc, x + n, y - size);

    MoveToEx(dc, x - size, y + n, null);
    LineTo(dc, x - size, y + n + n);
    LineTo(dc, x - size + n, y + size);
    LineTo(dc, x - n, y + size);

    MoveToEx(dc, x + size, y + n, null);
    LineTo(dc, x + size, y + n + n);
    LineTo(dc, x + size - n, y + size);
    LineTo(dc, x + n, y + size);
}

void draw_hotspot(HDC dc, int x, int y, int hotspot = 32, bool show_pos = false)
{
    HPEN pen = CreatePen(PS_DOT, 0, 0x0);
    HPEN penbak = (HPEN)SelectObject(dc, pen);
    int rop2_bak = SetROP2(dc, R2_NOT);

    MoveToEx(dc, -INT16_MAX, y, null);
    LineTo(dc, x - hotspot, y);

    MoveToEx(dc, x + hotspot, y, null);
    LineTo(dc, INT16_MAX, y);

    MoveToEx(dc, x, -INT16_MAX, null);
    LineTo(dc, x, y - hotspot);

    MoveToEx(dc, x, y + hotspot, null);
    LineTo(dc, x, INT16_MAX);

    draw_focus_frame(dc, x, y, hotspot);

    SetROP2(dc, rop2_bak);

    SetBkMode(dc, TRANSPARENT);

    if(show_pos){
        wchar_t buf[32] = {0};
        _itow(x, buf, 10);
        SetTextColor(dc, 0xFF000000);
        TextOutW(dc, x + 2, y + hotspot + 2, buf, wcslen(buf));

        _itow(y, buf, 10);
        SetTextColor(dc, 0xFFFFFFFF);
        TextOutW(dc, x + hotspot + 2, y + 2, buf, wcslen(buf));
    }

    DeleteObject(SelectObject(dc, penbak));
}

template<typename T>
void draw_polyline(HDC dc, const std::vector<T>& ls)
{
    if(ls.empty()){
        return ;
    }
    MoveToEx(dc, ls[0].x, ls[0].y, NULL);
    for(size_t i=1; i<ls.size(); ++i){
        LineTo(dc, ls[i].x, ls[i].y);
    }
}

template<typename T>
void draw_polygon(HDC dc, const std::vector<T>& ls)
{
    if(ls.empty()){
        return ;
    }
    MoveToEx(dc, ls[0].x, ls[0].y, NULL);
    for(size_t i=1; i<ls.size(); ++i){
        LineTo(dc, ls[i].x, ls[i].y);
    }
    LineTo(dc, ls[0].x, ls[0].y);
}

}//end namespace graphics
}//end namespace cgl

#endif //GDI_HPP_20181010130140
