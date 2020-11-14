/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vectype.h

 base class

 2015-09-09 20:32:28

 revise 2018/8/10 8:51

*/
#ifndef CGL_VECTYPE_H_20150909203228
#define CGL_VECTYPE_H_20150909203228

#include <algorithm>
#include <cmath>

#ifdef CGL_PUBLIC_H
    #include <cgl/public.h>
#else
    #include <stdint.h>
    #include <float.h>
    namespace cgl{
        //CGL_ALIGN
        enum{
            CGL_LEFT        = 0x01,
            CGL_RIGHT       = 0x02,
            CGL_HORIZONTAL  = 0x03,    //horizontal水平居中
            CGL_TOP         = 0x04,
            CGL_BOTTOM      = 0x08,
            CGL_VERTICAL    = 0x0C,    //vertical垂直居中
            CGL_CENTER      = 0x0F,    //居中
            CGL_CLIENT      = 0xF0     //填充区域
        };
    }
#endif

#ifndef M_PI
    #define M_PI 3.141592653589793238462
#endif

#ifndef M_RD
    #define M_RD 0.017453292519943295769
#endif

#ifdef CGL_VECTOR_FAST_CAST
    #include <cgl/fast_cast.hpp>
    #define VEC_FAST_CAST(T,n) cgl::fast_cast<T>(n)
#else
    #define VEC_FAST_CAST(T,n) (n)
#endif

