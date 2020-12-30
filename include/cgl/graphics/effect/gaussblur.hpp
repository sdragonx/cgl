/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gaussblur.hpp

 2014-08-01 02:16:50

*/
#ifndef GAUSSBLUR_HPP_20140801
#define GAUSSBLUR_HPP_20140801

#include <stdint.h>
#include <cmath>
#include <algorithm>

#include <cgl/graphics/clipbox.hpp>


#ifdef CGL_PLATFORM_CBUILDER
    #pragma option push
    #pragma option -w-8004        //is assigned a value that is never used
//    #pragma option -w-8008        //Condition is always true
//    #pragma option -w-8012        //Comparing signed and unsigned values
//    #pragma option -w-8066        //Unreachable code
//    #pragma option -w-8062        //Previous options and warnings not restored
#endif

typedef uint8_t byte_t;

namespace cgl{
namespace graphics{

void compute_gauss_coefs(float &B, float &b1, float &b2, float &b3, float sigma)
{
    float q  = 0;
    float q2 = 0, q3 = 0;
    float b0;

    if (sigma >= 2.5f){
        q = 0.98711f * sigma - 0.96330f;
    }
    else if ((sigma >= 0.5f) && (sigma < 2.5f)){
        q = 3.97156f - 4.14554f * (float) sqrt (1.0f - 0.26891f * sigma);
    }
    else{
        q = 0.1147705018520355224609375f;
    }

    q2 = q * q;
    q3 = q * q2;
    b0 = (1.57825f+ (2.44413f*q)+(1.4281f *q2)+(0.422205f*q3));
    b1 = (          (2.44413f*q)+(2.85619f*q2)+(1.26661f* q3));
    b2 = (                      -((1.4281f*q2)+(1.26661f* q3)));
    b3 = (                                     (0.422205f*q3));

    b0 = 1.0f / b0;

    B = 1.0f-((b1+b2+b3) * b0);

    b1 *= b0;
    b2 *= b0;
    b3 *= b0;
}

class gauss_coefs
{
public:
    float sigma;
    float a, b, c, d;

