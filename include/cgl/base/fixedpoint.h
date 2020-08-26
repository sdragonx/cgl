/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fixedpoint.h

 2020-03-26 11:13:58

*/
#ifndef FIXEDPOINT_H_20200326111358
#define FIXEDPOINT_H_20200326111358

#include <cgl/public.h>

namespace cgl{

const int FP_SHIFT      = 16;
const int FP_HALF_SHIFT = FP_SHIFT / 2;
const int FP_ONE        = 1 << FP_SHIFT;
const int FP_MASK       = FP_ONE - 1;
const int FP_HALF       = FP_MASK >> 1;

inline int fp_set(int a)
{
    return a << FP_SHIFT;
}

inline int fp_get(int a)
{
    return a >> FP_SHIFT;
}

inline int fp_pack(int a)
{
    return a << FP_SHIFT;
}

inline int fp_unpack(int a)
{
    return a >> FP_SHIFT;
}

//倒数 正数
inline int fp_inv(int a)
{
    //a < FP_MASK == a < 0
    return a < FP_MASK ? a << FP_SHIFT : a >> FP_SHIFT;
}

//这个定点数的乘法貌似比浮点数还慢...
/*
inline int fp_mul(int a, int b)
{
    if(a < FP_MASK){
        if(b < FP_MASK){
            return (a * b) >> FP_SHIFT;
        }
        else{
            b >>= FP_HALF_SHIFT;
            return (a * b) >> FP_HALF_SHIFT;
        }
    }
    else{
        if(b < FP_MASK){
            a >>= FP_HALF_SHIFT;
            return (a * b) >> FP_HALF_SHIFT;
        }
        else{
            a >>= FP_HALF_SHIFT;
            b >>= FP_HALF_SHIFT;
            return a * b;
        }
    }
}
*/
inline int fp_mul(int a, int b)
{
    return a < FP_MASK ?
        (b < FP_MASK ? (a * b) >> FP_SHIFT : (a * (b >> FP_HALF_SHIFT)) >> FP_HALF_SHIFT) :
        (b < FP_MASK ? ((a >> FP_HALF_SHIFT) * b) >> FP_HALF_SHIFT : (a >> FP_HALF_SHIFT) * (b >> FP_HALF_SHIFT));
}

}//end namespace cgl

#endif //FIXEDPOINT_H_20200326111358