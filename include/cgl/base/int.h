/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 int.hpp

 2011-04-07 00:33:27
 2018-07-27 0:48

*/
#ifndef INT_H_20110407003327
#define INT_H_20110407003327

#if defined(_MSC_VER)
    #if (_MSC_VER < 1300)
        typedef signed char int8_t;
        typedef unsigned char uint8_t;
        typedef signed short int16_t;
        typedef unsigned short uint16_t;
        typedef signed int int32_t;
        typedef unsigned int uint32_t;
        typedef signed long long int64_t;
        typedef unsigned long long uint64_t;

        typedef uint16_t uint_least16_t;
        typedef uint32_t uint_least32_t;
    #elif (_MSC_VER < 1600)
        typedef signed __int8 int8_t;
        typedef unsigned __int8 uint8_t;
        typedef signed __int16 int16_t;
        typedef unsigned __int16 uint16_t;
        typedef signed __int32 int32_t;
        typedef unsigned __int32 uint32_t;
        typedef signed long long int64_t;
        typedef unsigned long long uint64_t;

        typedef uint16_t uint_least16_t;
        typedef uint32_t uint_least32_t;
    #endif
#else
    #ifdef __GNUC__
        #ifndef __STDC_LIMIT_MACROS
            //#error not defined __STDC_LIMIT_MACROS
        #endif
    #endif
    #include <stdint.h>
    #include <limits.h>
#endif

#ifndef HAVE_STDINT_H
    #define _STDINT_H_
    #define HAVE_STDINT_H 1
#endif

#ifndef INT8_MIN
#define INT8_MIN    ((int8_t) -128)
#define INT8_MAX    ((int8_t)  127)
#define UINT8_MAX   ((uint8_t) 255)
#endif

#ifndef INT16_MIN
#define INT16_MIN   ((int16_t) -32768)
#define INT16_MAX   ((int16_t)  32767)
#define UINT16_MAX  ((uint16_t) 65535)
#endif

#ifndef INT32_MIN
#define INT32_MIN   ((int32_t) -2147483647L - 1)
#define INT32_MAX   ((int32_t)  2147483647L)
#define UINT32_MAX  ((uint32_t) 4294967295UL)
#endif

#ifndef INT64_MIN
#define INT64_MIN   ((int64_t) -9223372036854775807LL - 1)
#define INT64_MAX   ((int64_t)  9223372036854775807LL)
#define UINT64_MAX  ((uint64_t) 18446744073709551615ULL)
#endif

#define INT24_MIN   ((int32_t) -8388608L)          //0x800000
#define INT24_MAX   ((int32_t)  8388607L)          //0x7FFFFF
#define UINT24_MAX  ((uint32_t) 16777215UL)        //0xFFFFFF

#define INT40_MIN   ((int64_t) -549755813888LL)    //0x8000000000
#define INT40_MAX   ((int64_t)  549755813887LL)    //0x7FFFFFFFFF
#define UINT40_MAX  ((uint64_t) 1099511627775ULL)  //0xFFFFFFFFFF

#define INT48_MIN   ((int64_t) -140737488355328LL) //0x800000000000
#define INT48_MAX   ((int64_t)  140737488355327LL) //0x7FFFFFFFFFFF
#define UINT48_MAX  ((uint64_t) 281474976710655ULL)//0xFFFFFFFFFFFF