    void compute(float _sigma)
    {
        sigma = _sigma;
        compute_gauss_coefs(a, b, c, d, sigma);
    }
};

// by 2020/1/24
int gauss_blur(
    byte_t* image,
    int width,
    int height,
    int channels,    //行步进
    int stride,
    gauss_coefs& coefs)
{
    int x = 0, y = 0, n = 0;
    int line_size = std::max(width, height) + 4;

    float *w1 = (float*)malloc(line_size * sizeof(float));
    if(!w1){
        return -1;
    }

    float *w2 = (float*)malloc(line_size * sizeof(float));
    if(!w2){
        free(w1);
        return -1;
    }

    float *channel_buffer = (float*)malloc(width * height * sizeof(float));
    if(!channel_buffer){
        free(w1);
        free(w2);
        return -1;
    }

    #ifdef CGL_ENABLE_SSE
    __m128 a;
    __m128 coef(coefs.a, coefs.b, coefs.c, coefs.d);
    #endif

    // 处理图像的多个通道
    for (int channel = 0; channel < channels; ++channel){
        // 获取一个通道的所有像素值，并预处理
        byte_t* source = image + channel;
        float* buffer = channel_buffer;
        for(y = 0; y < height; ++y){
            for(x = 0, n = 0; x < width; ++x, n += channels){
                buffer[x] = source[n];
            }
            source += stride;
            buffer += width;
        }

        for (x = 0; x < width; ++x){//横向处理
            buffer = channel_buffer + x;

            //初始化行数据
            w1[0] = buffer[0];
            w1[1] = buffer[0];
            w1[2] = buffer[0];

            for (y = 0; y < height; ++y){
                #ifdef CGL_ENABLE_SSE
                a.m0 = buffer[0];
                a.m1 = w1[y+2];
                a.m2 = w1[y+1];
                a.m3 = w1[y+0];
                mm_mulps(a, coef);
                w1[y+3] = a.m0 + a.m1 + a.m2 + a.m3;
                #else
                w1[y+3] = coefs.a * buffer[0] + (coefs.b * w1[y+2] + coefs.c * w1[y+1] + coefs.d * w1[y+0]);
                #endif
                buffer += width;
            }

            w2[height+0]= w1[height+2];
            w2[height+1]= w1[height+1];
            w2[height+2]= w1[height+0];

            //buffer = imgbuf + width * (height - 1) + x;
            buffer -= width;
            for (int y = height - 1; y >= 0; --y)
            {
                #ifdef CGL_ENABLE_SSE
                a.m0 = w1[y+3];
                a.m1 = w2[y+1];
                a.m2 = w2[y+2];
                a.m3 = w2[y+3];
                mm_mulps(a, coef);
                buffer[0] = w2[y] = a.m0 + a.m1 + a.m2 + a.m3;
                #else
                buffer[0] = w2[y] = coefs.a * w1[y+3] + (coefs.b * w2[y+1] + coefs.c * w2[y+2] + coefs.d * w2[y+3]);
                #endif
                buffer -= width;
            }
        }//横向处理

        source = image + channel;
        buffer = channel_buffer;
        for (y = 0; y < height; ++y){//纵向处理
            //初始化行数据
            w1[0] = buffer[0];
            w1[1] = buffer[0];
            w1[2] = buffer[0];

            //正方向横向处理3个点的数据
            for (x = 0; x < width; ++x){
                #ifdef CGL_ENABLE_SSE
                a.m0 = buffer[x];
                a.m1 = w1[x+2];
                a.m2 = w1[x+1];
                a.m3 = w1[x+0];
                mm_mulps(a, coef);
                w1[x+3] = a.m0 + a.m1 + a.m2 + a.m3;
                #else
                w1[x+3] = coefs.a * buffer[x] + (coefs.b * w1[x+2] + coefs.c * w1[x+1] + coefs.d * w1[x+0]);
                #endif
            }

            w2[width + 0]= w1[width + 2];
            w2[width + 1]= w1[width + 1];
            w2[width + 2]= w1[width + 0];

            //反方向处理
            x = width - 1;
            n = x * channels;
            for ( ; x >= 0; --x){
                #ifdef CGL_ENABLE_SSE
                a.m0 = w1[x+3];
                a.m1 = w2[x+1];
                a.m2 = w2[x+2];
                a.m3 = w2[x+3];
                mm_mulps(a, coef);
                w2[x] = a.m0 + a.m1 + a.m2 + a.m3;
                #else
                w2[x] = coefs.a * w1[x+3] + (coefs.b * w2[x+1] + coefs.c * w2[x+2] + coefs.d * w2[x+3]);
                #endif
                source[n] = round(w2[x]);//保存数据
                n -= channels;
            }
            source += stride;
            buffer += width;
        }//纵向处理
    }//通道循环

    free(w1);
    free(w2);
    free(channel_buffer);
    w1 = w2 = channel_buffer = NULL;

    return 0;
}

//快速高斯模糊函数，by 2014-08-01
int gauss_blur(
    byte_t* image,    //位图数据
    int width,        //位图宽度
    int height,        //位图高度
    int channels,    //颜色通道数量
    int stride,        //位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
    int left,        //滤镜剪裁区域，都为0是不剪裁
    int top,
    int right,
    int bottom,
    float sigma        //高斯系数
    )
{
    if(!cliprect(0, 0, width, height, left, top, right, bottom)){
        return -1;
    }

    image += top * stride + left * channels;
    width = right - left;
    height = bottom - top;
    gauss_coefs coefs;
    coefs.compute(sigma);
    return gauss_blur(image, width, height, channels, stride, coefs);
}

int gauss_blur(
    byte_t* image,    //位图数据
    int width,        //位图宽度
    int height,        //位图高度
    int channels,    //颜色通道数量
    int stride,        //位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
    float sigma        //高斯系数
    )
{
    gauss_coefs coefs;
    coefs.compute(sigma);
    return gauss_blur(image, width, height, channels, stride, coefs);
}

#if 0

//快速高斯模糊函数，by 2014-08-01
int gauss_blur(
    byte_t* image,    //位图数据
    int linebytes,    //位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
    int width,        //位图宽度
    int height,        //位图高度
    int channels,    //颜色通道数量
    int left,        //滤镜剪裁区域，都为0是不剪裁
    int top,
    int right,
    int bottom,
    float sigma        //高斯系数
    )
{
    int x = 0, y = 0, n = 0;
    int channel = 0;
    int srcline = 0, dstline = 0;
    int channelsize = 0;
    int bufsize = 0;
    float *w1 = NULL, *w2 = NULL, *imgbuf = NULL;

    if(!cliprect(0, 0, width, height, left, top, right, bottom)){
        return -1;
    }

    //剪裁宽度和高度
    int rc_width  = right - left;
    int rc_height = bottom - top;

    channelsize = rc_width * rc_height;
    bufsize     = rc_width > rc_height ? rc_width + 4 : rc_height + 4;

    w1 = (float*)malloc(bufsize * sizeof(float));
    if(!w1){
        return -1;
    }

    w2 = (float*)malloc(bufsize * sizeof(float));
    if(!w2){
        free(w1);
        return -1;
    }

    imgbuf = (float*)malloc(channelsize * sizeof(float));
    if(!imgbuf){
        free(w1);
        free(w2);
        return -1;
    }

//----------------计算高斯核---------------------------------------//

    float q  = 0;
    float q2 = 0, q3 = 0;
    float b0 = 0, b1 = 0, b2 = 0, b3 = 0;
    float B  = 0;
    float rb;

    if (sigma >= 2.5f){
        q = 0.98711f * sigma - 0.96330f;
    }
    else if ((sigma >= 0.5f) && (sigma < 2.5f)){
        q = 3.97156f - 4.14554f * (float) std::sqrt (1.0f - 0.26891f * sigma);
    }
    else{
        q = 0.1147705018520355224609375f;
    }

    q2 = q * q;
    q3 = q * q2;
    b0 = (1.57825f+ (2.44413f*q)+(1.4281f *q2)+(0.422205f*q3));
    b1 = (          (2.44413f*q)+(2.85619f*q2)+(1.26661f* q3));
    b2 = (                      -((1.4281f*q2)+(1.26661f* q3)));
    b3 = (                                     (0.422205f*q3));
    rb = 1.0f / b0;
    B = 1.0f-((b1+b2+b3) * rb);

    b1 *= rb;
    b2 *= rb;
    b3 *= rb;

//----------------计算高斯核结束---------------------------------------//

    // 处理图像的多个通道
    for (channel = 0; channel < channels; ++channel)
    {

        // 获取一个通道的所有像素值，并预处理
        srcline = top*linebytes + left*channels + channel;
        dstline = 0;
        for(y=0; y<rc_height; ++y, srcline+=linebytes, dstline+=rc_width)
        {
            for(x=0, n=0; x<rc_width; ++x, n+=channels){
                (imgbuf+dstline)[x] = (image+srcline)[n];
            }
        }

        //for (int x=0; x<rc_width; ++x)
        for (x=0; x<rc_width; ++x)
        {//横向处理

            w1[0] = (imgbuf + x)[0];
            w1[1] = (imgbuf + x)[0];
            w1[2] = (imgbuf + x)[0];

            for (y=0, n=0; y<rc_height; ++y, n+=rc_width)
            {
                w1[y+3] = B*(imgbuf + x)[n] + (b1*w1[y+2] + b2*w1[y+1] + b3*w1[y+0]);
            }

            w2[rc_height+0]= w1[rc_height+2];
            w2[rc_height+1]= w1[rc_height+1];
            w2[rc_height+2]= w1[rc_height+0];

            for (int y=rc_height-1, n=y*rc_width; y>=0; --y, n-=rc_width)
            {
                (imgbuf + x)[n] = w2[y] = B*w1[y+3] + (b1*w2[y+1] + b2*w2[y+2] + b3*w2[y+3]);
            }
        }//横向处理

        srcline = 0;
        dstline = top*linebytes + left*channels + channel;
        for (y=0; y<rc_height; ++y, srcline+=rc_width, dstline+=linebytes)
        {//纵向处理

            //取当前行数据
            w1[0] = (imgbuf + srcline)[0];
            w1[1] = (imgbuf + srcline)[0];
            w1[2] = (imgbuf + srcline)[0];

            //正方向横向处理3个点的数据
            for (x=0; x<rc_width; ++x)
            {
                w1[x+3] = B*(imgbuf + srcline)[x] + (b1*w1[x+2] + b2*w1[x+1] + b3*w1[x+0]);
            }

            w2[rc_width+0]= w1[rc_width+2];
            w2[rc_width+1]= w1[rc_width+1];
            w2[rc_width+2]= w1[rc_width+0];

            //反方向处理
            for (x=rc_width-1, n=x*channels; x>=0; --x, n-=channels)
            {
                w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);
                (image + dstline)[n] = f_toint(w2[x]);
            }
        }//纵向处理

        /*
        //存储处理完毕的通道
        for(int y=0; y<rc_height; ++y)
        {
            int dstline = (y+top)*linebytes + left*cbyte;
            int srcline = y*rc_width;
            for (int x=0, n=channel; x<rc_width; ++x, n+=cbyte)
            {
                (image + dstline)[n] = (imgbuf + srcline)[x];
                    //byte_edge((imgbuf + srcline)[x]);

            }
        }//存储循环
        //*/
    }//通道循环

