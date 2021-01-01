/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 numeric.hpp

 2020-09-25 02:19:51

*/
#ifndef NUMERIC_HPP_20200925021951
#define NUMERIC_HPP_20200925021951

#include <cgl/public.h>

namespace cgl{
namespace math{

//
// int sign(value)
//

//����T�ķ��� -1 0 1
template<typename T>
inline int sign(const T& n)
{
    return n > 0 ? 1 : (n < 0 ? -1 : 0);
}

//
// T abs(value)
//

//ȡ����ֵ
template<typename T>
inline T abs(T value)
{
    return value < 0 ? -value : value;
}

//
// half(value)
//

//ȡֵ��һ��
template<typename T>
inline T half(T value)
{
    return value >> 1;
}

template<>inline float half<float>(float value) { return value * 0.5f; }
template<>inline double half<double>(double value) { return value * 0.5; }
template<>inline long double half<long double>(long double value) { return value * 0.5; }

//
// T middle(x, y)
//

//ȡ��ֵ
template<typename T>
inline T middle(T x, T y)
{
    return x + half(y - x);
}

//
// T clamp(value, min, max)
//

//����ֵ����������
template<typename T>
inline T clamp(T value, T min, T max)
{
    return value < min ? min : (max < value ? max : value);
}

//
// T mix(x, y, a)
//

//��� a = 0 ~ 1
template<typename T>
inline T mix(T x, T y, T a)
{
    return x + (y - x) * a;
}

template<>inline float mix<float>(float x, float y, float a)
{
    return x + (y - x) * a;
}

template<>inline double mix<double>(double x, double y, double a)
{
    return x + (y - x) * a;
}

template<>inline long double mix<long double>(long double x, long double y, long double a)
{
    return x + (y - x) * a;
}

//
// T step(T source, T dest, T speed)
//

//��source�ݽ���dest������Ϊspeed������ֵ������dest
template<typename T>
T step(T source, T dest, T speed)
{
    if(source < dest){
        source += speed;
        if(source > dest){
            source = dest;
        }
    }
    else if(source > dest){
        source -= speed;
        if(source < dest){
            source = dest;
        }
    }
    return source;
}

//
// bool between(value, min, max)
//

//�ж���ֵ�Ƿ���A��B֮��
//min <= value < max
template<typename T>
inline bool between(T value, T min, T max)
{
    return min <= value && value < max;
}

//
// bool in_range(value, min, max)
//

//min <= value < max
template<typename T>
inline bool in_range(T value, T min, T max)
{
    return min <= value && value < max;
}

//template<typename T>
//inline bool in_range(T value, T max)
//{
//    return 0 <= value && value < max;
//}


}//end namespace math
}//end namespace cgl

#endif //NUMERIC_HPP_20200925021951