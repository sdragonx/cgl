/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shadow_text.hpp

 2017-02-06 14:45:00

*/
#ifndef SHADOW_TEXT_HPP_20170206144500
#define SHADOW_TEXT_HPP_20170206144500

#include "bitmap.hpp"

namespace cgl{
namespace graphics{

//制作alpha通道，绘制的背景为黑色，字体颜色为白色
//再以个颜色分量创建alpha通道
//GetGrayOutLine好像更好完成
//invert = true，返回带alpha通道的黑色字体
int bm_mkalpha(HBITMAP hbmp, bool invert)
{
    BITMAP bm = {0};
    GetObject(hbmp, sizeof(BITMAP), &bm);
    color32* pclr = NULL;
    int n = 0;
    for(int y=0; y<bm.bmHeight; ++y)
    {
        pclr = (bgra32_t*)bm_scan(bm, y);
        for(int x=0; x<bm.bmWidth; ++x)
        {
            if(pclr[x].alpha)
            {
                pclr[x].value = 0;
            }
            else
            {
                //n = (pclr[x].red+pclr[x].green+pclr[x].blue)/3;
                //取最大值做alpha
                n = std::max(std::max(pclr[x].red, pclr[x].green), pclr[x].blue);
                if(invert)
                {
                    pclr[x].value = 0;
                }
                pclr[x].alpha = n;
            }
        }//end for x
    }//end for y
    return 0;
}

namespace internal{

int draw_text(HDC handle, LPCTSTR str, int length,
    RECT& rc, DWORD color, DWORD flag, int alpha, bool shadow)
{
    HDC dc = NULL;
    HDC dc_text = NULL;
    HBITMAP hbmp = NULL;
    HBITMAP hbmp_text = NULL;
    BITMAP bm = {0};
    int x = rc.left;
    int y = rc.top;
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    //矩形移动到原点
    OffsetRect(&rc, -rc.left, -rc.top);

    //创建GDI对象
    dc = CreateCompatibleDC(0);
    //创建的位图大8个像素，用来储存阴影
    hbmp = graphics::bm_create(width+8, height+8, 32, null);
    SelectObject(dc, GetCurrentObject(handle, OBJ_FONT));
    SelectObject(dc, hbmp);
    SetBkMode(dc, TRANSPARENT);    //背景透明
    SetTextColor(dc, color);    //文字颜色必须为白色??
    bm_clear(hbmp, 0xFF000000);    //清除为带alpha通道的黑色
    GetObject(hbmp, sizeof(bm), &bm);

    if(!shadow)
    {//绘制不带阴影的文字
        DrawText(dc, str, length, &rc, flag);
        bm_mkalpha(hbmp, false);
        alphablend(handle, x, y, bm.bmWidth, bm.bmHeight, dc, 0, 0, bm.bmWidth, bm.bmHeight, alpha);
    }
    else
    {//绘制带阴影的文字
        dc_text = CreateCompatibleDC(0);
        hbmp_text = graphics::bm_create(width+8, height+8, 32, null);
        SelectObject(dc_text, hbmp_text);

        //x、y偏移4个像素
        OffsetRect(&rc, 4, 4);
        DrawText(dc, str, length, &rc, flag);
        //复制一份到dc_shadow，如果图像是32位色，BitBlt会原样把alpha通道复制过去(win8)
        BitBlt(dc_text, 0, 0, bm.bmWidth, bm.bmHeight, dc, 0, 0, SRCCOPY);
        //偏移一个像素，再绘制一次字体，目的是为了效果更好
        OffsetRect(&rc, 1, 1);
        DrawText(dc, str, length, &rc, flag);

        //创建alpha通道
        bm_mkalpha(hbmp, true);
        bm_mkalpha(hbmp_text, false);

        //高斯模糊
        cgl::graphics::gauss_blur((byte_t*)bm.bmBits, bm.bmWidth, bm.bmHeight, 4, bm.bmWidthBytes, 2.0);

        //绘制字体
        alphablend(dc, 0, 0, bm.bmWidth, bm.bmHeight, dc_text, 0, 0, bm.bmWidth, bm.bmHeight, 255);
        //绘制到目标DC
        alphablend(handle, x - 4, y - 4, bm.bmWidth, bm.bmHeight, dc, 0, 0, bm.bmWidth, bm.bmHeight, 255);

        DeleteDC(dc_text); dc_text = NULL;
        DeleteObject(hbmp_text); hbmp_text = NULL;
    }//end if

    DeleteDC(dc); dc = NULL;
    DeleteObject(hbmp); hbmp = NULL;
    return 0;
}

}//internal

//绘制带alpha通道的文字
int draw_text(HDC dc, PCTSTR str, int length, RECT rc,
    DWORD color = 0x00FFFFFF,
    DWORD flag = DT_CENTER|DT_WORDBREAK|DT_EDITCONTROL,
    int alpha = 255,
    bool shadow = true)
{
    int width;

    //留出阴影范围
    rc.left   += 4;
    rc.top    += 4;
    rc.right  -= 4;
    rc.bottom -= 4;
    if(rc.right <= rc.left){
        return 0;
    }

    //计算输出矩形，返回字符串高度
    if(flag & DT_CALCRECT){
        width = rc.right - rc.left;
        DrawText(dc, str, length, &rc, flag);
        if(rc.right - rc.left < width){
            rc.right = rc.left + width;
        }
        return rc.bottom - rc.top + 8;
    }

    return internal::draw_text(dc, str, length, rc, color, flag, alpha, shadow);
}

}//end namespace graphics
}//end namespace cgl

#endif //SHADOW_TEXT_HPP_20170206144500