namespace cgl{

/*
template<typename T, int bits>
T limits_max()
{
    //return T(-1)<0 ? ~(0xFFFFFFFFFFFFFFFF<<bits-1) : ~(0xFFFFFFFFFFFFFFFF>>(64-bits));
    return __is_signed(T) ?
        ~((T(1))<<sizeof(T)*CHAR_BIT-1)
        : ~T(0);
}
template<typename T, int bits>
T limits_min()
{
    return is_signed<T>() ? -(T(1)<<(bits-1)) : 0;
}
*/
/*
int64_t gen_int64(int32_t lo_val, int32_t hi_val)
{
    union{
        struct{
            int32_t a, b;
        };
        int64_t value;
    }n = {lo_val, hi_val};

    return n.value;
}

int32_t int64lo(int64_t value)
{
    return value;
}

int32_t int64hi(int64_t value)
{
    return value >> 32;
}

int32_t gen_int32(int16_t lo_val, int16_t hi_val)
{
    union{
        struct{
            int16_t a, b;
        };
        int32_t value;
    }n = {lo_val, hi_val};

    return n.value;
}

int16_t int32lo(int32_t value)
{
    return value;
}

int16_t int32hi(int32_t value)
{
    return value >> 16;
}
*/

/*

#pragma pack(push, 1)

struct __i8
{
    typedef int8_t value_type;
    value_type value;
    value_type max()const { return INT8_MAX; }
    value_type min()const { return INT8_MIN; }
    bool is_signed()const { return true; }
};

class __ui8
{
public:
    typedef uint8_t value_type;
    value_type value;
    value_type max()const { return UINT8_MAX; }
    value_type min()const { return 0; }
    bool is_signed()const { return false; }
};

typedef struct __INT32
{
    typedef int32_t value_type;
    value_type value;
    value_type max()const { return INT32_MAX; };
    value_type min()const { return INT32_MIN; };
}__i32;

typedef struct __INT24
{
    typedef int32_t value_type;
    value_type value:24;
    value_type max()const { return INT24_MAX; }
    value_type min()const { return INT24_MIN; }
}INT24;

typedef struct __UINT24
{
    typedef uint32_t value_type;
    value_type value:24;
    value_type max()const { return UINT24_MAX; }
    value_type min()const { return UINT24_MIN; }
}UINT24;

typedef struct __INT40
{
    typedef int64_t value_type;
    value_type value:40;
    value_type max()const { return INT40_MAX; };
    value_type min()const { return INT40_MIN; };
}INT40;

typedef struct __UINT40
{
    typedef uint64_t value_type;
    value_type value:40;
    value_type max()const { return UINT40_MAX; };
    value_type min()const { return UINT40_MIN; };
}UINT40;

typedef struct __INT48
{
    typedef int64_t value_type;
    value_type value:48;
    value_type max()const { return INT48_MAX; };
    value_type min()const { return INT48_MIN; };
}INT48;

typedef struct __UINT48
{
    typedef uint64_t value_type;
    value_type value:48;
    value_type max()const { return UINT48_MAX; };
    value_type min()const { return UINT48_MIN; };
}UINT48;


template<class T>
class integer//: public T
{
public:
    typedef typename T::value_type value_type;
    typedef integer<T> this_type;
private:
    T num;
public:
    integer()//:value(0)
    {
        num.value = value_type();
    }
    integer(const value_type& n)
    {
        num.value = n;
    }
    integer(const integer& i)
    {
        num.value = i.num.value;
    }
    //~integer() { }//有这个东西会很慢，原因未知。

    value_type max()const { return num.max(); }
    value_type min()const { return num.min(); }

    bool is_signed()const { return __is_signed(num.value); }

    operator value_type()const { return num.value; }

    value_type value()const { return num.value; }

    this_type& operator = (const this_type& i) { num.value = i.num.value; return *this; }
    this_type& operator = (const value_type& n)  { num.value = n; return *this; }

    this_type& operator++(){ ++num.value; return *this; }
    this_type& operator--(){ --num.value; return *this; }
    this_type operator++(int){ return this_type(num.value++); }
    this_type operator--(int){ return this_type(num.value--); }

    this_type operator+(const this_type& i)const { return this_type(num.value + i.num.value); }
    this_type operator-(const this_type& i)const { return this_type(num.value - i.num.value); }
    this_type operator*(const this_type& i)const { return this_type(num.value * i.num.value); }
    this_type operator/(const this_type& i)const { return this_type(num.value / i.num.value); }
    this_type operator+(const value_type & n)const { return this_type(num.value + n); }
    this_type operator-(const value_type & n)const { return this_type(num.value - n); }
    this_type operator*(const value_type & n)const { return this_type(num.value * n); }
    this_type operator/(const value_type & n)const { return this_type(num.value / n); }

    this_type& operator+=(const this_type& i) { num.value += i.num.value; return *this; }
    this_type& operator-=(const this_type& i) { num.value -= i.num.value; return *this; }
    this_type& operator*=(const this_type& i) { num.value *= i.num.value; return *this; }
    this_type& operator/=(const this_type& i) { num.value /= i.num.value; return *this; }
    this_type& operator+=(const value_type & n) { num.value += n;           return *this; }
    this_type& operator-=(const value_type & n) { num.value -= n;           return *this; }
    this_type& operator*=(const value_type & n) { num.value *= n;           return *this; }
    this_type& operator/=(const value_type & n) { num.value /= n;           return *this; }

    this_type operator>>(const int& n) { return this_type(num.value >> n); }
    this_type operator<<(const int& n) { return this_type(num.value << n); }

    this_type& operator>>=(const int& n) { num.value >>= n; return *this; }
    this_type& operator<<=(const int& n) { num.value <<= n; return *this; }

    this_type operator~() { return this_type(~num.value); }
    this_type operator^(const this_type& i) { return this_type(num.value ^ i.num.value); }
    this_type operator|(const this_type& i) { return this_type(num.value | i.num.value); }
    this_type operator&(const this_type& i) { return this_type(num.value & i.num.value); }

    bool operator!()const { return !num.value; }

    bool operator< (const this_type& i)const { return num.value <  i.num.value; }
    bool operator> (const this_type& i)const { return num.value >  i.num.value; }
    bool operator<=(const this_type& i)const { return num.value <= i.num.value; }
    bool operator>=(const this_type& i)const { return num.value >= i.num.value; }
    bool operator==(const this_type& i)const { return num.value == i.num.value; }
    bool operator!=(const this_type& i)const { return num.value != i.num.value; }
    bool operator< (const value_type & n)const { return num.value <  n; }
    bool operator> (const value_type & n)const { return num.value >  n; }
    bool operator<=(const value_type & n)const { return num.value <= n; }
    bool operator>=(const value_type & n)const { return num.value >= n; }
    bool operator==(const value_type & n)const { return num.value == n; }
    bool operator!=(const value_type & n)const { return num.value != n; }
};

typedef integer<INT24>  __int24,  int24_t;
typedef integer<UINT24> __int24u, uint24_t;
typedef integer<INT40>  __int40,  int40_t;
typedef integer<UINT40> __int40u, uint40_t;
typedef integer<INT48>  __int48,  int48_t;
typedef integer<UINT48> __int48u, uint48_t;

#pragma pack(pop)

*/

}//end namespace cgl

#endif //INT_H_20110407003327