namespace cgl{

#ifndef CGL_IS_ZERO
#define CGL_IS_ZERO
template<typename T> inline bool is_zero(const T& n) { return n == 0; }
template<> inline bool is_zero<float>(const float& n) { return n < 0.0f ? (n > -FLT_EPSILON) : (n < FLT_EPSILON); }
template<> inline bool is_zero<double>(const double& n) { return n < 0.0f ? (n > -DBL_EPSILON) : (n < DBL_EPSILON); }
template<> inline bool is_zero<long double>(const long double& n) { return n < 0.0f ? (n > -LDBL_EPSILON) : (n < LDBL_EPSILON); }
#endif//CGL_IS_ZERO

#ifndef CGL_IS_EQUAL
#define CGL_IS_EQUAL
template<typename T> inline bool is_equal(const T& a, const T& b) { return a == b; }
template<> inline bool is_equal(const float& a, const float& b) { return is_zero(b - a); }
template<> inline bool is_equal(const double& a, const double& b) { return is_zero(b - a); }
template<> inline bool is_equal(const long double& a, const long double& b) { return is_zero(b - a); }
#else
template<typename T> extern inline bool is_equal(const T& a, const T& b);
#endif//CGL_IS_EQUAL


template<typename T>
inline T absT(const T& value)
{
    return abs(value);
}

template<>inline float absT<float>(const float& value) { return std::fabs(value); }
template<>inline double absT<double>(const double& value) { return std::fabs(value); }
template<>inline long double absT<long double>(const long double& value) { return std::fabs(value); }

//vector operation
#define VEC2_OPERATION(op)\
    template<typename X>\
    this_type operator op(const X& value)const\
    {\
        return this_type(x op value, y op value);\
    }\
    template<typename X>\
    this_type operator op(const vec2<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y);\
    }\
    template<typename X>\
    const this_type& operator op##=(const X& value)\
    {\
        x op##= value; y op##= value;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec2<X>& v)\
    {\
        x op##= v.x; y op##= v.y;\
        return *this;\
    }

#define VEC3_OPERATION(op)\
    template<typename X>\
    this_type operator op(const X& value)const\
    {\
        return this_type(x op value, y op value, z op value);\
    }\
    template<typename X>\
    this_type operator op(const vec2<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y, z);\
    }\
    template<typename X>\
    this_type operator op(const vec3<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y, z op v.z);\
    }\
    template<typename X>\
    const this_type& operator op##=(const X& value)\
    {\
        x op##= value; y op##= value; z op##= value;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec2<X>& v)\
    {\
        x op##= v.x; y op##= v.y;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec3<X>& v)\
    {\
        x op##= v.x; y op##= v.y; z op##= v.z;\
        return *this;\
    }

#define VEC4_OPERATION(op)\
    template<typename X>\
    this_type operator op(const X& value)const\
    {\
        return this_type(x op value, y op value, z op value, w op value);\
    }\
    template<typename X>\
    this_type operator op(const vec2<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y, z, w);\
    }\
    template<typename X>\
    this_type operator op(const vec3<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y, z op v.z, w);\
    }\
    template<typename X>\
    this_type operator op(const vec4<X>& v)const\
    {\
        return this_type(x op v.x, y op v.y, z op v.z, w op v.w);\
    }\
    template<typename X>\
    const this_type& operator op##=(const X& value)\
    {\
        x op##= value; y op##= value; z op##= value; w op##= value;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec2<X>& v)\
    {\
        x op##= v.x; y op##= v.y;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec3<X>& v)\
    {\
        x op##= v.x; y op##= v.y; z op##= v.z;\
        return *this;\
    }\
    template<typename X>\
    const this_type& operator op##=(const vec4<X>& v)\
    {\
        x op##= v.x; y op##= v.y; z op##= v.z; w op##= v.w;\
        return *this;\
    }


#define VEC2_OPERATOR_COMMON(op)\
    template<typename T>\
    vec2<T> operator op (T n, const vec2<T>& vec)\
    {\
        return vec2<T>(n op vec.x, n op vec.y);\
    }

#define VEC3_OPERATOR_COMMON(op)\
    template<typename T>\
    vec3<T> operator op (T n, const vec3<T>& vec)\
    {\
        return vec3<T>(n op vec.x, n op vec.y, n op vec.z);\
    }

#define VEC4_OPERATOR_COMMON(op)\
    template<typename T>\
    vec4<T> operator op (T n, const vec4<T>& vec)\
    {\
        return vec4<T>(n op vec.x, n op vec.y, n op vec.z, n op vec.w);\
    }

//end vector operation

#pragma pack(push, 1)

template<typename T>class vec2;
template<typename T>class vec3;
template<typename T>class vec4;

template<typename T>
class vec2
{
public:
    typedef vec2<T> this_type;
    typedef T value_type;
    typedef value_type& refrence;
    typedef const value_type& const_refrence;

    union{
        value_type data[2];
        struct{ value_type x, y; };
        struct{ value_type u, v; };
        struct{ value_type s, t; };
        struct{ value_type w, h; };
        struct{ value_type width, height; };
        struct{ value_type begin, end; };
        struct{ value_type min, max; };
    };
public:
    vec2():x(), y(){ /*void*/ }

    vec2(const_refrence vx, const_refrence vy) : x(vx), y(vy) { /*void*/ }

    template<typename X>vec2(const vec2<X>& vec) : x(VEC_FAST_CAST(T, vec.x)), y(VEC_FAST_CAST(T, vec.y)) { /*void*/ }
    template<typename X>vec2(const vec3<X>& vec) : x(VEC_FAST_CAST(T, vec.x)), y(VEC_FAST_CAST(T, vec.y)) { /*void*/ }
    template<typename X>vec2(const vec4<X>& vec) : x(VEC_FAST_CAST(T, vec.x)), y(VEC_FAST_CAST(T, vec.y)) { /*void*/ }

    this_type& operator=(const_refrence value)
    {
        x = y = value;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec2<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec3<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec4<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        return *this;
    }

    template<typename X>
    this_type& set(X vx, X vy)
    {
        x = VEC_FAST_CAST(T, vx);
        y = VEC_FAST_CAST(T, vy);
        return *this;
    }

    bool is_null()const { return is_zero<value_type>(x) && is_zero<value_type>(y); }

    this_type operator -()const { return this_type(-x, -y); }

    bool operator==(const this_type& vec)const
    {
        return !this->is_not_equal(vec);
    }
    bool operator!=(const this_type& vec)const
    {
        return this->is_not_equal(vec);
    }

    bool operator<(const this_type& vec)const
    {
        return x < vec.x || y < vec.y;
    }

    refrence operator[](size_t n) { return data[n]; }
    const_refrence operator[](size_t n)const { return data[n]; }

    VEC2_OPERATION(+)
    VEC2_OPERATION(-)
    VEC2_OPERATION(*)
    VEC2_OPERATION(/)

private:
    bool is_not_equal(const this_type& vec)const
    {
        return !is_equal(x, vec.x) || !is_equal(y, vec.y);
    }

public:
    this_type absolute()const
    {
        return this_type(absT(x), absT(y));
    }

    value_type length()const
    {
        return sqrt(static_cast<double>(this->dot()));
    }

    value_type distance(const_refrence vx, const_refrence vy)const
    {
        return this_type(x - vx, y - vy).length();
    }

    value_type distance(const this_type& vec)const
    {
        return this->operator-(vec).length();
    }

    const this_type& offset(const_refrence vx, const_refrence vy)
    {
        x += vx;
        y += vy;
        return *this;
    }

    const this_type& offset(const this_type& vec)
    {
        return this->operator+=(vec);
    }

    //判断是否在矩形区域内
    bool inside(const_refrence a_x, const_refrence a_y, const_refrence a_width, const_refrence a_height)const
    {
        return !(x < a_x || y < a_y || x >= a_x + a_width || y >= a_y + a_height);
    }

    bool inside(const vec4<value_type>& rect)const
    {
        return this->inside(rect.x, rect.y, rect.width, rect.height);
    }

    //约束到矩形区域
    this_type& clamp(const_refrence a_x, const_refrence a_y, const_refrence a_width, const_refrence a_height)
    {
        if(x < a_x)
            x = a_x;
        else if(x >= a_x + a_width)
            x = a_x + a_width - 1;

        if(y < a_y)
            y = a_y;
        else if(y >= a_y + a_height)
            y = a_y + a_height - 1;

        return *this;
    }

    this_type& clamp(const vec4<value_type>& rect)
    {
        return this->clamp(rect.x, rect.y, rect.width, rect.height);
    }

    this_type midpoint(const_refrence vx, const_refrence vy)const
    {
        return this_type(x + (vx - x) / 2, y + (vy - y) / 2);
    }

    this_type midpoint(const this_type& vec)const
    {
        return this->midpoint(vec.x, vec.y);
    }

    //inner product内积
    value_type dot()const
    {
        return x * x + y * y;
    }

    //dot product 点积
    value_type dot(const this_type& vec)const
    {
        return x * vec.x + y * vec.y;
    }

    //cross product 乘积
    value_type cross(const this_type& vec)const
    {
        return x * vec.y - y * vec.x;
    }

    this_type& normalize()
    {
        float m = 1.0f / (this->length() + FLT_EPSILON);
        x *= m;
        y *= m;
        return *this;
    }

    this_type& rotate(float angle)
    {
        angle *= M_RD;
        float sine = sin(angle);
        float cosine = cos(angle);
        return this->set(
            x * cosine - y * sine,
            y * cosine + x * sine);
    }
};

template<typename T>
class vec3
{
public:
    typedef vec3<T> this_type;
    typedef T value_type;
    typedef value_type& refrence;
    typedef const value_type& const_refrence;

    union{
        value_type data[3];
        struct{ value_type x, y, z; };
        struct{ value_type r, g, b; };
        struct{ value_type red, green, blue; };
        struct{ value_type begin, end, current; };
    };
public:
    vec3() : x(), y(), z(){ /*void*/ }

    vec3(const_refrence vx, const_refrence vy, const_refrence vz = value_type())
        : x(vx), y(vy), z(vz) { /*void*/ }

    template<typename X>
    vec3(const vec2<X>& vec, const_refrence vz = value_type()) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vz)) { /*void*/}

    template<typename X> vec3(const vec3<X>& vec) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vec.z)) { /*void*/ }

    template<typename X> vec3(const vec4<X>& vec) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vec.z)) { /*void*/ }

    this_type& operator=(const_refrence value)
    {
        x = y = z = value;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec2<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = 0;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec3<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = VEC_FAST_CAST(T, vec.z);
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec4<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = VEC_FAST_CAST(T, vec.z);
        return *this;
    }

    template<typename X>
    this_type& set(X vx, X vy, X vz)
    {
        x = VEC_FAST_CAST(T, vx);
        y = VEC_FAST_CAST(T, vy);
        z = VEC_FAST_CAST(T, vz);
        return *this;
    }

    bool is_null()const { return is_zero(x) && is_zero(y) && is_zero(z); }

    this_type operator -()const { return this_type(-x, -y, -z); }

    bool operator==(const this_type& vec)const
    {
        return !this->is_not_equal(vec);
    }

    bool operator!=(const this_type& vec)const
    {
        return this->is_not_equal(vec);
    }

    bool operator<(const this_type& vec)const
    {
        return x < vec.x || y < vec.y || z < vec.z;
        //return x < vec.x || (x == vec.x && y < vec.y) || (x == vec.x && y == vec.y && z < vec.z);
    }

    refrence operator[](size_t n)
    {
        return data[n];
    }

    const_refrence operator[](size_t n)const
    {
        return data[n];
    }

    VEC3_OPERATION(+)
    VEC3_OPERATION(-)
    VEC3_OPERATION(*)
    VEC3_OPERATION(/)

