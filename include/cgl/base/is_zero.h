/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 is_zero.h

 2018-07-29 22:21:24

*/
#ifndef IS_ZERO_H_20180729222124
#define IS_ZERO_H_20180729222124

#include <float.h>

namespace cgl{

#ifndef CGL_IS_ZERO
#define CGL_IS_ZERO

template<typename T>
inline bool is_zero(const T& n)
{
    return n == 0;
}

template<>inline bool is_zero<float>(const float& n)
{
    //n > -FLT_EPSILON && n < FLT_EPSILON
    return n < 0.0f ? (n > -FLT_EPSILON) : (n < FLT_EPSILON);
}

template<>inline bool is_zero<double>(const double& n)
{
    return n < 0.0f ? (n > -DBL_EPSILON) : (n < DBL_EPSILON);
}

template<>inline bool is_zero<long double>(const long double& n)
{
    return n < 0.0f ? (n > -LDBL_EPSILON) : (n < LDBL_EPSILON);
}

#endif

}//end namespace cgl

#endif //IS_ZERO_H_20180729222124
