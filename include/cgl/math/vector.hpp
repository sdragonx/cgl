/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vector.hpp

 2020-02-15 14:03:10

*/
#ifndef VECTOR_HPP_20200215140310
#define VECTOR_HPP_20200215140310

#include <cgl/public.h>
#include "numeric.hpp"

namespace cgl{
namespace math{

//
// dot(a, b)
//

template<typename T>
inline T dot(T x1, T y1, T x2, T y2)
{
    return x1 * x2 + y1 * y2;
}

template<typename T>
inline T dot(const vec2<T>& a, const vec2<T>& b)
{
    return dot(a.x, a.y, b.x, b.y);
}

//
// dot(a, b, c)
//

template<typename T>
inline T dot(T x1, T y1, T z1, T x2, T y2, T z2)
{
    return x1 * x2 + y1 * y2 + z1 * z2;
}

template<typename T>
inline T dot(const vec3<T>& a, const vec3<T>& b)
{
    return dot(a.x, a.y, a.z, b.x, b.y, b.z);
}

//
// distance(a, b)
//

template<typename T>
inline T distance(T x1, T y1, T x2, T y2)
{
    using namespace std;
    x2 -= x1;
    y2 -= y1;
    return sqrtf(dot<int>(x2, y2, x2, y2));
}

template<typename T>
inline T distance(const vec2<T>& a, const vec2<T>& b)
{
    return distance(a.x, a.y, b.x, b.y);
}

//
// cross(a, b)
//

template<typename T>
inline T cross(T x1, T y1, T x2, T y2)
{
    return x1 * y2 - y1 * x2;
}

template<typename T>
inline T cross(const vec2<T>& a, const vec2<T>& b)
{
    return cross(a.x, a.y, b.x, b.y);
}

//
// cross(a, b, c) == cross(b-a, c-b)
//

template<typename T>
inline T cross(T x1, T y1, T x2, T y2, T x3, T y3)
{
    return math::cross(x2 - x1, y2 - y1, x3 - x2, y3 - y2);
}

//*向量 ac 在 ab 的方向 顺时针是负？
template<typename T>
inline T cross(const vec2<T>& a, const vec2<T>& b, const vec2<T>& c)
{
    return cross(a.x, a.y, b.x, b.y, c.x, c.y);
}

template<typename T>
vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2)
{
    return vec3<T>(
        v1.y * v2.z - v1.z * v2.y, 
        v1.z * v2.x - v1.x * v2.z, 
        v1.x * v2.y - v1.y * v2.x);
}

//
// midpoint(a, b)
//

template<typename T>
inline vec2<T> midpoint(T x1, T y1, T x2, T y2)
{
    return vec2<T>(middle(x1, x2), middle(y1, y2));
}

template<typename T>
inline vec2<T> midpoint(const vec2<T>& a, const vec2<T>& b)
{
    return midpoint(a.x, a.y, b.x, b.y);
}

//
// midpoint(a, b, c)
//

//取三角形的中点
template<typename T>
vec2<T> midpoint(T x1, T y1, T x2, T y2, T x3, T y3)
{
    return vec2<T>(middle(middle(x1, x2), x3), middle(middle(y1, y2), y3));
}

//取三角形的中点
template<typename T>
vec2<T> midpoint(const vec2<T>& a, const vec2<T>& b, const vec2<T>& c)
{
    return midpoint(a.x, a.y, b.x, b.y, c.x, c.y);
}

//
// clamp()
//

template<typename T>
inline vec2<T> clamp(const vec2<T>& vec, T min, T max)
{
    T x = math::clamp(vec.x, min, max);
    T y = math::clamp(vec.y, min, max);
    return vec2<T>(x, y);
}

template<typename T>
inline vec3<T> clamp(const vec3<T>& vec, T min, T max)
{
    T x = math::clamp(vec.x, min, max);
    T y = math::clamp(vec.y, min, max);
    T z = math::clamp(vec.z, min, max);
    return vec3<T>(x, y, z);
}

template<typename T>
inline vec4<T> clamp(const vec4<T>& vec, T min, T max)
{
    T x = math::clamp(vec.x, min, max);
    T y = math::clamp(vec.y, min, max);
    T z = math::clamp(vec.z, min, max);
    T w = math::clamp(vec.w, min, max);
    return vec4<T>(x, y, z, w);
}

//
// mix(vmin, vmax, a)
//

template<typename T>
inline vec2<T> mix(const vec2<T>& vmin, const vec2<T>& vmax, T a)
{
    T x = math::mix(vmin.x, vmax.x, a);
    T y = math::mix(vmin.y, vmax.y, a);
    return vec2<T>(x, y);
}

template<typename T>
inline vec3<T> mix(const vec3<T>& vmin, const vec3<T>& vmax, T a)
{
    T x = math::mix(vmin.x, vmax.x, a);
    T y = math::mix(vmin.y, vmax.y, a);
    T z = math::mix(vmin.z, vmax.z, a);
    return vec3<T>(x, y, z);
}

template<typename T>
inline vec4<T> mix(const vec4<T>& vmin, const vec4<T>& vmax, T a)
{
    T x = math::mix(vmin.x, vmax.x, a);
    T y = math::mix(vmin.y, vmax.y, a);
    T z = math::mix(vmin.z, vmax.z, a);
    T w = math::mix(vmin.w, vmax.w, a);
    return vec4<T>(x, y, z, w);
}

//
// normalize(vec3)
//

template<typename T>
vec3<T> normalize(const vec3<T>& v)
{
	T m = 1.0f / (distance(v) + FLT_EPSILON);
	return vec3<T>(v.x * m, v.y * m, v.z * m);
}

//
// reflect(I, N)
//

//光照反射方向
//reflect(I, N) 计算I的反射方向, I-2*dot(N, I)*N, N必须先归一化
//template<typename T>
vec3f reflect(const vec3f& lightDir, const vec3f& norm)
{
	float n = dot(norm, lightDir) * 2;
	vec3f v = norm * n;
	return lightDir - v;
}

//三维点角度
//template <class T> double angle(const Vector3D<T>& v1, const Vector3D<T>& v2)
//{
//	return atan2(dot(normalize(cross(v1, v2)), cross(normalize(v1), normalize(v2))), dot(normalize(v1), normalize(v2)));
//
//}






}//end namespace math
}//end namespace cgl

#endif //VECTOR_HPP_20200215140310
