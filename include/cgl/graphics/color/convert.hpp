                                                                                /*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 color_convert.hpp

 2020-04-11 12:47:41

*/
#ifndef COLOR_CONVERT_HPP_20200411124741
#define COLOR_CONVERT_HPP_20200411124741

#include <cgl/public.h>
#include "colordef.hpp"

namespace cgl{
namespace graphics{

template<typename DST, typename SRC>
inline void color_convert(DST& dest, const SRC& source)
{
    dest = source;
}

//
// RGB565 << any
//

template<>
void color_convert<rgb565_t, rgb24_t>(rgb565_t& dest, const rgb24_t& source)
{
     dest.r = source.r << 3;
     dest.g = source.g << 3;
     dest.b = source.b << 3;
}

template<>
inline void color_convert<rgb565_t, bgr24_t>(rgb565_t& dest, const bgr24_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
}

template<>
inline void color_convert<rgb565_t, rgba32_t>(rgb565_t& dest, const rgba32_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
}

template<>
inline void color_convert<rgb565_t, bgra32_t>(rgb565_t& dest, const bgra32_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
}


//
// rgb24_t << any
//

template<>
inline void color_convert<rgb24_t, rgb565_t>(rgb24_t& dest, const rgb565_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
}

template<>
inline void color_convert<rgb24_t, bgr24_t>(rgb24_t& dest, const bgr24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

template<>
inline void color_convert<rgb24_t, rgba32_t>(rgb24_t& dest, const rgba32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

template<>
inline void color_convert<rgb24_t, bgra32_t>(rgb24_t& dest, const bgra32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

//
// bgr24_t << any
//

template<>
inline void color_convert<bgr24_t, rgb565_t>(bgr24_t& dest, const rgb565_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
}

template<>
inline void color_convert<bgr24_t, rgb24_t>(bgr24_t& dest, const rgb24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

template<>
inline void color_convert<bgr24_t, rgba32_t>(bgr24_t& dest, const rgba32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

template<>
inline void color_convert<bgr24_t, bgra32_t>(bgr24_t& dest, const bgra32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
}

//
// rgba32_t << any
//

template<>
inline void color_convert<rgba32_t, rgb565_t>(rgba32_t& dest, const rgb565_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
    dest.a = 255;
}

template<>
inline void color_convert<rgba32_t, rgb24_t>(rgba32_t& dest, const rgb24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = 255;
}

template<>
inline void color_convert<rgba32_t, bgr24_t>(rgba32_t& dest, const bgr24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = 255;
}

template<>
inline void color_convert<rgba32_t, bgra32_t>(rgba32_t& dest, const bgra32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = source.a;
}

//
// bgra32_t << any
//

template<>
inline void color_convert<bgra32_t, rgb565_t>(bgra32_t& dest, const rgb565_t& source)
{
    dest.r = source.r << 3;
    dest.g = source.g << 3;
    dest.b = source.b << 3;
    dest.a = 255;
}

template<>
inline void color_convert<bgra32_t, rgb24_t>(bgra32_t& dest, const rgb24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = 255;
}

template<>
inline void color_convert<bgra32_t, bgr24_t>(bgra32_t& dest, const bgr24_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = 255;
}

template<>
inline void color_convert<bgra32_t, rgba32_t>(bgra32_t& dest, const rgba32_t& source)
{
    dest.r = source.r;
    dest.g = source.g;
    dest.b = source.b;
    dest.a = source.a;
}

}//end namespace graphics
}//end namespace cgl

#endif //COLOR_CONVERT_HPP_20200411124741
