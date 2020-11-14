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

//����alphaͨ�������Ƶı���Ϊ��ɫ��������ɫΪ��ɫ
//���Ը���ɫ��������alphaͨ��
//GetGrayOutLine����������
//invert = true�����ش�alphaͨ���ĺ�ɫ����
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
                //ȡ���ֵ��alpha
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

    //�����ƶ���ԭ��
    OffsetRect(&rc, -rc.left, -rc.top);

    //����GDI����
    dc = CreateCompatibleDC(0);
    //������λͼ��8�����أ�����������Ӱ
    hbmp = graphics::bm_create(width+8, height+8, 32, null);
    SelectObject(dc, GetCurrentObject(handle, OBJ_FONT));
    SelectObject(dc, hbmp);
    SetBkMode(dc, TRANSPARENT);    //����͸��
    SetTextColor(dc, color);    //������ɫ����Ϊ��ɫ??
    bm_clear(hbmp, 0xFF000000);    //���Ϊ��alphaͨ���ĺ�ɫ
    GetObject(hbmp, sizeof(bm), &bm);

    if(!shadow)
    {//���Ʋ�����Ӱ������
        DrawText(dc, str, length, &rc, flag);
        bm_mkalpha(hbmp, false);
        alphablend(handle, x, y, bm.bmWidth, bm.bmHeight, dc, 0, 0, bm.bmWidth, bm.bmHeight, alpha);
    }
    else
    {//���ƴ���Ӱ������
        dc_text = CreateCompatibleDC(0);
        hbmp_text = graphics::bm_create(width+8, height+8, 32, null);
        SelectObject(dc_text, hbmp_text);

        //x��yƫ��4������
        OffsetRect(&rc, 4, 4);
        DrawText(dc, str, length, &rc, flag);
        //����һ�ݵ�dc_shadow�����ͼ����32λɫ��BitBlt��ԭ����alphaͨ�����ƹ�ȥ(win8)
        BitBlt(dc_text, 0, 0, bm.bmWidth, bm.bmHeight, dc, 0, 0, SRCCOPY);
        //ƫ��һ�����أ��ٻ���һ�����壬Ŀ����Ϊ��Ч������
        OffsetRect(&rc, 1, 1);
        DrawText(dc, str, length, &rc, flag);

        //����alphaͨ��
        bm_mkalpha(hbmp, true);
        bm_mkalpha(hbmp_text, false);

        //��˹ģ��
        cgl::graphics::gauss_blur((byte_t*)bm.bmBits, bm.bmWidth, bm.bmHeight, 4, bm.bmWidthBytes, 2.0);

        //��������
        alphablend(dc, 0, 0, bm.bmWidth, bm.bmHeight, dc_text, 0, 0, bm.bmWidth, bm.bmHeight, 255);
        //���Ƶ�Ŀ��DC
        alphablend(handle, x - 4, y - 4, bm.bmWidth, bm.bmHeight, dc, 0, 0, bm.bmWidth, bm.bmHeight, 255);

        DeleteDC(dc_text); dc_text = NULL;
        DeleteObject(hbmp_text); hbmp_text = NULL;
    }//end if

    DeleteDC(dc); dc = NULL;
    DeleteObject(hbmp); hbmp = NULL;
    return 0;
}

}//internal

//���ƴ�alphaͨ��������
int draw_text(HDC dc, PCTSTR str, int length, RECT rc,
    DWORD color = 0x00FFFFFF,
    DWORD flag = DT_CENTER|DT_WORDBREAK|DT_EDITCONTROL,
    int alpha = 255,
    bool shadow = true)
{
    int width;

    //������Ӱ��Χ
    rc.left   += 4;
    rc.top    += 4;
    rc.right  -= 4;
    rc.bottom -= 4;
    if(rc.right <= rc.left){
        return 0;
    }

    //����������Σ������ַ����߶�
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
