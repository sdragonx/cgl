/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 triangle.hpp

 2020-09-25 03:10:29

*/
#ifndef TRIANGLE_HPP_20200925031029
#define TRIANGLE_HPP_20200925031029

#include <cgl/public.h>
#include <cgl/vec.hpp>

namespace cgl{
namespace model{

template<typename T>
class triangle
{
public:
    typedef T point_type;

    point_type a, b, c;

public:
    triangle() : a(), b(), c()
    {
    }

    triangle(const point_type& v1, const point_type& v2, const point_type& v3) :
        a(v1), b(v2), c(v3)
    {
    }

};

}//end namespace model
}//end namespace cgl

#endif //TRIANGLE_HPP_20200925031029
