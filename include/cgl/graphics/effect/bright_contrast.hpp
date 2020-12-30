/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 bright_contrast.hpp

 2020-11-28 08:43:04

*/
#ifndef BRIGHT_CONTRAST_HPP_20201128084304
#define BRIGHT_CONTRAST_HPP_20201128084304

#include <cgl/graphics/pixel/lookup_table.hpp>

namespace cgl{
namespace graphics{

#if 0

//非线性亮度调整（Phoposhop CS3以下版本是这种亮度调整方式，CS3及以上版本保留了该亮度调整方式的选项）
//bright    亮度
//contrast  对比度
//threshold 128
void bright_contrast(void *data, int width, int height, int stride, int channels,
    int brightness, int contrast, int threshold)
{
    int table[256];

    float cv = contrast <= -255? -1.0f : contrast / 255.0f;
    if (contrast > 0 && contrast < 255){
        cv = 1.0f / (1.0f - cv) - 1.0f;
    }

    for (int i = 0; i < 256; ++i){
        int v = contrast > 0 ? byte_clamp(i + bright) : i;
        if (contrast >= 255){
            v = v >= threshold ? 255 : 0;
        }
        else{
            v = byte_clamp(v + int((v - threshold) * cv + 0.5f));
        }
        table[i] = contrast <= 0 ? byte_clamp(v + bright) : v;
    }

    byte_t *image = (byte_t*)data;
    byte_t *c;

    width *= channels;
    for (int y = 0; y < height; ++y){
        c = image + stride * y;
        for (int x = 0; x < width; ++x){
            *c = table[*c];
            ++c;
        }
    }
}

#else

template<typename T>
void bright_contrast(void *data, int width, int height, int stride,
    int brightness, int contrast)
{
    byte_t table[256];

    double B = brightness / 255.;
    double C = contrast / 255. ;
    double K = tan( (45 + 44 * C) / 180 * M_PI );

    for (int i = 0; i < 256; i++){
        table[i] = byte_clamp( (i - 127.5 * (1 - B)) * K + 127.5 * (1 + B) );
    }

    byte_t *image = (byte_t*)data;
    T *c;

    for (int y = 0; y < height; ++y){
        c = reinterpret_cast<T*>(image + stride * y);
        for (int x = 0; x < width; ++x){
            lookup_table<T>(*c, table);
            ++c;
        }
    }
}

#endif

}//end namespace graphics
}//end namespace cgl

#endif //BRIGHT_CONTRAST_HPP_20201128084304
