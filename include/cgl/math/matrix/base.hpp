/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 matrix.base.hpp

 2020-10-27 02:05:52

*/
#ifndef MATRIX_BASE_HPP_20201027020552
#define MATRIX_BASE_HPP_20201027020552

#include <cgl/public.h>

namespace cgl{

template<typename T, size_t CX, size_t CY>
class matrix
{
public:
    typedef T value_type;

    const static size_t X = CX;
    const static size_t Y = CY;

public:
    union{
        T data[X * Y];
        T m[Y][X];
    };

public:
    T* operator[](size_t i)
    {
        return m[i];
    }

    T& operator()(size_t x, size_t y)
    {
        return m[y][x];
    }

    const T& operator()(size_t x, size_t y)const
    {
        return m[y][x];
    }
};

}//end namespace cgl

#endif //MATRIX_BASE_HPP_20201027020552
