/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 convolution.hpp

 2020-10-27 01:49:59

*/
#ifndef CONVOLUTION_HPP_20201027014959
#define CONVOLUTION_HPP_20201027014959

#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/pixel/pixelbuffer.hpp>

namespace cgl{
namespace graphics{

template<typename T>
class convolution
{
public:
    typedef T value_type;
    typedef value_type kernel[3][3];

    //kernels
    const static kernel smooth;
    const static kernel gaussian;
    const static kernel sharpening;
    const static kernel findedge;
    const static kernel soble_h;
    const static kernel soble_v;
    const static kernel laplacian;

public:
    //计算卷积核总值
    value_type total_kernel(const kernel& m)
    {
        value_type n = 0;
        for (int y = 0; y < 3; ++y) {
            for(int x = 0; x < 3; ++x) {
                n += m[y][x];
            }
        }
        return n;
    }

    template<typename COLOR>
    int execute(pixelbuffer<COLOR>& out_image, const pixelbuffer<COLOR>& in_image, const kernel& mat)
    {
        #pragma warning pixelbuffer.stride()
        if(sizeof(COLOR) == 3){
            execute24<vec3ub>((byte_t*)out_image.data(), (const byte_t*)in_image.data(), in_image.width(), in_image.height(), -in_image.stride(), mat);
        }
        if(sizeof(COLOR) == 4){
            execute32<vec4ub>((byte_t*)out_image.data(), (const byte_t*)in_image.data(), in_image.width(), in_image.height(), -in_image.stride(), mat);
        }

        return 0;
    }

protected:
    template<typename COLOR>
    void execute24(byte_t* outBits, const byte_t* inBits, int width, int height, int stride, const kernel& mat)
    {
        //三条扫描线
        COLOR* line1;
        COLOR* line2;
        COLOR* line3;
        COLOR* outline;
        int x1, x2, x3;
        int y1, y2, y3;
        int r, g, b;
        int t = total_kernel(mat);

        for (y2 = 0; y2 < height; ++y2) {
            //扫描行，使用的是边界扩种法
            y1 = y2 - 1;
            if (y1 < 0)y1 = y2;
            y3 = y2 + 1;
            if (y3 == height)y3 = y2;
            line1 = (COLOR*)(inBits + stride * y1);
            line2 = (COLOR*)(inBits + stride * y2);
            line3 = (COLOR*)(inBits + stride * y3);
            outline = (COLOR*)(outBits + stride * y2);
            for (x2 = 0; x2 < width; ++x2) {
                x1 = x2 - 1;
                if (x1 < 0)x1 = x2;
                x3 = x2 + 1;
                if (x3 == width)x3 = x2;

                /*
                r = line1[x1].r * mat(0, 0) + line1[x2].r * mat(1, 0) + line1[x3].r * mat(2, 0) +
                    line2[x1].r * mat(0, 1) + line2[x2].r * mat(1, 1) + line2[x3].r * mat(2, 1) +
                    line3[x1].r * mat(0, 2) + line3[x2].r * mat(1, 2) + line3[x3].r * mat(2, 2);

                g = line1[x1].g * mat(0, 0) + line1[x2].g * mat(1, 0) + line1[x3].g * mat(2, 0) +
                    line2[x1].g * mat(0, 1) + line2[x2].g * mat(1, 1) + line2[x3].g * mat(2, 1) +
                    line3[x1].g * mat(0, 2) + line3[x2].g * mat(1, 2) + line3[x3].g * mat(2, 2);

                b = line1[x1].b * mat(0, 0) + line1[x2].b * mat(1, 0) + line1[x3].b * mat(2, 0) +
                    line2[x1].b * mat(0, 1) + line2[x2].b * mat(1, 1) + line2[x3].b * mat(2, 1) +
                    line3[x1].b * mat(0, 2) + line3[x2].b * mat(1, 2) + line3[x3].b * mat(2, 2);
                */

                r = line1[x1].r * mat[0][0] + line1[x2].r * mat[0][1] + line1[x3].r * mat[0][2] +
                    line2[x1].r * mat[1][0] + line2[x2].r * mat[1][1] + line2[x3].r * mat[1][2] +
                    line3[x1].r * mat[2][0] + line3[x2].r * mat[2][1] + line3[x3].r * mat[2][2];

                g = line1[x1].g * mat[0][0] + line1[x2].g * mat[0][1] + line1[x3].g * mat[0][2] +
                    line2[x1].g * mat[1][0] + line2[x2].g * mat[1][1] + line2[x3].g * mat[1][2] +
                    line3[x1].g * mat[2][0] + line3[x2].g * mat[2][1] + line3[x3].g * mat[2][2];

                b = line1[x1].b * mat[0][0] + line1[x2].b * mat[0][1] + line1[x3].b * mat[0][2] +
                    line2[x1].b * mat[1][0] + line2[x2].b * mat[1][1] + line2[x3].b * mat[1][2] +
                    line3[x1].b * mat[2][0] + line3[x2].b * mat[2][1] + line3[x3].b * mat[2][2];

                //写入目标图像
                if (t != 0) {
                    outline[x2].r = math::clamp(r / t, 0, 255);
                    outline[x2].g = math::clamp(g / t, 0, 255);
                    outline[x2].b = math::clamp(b / t, 0, 255);
                }
                else {
                    outline[x2].r = math::clamp(r, 0, 255);
                    outline[x2].g = math::clamp(g, 0, 255);
                    outline[x2].b = math::clamp(b, 0, 255);
                }
            }
        }
    }

