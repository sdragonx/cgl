/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 bilinear_blt.hpp

 2017-02-06 14:21:36

*/
#ifndef BILINEAR_BLT_HPP_20170206142136
#define BILINEAR_BLT_HPP_20170206142136

#include <cgl/graphics/image.hpp>
#include <cgl/graphics/clipbox.hpp>

namespace cgl{
namespace graphics{

/*
    bilinear_blt()
    双线性插值缩放图像，效果很不错。
    支持24位和32位图像
    by sdragonx 2:51 2015/2/5
*/
int bilinear_blt(pixelimage& img_dst, int dx, int dy, int dw, int dh,
    const pixelimage& img_src, int sx, int sy, int sw, int sh)
{
    byte_t *pdst, *psrc0, *psrc1;
    int channels;

    uint32_t srcx_16=0;
    uint32_t srcy_16=0;
    uint32_t u_8, v_8;
    uint32_t u0, u1;
    uint32_t pm3_16;
    uint32_t pm2_16;
    uint32_t pm1_16;
    uint32_t pm0_16;
    int sx_v, dx_v;

    if(img_dst.pixelbits()!= img_src.pixelbits())
    {//暂时不支持颜色深度不一样的图像
        return -1;
    }

    if(img_src.pixelbits()< 24)
    {//不支持颜色深度低于24位的图像
        return -2;
    }
    else
    {//计算通道数量
        channels = img_src.pixelbits()/8;
    }

    //检查图像宽度
    if (  (0==img_dst.width())||(0==img_dst.height())
        ||(0==img_src.width())||(0==img_src.height()))
    {
        return -3;
    }

    //检查边界
    //if(!cliprect(rc_dst, img_dst.width, img_dst.height,
     //    rc_src, img_src.width, img_src.height))
      //    return -4;
    //*
    //有问题
    if(dx<0)dx=0;
    if(dy<0)dy=0;
    dw=std::min<int>(dw, img_dst.width()-dx);
    dh=std::min<int>(dh, img_dst.height()-dy);
    if(dw<=dx || dh<=dy)return -1;

    if(sx<0)sx=0;
    if(sy<0)sy=0;
    sw=std::min<int>(sw, img_src.width()-sx);
    sh=std::min<int>(sh, img_src.height()-sy);
    if(sw<=sx || sh<=sy)return -1;
    //*/

    //计算图像缩放因子
    uint32_t zoom_x16 = ((sw-1)<<16)/dw;
    uint32_t zoom_y16 = ((sh-1)<<16)/dh;

    //计算扫描偏移
    sx_v = sx*channels;
    dx_v = dx*channels;

    for (int y=0; y<dh; ++y)
    {
        v_8 = srcy_16>>16;//暂时保存src_y

        psrc0 = (byte_t*)img_src.scanline(sy+v_8);;//源图像第一行像素，连续获得两行像素
        psrc1 = (byte_t*)img_src.scanline(sy+(++v_8));//源图像第二行像素
        pdst = (byte_t*)img_dst.scanline(dy+y);//目标图像当前行

        //扫描偏移
        psrc0 += sx_v;
        psrc1 += sx_v;
        pdst += dx_v;

        srcx_16 = 0;
        v_8 = (srcy_16 & 0xFFFF)>>8;
        for (int x=0; x<dw; ++x)
        {
            u_8 = (srcx_16 & 0xFFFF)>>8;
            //计算4个像素各占的分量
            pm3_16=u_8*v_8;
            pm2_16=(u_8<<8)-pm3_16;
            pm1_16=(v_8<<8)-pm3_16;
            pm0_16=(1<<16)-pm1_16-pm2_16-pm3_16;

            //计算源像素x坐标
            u0 = ((srcx_16>>16)&0xFFFF)*channels;
            u1 = u0+channels;
            for(int z=0; z<channels; ++z)
            {
                *pdst =((
                     pm0_16*psrc0[u0+z]
                    +pm2_16*psrc0[u1+z]
                    +pm1_16*psrc1[u0+z]
                    +pm3_16*psrc1[u1+z])>>16);
                ++pdst;
            }
            srcx_16 += zoom_x16;
        }
        srcy_16 += zoom_y16;
    }
    return 0;
}

#if 0
uint32_t bilinear_color(pixellines& lines, uint32_t x, uint32_t y)
{
    byte_t *ps0, *ps1;
    int channels = lines.pixelbits / 8;
    uint32_t u, v;
    uint32_t pm00, pm10;
    uint32_t pm01, pm11;
    union{
        uint32_t color;
        byte_t   data[4];
    };

    u = x >> 16;
    v = y >> 16;

    ps0 = (byte_t*)lines[v];//源图像第一行像素，连续获得两行像素
    ps1 = (byte_t*)lines[v + 1];//源图像第二行像素
    u *= channels;
    ps0 += u;
    ps1 += u;

    //计算4个像素各占的分量

    u = x & 0xFFFF;
    v = y & 0xFFFF;
    pm11 = (u * v) >> 16;
    pm10 = u - pm11;
    pm01 = v - pm11;
    pm00 = 65536 - pm10 - pm01 - pm11;

    //计算源像素x坐标
    for(int z=0; z<channels; ++z)
    {
        data[z] =((
             pm00 * ps0[z]
            +pm10 * ps0[z + channels]
            +pm01 * ps1[z]
            +pm11 * ps1[z + channels])>>16);
    }

    return color;
}

uint32_t bilinear_colorf(pixellines& lines, float x, float y)
{
    return bilinear_color(lines, x * 65536, y * 65536);
}

#endif

}//end namespace graphics
}//end namespace cgl

#endif //BILINEAR_BLT_HPP_20170206142136