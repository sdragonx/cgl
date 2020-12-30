/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 color.hpp

 2006.06.01 21:27

 revise 2006.06.20 23:02

Format               As byte-order           As word-order
byte endian          LE          BE          LE            BE
RGBA (byte-order)    RGBA8888    RGBA8888    ABGR32        RGBA32
ARGB (word-order)    BGRA8888    ARGB8888    ARGB32        ARGB32
RGBA (word-order)    ABGR8888    RGBA8888    RGBA32        RGBA32
ABGR (word-order)    RGBA8888    ABGR8888    ABGR32        ABGR32

gl shader
vec3f rgb2hsv(vec3ub c) {
    vec4f K = vec4f(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4f p = mix(vec4f(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));

    vec4f q = mix(vec4f(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);

    float e = 1.0e-10;
    vec3 hsv = vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);

    return hsv;

}

*/
#ifndef CGL_COLOR_HPP_200606012127
#define CGL_COLOR_HPP_200606012127

#include "color/base.hpp"
#include "color/colordef.hpp"

#if defined(CGL_PLATFORM_ANDROID)
    #include "color/colors_utf8.hpp"
#else
    #include "color/colors.hpp"
#endif

#include "color/convert.hpp"
#include "color/copy.hpp"

#ifdef CGL_PLATFORM_GNUC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
#endif

namespace cgl{
namespace graphics{

//把颜色值限制在byte_t范围内
inline byte_t byte_clamp(int n)
{
    return n < 0 ? 0 : (n > 255 ? 255 : n);
}

//
//像素加减操作不越界
//
inline byte_t byte_add(byte_t m, byte_t n)
{
    return BYTE_MAX - m < n ? BYTE_MAX : m + n;
}

inline byte_t byte_sub(byte_t m, byte_t n)
{
    return m < n ? 0 : m - n;
}

//---------------------------------------------------------------------------
//
// effect grayscale
//

inline int color_gray(int red, int green, int blue)
{
    //return (red * 77 + green * 150 + blue * 29) / 256;
    return (red * 306 + green * 601 + blue * 117) / 1024;
}

inline float color_gray(float red, float green, float blue)
{
    return red * 0.299f + green * 0.587f + blue * 0.114f;
}

inline int gray_weight(uint32_t c)
{
    return color_gray(BGRA_RED(c), BGRA_GREEN(c), BGRA_BLUE(c));
}

inline uint32_t grayscale(uint32_t c)
{
    int n = color_gray(BGRA_RED(c), BGRA_GREEN(c), BGRA_BLUE(c));
    return BGRA(n, n, n, BGRA_ALPHA(c));
}

inline vec3ub grayscale(vec3ub c)
{
    int n = color_gray(c.r, c.g, c.b);
    return vec3ub(n, n, n);
}

inline vec4ub grayscale(vec4ub c)
{
    int n = color_gray(c.r, c.g, c.b);
    return vec4ub(n, n, n, c.a);
}

//---------------------------------------------------------------------------
//
// sepia效果
//

uint32_t color_sepia(int r, int g, int b, int a)
{
    /*double Y = 0.299 * r + 0.587 * g + 0.114 * b;
    double I = 0.596 * r - 0.274 * g - 0.322 * b;
    double Q = 0.212 * r - 0.523 * g + 0.311 * b;
    I = 51;
    Q = 0;
    Data[i]   = (ILubyte)IL_MIN(255, 1.0 * Y + 0.956 * I + 0.621 * Q);
    Data[i+1] = (ILubyte)IL_MIN(255, 1.0 * Y - 0.272 * I - 0.647 * Q);
    Data[i+2] = (ILubyte)IL_MIN(255, 1.0 * Y - 1.105 * I + 1.702 * Q);*/
    r = byte_clamp(round(r * 0.393 + g * 0.769 + b * 0.189));
    g = byte_clamp(round(r * 0.349 + g * 0.686 + b * 0.168));
    b = byte_clamp(round(r * 0.272 + g * 0.534 + b * 0.131));
    return BGRA(r, g, b, a);
}

//---------------------------------------------------------------------------
//YCBCR

inline void rgb_to_ycbcr(const size_t& length,
    double* red, double* green, double* blue,
    double* y,   double* cb,    double* cr)
{
   size_t i = 0;

   while (i < length){
      ( *y) =   16.0 + (  65.481 * (*red) +  128.553 * (*green) +  24.966 * (*blue));
      (*cb) =  128.0 + ( -37.797 * (*red) +  -74.203 * (*green) + 112.000 * (*blue));
      (*cr) =  128.0 + ( 112.000 * (*red) +  -93.786 * (*green) -  18.214 * (*blue));

      ++i;
      ++red; ++green; ++blue;
      ++y;   ++cb;    ++cr;
   }
}

inline void ycbcr_to_rgb(const size_t& length,
    double* y,   double* cb,    double* cr,
    double* red, double* green, double* blue)
{
   size_t i = 0;

   while (i < length){
      double y_  =  (*y) -  16.0;
      double cb_ = (*cb) - 128.0;
      double cr_ = (*cr) - 128.0;

        (*red) = 0.000456621 * y_                    + 0.00625893 * cr_;
      (*green) = 0.000456621 * y_ - 0.00153632 * cb_ - 0.00318811 * cr_;
       (*blue) = 0.000456621 * y_                    + 0.00791071 * cb_;

      ++i;
      ++red; ++green; ++blue;
      ++y;   ++cb;    ++cr;
   }
}

//---------------------------------------------------------------------------
//windows以240为底
//HSV色彩模型 通过颜色RGB计算对应的HSV
//H: Hue        色度   360
//S: Saturation 饱和度 1
//V: Value      纯度   1
//---------------------------------------------------------------------------

double Hue2RGB(double v1, double v2, double vH)
{
    if (vH < 0) vH += 1;
    if (vH > 1) vH -= 1;
    if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
    if (2.0 * vH < 1) return v2;
    if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
    return (v1);
}

void RGB2HSL(uint32_t AColor, double &H, double &S, double &L)
{
    double R, G, B, Max, Min, del_R, del_G, del_B, del_Max;
    R = RGBA_RED(AColor) / 255.0;
    G = BGRA_GREEN(AColor) / 255.0;
    B = BGRA_BLUE(AColor) / 255.0;

    Min = std::min(R, std::min(G, B));
    Max = std::max(R, std::max(G, B));
    del_Max = Max - Min;

    L = (Max + Min) / 2.0;

    if (del_Max == 0)           //This is a gray, no chroma...
    {
        //H = 2.0/3.0;
        H = 0;
        S = 0;
    }
    else
    {
        if (L < 0.5) S = del_Max / (Max + Min);
        else         S = del_Max / (2 - Max - Min);

        del_R = (((Max - R) / 6.0) + (del_Max / 2.0)) / del_Max;
        del_G = (((Max - G) / 6.0) + (del_Max / 2.0)) / del_Max;
        del_B = (((Max - B) / 6.0) + (del_Max / 2.0)) / del_Max;

        if (R == Max) H = del_B - del_G;
        else if (G == Max) H = (1.0 / 3.0) + del_R - del_B;
        else if (B == Max) H = (2.0 / 3.0) + del_G - del_R;

        if (H < 0)  H += 1;
        if (H > 1)  H -= 1;
    }
}

void RGB2HSL(uint32_t AColor, int &H, int &S, int &L)
{
    double h, s, l;
    RGB2HSL(AColor, h, s, l);

    //round
    H = int(h*240.0f);
    S = int(s*240.0f);
    L = int(l*240.0f);
}

uint32_t HSL2RGB(double H, double S, double L)
{
    double R, G, B;
    double var_1, var_2;
    if (S == 0)
    {
        R = L * 255.0;
        G = L * 255.0;
        B = L * 255.0;
    }
    else
    {
        if (L < 0.5) var_2 = L * (1 + S);
        else         var_2 = (L + S) - (S * L);

        var_1 = 2.0 * L - var_2;

        R = 255.0 * Hue2RGB(var_1, var_2, H + (1.0 / 3.0));
        G = 255.0 * Hue2RGB(var_1, var_2, H);
        B = 255.0 * Hue2RGB(var_1, var_2, H - (1.0 / 3.0));
    }
    return RGBA(R, G, B, 255);
}

uint32_t HSL2RGB(int H, int S, int L)
{
    double h, s, l;
    h = H / 240.0;
    s = S / 240.0;
    l = L / 240.0;
    return HSL2RGB(h, s, l);
}

//---------------------------------------------------------------------------
//颜色混合

template<typename T>
inline T blend(const T& dest, const T& source);

template<typename T, typename A>
inline T blend(const T& dest, const T& source, A alpha);

//alpha混合
inline byte_t blend8(const byte_t& dest, const byte_t& source, const byte_t& alpha)
{
    return (((int(source)-int(dest))*alpha)>>8)+dest;
}

//RGBA BGRA
template<>
uint32_t blend<uint32_t>(uint32_t dest, uint32_t source)
{//((c1-c2)*alpha)/256;
    const static uint32_t mask = 0x00FF00FF;
    uint32_t alpha = source>>24;
    uint32_t c13 = ( ((source & mask)-(dest & mask)) * alpha) >> 8;
    uint32_t c24 = ( ((source >> 8) & mask) - ((dest >> 8) & mask) ) * alpha;
    c13 += dest & mask;
    c24 += dest &(~mask);
    return (c13 & mask) | ((c24 & (~mask)));
}

//32位颜色混合
template<>
uint32_t blend<uint32_t, uint32_t>(uint32_t dest, uint32_t source, uint32_t alpha)
{//dest=dest+(source-dest)*alpha;
    const static uint32_t mask = 0x00FF00FF;
    alpha *= RGBA_ALPHA(source);
    alpha >>= 8;
    uint32_t c13 = ( ((source & mask)-(dest & mask)) * alpha) >> 8;
    uint32_t c24 = ( ((source >> 8) & mask) - ((dest >> 8) & mask) ) * alpha;
    c13 += dest & mask;
    c24 += dest &(~mask);
    return (c13 & mask) | ((c24 & (~mask)));
}

template<>
inline vec4ub blend<vec4ub>(vec4ub dest, vec4ub source)
{
    uint32_t c = blend<uint32_t>(*((uint32_t*)&dest), *((uint32_t*)&source));
    return *((vec4ub*)&c);
}

template<>
inline vec4ub blend<vec4ub>(vec4ub dest, vec4ub source, uint32_t alpha)
{
    uint32_t c = blend<uint32_t>(*((uint32_t*)&dest), *((uint32_t*)&source), alpha);
    return *((vec4ub*)&c);
}

template<>
inline vec4f blend<vec4f>(vec4f& a, vec4f& b)
{
    vec4f c;
    c.r = math::mix(a.r, b.r, b.a);
    c.g = math::mix(a.g, b.g, b.a);
    c.b = math::mix(a.b, b.b, b.a);
    c.a = math::mix(a.a, b.a, b.a);
    return c;
}

template<>
inline vec4f blend<vec4f, float>(vec4f& a, vec4f& b, float alpha)
{
    vec4f c;
    c.r = math::mix(a.r, b.r, alpha);
    c.g = math::mix(a.g, b.g, alpha);
    c.b = math::mix(a.b, b.b, alpha);
    c.a = math::mix(a.a, b.a, alpha);
    return c;
}

#if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)

//速度总算是超过上面那个了
uint32_t blend32_asm(uint32_t c1, uint32_t c2)
{
    __asm{
        mov  eax,c2
        mov  ebx,c1
        mov  ecx,eax
        push eax
        and  eax,0x00FF00FF
        push ebx
        and  ebx,0x00FF00FF

        shr  ecx,24//提取alpha
        //c13
        sub  eax,ebx
        imul eax,ecx
        shr  eax,8
        add  eax,ebx
        and  eax,0x00FF00FF
        mov  edx,eax//保存结果

        //c24
        pop  ebx
        shr  ebx,8
        and  ebx,0x00FF00FF

        pop  eax
        shr  eax,8
        and  eax,0x00FF00FF

        sub  eax,ebx
        imul eax,ecx

        shl  ebx,8//&mask
        add  eax,ebx
        and  eax,0xFF00FF00

        or   eax,edx
        mov  c1,eax
    }
    return c1;
}

/* 64位，但是速度比32位的慢，可能是32位系统的原因
uint32_t blend32a(uint64_t c1, uint64_t c2, uint64_t alpha)
{//((c1-c2)*alpha)/256;
    const static uint64_t mask = 0x00FF00FF00FF00FF;
    uint64_t c64 = ( ((c1 & mask)-(c2 & mask)) | ( ((c1 >> 8) & mask) - ((c2 >> 8) & mask) )<<32 ) * alpha;
    //c64 *= alpha;
    c64 += c2 & mask | (c2&(~mask))<<32;
    return (c64 & mask) | ((c64 & (~mask)));
}
//*/

//mmx加速。处理一个像素
uint32_t AlphaBlend_MMX(uint32_t dest, uint32_t source)
{
    //mm0 dest, mm2 source, mm3 alpha, mm4 dstcopy
    __asm
    {
        pxor      mm7,mm7
        movd      mm0,dest
        movd      mm1,source
        punpcklbw mm0,mm7//展开
        punpcklbw mm1,mm7
        movq      mm3,mm1//alpha
        punpckhwd mm3,mm3
        punpckhdq mm3,mm3
        psubsw    mm1,mm0//source-dest
        pmullw    mm1,mm3//(source-dest)*alpha
        psllw     mm0,8  //dest*256
        paddw     mm0,mm1
        psrlw     mm0,8
        packuswb  mm0,mm0
        movd      dest,mm0
        emms
    }
    return dest;
}

//第二种算法，mmx可以并行处理2个像素的，这里只当一个消遣了
uint32_t blend32_mmx(uint32_t dest, uint32_t source)
{
    //mm0=dest, mm1=source, mm2=alpha, mm3=dest, mm4=source, mm7=0
    __asm{
        pxor       mm7,mm7    //把MM2清0
        movd       mm0,dest   //mm0 = 00 00 RG BA
        movd       mm1,source //mm1 = 00 00 RG BA
        punpcklbw  mm0,mm7    //mm0 = 0A 0B 0G 0R
        punpcklbw  mm1,mm7    //mm1 = 0A 0B 0G 0R
        movq       mm2,mm1    //mm2 = 0A 0B 0G 0R
        punpckhwd  mm2,mm2
        punpckhdq  mm2,mm2    //mm2 = 0A 0A 0A 0A
        movq       mm3,mm0    //dest->mm4
        movq       mm4,mm1    //source->mm5
        psubusw    mm3,mm1    //dest-source,饱和减,小于0为0
        psubusw    mm4,mm0    //source-dest,饱和减,小于0为0
        pmullw     mm3,mm2    //(dest-source)*alpha
        pmullw     mm4,mm2    //(source-dest)*alpha
        psrlw      mm3,8      //除以256,mm4=(source-dest)<0
        psrlw      mm4,8      //除以256,mm5=(dest-source)>0
        paddusw    mm0,mm4    //饱和加到原图象:D=Alpha*(O-S)+S,(source-dest)<0 部分
        psubusw    mm0,mm3    //饱和加到原图象D=S-Alpha*(S-O),(dest-source)>0 部分
        packuswb   mm0,mm0    //紧缩到低32bit
        movd dest, mm0
        emms //empty MMX Status
    }
    return dest;
}

//__declspec(align(64)) a;
//SSE加速，并行处理4个像素
int blend_sse128(__x128* dest, __x128* source)
{
    //xmm0=src13 xmm1=dst13 xmm2=src24 xmm3=dst24 xmm4=alpha xmm5= xmm6=mask xmm7=null
    __asm{
        pcmpeqw xmm6,xmm6//制作掩码，全置FF
        psrlw   xmm6,8   //00FF00FF...

        mov ecx,dest
        mov edx,source
        movupd xmm0,[edx]
        movapd xmm2,xmm0
        movupd xmm1,[ecx]
        movapd xmm3,xmm1

        movapd xmm4,xmm0//提取alpha 00A100A1 00A200A2 00A300A3 00A400A4
        psrld  xmm4,24
        movapd xmm5,xmm4
        pslld  xmm5,16
        por       xmm4,xmm5

        pand   xmm2,xmm6//c24
        pand   xmm3,xmm6
        psubw  xmm2,xmm3//source-dest
        pmullw xmm2,xmm4//*alpha
        psrlw  xmm2,8   //>>8
        paddw  xmm2,xmm3//+dest
        pand   xmm2,xmm6//and mask

        psrlw  xmm0,8    //c13
        psrlw  xmm1,8
        psubw  xmm0,xmm1//source-dest
        pmullw xmm0,xmm4//*alpha
        psllw  xmm1,8
        paddw  xmm0,xmm1//+dest
        psllw  xmm6,8    //~mask
        pand   xmm0,xmm6//and mask

        por xmm0,xmm2    //c13|c24

        movupd [ecx],xmm0//返回
    }
    return 0;
}

//15:29 2018/10/27
uint32_t fusionMMX(uint32_t dest, uint32_t source)
{
    __asm{
        pxor      mm7,mm7
        movd      mm0,dest
        movd      mm1,source
        punpcklbw mm0,mm7   //展开
        punpcklbw mm1,mm7
        pmullw    mm0,mm1   //相乘
        psrlw     mm0,8     // >>8
        packuswb  mm0,mm0   //打包
        movd      dest,mm0  //return
        emms
    }
    return dest;
}

#endif //__GNUC__

//---------------------------------------------------------------------------
//
// uint32_t fusion32(uint32_t , uint32_t)
//
// 颜色融合
//
// return (dest.r * source.r) | (dest.g * source.g) | (dest.b * source.b) | (dest.a * source.a)
//
// 15:29 2018/10/27
//
//---------------------------------------------------------------------------
inline uint32_t fusion32(uint32_t dest, uint32_t source)
{
    return (((dest & 0xFF) * (source & 0xFF)) >> 8)|
        ((((dest >> 8) & 0xFF) * ((source >> 8) & 0xFF)) & 0xFF00) |
        (((((dest >> 16) & 0xFF) * ((source >> 16) & 0xFF))<<8) & 0xFF0000) |
        (((((dest >> 24) & 0xFF) * ((source >> 24) & 0xFF))<<16) & 0xFF000000);
}


/*
create windows style colorpad
return color of rectangle's position

//paint a colorpad
for(int x=0; x < width; ++x)
{
    for(int y=0; y < height; ++y){
        uint32_t color = colorpad_color(width, height, x, y);
        draw_pixel(x, y, color);
    }
}
*/
uint32_t colorpad_color(int width, int height, int x, int y)
{
    static uint32_t color[7] = {
        0x000000FF,
        0x0000FFFF,
        0x0000FF00,
        0x00FFFF00,
        0x00FF0000,
        0x00FF00FF,
        0x000000FF
    };

    uint32_t i, alpha;
    uint32_t c1, c2;
    uint32_t half_h = height / 2;

    i = x * 6 / width;
    c1 = color[i];
    c2 = color[i+1];
    alpha = (x - i * width / 6) * 255 * 6 / width;
    if(alpha > 255)alpha = 255;
    c1 = blend<uint32_t>(c1, c2, alpha);

    if(uint32_t(y) < half_h){
        c2 = 0xFFFFFFFF;
        c2 = blend<uint32_t>(c2, c1, y * 255 / half_h);
    }
    else{
        c2 = 0xFF000000;
        c2 = blend<uint32_t>(c2, c1, (height - y - 1) * 255 / half_h);
    }
    return c2;
}

}//end namespace graphics
}//end namespace cgl

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#endif //CGL_COLOR_HPP_200606012127
