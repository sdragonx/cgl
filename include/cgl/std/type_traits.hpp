/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 type_traits.hpp

 2018-07-26 13:52:05

 stdtype
 stdclass
 anytype
 is_stdc_type
 typestd
 stdinfo
 string_traits



*/
#ifndef STL_TYPE_TRAITS_H_20180726135205
#define STL_TYPE_TRAITS_H_20180726135205

#include <algorithm>
#include <limits>
#include <cgl/base/is_zero.h>

namespace cgl{

struct empty_t { };

template <class T, T n>
struct constant
{
    typedef constant<T, n> type;
    typedef T value_type;
    static const value_type value;
};

template <class T, T n>
const T constant<T, n>::value = n;

//---------------------------------------------------------------------------
//
// true_type false_type
//
//---------------------------------------------------------------------------

typedef constant<bool, true>  true_type;
typedef constant<bool, false> false_type;

//
// possible_static<T, bool>
//

template<typename T, bool ST = true>
struct possible_static
{
    typedef T value_type;
    static value_type value;
};

template<typename T, bool ST>
T possible_static<T, ST>::value = T();

template<typename T>
struct possible_static<T, false>
{
    typedef T value_type;
    value_type value;
};

//---------------------------------------------------------------------------
//
// is_in_types<T, ...> 判断类型是否在列表里面，列表支持32个
//
//---------------------------------------------------------------------------

#include "is_typeof.hpp"

//---------------------------------------------------------------------------
//
// is_same<T> 判断类型是否相同
//
//---------------------------------------------------------------------------

template <class _Tp, class _Up> struct is_same : public false_type {};
template <class _Tp> struct is_same<_Tp, _Tp>  : public true_type {};

//---------------------------------------------------------------------------
//
// is_ctype<T> 判断类型是否c++原生类型
//
//---------------------------------------------------------------------------

template<class T>
struct is_ctype : public is_typeof<T,
    bool,
    char, unsigned char,
    wchar_t,

    #ifdef CGL_NATIVE_UCHAR_SUPPERT
    char16_t, char32_t,
    #endif

