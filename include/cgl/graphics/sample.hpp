/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sample.hpp

 2018-06-03 11:07:34

*/
#ifndef SAMPLE_HPP_20180603110734
#define SAMPLE_HPP_20180603110734

#include <cgl/vec.hpp>

namespace cgl{
namespace graphics{

//
// sample_image(data[width][height], width, height)
//
// ºìÂÌ
// »ÆÀ¶
//
void sample_image(vec3ub* data, size_t width, size_t height)
{
    for(size_t y = 0; y < height; ++y)
    {
        for(size_t x = 0; x < width; ++x){
            if(x < width / 2){
                *data = y < height / 2 ? vec3ub(255, 0, 0) : vec3ub(255, 255, 0);
            }
            else{
                *data = y < height / 2 ? vec3ub(0, 255, 0) : vec3ub(0, 0, 255);
            }
            ++data;
        }
    }
}

//
// make_light(data[size][size], size)
//
// ¹âÕÕÍ¼
//
void make_light(vec4ub* data, size_t size)
{
    float r = size * 0.5f;
    float d;
    for(int y=0; y<64; ++y)
    for(int x=0; x<64; ++x)
    {
        d = sqrt((r-x)*(r-x) + (r-y)*(r-y));
        d = r - d;
        if(0 <= d){
            //d = 32-d;
            //d = d;
            //d = d * 1.5;
            if(d >=255)d = 255;
            if(d < 0)d = 0;
            //print(d);
            //c[y][x] = vec4ub(d, d, d, 255);
            //c[y][x] = vec4ub(255, 255, 255, d);
            data[y * size + x] = vec4ub(d, d, d, d);
            //c[y][x] = vec4ub(d, d, d, 0);
        }
    }
    //GL_LUMINANCE_ALPHA
    //return luna.gl.textures.create(64, 64, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, c[0][0].data);
}

}//end namespace graphics
}//end namespace cgl

#endif //SAMPLE_HPP_20180603110734
