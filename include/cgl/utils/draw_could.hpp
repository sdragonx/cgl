/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 draw_could.hpp

 2018-11-10 14:47:45

*/
#ifndef DRAW_COULD_HPP_20181110144745
#define DRAW_COULD_HPP_20181110144745

#include <cgl/public.h>

namespace cgl{
namespace windows{

int draw_could(HDC dc, RECT& rect, DWORD color, int size, bool bDraw)
{
    std::vector<int> szbuf;
    size_t index;

    rect.left += size;
    rect.top += size;
    rect.right -=size;
    rect.bottom -=size;
    if(rect.right<rect.left)return -1;
    if(rect.bottom<rect.top)return -1;
    if(!bDraw)return 0;

    int px = rect.left;
    int py = rect.top;

    //randomize();
    index = 0;

    HPEN oldPen = (HPEN)SelectObject(dc, GetStockObject(DC_PEN));
    HBRUSH oldBrush = (HBRUSH)SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCPenColor(dc, color);
    SetDCBrushColor(dc, color);
    for(; px < rect.right; )
    {
        if(index==szbuf.size())szbuf.push_back(size/4+random(size/4*3));
        int n = rect.right-px;
        if(szbuf[index] < n)
        {
            Ellipse(dc, px-szbuf[index], py-szbuf[index], px+szbuf[index], py+szbuf[index]);
            px += szbuf[index];
            ++index;
        }
        else
        {
            Ellipse(dc, px-n, py-n, px+n, py+n);
            px += n;
        }
    }
    for(; py < rect.bottom; )
    {
        if(index==szbuf.size())szbuf.push_back(size/4+random(size/4*3));
        int n = rect.bottom-py;
        if(szbuf[index] < n)
        {
            Ellipse(dc, px-szbuf[index], py-szbuf[index], px+szbuf[index], py+szbuf[index]);
            py += szbuf[index];
            ++index;
        }
        else
        {
            Ellipse(dc, px-n, py-n, px+n, py+n);
            py += n;
        }
    }

    for(; px > rect.left; )
    {
        if(index==szbuf.size())szbuf.push_back(size/4+random(size/4*3));
        int n = -rect.left + px;
        if(szbuf[index] < n)
        {
            Ellipse(dc, px-szbuf[index], py-szbuf[index], px+szbuf[index], py+szbuf[index]);
            px -= szbuf[index];
            ++index;
        }
        else
        {
            Ellipse(dc, px-n, py-n, px+n, py+n);
            px -= n;
        }
    }

    for(; py > rect.top; )
    {
        if(index==szbuf.size())szbuf.push_back(size/4+random(size/4*3));
        int n = -rect.top+py;
        if(szbuf[index] < n)
        {
            Ellipse(dc, px-szbuf[index], py-szbuf[index], px+szbuf[index], py+szbuf[index]);
            py -= szbuf[index];
            ++index;
        }
        else
        {
            Ellipse(dc, px-n, py-n, px+n, py+n);
            py -= n;
        }
    }
    rect.left -= size / 8;
    rect.top -= size / 8;
    rect.right += size / 8;
    rect.bottom += size / 8;
    FillRect(dc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));// (HBRUSH)GetCurrentObject(dc, OBJ_BRUSH));

    SelectObject(dc, oldPen);
    SelectObject(dc, oldBrush);
    return 0;
}

}//end namespace windows
}//end namespace cgl

#endif //DRAW_COULD_HPP_20181110144745