    free(w1);
    free(w2);
    free(imgbuf);

    return 0;
}
#endif

}//end namespace cgl
}//end namespace cgl

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option pop
#endif

#endif //GAUSSBLUR_HPP_20140801

/*
速度还没float版本快

const static int BLUR_SHIFT = 16;

void calc_gauss(int& d0, int& d1, int& d2, int& d3, float sigma)
{
    float q  = 0;
    float q2 = 0, q3 = 0;
    float b0 = 0, b1 = 0, b2 = 0, b3 = 0;
    float B  = 0;

    if (sigma >= 2.5f)
    {
        q = 0.98711f * sigma - 0.96330f;
    }
    else if ((sigma >= 0.5f) && (sigma < 2.5f))
    {
        q = 3.97156f - 4.14554f * (float) std::sqrt (1.0f - 0.26891f * sigma);
    }
    else
    {
        q = 0.1147705018520355224609375f;
    }

    q2 = q * q;
    q3 = q * q2;
    b0 = (1.57825f+ (2.44413f*q)+(1.4281f *q2)+(0.422205f*q3));
    b1 = (          (2.44413f*q)+(2.85619f*q2)+(1.26661f* q3));
    b2 = (                      -((1.4281f*q2)+(1.26661f* q3)));
    b3 = (                                     (0.422205f*q3));
    B = 1.0f-((b1+b2+b3)/b0);

    b1 /= b0;
    b2 /= b0;
    b3 /= b0;

    String s;
    s.printf(L"%f, %f, %f, %f", B, b1, b2, b3);
    print(s);

    int n = 1 << BLUR_SHIFT;

    d0 = B * n;
    d1 = b1 * n;
    d2 = b2 * n;
    d3 = b3 * n;
}

//不带剪裁的版本
int gauss_blur2(
    byte_t* image,    //位图数据
    int linebytes,    //位图行字节数，BMP数据在windows中是4字节对齐的。否则在处理非二次幂的图像时会有偏差
    int width,        //位图宽度
    int height,        //位图高度
    int cbyte,        //颜色通道数量
    float sigma        //高斯系数
    )
{
    int x = 0, y = 0, n = 0;
    int channel = 0;
    int srcline = 0, dstline = 0;
    int channelsize = width * height;
    int bufsize = width > height ? width + 4 : height + 4;

    typedef int64_t value_type;
    const int BASE_SHIFT = 10;

    value_type *w1 = NULL, *w2 = NULL, *imgbuf = NULL;

    w1 = (value_type*)malloc(bufsize * sizeof(value_type));
    if(!w1)
    {
        return -1;
    }
    w2 = (value_type*)malloc(bufsize * sizeof(value_type));
    if(!w2)
    {
        free(w1);
        return -1;
    }
    imgbuf = (value_type*)malloc(channelsize * sizeof(value_type));
    if(!imgbuf)
    {
        free(w1);
        free(w2);
        return -1;
    }

//----------------计算高斯核---------------------------------------//

    int B, b1, b2, b3;
    calc_gauss(B, b1, b2, b3, sigma);

//----------------计算高斯核结束---------------------------------------//

    // 处理图像的多个通道
    for (channel = 0; channel < cbyte; ++channel)
    {
        // 获取一个通道的所有像素值，并预处理
        for(y=0; y<height; ++y){
            srcline = y*linebytes;
            dstline = y*width;
            for(x=0, n=channel; x<width; ++x, n+=cbyte)
            {
                (imgbuf+dstline)[x] = (image + srcline)[n] << BASE_SHIFT;
            }
        }


        for (int x=0; x<width; ++x)
        {//横向处理

            w1[0] = (imgbuf + x)[0];
            w1[1] = (imgbuf + x)[0];
            w1[2] = (imgbuf + x)[0];

            for (y=0, n=0; y<height; ++y, n+=width)
            {
                w1[y+3] = B*(imgbuf + x)[n] + (b1*w1[y+2] + b2*w1[y+1] + b3*w1[y+0]);
                w1[y+3] >>= BLUR_SHIFT;
            }

            w2[height+0]= w1[height+2];
            w2[height+1]= w1[height+1];
            w2[height+2]= w1[height+0];

            for (int y=height-1, n=y*width; y>=0; --y, n-=width)
            {
                //保存数据到缓存
                w2[y] = B*w1[y+3] + (b1*w2[y+1] + b2*w2[y+2] + b3*w2[y+3]);
                w2[y] >>= BLUR_SHIFT;
                (imgbuf + x)[n] = w2[y];
            }
        }//横向处理

        for (y=0, srcline=0, dstline=0; y<height; ++y, srcline+=width, dstline+=linebytes)
        {//纵向处理

            //取当前行数据
            w1[0] = (imgbuf + srcline)[0];
            w1[1] = (imgbuf + srcline)[0];
            w1[2] = (imgbuf + srcline)[0];

            //正方向横向处理3个点的数据
            for (x=0; x<width ; ++x)
            {
                w1[x+3] = B*(imgbuf + srcline)[x] + (b1*w1[x+2] + b2*w1[x+1] + b3*w1[x+0]);
                w1[x+3] >>= BLUR_SHIFT;
            }

            w2[width+0]= w1[width+2];
            w2[width+1]= w1[width+1];
            w2[width+2]= w1[width+0];

            //反方向处理
            for (x=width-1, n=x*cbyte+channel; x>=0; --x, n-=cbyte)
            {
                //处理保存数据到缓存
                //(imgbuf + dstline)[x] = w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);

                //存储返回数据
                w2[x] = B*w1[x+3] + (b1*w2[x+1] + b2*w2[x+2] + b3*w2[x+3]);
                w2[x] >>= BLUR_SHIFT;
                (image + dstline)[n] = w2[x] >> BASE_SHIFT;
            }

        }//纵向处理
    }//通道循环

    free(w1);
    w1=NULL;
    free(w2);
    w2=NULL;
    free(imgbuf);
    imgbuf=NULL;

    return 0;
}
*/

