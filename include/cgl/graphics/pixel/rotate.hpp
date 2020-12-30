/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 rotate.hpp

 2015-04-03 05:30:37

*/
#ifndef ROTATE_HPP_20150403053037
#define ROTATE_HPP_20150403053037

#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/math.hpp>
#include <cgl/vec.hpp>
#include "image.hpp"

namespace cgl{
namespace graphics{

void image_rotate(pixelimage& dst, int dx, int dy, const pixelimage& src, int angle, float scale=1.0f, int alpha = 0xFF);

//返回旋转后的矩形大小
void image_ratate_bounds(int& dst_width, int& dst_height, int width, int height, int angle)
{
    const static int bit = 16;
    int cosine = math::cosi32(angle);
    int sine = math::sin32(angle);
    int p1x = int(width*cosine)>>bit;
    int p1y = int(width*sine)>>bit;
    int p2x = int(height*sine)>>bit;
    int p2y = int(height*cosine)>>bit;

    dst_width = abs(p2x)+ abs(p1x);
    dst_height = abs(p2y)+ abs(p1y);
}

//旋转函数
//支持 缩放、双线过滤、alpha混合
//
void image_rotate(pixelimage& dst, int dx, int dy, const pixelimage& src, int angle, float scale, int alpha)
{
    const static int bit = 16;
    const static int iscale_range = 256;
    int cosine, sine;
    int src_width, src_height;
    int dst_width, dst_height;
    int px = 0, py = 0;
    int vx = 0, vy = 0;
    int sinx=0, cosx=0;
    int siny=0, cosy=0;
    //color24* pdst;

    uint32_t u_8, v_8;
    uint32_t u0, u1;
    uint32_t pm3_16;
    uint32_t pm2_16;
    uint32_t pm1_16;
    uint32_t pm0_16;

    int iscale = scale*iscale_range;
    if(iscale==0)return ;

    angle = angle%360;

    cosine = int(cos(M_RD*angle)*65536.0f);
    sine = int(sin(M_RD*angle)*65536.0f);
    //cosine = math::cosi32(angle);
    //sine = math::sin32(angle);

    int p1x = int(src.width()*cosine)>>bit;
    int p1y = int(src.width()*sine)>>bit;
    int p2x = int(src.height()*sine)>>bit;
    int p2y = int(src.height()*cosine)>>bit;

    dst_width  = abs(p2x)+ abs(p1x);
    dst_height = abs(p2y)+ abs(p1y);
    dst_width  *= scale;
    dst_height *= scale;

    //修正偏移量
    vx = (src.width()<<bit)/2;
    vy = (src.height()<<bit)/2;

    int begin_y = dy-dst_height/2;
    int end_y = dy+dst_height/2;
    int begin_x = dx-dst_width/2;
    int end_x = dx+dst_width/2;

    if(begin_x<0)begin_x=0;
    if(end_x>=dst.width())end_x=dst.width()-1;
    if(begin_y<0)begin_y=0;
    if(end_y >= dst.height())end_y=dst.height()-1;

    color24* pdst = (color24*)(dst.data()+dst.linebytes()*(dst.height()-1-begin_y));
    color24 *psrc0, *psrc1;
    int sin_dx = dx*sine;
    int cos_dx = dx*cosine;
    int sin_dy = dy*sine;
    int cos_dy = dy*cosine;
    cosy = begin_y*cosine;
    siny = begin_y*sine;
    color24 clr;
    for(int y=begin_y; y<end_y; ++y)
    {
        //pdst = (color24*)(dst.data+dst.linebytes*(dst.height-1-y));
        //pdst += begin_x*3;
        for(int x=begin_x;x<end_x;++x)
        {
            //px = ((x-dx)*cosine + (y-dy)*sine)/iscale*256 + vx;
            //py = ((y-dy)*cosine - (x-dx)*sine)/iscale*256 + vy;
            px = ((cosx-cos_dx) + (siny-sin_dy))/iscale*iscale_range + vx;
            py = ((cosy-cos_dy) - (sinx-sin_dx))/iscale*iscale_range + vy;
            //px = ((cosx-cos_dx) + (siny-sin_dy))*scale + vx;
            //py = ((cosy-cos_dy) - (sinx-sin_dx))*scale + vy;
            v_8 = (py & 0xFFFF)>>8;
            u_8 = (px & 0xFFFF)>>8;
            px>>=bit;
            py>>=bit;
            if(px>=0 && px<src.width()-1 && py>=0 && py<src.height()-1)
            {
                //计算4个像素各占的分量
                pm3_16=u_8*v_8;
                pm2_16=(u_8<<8)-pm3_16;
                pm1_16=(v_8<<8)-pm3_16;
                pm0_16=(1<<16)-pm1_16-pm2_16-pm3_16;

                u0 = px;
                u1 = u0+1;
                psrc0 = (color24*)(src.data() + src.linebytes()*py);
                psrc1 = (color24*)(src.data() + src.linebytes()*py+src.linebytes());

                clr.red =((
                     pm0_16*psrc0[u0].red
                    +pm2_16*psrc0[u1].red
                    +pm1_16*psrc1[u0].red
                    +pm3_16*psrc1[u1].red)>>16);
                clr.green =((
                     pm0_16*psrc0[u0].green
                    +pm2_16*psrc0[u1].green
                    +pm1_16*psrc1[u0].green
                    +pm3_16*psrc1[u1].green)>>16);
                clr.blue =((
                     pm0_16*psrc0[u0].blue
                    +pm2_16*psrc0[u1].blue
                    +pm1_16*psrc1[u0].blue
                    +pm3_16*psrc1[u1].blue)>>16);
                pdst[x].color.value = clr.color.value;//blend32(pdst[x].color.value, clr.color.value, alpha);
            }
            cosx += cosine;
            sinx += sine;
        }
        siny += sine;
        cosy += cosine;
        sinx = begin_x * sine;
        cosx = begin_x * cosine;
        pdst = (color24*)((byte_t*)(pdst)-dst.linebytes());
    }
}


//获得每行扫描线的X值
vec2i* get_line_x(vec2i* p, int x1, int y1, int x2, int y2)
{
    if(y1==y2)return p;
    int step = y1 < y2? 1 : -1;
    int wx = x2-x1;
    int wy = y2-y1;
    int offset;
    int dstp;
    if(wx)
    {
        offset = (wx<<16)/wy*step;
        dstp = x1<<16;
        for(int i=y1; i!=y2; i+=step, ++p)
        {
            p->x = dstp>>16;
            p->y = i;
            dstp += offset;
        }
    }
    else
    {
        for(int i=y1; i!=y2; i+=step, ++p)
        {
            p->x = x1;
            p->y = i;
        }
    }
    return p;
}

//经过裁剪的旋转函数
void image_rotate2(pixelimage& dst, int dx, int dy, const pixelimage& src, int angle)
{
    const static int bit = 16;
    int cosine, sine;
    int src_width, src_height;
    int dst_width, dst_height;
    int px = 0, py = 0;
    int vx = 0, vy = 0;
    int sinx=0, cosx=0;
    int siny=0, cosy=0;
    color24* pdst;
    vec2i *pbegin, *pend;
    size_t size;

    angle = angle%360;

    cosine = int(std::cos(M_RD*angle)*65536.0f);
    sine = int(std::sin(M_RD*angle)*65536.0f);
    //cosine = math.cosi32(angle);
    //sine = math.sini32(angle);

    src_width = src.width();
    src_height = src.height();

    int p1x = int(-src.width()*cosine/2)>>bit;
    int p1y = int(-src.height()*sine/2)>>bit;
    int p2x = int(src.width()*sine/2)>>bit;
    int p2y = int(src.height()*cosine/2)>>bit;

    p1x = ((-src_width*cosine + src_height*sine)/2)>>bit;
    p1y = ((-src_width*sine - src_height*cosine)/2)>>bit;
    p2x = ((src_width*cosine + src_height*sine)/2)>>bit;
    p2y = ((src_width*sine - src_height*cosine)/2)>>bit;
    //p3x = ((src_width*cosine - src_height*sine)/2)>>bit;
    //p3y = ((src_width*sine + src_height*cosine)/2)>>bit;
    /*
    print(p1x);
    print(p1y);
    print(p3x);
    print(p3y);
    print(src_width-p2x);
    print(src_height-p2y);
    */

    dst_width = abs(p2x)+ abs(p1x);
    dst_height = abs(p2y)+ abs(p1y);
    dst_width *=2;
    dst_height*=2;

    pbegin = new vec2i[dst_height];

    if(angle%180<90)
    {
        pend = get_line_x(pbegin, p1x, p1y, p2x, p2y);
        pend = get_line_x(pend, p2x, p2y, -p1x, -p1y);
    }
    else
    {
        pend = get_line_x(pbegin, p2x, p2y, p1x, p1y);
        pend = get_line_x(pend, p1x, p1y, -p2x, -p2y);
        //pend = get_line_x(pbegin, -p2x, -p2y, -p1x, -p1y);
        //pend = get_line_x(pend, -p1x, -p1y, p2x, p2y);
    }
    size = pend - pbegin;
    --pend;

    #if 0
    Form1->Image2->Canvas->Pen->Color = clBlue;

    for(int i=0; i<size; ++i)
    {
        Form1->Image2->Canvas->Pixels[dx-pbegin[i].x][dy-pbegin[i].y] = clBlue;
        //Form1->Image2->Canvas->Pixels[dx+lines[size-i-1].x][dy+lines[size-i-1].y] = i;
        //Form1->Image2->Canvas->Pixels[dx+1+lines[size-i-1].x][dy+lines[size-i-1].y] = i;
        //Form1->Image2->Canvas->Pixels[dx+pbegin[dst_height-i-1].x][dy-pbegin[i].y] = clRed;
        Form1->Image2->Canvas->Pixels[dx+(pend-i)->x][dy-pbegin[i].y] = clRed;

        //Form1->Image2->Canvas->Pixels[dx-lines[i].x][i] = clBlue;
        //Form1->Image2->Canvas->Pixels[dx+lines[size-i-1].x][i] = clRed;
        //print(dy-lines[i].y);


        Form1->Image2->Canvas->Pen->Color = TColor(i*dst_height>>8);
        //Form1->Image2->Canvas->MoveTo(dx-(lines[i].x), dy-(lines[i].y));
        //Form1->Image2->Canvas->LineTo(dx+(lines[size-i-1].x), dy-(lines[i].y));
        //print(lines[size-i-1].x);
        //print(lines[size-i-1].y);

        Form1->Image2->Canvas->MoveTo(dx-(pbegin[i].x), i);
        //Form1->Image2->Canvas->LineTo(dx+(-lines[size-i-1].x), i);
    }
    #endif;

    vx = (-dst_width*cosine - dst_height*sine + (src_width<<bit))/2+0x8000;
    vy = (dst_width*sine - dst_height*cosine + (src_height<<bit))/2+0x8000;
    vx = (src.width()<<bit)/2;
    vy = (src.height()<<bit)/2;

    //return ;

    pdst = (color24*)(dst.data()+dst.linebytes()*(dst.height()-1));
    color24* psrc = (color24*)(src.data());

    int step;
    for(size_t y=0; y<size; ++y)
    {
        p1x = dx-pbegin[y].x;
        p2x = dx+(pend-y)->x;
        step = p1x < p2x ? 1 : -1;
        p1y = dy-pbegin[y].y;
        //if(p1y<0 || p1y>=dst.height)continue;
        pdst = (color24*)(dst.data()+dst.linebytes()*(dst.height()-1-p1y));
        //pdst = (color24*)(dst.data+dst.linebytes*(dst.height-1-y));
        ///if(p1x<0)p1x = 0;
        //if(p2x>=dst.width)p2x = dst.width-1;
        for(int x=p1x; x!=p2x; x+=step)
        {
            px = ((dx-x)*cosine + (dy-p1y)*sine + vx)>>bit;
            py = ((dy-p1y-0)*cosine - (dx-x)*sine + vy)>>bit;

            //px = (cosx+siny+vx)>>bit;
            //py = (cosy-sinx+vy)>>bit;
            if(px>=0 && px<src.width() && py>=0 && py<src.height())
            {
                pdst[x] = ((color24*)(((byte_t*)psrc) + src.linebytes()*py))[px];
            }
            //pdst[x].color.value = 0xFF00;
            cosx += cosine;
            sinx += sine;
        }
        siny += sine;
        cosy += cosine;
        cosx = 0;
        sinx = 0;
        //pdst = (color24*)(((byte_t*)pdst)-dst.linebytes);
    }

    delete []pbegin; pbegin = NULL;

    #if 0
    return ;

    vx = (src.width<<bit)/2;
    vy = (src.height<<bit)/2;

    int cx = dst_width/2;
    int cy = dst_height/2;
    int begin_y = dy-dst_height/2;
    int end_y = dy+dst_height/2;
    int begin_x = dx-dst_width/2;
    int end_x = dx+dst_width/2;

    //siny = -(begin_y)*sine;
    //cosy = -(begin_y)*cosine;

    if(begin_x<0)begin_x=0;
    if(end_x>=dst.width)end_x=dst.width-1;
    if(begin_y<0)begin_y=0;
    if(end_y>=dst.height)end_y=dst.height-1;

    pdst = (color24*)(dst.data+dst.linebytes*(dst.height-1-begin_y));
    //color24* psrc = (color24*)(src.data);
    for(int y=begin_y; y<end_y; ++y)
    {
        for(int x=begin_x;x<end_x;++x)
        {
            px = ((x-dx)*cosine + (y-dy)*sine + vx)>>bit;
            py = ((y-dy)*cosine - (x-dx)*sine + vy)>>bit;
            //px = (cosx+siny+vx)>>bit;
            //py = (cosy-sinx+vy)>>bit;
            if(px>=0 && px<src.width && py>=0 && py<src.height)
            {
                psrc = (color24*)(src.data + src.linebytes*py);
                if(psrc[px].red != 255)
                    pdst[x] = psrc[px];
            }
            //cosx += cosine;
            //sinx += sine;
        }
        //siny += sine;
        //cosy += cosine;
        //sinx = 0;//p1x;//(-dst_width/2)*cosine;
        //cosx = 0;//p1y;//(-dst_width/2)*sine;
        pdst = (color24*)(((byte_t*)pdst)-dst.linebytes);
    }
    //delete []lines; lines = NULL;
    #endif
}

}//end namespace graphics
}//end namespace cgl

#endif //ROTATE_HPP_20150403053037
