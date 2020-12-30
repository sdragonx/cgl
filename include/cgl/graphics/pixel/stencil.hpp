/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stencil.hpp

 2020-11-28 03:37:09

*/
#ifndef STENCIL_HPP_20201128033709
#define STENCIL_HPP_20201128033709

#include <cgl/public.h>

namespace cgl{
namespace graphics{

//Ä£°å»º³å
template<typename T, size_t X, size_t Y>
class stencil
{
public:
    static T buffer[Y][X];

    bool is_invalid(int x, int y)const
    {
        return x < 0 || y < 0;
    }

    void reset()
    {
        memset(buffer, 0, sizeof(buffer));
    }

    void set(int x, int y, T value)
    {
        if(is_invalid(x, y)){
            return ;
        }
        buffer[y][x] = value;
    }

    void reset(int x1, int y1, int width, int height)
    {
        if(x1 < 0){
            width += x1;
            x1 = 0;
        }
        if(y1 < 0){
            height += y1;
            y1 = 0;
        }

        if(x1 + width > X){
            width = X - x1;
        }
        int y2 = y1 + height;
        if(y2 >= Y)y2 = Y;

        for(int y=y1; y<y2; ++y){
            memset(buffer[y] + x1, 0, width);
        }
    }

    //Ìî³äºáÏòÉ¨ÃèÏß
    void fill(int x1, int x2, int y, T value)
    {
        if(y < 0){
            return ;
        }
        if(x1 < 0){
            x1 = 0;
        }
        if(x2 < 0){
            x2 = 0;
        }
        memset(buffer[y] + x1, value, x2 - x1);
    }

    T* operator[](int y)
    {
        return buffer[y];
    }

    bool at(int x, int y)const
    {
        if(is_invalid(x, y)){
            return true;
        }
        return buffer[y][x] != 0;
    }
};

template<typename T, size_t X, size_t Y>
T stencil<T, X, Y>::buffer[X][Y];

}//end namespace graphics
}//end namespace cgl

#endif //STENCIL_HPP_20201128033709
