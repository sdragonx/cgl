/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pixel_for_each.hpp

 2020-04-04 19:03:47

*/
#ifndef PIXEL_FOR_EACH_HPP_20200404190347
#define PIXEL_FOR_EACH_HPP_20200404190347

#include <cgl/public.h>
#include <cgl/graphics/pixel/pixelbuffer.hpp>

namespace cgl{
namespace graphics{

//template<typename T>
//struct pixel_processor
//{
//    typedef void (*type)(T& color);
//};


//
// void for_each_pixel(pixelbuf, function)
//

//对图像的每一个像素进行操作
//template<typename T>
//inline void for_each_pixel(pixelbuffer<T>& pixelbuf, pixel_processor<T>::type pfn)
template<typename T, typename PFN>
inline void for_each_pixel(pixelbuffer<T>& pixelbuf, PFN pfn)
{
    for(int y = 0; y < pixelbuf.height(); ++y){
        for(int x = 0; x < pixelbuf.width(); ++x){
            pfn(pixelbuf[y][x]);
        }
    }
}

//
// void for_each_pixel(pixelbuf, function, arg)
//

template<typename T, typename PFN, typename ARG>
inline void for_each_pixel(pixelbuffer<T>& pixelbuf, PFN pfn, ARG arg)
{
    for(int y = 0; y < pixelbuf.height(); ++y){
        for(int x = 0; x < pixelbuf.width(); ++x){
            pfn(pixelbuf[y][x], arg);
        }
    }
}

//
// functions
//

template<typename T>
inline void pixel_reset(T& c)
{
    c = T();
}

//err
template<typename T>
inline void pixel_grayscale(T& c)
{
    int n = color_gray(c.r, c.g, c.b);
    c.r = c.b = c.g = n;
}

template<>
inline void pixel_grayscale<uint32_t>(uint32_t& c)
{
    int n = color_gray(BGRA_RED(c), BGRA_GREEN(c), BGRA_BLUE(c));
    c = BGRA(n, n, n, BGRA_ALPHA(c));
}

template<typename T>
inline void pixel_swap_red_blue(T& c)
{
    std::swap(c.red, c.blue);
}

template<>
inline void pixel_swap_red_blue<uint32_t>(uint32_t& c)
{
    RedBlueSwap(c);
}

inline void pixel_invert(vec3ub& c)
{
    c.r = ~c.r;
    c.g = ~c.g;
    c.b = ~c.b;
}

inline void pixel_invert(vec4ub& c)
{
    uint32_t* p = (uint32_t*)&c;
    *p = ~(*p);
}

//sepia效果
void pixel_effect_sepia(uint32_t& color)
{
    color = color_sepia(BGRA_RED(color), BGRA_GREEN(color), BGRA_BLUE(color), BGRA_ALPHA(color));
}


}//end namespace graphics
}//end namespace cgl

#endif //PIXEL_FOR_EACH_HPP_20200404190347