private:
    bool is_not_equal(const this_type& vec)const
    {
        return !cgl::is_equal<value_type>(x, vec.x) ||
               !cgl::is_equal<value_type>(y, vec.y) ||
               !cgl::is_equal<value_type>(z, vec.z);
    }

public:
    this_type absolute()const
    {
        return this_type(absT(x), absT(y), absT(z));
    }

    value_type length()const
    {
        return sqrt(static_cast<double>(this->dot()));
    }

    value_type distance(const_refrence vx, const_refrence vy)const
    {
        return this_type(x-vx, y-vy).length();
    }

    value_type distance(const_refrence vx, const_refrence vy, const_refrence vz)const
    {
        return this_type(x-vx, y-vy, z-vz).length();
    }

    value_type distance(const vec2<value_type>& vec)const
    {
        return this->operator-(vec).length();
    }

    value_type distance(const this_type& vec)const
    {
        return this->operator-(vec).length();
    }

    const this_type& offset(const_refrence vx, const_refrence vy)
    {
        x += vx;
        y += vy;
        return *this;
    }

    const this_type& offset(const_refrence vx, const_refrence vy, const_refrence vz)
    {
        x += vx;
        y += vy;
        z += vz;
        return *this;
    }

    const this_type& offset(const vec2<value_type>& vec)
    {
        return this->operator+=(vec);
    }

    const this_type& offset(const this_type& vec)
    {
        return this->operator+=(vec);
    }

    this_type midpoint(const_refrence vx, const_refrence vy, const_refrence vz)const
    {
        return this_type(x + (vx - x) / 2, y + (vy - y) / 2, z + (vz - z) / 2);
    }

    this_type midpoint(const this_type& vec)const
    {
        return this->midpoint(vec.x, vec.y, vec.z);
    }

    //inner product
    value_type dot()const
    {
        return x * x + y * y + z * z;
    }

    //dot product
    value_type dot(const this_type& vec)const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    //cross product
    this_type cross(const this_type& vec)const
    {
        return this_type(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    //归一化
    this_type& normalize()
    {
        float m = 1.0f/(length() + FLT_EPSILON);
        x *= m;
        y *= m;
        z *= m;
        return *this;
    }

    this_type& rotate(value_type angle, value_type vx, value_type vy, value_type vz)
    {
        float theta = angle * M_RD;
        float cosine = cos(theta);
        float sine = sin(theta);
        float even_cos = 1.0f - cosine;
        vec3<float> u(vx, vy, vz);
        u.normalize();

        this_type p;

        p.x = x * (cosine + (u.x * u.x * even_cos)) +
              y * ((u.x * u.y * even_cos) - (u.z * sine)) +
              z * ((u.x * u.z * even_cos) + (u.y * sine));

        p.y = x * ((u.y * u.x * even_cos) + (u.z * sine)) +
              y * (cosine + (u.y * u.y * even_cos)) +
              z * ((u.y * u.z * even_cos) - (u.x * sine));

        p.z = x * ((u.z * u.x * even_cos) - (u.y * sine)) +
              y * ((u.z * u.y * even_cos) + (u.x * sine)) +
              z * (cosine + (u.z * u.z * even_cos));

        return this->operator=(p);
    }

    this_type& rotateX(value_type angle)
    {
        float theta = angle * M_RD;
        float cosine = cos(theta);
        float sine = sin(theta);
        return this->set(
            x,
            y * cosine - z * sine,
            y * sine + z * cosine);
    }

    this_type& rotateY(value_type angle)
    {
        float theta = angle * M_RD;
        float cosine = cos(theta);
        float sine = sin(theta);
        return this->set(
            x * cosine + z * sine,
            y,
            -x * sine + z * cosine);
    }

    this_type& rotateZ(value_type angle)
    {
        float theta = angle * M_RD;
        float cosine = cos(theta);
        float sine = sin(theta);
        return this->set(
            x * cosine - y * sine,
            x * sine + y * cosine,
            z);
    }
};

template<typename T>
class vec4
{
public:
    typedef vec4<T> this_type;
    typedef T value_type;
    typedef value_type& refrence;
    typedef const value_type& const_refrence;

    union{
        value_type data[4];
        struct{ value_type x, y, z, w; };
        struct{ value_type s, t, p, q; };
        struct{ value_type r, g, b, a; };
        struct{ value_type red, green, blue, alpha;  };
        struct{ value_type u, v, width, height;      };    //rect
        struct{ value_type left, top, right, bottom; };    //bounds
        struct{ value_type amin, bmin, amax, bmax;   };    //aabb
        struct{ value_type xmin, ymin, xmax, ymax;   };    //aabb
    };
public:
    vec4() : x(), y(), z(), w(){ /*void*/ }

    vec4(const_refrence vx, const_refrence vy,
        const_refrence vz = value_type(),
        const_refrence vw = value_type())
        : x(vx), y(vy), z(vz), w(vw) { /*void*/ }

    template<typename X>
    vec4(const vec2<X>& vec, const_refrence vz = value_type(), const_refrence vw = value_type()) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vz)),
        w(VEC_FAST_CAST(T, vw)) { /*void*/ }

    template<typename X>
    vec4(const vec2<X>& v1, const vec2<X>& v2) :
        x(VEC_FAST_CAST(T, v1.x)),
        y(VEC_FAST_CAST(T, v1.y)),
        z(VEC_FAST_CAST(T, v2.x)),
        w(VEC_FAST_CAST(T, v2.y)) { /*void*/ }

    template<typename X>
    vec4(const vec3<X>& vec, const_refrence vw = value_type()) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vec.z)),
        w(VEC_FAST_CAST(T, vw)) { /*void*/ }

    template<typename X>
    vec4(const vec4<X>& vec) :
        x(VEC_FAST_CAST(T, vec.x)),
        y(VEC_FAST_CAST(T, vec.y)),
        z(VEC_FAST_CAST(T, vec.z)),
        w(VEC_FAST_CAST(T, vec.w)) { /*void*/ }

    this_type& operator=(const_refrence value)
    {
        x = y = z = w = value;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec2<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = 0;
        w = 0;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec3<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = VEC_FAST_CAST(T, vec.z);
        w = 0;
        return *this;
    }

    template<typename X>
    this_type& operator=(const vec4<X>& vec)
    {
        x = VEC_FAST_CAST(T, vec.x);
        y = VEC_FAST_CAST(T, vec.y);
        z = VEC_FAST_CAST(T, vec.z);
        w = VEC_FAST_CAST(T, vec.w);
        return *this;
    }

    template<typename X>
    this_type& set(X vx, X vy, X vz, X vw)
    {
        x = VEC_FAST_CAST(T, vx);
        y = VEC_FAST_CAST(T, vy);
        z = VEC_FAST_CAST(T, vz);
        w = VEC_FAST_CAST(T, vw);
        return *this;
    }

    bool is_null()const { return is_zero(x) && is_zero(y) && is_zero(z) && is_zero(w); }

    this_type operator -()const { return this_type(-x, -y, -z, -w); }

    bool operator==(const this_type& vec)const
    {
        return !this->is_not_equal(vec);
    }

    bool operator!=(const this_type& vec)const
    {
        return this->is_not_equal(vec);
    }

    bool operator<(const this_type& vec)const
    {
        return x < vec.x || y < vec.y || z < vec.z || w < vec.w;
    }

    refrence operator[](size_t n)
    {
        return data[n];
    }

    const_refrence operator[](size_t n)const
    {
        return data[n];
    }

    VEC4_OPERATION(+)
    VEC4_OPERATION(-)
    VEC4_OPERATION(*)
    VEC4_OPERATION(/)

