/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pixel_traits.hpp

 2020-12-16 05:44:16

*/
#ifndef PIXEL_TRAITS_HPP_20201216054416
#define PIXEL_TRAITS_HPP_20201216054416

#include <cgl/graphics/color.hpp>

#define PIXEL_FIXED_BITS    8
#define PIXEL_FIXED_ONE     (1<<PIXEL_FIXED_BITS)
#define PIXEL_FIXED_MASK    (PIXEL_FIXED_ONE - 1)

namespace cgl{
namespace graphics{

template<typename T>
struct pixel_traits
{
    typedef T (*getter)(const pixelbuffer<T>&, int x, int y);
    typedef void (*setter)(pixelbuffer<T>&, int x, int y, T source);
    typedef T (*function)(const T& dest, const T& source);
};



//
// 取色器
//

// nearest_color 临近取值取色器
template<typename T>
inline T nearest_color(const pixelbuffer<T>& pixelbuf, int x, int y)
{
    return pixelbuf[y>>PIXEL_FIXED_BITS][x>>PIXEL_FIXED_BITS];
}

// bilinear_color 双线插值取色器
// xy   定点整形坐标
uint32_t bilinear_color(const pixelbuffer<uint32_t>& pixelbuf, int x, int y)
{
    uint32_t u = x >> PIXEL_FIXED_BITS;
    uint32_t v = y >> PIXEL_FIXED_BITS;
    if(u >= uint32_t(pixelbuf.width()) || v >= uint32_t(pixelbuf.height())){
        return 0;
    }

    byte_t *p00 = (byte_t*)&pixelbuf[v][u];
    byte_t *p01 = p00;
    byte_t *p10 = p00;
    byte_t *p11 = p00;
    if(u + 1 < uint32_t(pixelbuf.width())){//检测x过界
        p01 += 4;
        p11 += 4;
    }
    if(v + 1 < uint32_t(pixelbuf.height())){//检测y过界
        p10 += pixelbuf.stride();
        p11 += pixelbuf.stride();
    }

    u = x & PIXEL_FIXED_MASK;
    v = y & PIXEL_FIXED_MASK;
    uint32_t m11 = (u * v) >> PIXEL_FIXED_BITS;
    uint32_t m01 = u - m11;
    uint32_t m10 = v - m11;
    uint32_t m00 = PIXEL_FIXED_ONE - m01 - m10 - m11;

    uint32_t r, g, b, a;
    b = p00[0] * m00 + p01[0] * m01 + p10[0] * m10 + p11[0] * m11;
    b >>= PIXEL_FIXED_BITS;
    g = p00[1] * m00 + p01[1] * m01 + p10[1] * m10 + p11[1] * m11;
    g >>= PIXEL_FIXED_BITS;
    r = p00[2] * m00 + p01[2] * m01 + p10[2] * m10 + p11[2] * m11;
    r >>= PIXEL_FIXED_BITS;
    a = p00[3] * m00 + p01[3] * m01 + p10[3] * m10 + p11[3] * m11;
    a >>= PIXEL_FIXED_BITS;
    return RGBA(r, g, b, a);
}



//
// 混合程序
//

template<typename T>
inline T dest_one(const T& dest, const T& source)
{
    return dest;
}

template<typename T>
inline T source_one(const T& dest, const T& source)
{
    return source;
}

template<typename T>
inline T blend_copy(const T& dest, const T& source)
{
    return source;
}

template<typename T>
inline T blend_and(const T& dest, const T& source)
{
    return dest & source;
}

template<typename T>
inline T blend_or(const T& dest, const T& source)
{
    return dest | source;
}

template<typename T>
inline T blend_not(const T& dest, const T& source)
{
    return ~dest;
}

template<typename T>
inline T source_alpha(const T& dest, const T& source)
{
    return blend<T>(dest, source);
}


}//end namespace graphics
}//end namespace cgl

#endif //PIXEL_TRAITS_HPP_20201216054416
