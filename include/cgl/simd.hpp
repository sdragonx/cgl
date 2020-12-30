/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 simd.hpp

 2018-10-31 17:08:01

 beta

*/
#ifndef SIMD_HPP_20181031170801
#define SIMD_HPP_20181031170801

#include <cgl/public.h>

#if defined(CGL_PLATFORM_GNUC) || defined(CGL_PLATFORM_CLANG) || defined(CGL_PLATFORM_MSVC)

#ifndef __MMX__
    //#define __MMX__
#endif
#ifndef __SSE__
    //#define __SSE__
#endif

#include <mmintrin.h>
#include <xmmintrin.h>

#else

class __m128
{
public:
    union{
        float m[4];
        struct{
            float m0, m1, m2, m3;
        };
    };

public:
    __m128()
    {

    }

    __m128(float a, float b, float c, float d) : m0(a), m1(b), m2(c), m3(d)
    {

    }

    float& operator[](int i)
    {
        return m[i];
    }
};

#endif

namespace cgl{

#ifndef __XMMINTRIN_H

//速度没有c代码快...

void mm_addps(__m128& __a, __m128& __b)
{
    __asm{
//        mov ecx,__a.m
//        mov edx,__b.m
        movupd xmm0,[ecx]
        movupd xmm1,[edx]
        addps  xmm0,xmm1
        movupd [ecx],xmm0
    }
}

void mm_mulps(__m128& __a, __m128& __b)
{
    __asm{
        mov ecx,__a.m
        mov edx,__b.m
        movupd xmm0,[ecx]
        movupd xmm1,[edx]
        mulps  xmm0,xmm1
        movups [ecx],xmm0
    }
}

void mul4f(float a[4], float b)
{
    __asm{
        mov eax, a
        movups xmm0,[eax]
        movss  xmm1,b
        shufps xmm1,xmm1,0  //xmm1[123] = xmm1[0]
        mulps  xmm0,xmm1
        movups [eax],xmm0
    }
}

// 快速SSE实现
float sqrtSSE(float x)
{
    __asm
    {
        movss   xmm0, x
        rsqrtss xmm0, xmm0
        rcpss   xmm0, xmm0
        movss   x, xmm0
    }
    return x;
}

/*
//很慢，release报错
void mix_sse(float x[4], float y[4], float a)
{
    __asm{
        mov eax, x
        mov ebx, y
        movups xmm0,[eax]
        movups xmm1,[ebx]
        movss  xmm2,a
        shufps xmm2,xmm2,0  //xmmX[123] = xmmX[0]
        subps  xmm1,xmm0
        mulps  xmm1,xmm2
        addps  xmm0,xmm1
        movups [eax],xmm0
    }
}

void mm_movaps(__x128* dst, __x128* src)
{
    __asm{
        mov ecx,dst
        mov edx,src
        movapd xmm0,[edx]
        movapd xmm1,[ecx]
//        movapd xmm0,xmmword ptr [dst]
//        movapd xmm1,xmmword ptr [src]
    }
}

void mm_movups(__x128* dst, __x128* src)
{
    __asm{
        mov ecx,dst
        mov edx,src
        movupd xmm0,[edx]
        movupd xmm1,[ecx]
//        movapd xmm0,xmmword ptr [dst]
//        movapd xmm1,xmmword ptr [src]
    }
}

void mm_movaps(__x128* dst)
{
    __asm{
        movupd [ecx],xmm0//return
    }
}

void mm_movups(__x128* dst)
{
    __asm{
        movupd [ecx],xmm0//return
    }
}

void mm_addps()
{
    __asm{
        addps  xmm0, xmm1
    }
}

void mm_subps()
{
    __asm{
        subps  xmm0, xmm1
    }
}

void mm_mulps()
{
    __asm{
        mulps  xmm0, xmm1
    }
}

void mm_divps()
{
    __asm{
        divps  xmm0, xmm1
    }
}

*/

#endif//__XMMINTRIN_H

}//end namespace cgl

#endif //SIMD_HPP_20181031170801