private:
    bool is_not_equal(const this_type& vec)const
    {
        return !is_equal<value_type>(x, vec.x) ||
               !is_equal<value_type>(y, vec.y) ||
               !is_equal<value_type>(z, vec.z) ||
               !is_equal<value_type>(w, vec.w);
    }

public://vector
    this_type absolute()const
    {
        return this_type(absT(x), absT(y), absT(z), absT(w));
    }

    value_type length()const
    {
        return sqrt(static_cast<double>((this->dot())));
    }

    value_type distance(const_refrence vx, const_refrence vy)
    {
        return this_type(x-vx, y-vy).length();
    }

    value_type distance(const_refrence vx, const_refrence vy,
        const_refrence vz)const
    {
        return this_type(x-vx, y-vy, z-vz).length();
    }

    value_type distance(const_refrence vx, const_refrence vy,
        const_refrence vz, const_refrence vw)const
    {
        return this_type(x-vx, y-vy, z-vz, w-vw).length();
    }

    value_type distance(const vec2<value_type>& vec)const
    {
        return this->operator-(vec).length();
    }

    value_type distance(const vec3<value_type>& vec)const
    {
        return this->operator-(vec).length();
    }

    value_type distance(const this_type& vec)const
    {
        return this->operator-(vec).length();
    }

    const this_type& offset(const_refrence vx, const_refrence vy)
    {
        x += vx;
        y += vy;
        return *this;
    }

    const this_type& offset(const_refrence vx, const_refrence vy, const_refrence vz)
    {
        x += vx;
        y += vy;
        z += vz;
        return *this;
    }

    const this_type& offset(const_refrence vx, const_refrence vy, const_refrence vz, const_refrence vw)
    {
        x += vx;
        y += vy;
        z += vz;
        w += vw;
        return *this;
    }

    const this_type& offset(const vec2<value_type>& vec)
    {
        return this->operator+=(vec);
    }

    const this_type& offset(const vec3<value_type>& vec)
    {
        return this->operator+=(vec);
    }

    const this_type& offset(const this_type& vec)
    {
        return this->operator+=(vec);
    }

    this_type midpoint(const_refrence vx, const_refrence vy,
        const_refrence vz, const_refrence vw)const
    {
        return this_type(x + (vx - x) / 2, y + (vy - y) / 2, z + (vz - z) / 2, w + (vw - w) / 2);
    }

    this_type midpoint(const this_type& vec)const
    {
        return this->midpoint(vec.x, vec.y, vec.z);
    }

    //inner product
    value_type dot()const
    {
        return x * x + y * y + z * z + w * w;
    }

    //dot product
    value_type dot(const this_type& vec)const
    {
        return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
    }

    //cross product
    value_type cross(const this_type& vec)const
    {
        return this_type(vec3<value_type>(x, y, z).cross_product(vec.x, vec.y, vec.z), w);
    }

    this_type& normalize()
    {
        float n = x * x + y * y + z * z + w * w;
        // Already normalized.
        if (is_equal<float>(n, 1.0f))
            return *this;

        n = sqrt(n);
        // Too close to zero.
        if (n < FLT_EPSILON)
            return *this;

        n = 1.0f / n;
        x *= n;
        y *= n;
        z *= n;
        w *= n;
        return *this;
    }

    //透视除法
    this_type& perspective_division()
    {
        if (is_zero(w)) {
            return *this;
        }

        double n = 1.0 / w;
        x *= n;
        y *= n;
        z *= n;
        w = 1.0;

        //把z从[-1,1]映射到[0,1]之间
        //z = (z + 1.0f) * 0.5f;

        return *this;
    }

    this_type& rotate(value_type angle, value_type vx, value_type vy, value_type vz)
    {
        vec3<value_type> p(x, y, z);
        p.rotate(angle, vx, vy, vz);
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    this_type& rotateX(value_type angle)
    {
        vec3<value_type> p(x, y, z);
        p.rotateX(angle);
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    this_type& rotateY(value_type angle)
    {
        vec3<value_type> p(x, y, z);
        p.rotateY(angle);
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

    this_type& rotateZ(value_type angle)
    {
        vec3<value_type> p(x, y, z);
        p.rotateZ(angle);
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

public://rectangle
//    vec2<value_type> xy()const
//    {
//        return vec2<value_type>(x, y);
//    }

    vec2<value_type> size()const
    {
        return vec2<value_type>(z, w);
    }

    value_type get_right()const
    {
        return x + width;
    }

    value_type get_bottom()const
    {
        return y + height;
    }

    //判断矩形是否不相交
    bool not_intersect(const this_type& rc)const
    {
        return x >= rc.get_right() || y >= rc.get_bottom() || get_right() <= rc.x || get_bottom() <= rc.y;//边界重合
        //return rc.get_right() < x || rc.get_bottom() < y || get_right() < rc.x || get_bottom() < rc.y;//边界不重合
    }

    //判断矩形是否相交
    //如果non_intersect边界不重合，这里就是边界重合
    bool intersect(const this_type& rc)const
    {
        return !not_intersect(rc);
    }

    //判断矩形是否反转
    bool reversed()const
    {
        return width < 0 || height < 0;
    }

    //返回两个矩形相交区域
    this_type operator&(const this_type& rc)const
    {
        int max_x = std::max(x, rc.x);
        int max_y = std::max(y, rc.y);
        return this_type(
            max_x,
            max_y,
            std::min(get_right(), rc.get_right())-max_x,
            std::min(get_bottom(), rc.get_bottom())-max_y);
    }

    //获得包围两个矩形的区域AABB
    this_type operator|(const this_type& rc)
    {
        int min_x = std::min(x, rc.x);
        int min_y = std::min(y, rc.y);
        return this_type(
            min_x,
            min_y,
            std::max(get_right(), rc.get_right())-min_x,
            std::max(get_bottom(), rc.get_bottom())-min_y);
    }

    //面积
    value_type area()const
    {
        return width * height;
    }

    //矩形的面积是否为0
    bool empty_area()const
    {
        return is_zero(width) || is_zero(height);
    }

    //判断矩形面积是否相等
    bool same_area(const this_type& rc)const
    {
        return is_equal(this->area(), rc.area());
    }

    //大小相等
    bool same_size(const this_type& rc)const
    {
        return is_equal(width, rc.width) && is_equal(height, rc.height);
    }

    //是否包含点
    bool contains(const_refrence a_x, const_refrence a_y)const
    {
        return !(a_x < x || a_y < y || a_x >= get_right() || a_y >= get_bottom());
    }

    bool contains(const vec2<value_type>& p)const
    {
        return this->contains(p.x, p.y);
    }

    //判断是否包含矩形
    bool contains(const this_type& rc)const
    {
        return x <= rc.x && y <= rc.y && get_right() >= rc.get_right() && get_bottom() >= rc.get_bottom();
    }

    //判断是否在rc内部
    bool inside(const this_type& rc)const
    {
        return rc.contain(*this);
    }

    //返回矩形中心点
    vec2<value_type> center()const
    {
        return vec2<value_type>(x + width/2, y + height/2);
    }

    //缩小
    const this_type& inflate(const_refrence size)
    {
        return this->inflate(size, size);
    };

    const this_type& inflate(const_refrence size_x, const_refrence size_y)
    {
        if(size_x < width){
            x += size_x;
            width  -= size_x * 2;
        }
        else{
            x += size_x / 2;
            width = 0;
        }

        if(size_y < height){
            y += size_y;
            height -= size_y * 2;
        }
        else{
            y += size_y / 2;
            height = 0;
        }

        return *this;
    };

    //放大
    const this_type& expand(const_refrence size)
    {
        return this->expand(size, size);
    };

    const this_type& expand(const_refrence size_x, const_refrence size_y)
    {
        x -= size_x;
        y -= size_y;
        width  += size_x * 2;
        height += size_y * 2;
        return *this;
    };

    //目标矩形宽高
    //对齐方式
    this_type align(const_refrence src_width, const_refrence src_height, int align)const
    {
        if(align == CGL_CLIENT){
            return *this;
        }

        this_type dest(x, y, src_width, src_height);


        if((align & CGL_HORIZONTAL) == CGL_HORIZONTAL){
            dest.x += (width - dest.width) / 2;
        }
        else if(align & CGL_RIGHT){
            dest.x += width - dest.width;
        }

        if((align & CGL_VERTICAL) == CGL_VERTICAL){
            dest.y += (height - dest.height) / 2;
        }
        else if(align & CGL_BOTTOM){
            dest.y += height - dest.height;
        }

        return dest;
    }

    this_type align(value_type src_width, value_type src_height, int align, bool fitBounds)const
    {
        if(fitBounds){//开启缩放
            //如果目标矩形太长或者太宽，等比例缩小矩形
            if(width < src_width || height < src_height){
                if(src_width < src_height){//竖长
                    src_width = height * src_width / src_height;
                    src_height = height;
                }
                else if(src_height < src_width){//横长
                    src_width = width;
                    src_height = width * src_height / src_width;
                }
                else{//方形
                    src_width = src_height = std::min(width, height);
                }
            }
            //目标矩形小于本矩形，等比例放大
            //else if(src_width < width && src_height < height)
        }
        return this->align(src_width, src_height, align);
    }

public://color
    vec3<value_type> rgb()const
    {
        return vec3<value_type>(r, g, b);
    }
};

#pragma pack(pop)


VEC2_OPERATOR_COMMON(+)
VEC2_OPERATOR_COMMON(-)
VEC2_OPERATOR_COMMON(*)
VEC2_OPERATOR_COMMON(/)

VEC3_OPERATOR_COMMON(+)
VEC3_OPERATOR_COMMON(-)
VEC3_OPERATOR_COMMON(*)
VEC3_OPERATOR_COMMON(/)

VEC4_OPERATOR_COMMON(+)
VEC4_OPERATOR_COMMON(-)
VEC4_OPERATOR_COMMON(*)
VEC4_OPERATOR_COMMON(/)

//
// typedef
//

typedef vec2<int8_t> vec2b;
typedef vec3<int8_t> vec3b;
typedef vec4<int8_t> vec4b;

typedef vec2<uint8_t> vec2ub;
typedef vec3<uint8_t> vec3ub;
typedef vec4<uint8_t> vec4ub;

typedef vec2<int16_t> vec2s;
typedef vec3<int16_t> vec3s;
typedef vec4<int16_t> vec4s;

typedef vec2<uint16_t> vec2us;
typedef vec3<uint16_t> vec3us;
typedef vec4<uint16_t> vec4us;

typedef vec2<int32_t> vec2i;
typedef vec3<int32_t> vec3i;
typedef vec4<int32_t> vec4i;

typedef vec2<uint32_t> vec2ui;
typedef vec3<uint32_t> vec3ui;
typedef vec4<uint32_t> vec4ui;

typedef vec2<float> vec2f;
typedef vec3<float> vec3f;
typedef vec4<float> vec4f;

typedef vec2<double> vec2d;
typedef vec3<double> vec3d;
typedef vec4<double> vec4d;

#undef VEC2_OPERATION
#undef VEC3_OPERATION
#undef VEC4_OPERATION
#undef VEC2_OPERATOR_COMMON
#undef VEC3_OPERATOR_COMMON
#undef VEC4_OPERATOR_COMMON

}//end namespace cgl

//
// iostream
//

#if defined(_OSTREAM_) || defined(_GLIBCXX_OSTREAM)

template<typename T>
std::ostream& operator<<(std::ostream& stm, const cgl::vec2<T>& v)
{
    stm << '{' << v.x << ", " << v.y << '}';
    return stm;
}

template<typename T>
std::ostream& operator<<(std::ostream& stm, const cgl::vec3<T>& v)
{
    stm << '{' << v.x << ", " << v.y << ", " << v.z << '}';
    return stm;
}

template<typename T>
std::ostream& operator<<(std::ostream& stm, const cgl::vec4<T>& v)
{
    stm << '{' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << '}';
    return stm;
}

#endif

#endif //CGL_VECTYPE_H_20150909203228
