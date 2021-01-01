/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 color_copy.hpp

 2020-02-17 22:33:48

*/
#ifndef COLOR_COPY_HPP_20200217223348
#define COLOR_COPY_HPP_20200217223348

#include "colordef.hpp"

namespace cgl{
namespace graphics{

#ifndef CGL_PLATFORM_CB6

template<typename T>
int color_copy(T* dest, T* source, size_t size)
{
    return memcpy(dest, source, sizeof(T) * size);
}

//
// RGB565 << any
//

int color_copy(rgb565_t* dest, rgb24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

int color_copy(rgb565_t* dest, bgr24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

int color_copy(rgb565_t* dest, rgba32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

int color_copy(rgb565_t* dest, bgra32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

//
// rgb24_t << any
//

int color_copy(rgb24_t* dest, rgb565_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

int color_copy(rgb24_t* dest, bgr24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

int color_copy(rgb24_t* dest, rgba32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

int color_copy(rgb24_t* dest, bgra32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

//
// bgr24_t << any
//

int color_copy(bgr24_t* dest, rgb565_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
    }
    return size;
}

int color_copy(bgr24_t* dest, rgb24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

int color_copy(bgr24_t* dest, rgba32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

int color_copy(bgr24_t* dest, bgra32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
    }
    return size;
}

//
// rgba32_t << any
//

int color_copy(rgba32_t* dest, rgb565_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(rgba32_t* dest, rgb24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(rgba32_t* dest, bgr24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(rgba32_t* dest, bgra32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = source[i].a;
    }
    return size;
}

//
// bgra32_t << any
//

int color_copy(bgra32_t* dest, rgb565_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r << 3;
        dest[i].g = source[i].g << 3;
        dest[i].b = source[i].b << 3;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(bgra32_t* dest, rgb24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(bgra32_t* dest, bgr24_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = 255;
    }
    return size;
}

int color_copy(bgra32_t* dest, rgba32_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i].r = source[i].r;
        dest[i].g = source[i].g;
        dest[i].b = source[i].b;
        dest[i].a = source[i].a;
    }
    return size;
}

#endif

}//end namespace graphics
}//end namespace cgl

#endif //COLOR_COPY_HPP_20200217223348
