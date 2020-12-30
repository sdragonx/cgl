/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 model_circle.hpp

 2019-12-23 08:34:30

 球形内接正方体边长 = r * / sqrt(3.0) * 2.0

*/
#ifndef MODEL_CIRCLE_HPP_20191223083430
#define MODEL_CIRCLE_HPP_20191223083430

#include <cgl/public.h>

namespace cgl{
namespace model{

template<typename T>
class circle
{
public:
    typedef circle this_type;
    typedef T value_type;

    typedef vec2<T> point_type;

public:
    point_type center;
    value_type r;

public:

};

}//end namespace model
}//end namespace cgl

#endif //MODEL_CIRCLE_HPP_20191223083430
