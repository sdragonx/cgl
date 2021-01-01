/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 is_zero.h

 2018-07-29 22:21:24

*/
#ifndef IS_ZERO_H_20180729222124
#define IS_ZERO_H_20180729222124

#include <float.h>

namespace cgl{

template<typename T>
class numeric_zero
{
public:
    static inline bool is_zero(const T& value)
    {
        return value == 0;
    }
};

template<>
inline bool numeric_zero<float>::is_zero(const float& n)
{
    //n > -FLT_EPSILON && n < FLT_EPSILON
    return n < 0.0f ? (n > -FLT_EPSILON) : (n < FLT_EPSILON);
}

template<>
inline bool numeric_zero<double>::is_zero(const double& n)
{
    return n < 0.0f ? (n > -DBL_EPSILON) : (n < DBL_EPSILON);
}

template<>
inline bool numeric_zero<long double>::is_zero(const long double& n)
{
    return n < 0.0f ? (n > -LDBL_EPSILON) : (n < LDBL_EPSILON);
}


#ifndef CGL_IS_ZERO
#define CGL_IS_ZERO

template<typename T>
inline bool is_zero(const T& value)
{
    return numeric_zero<T>::is_zero(value);
}

#endif

}//end namespace cgl

#endif //IS_ZERO_H_20180729222124
