/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 hexview.hpp

 2018-10-10 12:58:40



*/
#ifndef HEXVIEW_HPP_20181010125840
#define HEXVIEW_HPP_20181010125840

#include <cgl/public.h>
#include <cgl/windows/windows.hpp>

namespace cgl{

struct HEXVIEW_STATE
{
    int sx;    //char width
    int sy;    //char height
    int linespace;
    int selstart;
    int selsize;
};

void DrawFrame(HDC hDC, const HEXVIEW_STATE& state)
{
    using namespace cgl::windows::graphics;

    String str;
    TRect client;

    //填充背景
    HWND hwnd = WindowFromDC(hDC);
    GetClientRect(hwnd, &client);//GetBoundsRect
    FillRect(hDC, &client, (HBRUSH)GetStockObject(WHITE_BRUSH));

    //绘制分割线
    gdi::draw_line(hDC, 0, state.sy + state.linespace * 2,
        state.sx * 74 , state.sy + state.linespace*2,
        (DWORD)clSilver);
    gdi::draw_line(hDC, state.sx * 9, 0, state.sx * 9, client.Height(), (DWORD)clSilver);
    gdi::draw_line(hDC, state.sx * 57, 0, state.sx * 57, client.Height(), (DWORD)clSilver);
    gdi::draw_line(hDC, state.sx * 74, 0, state.sx * 74, client.Height(), (DWORD)clSilver);

    //绘制列号
    SetTextColor(hDC, 0x00FF0000);
    SetBkMode(hDC, TRANSPARENT);
    TextOut(hDC, state.sx*1.5, state.linespace, TEXT("Offset"), 6);
    str = TEXT(" 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15");
    str = TEXT(" 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F");
    TextOut(hDC, state.sx*9 + state.sx/2, state.linespace, str.c_str(), str.Length());
}

//绘制一行的数据
void TextLine(HDC hDC, int pos, int line, const byte_t* data, size_t size, const HEXVIEW_STATE& state)
{
    using namespace cgl::windows::graphics;
    const static TCHAR HEX[] = TEXT("0123456789ABCDEF");

    TCHAR text[48] = {0};
    int x = 0;
    //int y = GetPosY(line)+(state.linespace*line);
    int y = state.sy + state.linespace*3 + line * (state.sy + state.linespace);

    pos = line * 16;//处理选择数据
    const byte_t* ch = data;

    #if 1
    //绘制选择区域
    int start = state.selstart, last = state.selstart + state.selsize;
    if(start > last)std::swap(start, last);
    if(start - pos <=15 && last - pos >= 0)
    {
        start  = start > pos ? start - pos : 0;
        last   = last - pos > 15 ? 15 : last - pos;
        last += 1;

        //绘制HEX选择框
        gdi::draw_rect(hDC,
            state.sx*9.5 + state.sx*start*3,
            y,
            state.sx*9.5 + state.sx * last*3 - state.sx,
            y + state.sy, clSkyBlue, clSkyBlue);

        //绘制TEXT选择框
        gdi::draw_rect(hDC,
            state.sx * 57.5 + start * state.sx,
            y,
            state.sx * 57.5 + last * state.sx,
            y + state.sy, clSkyBlue, clSkyBlue);
    }
    #endif

    SetTextColor(hDC, 0);
    SetBkMode(hDC, TRANSPARENT);
    //绘制地址
    _stprintf(text, 8, TEXT("%08X"), line);
    TextOut(hDC, state.sx/2, y, text, 8);

    //绘制HEX字体
    for(size_t i=0; i<size; ++i)
    {
        text[i*3]   = HEX[((*(ch+i))>>4)&0x0F];
        text[i*3+1] = HEX[( *(ch+i))    &0x0F];
        text[i*3+2] = ' ';
    }
    x = state.sx * 9.5;
    TextOut(hDC, x, y, text, 47);

    //绘制TEXT字体
    for(size_t i=0; i<size; ++i)
    {
        text[i] = *(ch+i);
        if(iscntrl(text[i]))text[i]='.';
    }
    x = state.sx * 57.5;
    TextOut(hDC, x, y, text, size);
}

//数据输入，data 16字节对齐
int paint_hexdata(HDC dc, HFONT hFont,
    int x, int y,
    const void* data, size_t size,
    size_t selstart = 0, size_t selsize = 0)
{
    HEXVIEW_STATE state;
    HFONT hOldFont = (HFONT)SelectObject(dc, hFont);
    GetCharWidth32(dc, 'A', 'A', &state.sx);
    state.sy = state.sx * 2;
    state.linespace = 2;
    state.selstart = selstart;
    state.selsize = selsize;

    DrawFrame(dc, state);
    //绘制数据
    const byte_t* pdata = (const byte_t *)data;
    size_t length;
    for(int i=0; size < INT_MAX; ++i)
    {
        length = size >=16 ? 16 : size;
        TextLine(dc, 0, i, pdata, length, state);
        size -= 16;
        pdata += 16;
    }

    SelectObject(dc, hOldFont);
    return 0;
}


}//end namespace cgl

#endif //HEXVIEW_HPP_20181010125840