    template<typename COLOR>
    void execute32(byte_t* outBits, const byte_t* inBits, int width, int height, int stride, const kernel& mat)
    {
        //三条扫描线
        COLOR* line1;
        COLOR* line2;
        COLOR* line3;
        COLOR* outline;
        int x1, x2, x3;
        int y1, y2, y3;
        int r, g, b, a;
        int t = total_kernel(mat);

        for (y2 = 0; y2 < height; ++y2) {
            //扫描行，使用的是边界扩种法
            y1 = y2 - 1;
            if (y1 < 0)y1 = y2;
            y3 = y2 + 1;
            if (y3 == height)y3 = y2;
            line1 = (COLOR*)(inBits + stride * y1);
            line2 = (COLOR*)(inBits + stride * y2);
            line3 = (COLOR*)(inBits + stride * y3);
            outline = (COLOR*)(outBits + stride * y2);
            for (x2 = 0; x2 < width; ++x2) {
                x1 = x2 - 1;
                if (x1 < 0)x1 = x2;
                x3 = x2 + 1;
                if (x3 == width)x3 = x2;

                /*
                r = line1[x1].r * mat(0, 0) + line1[x2].r * mat(1, 0) + line1[x3].r * mat(2, 0) +
                    line2[x1].r * mat(0, 1) + line2[x2].r * mat(1, 1) + line2[x3].r * mat(2, 1) +
                    line3[x1].r * mat(0, 2) + line3[x2].r * mat(1, 2) + line3[x3].r * mat(2, 2);

                g = line1[x1].g * mat(0, 0) + line1[x2].g * mat(1, 0) + line1[x3].g * mat(2, 0) +
                    line2[x1].g * mat(0, 1) + line2[x2].g * mat(1, 1) + line2[x3].g * mat(2, 1) +
                    line3[x1].g * mat(0, 2) + line3[x2].g * mat(1, 2) + line3[x3].g * mat(2, 2);

                b = line1[x1].b * mat(0, 0) + line1[x2].b * mat(1, 0) + line1[x3].b * mat(2, 0) +
                    line2[x1].b * mat(0, 1) + line2[x2].b * mat(1, 1) + line2[x3].b * mat(2, 1) +
                    line3[x1].b * mat(0, 2) + line3[x2].b * mat(1, 2) + line3[x3].b * mat(2, 2);

                a = line1[x1].a * mat(0, 0) + line1[x2].a * mat(1, 0) + line1[x3].a * mat(2, 0) +
                    line2[x1].a * mat(0, 1) + line2[x2].a * mat(1, 1) + line2[x3].a * mat(2, 1) +
                    line3[x1].a * mat(0, 2) + line3[x2].a * mat(1, 2) + line3[x3].a * mat(2, 2);
                */
                r = line1[x1].r * mat[0][0] + line1[x2].r * mat[0][1] + line1[x3].r * mat[0][2] +
                    line2[x1].r * mat[1][0] + line2[x2].r * mat[1][1] + line2[x3].r * mat[1][2] +
                    line3[x1].r * mat[2][0] + line3[x2].r * mat[2][1] + line3[x3].r * mat[2][2];

                g = line1[x1].g * mat[0][0] + line1[x2].g * mat[0][1] + line1[x3].g * mat[0][2] +
                    line2[x1].g * mat[1][0] + line2[x2].g * mat[1][1] + line2[x3].g * mat[1][2] +
                    line3[x1].g * mat[2][0] + line3[x2].g * mat[2][1] + line3[x3].g * mat[2][2];

                b = line1[x1].b * mat[0][0] + line1[x2].b * mat[0][1] + line1[x3].b * mat[0][2] +
                    line2[x1].b * mat[1][0] + line2[x2].b * mat[1][1] + line2[x3].b * mat[1][2] +
                    line3[x1].b * mat[2][0] + line3[x2].b * mat[2][1] + line3[x3].b * mat[2][2];

                a = line1[x1].a * mat[0][0] + line1[x2].a * mat[0][1] + line1[x3].a * mat[0][2] +
                    line2[x1].a * mat[1][0] + line2[x2].a * mat[1][1] + line2[x3].a * mat[1][2] +
                    line3[x1].a * mat[2][0] + line3[x2].a * mat[2][1] + line3[x3].a * mat[2][2];

                //写入目标图像
                if (t != 0) {
                    outline[x2].r = math::clamp(r / t, 0, 255);
                    outline[x2].g = math::clamp(g / t, 0, 255);
                    outline[x2].b = math::clamp(b / t, 0, 255);
                    outline[x2].a = math::clamp(b / t, 0, 255);
                }
                else {
                    outline[x2].r = math::clamp(r, 0, 255);
                    outline[x2].g = math::clamp(g, 0, 255);
                    outline[x2].b = math::clamp(b, 0, 255);
                    outline[x2].a = math::clamp(b, 0, 255);
                }
            }
        }
    }
};

//平滑，平均值
template<typename T>
const convolution<T>::kernel convolution<T>::
smooth = {
    1,1,1,
    1,1,1,
    1,1,1
};

//高斯模糊
template<typename T>
const convolution<T>::kernel convolution<T>::
gaussian = (
    1,2,1,
    2,4,2,
    1,2,1
);

//锐化
template<typename T>
const convolution<T>::kernel convolution<T>::
sharpening = {
    -1,-1,-1,
    -1, 9,-1,
    -1,-1,-1
};

//查找边缘
template<typename T>
const convolution<T>::kernel convolution<T>::
findedge = {
    0, 0, 0,
    0, 1, 0,
    0, 0,-1
};

//soble
//a45=[-2 -1 0;-1 0 1;0 1 2]; 45度
//b45=[0 -1 -2;1 0 -1;2 1 0]; 135度

//Soble边缘检测，水平
template<typename T>
const convolution<T>::kernel convolution<T>::
soble_h = {
    1, 0, -1,
    2, 0, -2,
    1, 0, -1
};

//Soble边缘检测，垂直
template<typename T>
const convolution<T>::kernel convolution<T>::
soble_v = {
     1,  2,  1,
     0,  0,  0,
    -1, -2, -1
};

//拉普拉斯算子
//0,  1, 0
//1, -4, 1
//0,  1, 0

//梯度Laplacian
template<typename T>
const convolution<T>::kernel convolution<T>::
laplacian = {
    1,  1,  1,
    1, -8,  1,
    1,  1,  1
};

}//end namespace graphics
}//end namespace cgl

#endif //CONVOLUTION_HPP_20201027014959