/*
线性时间高斯模糊
inline int round(float n)
{
    return f_toint(n);
//    n = n < 0.0f ? ceil(n - 0.5f) : floor(n + 0.5);
//    return f_toint_asm(n);
}

inline vec4ub round(const vec4f& c)
{
    return vec4ub(round(c.r), round(c.g), round(c.b), round(c.a));
}

void boxesForGauss(int *boxes, int size, int sigma)  // standard deviation, number of boxes
{
    float wIdeal = std::sqrt((12.0*sigma*sigma/size)+1.0);  // Ideal averaging filter width
    int wl = std::floor(wIdeal);
    if(wl%2==0) wl--;
    int wu = wl+2;

    float mIdeal = (12.0*sigma*sigma - size*wl*wl - 4*size*wl - 3*size)/(-4*wl - 4);
    int m = round(mIdeal+0.0);
    // var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

    for(int i=0; i<size; ++i){
        boxes[i] = i < m ? wl : wu;
    }
}

void boxBlurH(byte_t* source, byte_t* buffer, int w, int h, int stride, int r)
{
    int ti, li, ri;
    int fv, lv, val;
    int iarr = r + r + 1;// 1.0 / (r + r + 1);
    for(int y = 0; y < h; ++y){
        ti = y * stride;
        li = ti;
        ri = ti + r;
        fv  = source[ti];
        lv  = source[ti + stride - 1];
        val = fv * (r + 1);
        for(int j = 0; j < r; ++j){
            val += source[ti + j];
        }
        for(int j = 0; j <= r ; ++j){
            val += source[ri] - fv;
            buffer[ti] = val / iarr;// round(val * iarr);
            ++ri;
            ++ti;
        }
        for(int j = r + 1; j < w - r; j++){
            val += source[ri] - source[li++];
            buffer[ti] = val / iarr;// round(val * iarr);
            ++ri;
            ++ti;
        }
        for(int j = w - r; j < w; j++){
            val += lv - source[li];
            buffer[ti] = val / iarr;//round(val * iarr);
            ++li;
            ++ti;
        }
    }
}

void boxBlurT(byte_t* source, byte_t* buffer, int w, int h, int stride, int r)
{
    int ti, li, ri;
    int fv, lv, val;
    int iarr = r + r + 1;//1.0 / (r + r + 1);
    for(int x = 0; x < w; ++x){
        ti = x;
        li = ti;
        ri = ti + r * stride;
        fv = source[ti];
        lv = source[ti + stride * (h - 1)];
        val = fv * (r + 1);
        for(int j = 0; j<r; ++j){
            val += source[ti + j * stride];
        }
        for(int j = 0; j <= r ; ++j){
            val += source[ri] - fv;
            buffer[ti] = val / iarr;// round(val * iarr);
            ri += stride;
            ti += stride;
        }
        for(int j = r + 1; j < h - r; ++j){
            val += source[ri] - source[li];
            buffer[ti] = val / iarr;// round(val * iarr);
            li += stride;
            ri += stride;
            ti += stride;
        }
        for(int j = h - r; j < h; ++j){
            val += lv - source[li];
            buffer[ti] = val / iarr;// round(val * iarr);
            li += stride;
            ti += stride;
        }
    }
}

//单通道处理
inline void boxBlur(byte_t* source, byte_t* buffer, int w, int h, int r)
{
    //int stride = w;
//    if(stride & 3)stride = (stride + 3) & ~3;
    //for(int i=0; i < w * h; i++) buffer[i] = source[i];
    //memcpy(buffer, source, stride * h);
//    boxBlurH(buffer, source, w, h, stride, r);
//    boxBlurT(source, buffer, w, h, stride, r);
    boxBlurH(source, buffer, w, h, w, r);
    boxBlurT(buffer, source, w, h, w, r);
}

void gaussBlur_4 (byte_t* source, int w, int h, int channels, int r)
{
    int boxes[3];
    boxesForGauss(boxes, 3, r);

    int stride = w * channels;
    if(stride & 3)stride = (stride + 3) & ~3;

    byte_t *image = new byte_t[w * h];
    byte_t *buffer = new byte_t[w * h];

    for(int i=0; i<channels; ++i){
        byte_t *dst_line = image;
        byte_t *src_line = source;
        for(int y=0; y < h; ++y){
            for(int x=0; x < w; ++x){
                dst_line[x] = src_line[x * channels + i];
            }
            dst_line += w;
            src_line += stride;
        }

        boxBlur(image, buffer, w, h, (boxes[0]-1)/2);
        boxBlur(image, buffer, w, h, (boxes[1]-1)/2);
        boxBlur(image, buffer, w, h, (boxes[2]-1)/2);

        dst_line = source;
        src_line = image;
        for(int y=0; y < h; ++y){
            for(int x=0; x < w; ++x){
                dst_line[x * channels + i] = src_line[x];
            }
            dst_line += stride;
            src_line += w;
        }
    }

    safe_delete(image);
    safe_delete(buffer);

//    boxBlur(source, buffer, w, h, (boxes[0]-1)/2);
//    boxBlur(source, buffer, w, h, (boxes[1]-1)/2);
//    boxBlur(source, buffer, w, h, (boxes[2]-1)/2);
}
*/
