/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ripple.hpp

 2019-05-24 00:41:03

 水波特效，只能计算固定振幅的水波

*/
#ifndef RIPPLE_HPP_20190524004103
#define RIPPLE_HPP_20190524004103

#include <cgl/public.h>
#include <cgl/array.hpp>
#include <cgl/vec.hpp>


#ifdef CGL_EFFECT_RIPPLE_DEBUG
extern void draw_point(int dx, int dy, int sx, int sy);
extern void draw_point(int dx, int dy, int value);
const int ripple_size = 20;
#endif//CGL_EFFECT_RIPPLE_DEBUG

namespace cgl{
namespace graphics{

//数据处理速度：
//float    fps124
//int   fps300

template<typename T, int SIZE_X = 1024, int SIZE_Y = 1024>
class ripple
{
public:
    typedef T value_type;

    const static int BIT_OFFSET = 16; //fixed_point

private:
    //array2d<value_type> water[2];
    value_type water[2][SIZE_X][SIZE_Y];//这个最少快1/3
    vec2i size;
    //vec2<value_type> normal[SIZE_Y][SIZE_X];
    int front, back;

public:
    ripple() : front(0), back(1), size(SIZE_X, SIZE_Y)
    {
    }

    int width()const
    {
        return size.x;
    }

    int height()const
    {
        return size.y;
    }

    void resize(int x, int y)
    {
        //water[0].resize(x, y);
        //water[1].resize(x, y);
        size = vec2i(x, y);
        front = 0;
        back = 1;
    }

    value_type& front_buffer(int x, int y)
    {
        return water[front][y][x];
    }

    value_type& back_buffer(int x, int y)
    {
        return water[back][y][x];
    }

    inline void pack(value_type& n)
    {

    }

    inline void unpack(value_type& n)
    {

    }

    //波能衰减
    inline void attenuation(value_type& n)
    {
        //n *= 0.9f;
        //n -= n / 16;
        //n -= n / 32;
        //n -= n / 64;
        n -= n * (1.0f / 64.0f);
        //if(abs(n) < 256)n = 0;
    }

    void update_normal()
    {
        return ;
        int x, y;
        for(y=1; y<SIZE_Y; ++y)
        for(x=1; x<SIZE_X; ++x){
            //normal[y][x].x = front_buffer(x, y)-front_buffer(x-1, y);
            //normal[y][x].y = front_buffer(x, y)-front_buffer(x, y-1);
            //normal[y][x] = vec2<value_type>(front_buffer(x+1, y)-front_buffer(x-1, y), front_buffer(x, y+1)-front_buffer(x, y-1));

            //normal[y][x].normalize();
        }
    }

    void update()
    {
        int x, y;
        int w = size.x - 1;
        int h = size.y - 1;
        value_type n;

        for(y=1; y<h; ++y)
        for(x=1; x<w; ++x){
            //波能扩散
            n = ((front_buffer(x-1, y) + front_buffer(x+1, y) + front_buffer(x, y-1) + front_buffer(x, y+1)) / 2) - back_buffer(x, y);
            attenuation(n);
            back_buffer(x, y) = n;

            //normal[y][x].x = n-back_buffer(x-1, y);
            //normal[y][x].y = n-back_buffer(x, y-1);
        }


        //处理4个边
        y = 0;
        for(x = 1; x < w; x++) {
            n = (water[front][y][x - 1] + water[front][y][x + 1] + water[front][y+1][x]) / 2 - water[back][y][x];
            attenuation(n);
            back_buffer(x, y) = n;
        }

        y = h;
        for(x = 1; x < w; x++) {
            n = (water[front][y][x-1] + water[front][y][x+1] + water[front][y-1][x]) / 2 - water[back][y][x];
            attenuation(n);
            back_buffer(x, y) = n;
        }

        x = 0;
        for(y = 1; y < h; y++) {
            n = (water[front][y][x + 1] + water[front][y - 1][x] + water[front][y + 1][x]) / 2 - water[back][y][x];
            attenuation(n);
            back_buffer(x, y) = n;
        }

        x = w;
        for(y = 1; y < h; y++) {
            n = (water[front][y][x - 1] + water[front][y - 1][x] + water[front][y + 1][x]) / 2 - water[back][y][x];
            attenuation(n);
            back_buffer(x, y) = n;
        }

        //交换波能数据缓冲区
        std::swap(front, back);

        //update normal map
    }

    bool is_valid(int x, int y)
    {
        return x >=0 && y >=0 && x < width() && y < height();
    }

    //p=32~128
    //ripple
    void add(int x, int y, int p)
    {
        if(!is_valid(x, y)){
            return ;
        }
        water[front][y][x] = p;
    }

    void set_ripple(
        int x, int y,    //位置
        value_type amplitude,//振幅
        value_type length)//波长
    {
        if(!is_valid(x, y)){
            return ;
        }

        this->pack(length);

        int x_begin = x - amplitude;
        int x_end = x + amplitude;
        int y_begin = y - amplitude;
        int y_end = y + amplitude;

        if(x_begin < 0)x_begin = 0;
        if(x_end > size.x)x_end = size.x;
        if(y_begin < 0)y_begin = 0;
        if(y_end > size.y)y_end = size.y;

        const float phase = 0;
        float distance;
        float amount;
        value_type w, h;

        float r_size = 1.0f / amplitude;
        r_size *= 0.5f;
        r_size *= M_PI;

        for (int py=y_begin; py<y_end; ++py)
        for (int px=x_begin; px<x_end; ++px){
            w = px - x;
            w *= w;
            h = py - y;
            h *= h;
            distance = sqrt(float(w + h));
            if (distance < amplitude){
                amount = length - float(length) * sin(distance * r_size - phase);
                water[front][py][px] += amount;
            }
        }
    }

    #ifdef CGL_EFFECT_RIPPLE_DEBUG
    void paint()
    {
        DWORD color;
        vec2<value_type> v;
        int px, py;
        for(int y=1; y<size.y; ++y)
        for(int x=1; x<size.x; ++x){
            //v = normal[y][x];
            //normal值，只是简单的相邻像素计算
            v.x = front_buffer(x, y) - front_buffer(x - 1, y);
            v.y = front_buffer(x, y) - front_buffer(x, y - 1);

            v *= ripple_size;
            this->unpack(v.x);
            this->unpack(v.y);
            px = x + v.x;
            py = y + v.y;

            /*
            if(px < 0)px = -px;
            if(py < 0)py = -py;
            if(px >= width())px /= 32;
            if(py >= height())py /= 32;
            */

            /*
            if(px < 0)px = -px;
            if(py < 0)py = -py;
            if(px >= width())px = width() + width() - px - 1;
            if(py >= height())py = height() + height() - py - 1;
            //*/
            #ifdef CGL_EFFECT_RIPPLE_DEBUG_COLOR
            draw_point(x, y, px, py);
            #else
            draw_point(x, y, front_buffer(x, y));
            #endif
        }
    }
    #endif //CGL_EFFECT_RIPPLE_DEBUG
};

template<>
inline void ripple<int>::pack(int& n)
{
    n <<= ripple::BIT_OFFSET;
}

template<>
inline void ripple<int>::unpack(int& n)
{
    n >>= ripple::BIT_OFFSET;
}

template<>
inline void ripple<int>::attenuation(int& n)
{
    n -= n >> 6;
}

}//end namespace graphics
}//end namespace cgl

#endif //RIPPLE_HPP_20190524004103

