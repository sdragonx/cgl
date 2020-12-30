/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gamma.hpp

 2020-11-28 03:30:27

*/
#ifndef GAMMA_HPP_20201128033027
#define GAMMA_HPP_20201128033027

#include <cgl/graphics/pixel/lookup_table.hpp>

namespace cgl{
namespace graphics{

//value > 0.0   必须大于0
//value < 1.0   颜色变暗
//value > 1.0   颜色变亮，6.0左右亮度已经很高，之后变化不大

template<typename T>
void gamma(void* data, int width, int height, int stride, float value)
{
    byte_t table[256];
    double temp;

    //最小值限定在0.1
    if(value < 0.1)value = 0.1;
    value = 1.0 / value;
    for(int i = 0; i < 256; ++i) {
        //这里的i/256.0可以去图像的灰度最大值
        temp = pow(i / 256.0, value);
        temp = temp * 256.0 - 0.5;
        table[i] = static_cast<int>(temp);
    }

    byte_t *image = (byte_t*)data;
    T *c;

    for(int y = 0; y < height; ++y){
        c = reinterpret_cast<T*>(image + stride * y);
        for(int x = 0; x < width; ++x){
            lookup_table<T>(*c, table);
            ++c;
        }
    }
}

}//end namespace graphics
}//end namespace cgl

#endif //GAMMA_HPP_20201128033027
