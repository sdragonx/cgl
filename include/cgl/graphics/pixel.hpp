/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pixel.hpp

 2020-02-15 00:11:00

*/
#ifndef PIXEL_HPP_20200215001100
#define PIXEL_HPP_20200215001100

#include <cgl/graphics/pixel/pixelbuffer.hpp>
#include <cgl/graphics/pixel/copy_image.hpp>
#include <cgl/graphics/pixel/for_each.hpp>
#include <cgl/graphics/pixel/stencil.hpp>

#include <cgl/graphics/effect/bright_contrast.hpp>
#include <cgl/graphics/effect/gamma.hpp>
#include <cgl/graphics/effect/gaussblur.hpp>


namespace cgl{

//set_point特化
template<>
void math::set_point<uint32_t>(
    graphics::pixelbuffer<uint32_t>& pixelbuf, int x, int y, uint32_t color)
{
    pixelbuf.set_pixel(x, y, color);
}

template<>
void math::set_point<uint32_t>(
    graphics::pixelbuffer<uint32_t>& pixelbuf, int x, int y, int alpha, uint32_t color)
{
    pixelbuf.blend(x, y, color, alpha);
}

namespace graphics{

template<typename T>
inline void draw_line(pixelbuffer<T>& pixelbuf, int x1, int y1, int x2, int y2, T color)
{
    math::smooth_line<uint32_t>(pixelbuf, x1, y1, x2, y2, color);
}

//绘制平行四边形
void parallelogram(pixelbuffer<uint32_t>& dest, int x1, int y1, int x2, int y2, int x3, int y3)
{
    int x4 = x1 + (x3 - x1) / 2;
    int y4 = y1 + (y3 - y1) / 2;
    x4 = x4 - (x2 - x4);
    y4 = y4 - (y2 - y4);

    math::smooth_line<uint32_t>(dest, x1, y1, x2, y2, 0x7f00ff00);
    math::smooth_line<uint32_t>(dest, x2, y2, x3, y3, 0x7f00ff00);
    math::smooth_line<uint32_t>(dest, x3, y3, x4, y4, 0x7f00ff00);
    math::smooth_line<uint32_t>(dest, x1, y1, x4, y4, 0x7f00ff00);
}

/*
//3x3卷积 Convolution
struct pixelmat
{
    vec3ub m[3][3];

    vec3ub get_pixel(pixelbuffer<vec3ub>& pixelbuf, int x, int y, int sx, int sy)
    {
        if(pixelbuf.is_invalid(x, y)){
            return pixelbuf[sy][sx];
        }
        else{
            return pixelbuf[y][x];
        }
    }

    void get(pixelbuffer<vec3ub>& pixelbuf, int x, int y)
    {
        for(int i=0; i<3; ++i){
            for(int j=0; j<3; ++j){
                m[i][j] = get_pixel(pixelbuf, x + j - 1, y + i - 1, x, y);
            }
        }
    }

    vec3ub max()
    {
        vec3ub c;
        for(int i=0; i<9; ++i){
            vec3ub& n = (&m[0][0])[i];
            if(RGB(c.r, c.g, c.b) < RGB(n.r, n.g, n.b)){
                c = n;
            }
        }
        return c;
    }

    vec3ub blur()
    {
        vec3i c;
        for(int i=0; i<9; ++i){
            vec3ub& n = (&m[0][0])[i];
            c += n;
        }
        return c;// / 9;
    }
};

//3x3过滤
int imagefilter(pixelbuffer<vec4ub>& obj, float mat[3][3], float total)
{
    vec4f t;
    vec4f c;
    float rev = 1.0f / total;
    for(int y = 1; y < obj.height() - 1; ++y){
        for(int x = 1; x < obj.width() - 1; ++x){
            t = obj[y-1][x-1]; c = t * mat[0][0];
            t = obj[y-1][x-0]; c += t * mat[0][1];
            t = obj[y-1][x+1]; c += t * mat[0][2];
            t = obj[y-0][x-1]; c += t * mat[1][0];
            t = obj[y-0][x-0]; c += t * mat[1][1];
            t = obj[y-0][x+1]; c += t * mat[1][2];
            t = obj[y+1][x-1]; c += t * mat[2][0];
            t = obj[y+1][x-0]; c += t * mat[2][1];
            t = obj[y+1][x+1]; c += t * mat[2][2];
            c *= rev;// /= total;
            obj[y][x] = math::clamp(c, 0.0f, 255.0f);
            //c = vec4i();
        }
    }
    return 0;
}
*/

inline void pixel_add(pixelbuffer<uint32_t>& pixelbuf, int x, int y, int n, int& r, int& g, int& b, int& a)
{
    uint32_t c = pixelbuf[y][x];
    r += n * RGBA_RED(c);
    g += n * RGBA_GREEN(c);
    b += n * RGBA_BLUE(c);
    a += n * RGBA_ALPHA(c);
}

//模糊一个像素
void blur_pixel(pixelbuffer<uint32_t>& pixelbuf, int x, int y)
{
    vec4f t;
    vec4f c;
    int total;
    int r = 0, g = 0, b = 0, a = 0;

    pixel_add(pixelbuf, x, y, 8, r, g, b, a);

    pixel_add(pixelbuf, x - 0, y - 1, 4, r, g, b, a);
    pixel_add(pixelbuf, x - 0, y + 1, 4, r, g, b, a);
    pixel_add(pixelbuf, x + 1, y - 0, 4, r, g, b, a);
    pixel_add(pixelbuf, x - 1, y - 0, 4, r, g, b, a);

    pixel_add(pixelbuf, x + 1, y - 1, 2, r, g, b, a);
    pixel_add(pixelbuf, x + 1, y + 1, 2, r, g, b, a);
    pixel_add(pixelbuf, x - 1, y - 1, 2, r, g, b, a);
    pixel_add(pixelbuf, x - 1, y + 1, 2, r, g, b, a);

    r /= 32;
    g /= 32;
    b /= 32;
    a /= 32;
    pixelbuf[y][x] = RGBA(byte_clamp(r), byte_clamp(g), byte_clamp(b), byte_clamp(a));
}

//亮度对比度
template<typename T>
void bright_contrast(pixelbuffer<T>& image, int brightness, int contrast)
{
    bright_contrast<T>(image.data(), image.width(), image.height(), image.stride(), brightness, contrast);
}

//gamma
template<typename T>
void gamma(pixelbuffer<T>& image, float value)
{
    gamma<T>(image.data(), image.width(), image.height(), image.stride(), value);
}

//高斯模糊
template<typename T>
inline void gauss_blur(pixelbuffer<T>& pixelbuf, float sigma)
{
    gauss_blur((byte_t*)pixelbuf.data(), pixelbuf.width(), pixelbuf.height(), sizeof(T), abs(pixelbuf.stride()), sigma);
}

}//end namespace graphics
}//end namespace cgl

#endif //PIXEL_HPP_20200215001100