    short, unsigned short,
    int, unsigned int,
    long, unsigned long,
    long long, unsigned long long,
    float, double, long double>
{ };

//---------------------------------------------------------------------------
//
// signed_type<T>
//
//---------------------------------------------------------------------------

template<typename T>struct signed_type           { typedef T type; };

template<>struct signed_type<unsigned char>      { typedef char      type; };
template<>struct signed_type<unsigned short>     { typedef short     type; };
template<>struct signed_type<unsigned int>       { typedef int       type; };
template<>struct signed_type<unsigned long>      { typedef long      type; };
template<>struct signed_type<unsigned long long> { typedef long long type; };

//---------------------------------------------------------------------------
//
// unsigned_type<T>
//
//---------------------------------------------------------------------------

template<typename T>struct unsigned_type  { typedef T type; };

template<>struct unsigned_type<char>      { typedef unsigned char      type; };
template<>struct unsigned_type<short>     { typedef unsigned short     type; };
template<>struct unsigned_type<int>       { typedef unsigned int       type; };
template<>struct unsigned_type<long>      { typedef unsigned long      type; };
template<>struct unsigned_type<long long> { typedef unsigned long long type; };

//---------------------------------------------------------------------------
//
// dual_type<T> 包含符号和无符号两种类型
//
//---------------------------------------------------------------------------

#ifndef _MSVCVER

template<typename T>
struct dual_type
{
    //typedef TYPENAME signed_type<T>::type type;
    typedef typename signed_type<T>::type signed_type;
    typedef typename unsigned_type<T>::type unsigned_type;
};

#endif

//---------------------------------------------------------------------------
//
// is_signed<T>() 判断类型是否有符号
// is_signed<T>(const T&) 判断变量是否有符号
// wchar_t char16_t char32_t is unsigned type
//
//---------------------------------------------------------------------------

template<typename T>
bool is_signed()
{
    return is_typeof<T, char, short, int, long, long long, float, double, long double>::value;
}

template<typename T>
bool is_signed(const T& )
{
    return is_signed<T>();
}

//---------------------------------------------------------------------------
//
// is_float<T> 判断类型是否浮点数
//
//---------------------------------------------------------------------------

template<typename T> struct is_float : public is_typeof<T, float, double, long double> { };

//---------------------------------------------------------------------------
//
// is_reference<T> 判断类型是否引用
//
//---------------------------------------------------------------------------

template <typename T> struct is_reference           : public false_type { };
template <typename T> struct is_reference<T&>       : public true_type  { };

template <typename T> struct is_reference<const T&> : public true_type  { };

template <typename T> struct is_const               : public false_type { };

template <typename T> struct is_const<const T>      : public true_type  { };

template <typename T> struct is_const<const T*>     : public true_type  { };

template <typename T> struct is_const<const T&>     : public true_type  { };

template <typename T> struct is_const<const T* &>   : public true_type  { };

//---------------------------------------------------------------------------
#ifndef CGL_IS_EQUAL
#define CGL_IS_EQUAL

template<typename T>
inline bool is_equal(const T& a, const T& b)
{
    return a == b;
}

template<> inline bool is_equal<float>(const float& a, const float& b) { return is_zero(b - a); }
template<> inline bool is_equal<double>(const double& a, const double& b) { return is_zero(b - a); }
template<> inline bool is_equal<long double>(const long double& a, const long double& b) { return is_zero(b - a); }

//template<> inline bool is_equal<char*>(const long double& a, const long double& b) { return is_zero(b - a); }

template<typename T>
inline bool is_not_equal(const T& a, const T& b)
{
    return !is_equal<T>(a, b);
}

#endif
//---------------------------------------------------------------------------


namespace internal{

template<size_t SIZE>
struct least_type;

template<>
struct least_type<8>
{
    typedef int8_t signed_type;
    typedef uint8_t unsigned_type;
};

template<>
struct least_type<16>
{
    typedef int16_t signed_type;
    typedef uint16_t unsigned_type;
};

template<>
struct least_type<32>
{
    typedef int32_t signed_type;
    typedef uint32_t unsigned_type;
};

template<>
struct least_type<64>
{
    typedef int64_t signed_type;
    typedef uint64_t unsigned_type;
};

}//end namespace internal

template<int SIZE>
struct least_type_size
{
    const static int size = (SIZE <= 8 ? 8 : (SIZE <= 16 ? 16 : (SIZE <= 32 ? 32 : (SIZE <= 64 ? 64 : 64))));
};

template<size_t SIZE>
struct least_type
{
    const static int size = least_type_size<SIZE>::size;
    typedef typename internal::least_type<least_type_size<SIZE>::size>::signed_type type;
};

#ifndef CGL_PLATFORM_MSVC

template<typename T>
struct type_traits_base
{
    typedef T type;
    //typename type value;
    //inline static bool is_ctype(){ return is_ctype<T>::value; }
    inline static type max() { return std::numeric_limits<T>::max(); }
    inline static type min() { return std::numeric_limits<T>::min(); }
    //inline static bool is_signed() { return ::is_signed<T>(); }
};

template<typename T>
class type_traits : public type_traits_base<T>
{
public:
    typedef T type;
public:
    type_traits() : type_traits_base<T>()
    {
    }

    inline static void fill(T* p, size_t size, const T& value)
    {
        std::fill(p, p+size, value);
    }

    inline static void copy(T* dst, const T* src, size_t size)
    {
        if(is_ctype<T>::value){
            memcpy(dst, src, size * sizeof(type));
        }
        else{
            std::copy(dst, src, size);
        }
    }

    inline static void move(T* dst, const T* src, size_t size)
    {
        if(is_ctype<T>::value){
            memmove(dst, src, size * sizeof(type));
        }
        else{
            std::copy(dst, src, size);
        }
    }
};

#endif

}//end namespace cgl

#endif //STL_TYPE_TRAITS_H_20180726135205

