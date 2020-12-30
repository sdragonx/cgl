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

//value > 0.0   �������0
//value < 1.0   ��ɫ�䰵
//value > 1.0   ��ɫ������6.0���������Ѿ��ܸߣ�֮��仯����

template<typename T>
void gamma(void* data, int width, int height, int stride, float value)
{
    byte_t table[256];
    double temp;

    //��Сֵ�޶���0.1
    if(value < 0.1)value = 0.1;
    value = 1.0 / value;
    for(int i = 0; i < 256; ++i) {
        //�����i/256.0����ȥͼ��ĻҶ����ֵ
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
