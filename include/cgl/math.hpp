/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 cgl_math.h

 2006.06.18 18:50

 revise 2007.01.26 17:56

 sin = y
 cos = x
 tan = sin/cos = y/x

float： 1bit 符号位 8bits  指数位 23bits 尾数位 精度为6~7位
double：1bit 符号位 11bits 指数位 52bits 尾数位 精度为15~16位

CGL_FORCE_REAL32

32位 int 定点角度计算
精度    最大操作数    误差
4096    524287        0.000244
8192    262143        0.000122


*/
#ifndef MATH_H_200606181850
#define MATH_H_200606181850

#include <cgl/math/base.hpp>
#include <cgl/math/random.hpp>
#include <cgl/math/sqrt.hpp>
#include <cgl/math/vector.hpp>
#include <cgl/math/trigonometry.hpp>

#include "math/arithmetic.hpp"
#include "math/collision.hpp"
#include "math/geometry.hpp"
#include "math/line.hpp"
#include "math/rasterization.hpp"
#include "math/spline.hpp"
#include "math/triangle.hpp"

#include "math/model/line.hpp"
#include "math/model/circle.hpp"

namespace cgl{
namespace math{

//---------------------------------------------------------------------------
//数值运算

//返回T的符号 -1 0 1
template<typename T>
inline int sign(const T& n)
{
    return n > 0 ? 1 : (n < 0 ? -1 : 0);
}

//判断一个数是否2的整数幂
inline bool is_pow2(int n)
{
    return !(n & ~(n - 1));
}

//计算一个数是2的多少次方
/*
inline int ilog2(int n)
{
    int m = 0;
    while(1 < n){
        ++m;
        n >>= 1;
    }
    return m;
}
*/

//这个基本没有消耗
inline int ilog2(int n)
{
    return
        (n < 2 ? 0 :
        (n & 2 ? 1:
        (n & 4 ? 2 :
        (n & 8 ? 3 :
        (n & 16 ? 4 :
        (n & 32 ? 5 :
        (n & 64 ? 6 :
        (n & 128 ? 7 :
        (n & 256 ? 8 :
        (n & 512 ? 9 :
        (n & 1024 ? 10:
        (n & 2048 ? 11 :
        (n & 4096 ? 12 :
        (n & 8192 ? 13 :
        (n & 16384 ? 14 :
        (n & 32768 ? 15 :
        (n & 65536 ? 16 :
        (n & 131072 ? 17 :
        (n & 262144 ? 18:
        (n & 524288 ? 19:
        (n & 1048576 ? 20:
        (n & 2097152 ? 21:
        (n & 4194304 ? 22:
        (n & 8388608 ? 23:
        (n & 16777216 ? 24:
        (n & 33554432 ? 25:
        (n & 67108864 ? 26:
        (n & 134217728 ? 27:
        (n & 268435456 ? 28:
        (n & 536870912 ? 29:
        (n & 1073741824 ? 30 :
        (n & 2147483648 ? 31 : 32
        ))))))))))))))))))))))))))))))));
}

template<int n>
struct const_ilog2
{
    const static int value =
        (n < 2 ? 0 :
        (n & 2 ? 1:
        (n & 4 ? 2 :
        (n & 8 ? 3 :
        (n & 16 ? 4 :
        (n & 32 ? 5 :
        (n & 64 ? 6 :
        (n & 128 ? 7 :
        (n & 256 ? 8 :
        (n & 512 ? 9 :
        (n & 1024 ? 10:
        (n & 2048 ? 11 :
        (n & 4096 ? 12 :
        (n & 8192 ? 13 :
        (n & 16384 ? 14 :
        (n & 32768 ? 15 :
        (n & 65536 ? 16 :
        (n & 131072 ? 17 :
        (n & 262144 ? 18:
        (n & 524288 ? 19:
        (n & 1048576 ? 20:
        (n & 2097152 ? 21:
        (n & 4194304 ? 22:
        (n & 8388608 ? 23:
        (n & 16777216 ? 24:
        (n & 33554432 ? 25:
        (n & 67108864 ? 26:
        (n & 134217728 ? 27:
        (n & 268435456 ? 28:
        (n & 536870912 ? 29:
        (n & 1073741824 ? 30 :
        (n & 2147483648 ? 31 : 32
        ))))))))))))))))))))))))))))))));
};

//返回大于num的2的最小整数幂
inline int next_pow2(int num)
{
    int n = 1;
    while(n < num)n <<= 1;
    return n;
}

//release和上面的执行效率是一样的
/*
n & 2 ? 2
inline int next_pow2(int n)
{
    return
        (n <= 2 ? 2:
        (n <= 4 ? 4 :
        (n <= 8 ? 8 :
        (n <= 16 ? 16 :
        (n <= 32 ? 32 :
        (n <= 64 ? 64 :
        (n <= 128 ? 128 :
        (n <= 256 ? 256 :
        (n <= 512 ? 512 :
        (n <= 1024 ? 1024 :
        (n <= 2048 ? 2048 :
        (n <= 4096 ? 4096 :
        (n <= 8192 ? 8192 :
        (n <= 16384 ? 16384 :
        (n <= 32768 ? 32768 :
        (n <= 65536 ? 65536 :
        (n <= 131072 ? 131072 :
        (n <= 262144 ? 262144 :
        (n <= 524288 ? 524288 :
        (n <= 1048576 ? 1048576 :
        (n <= 2097152 ? 2097152 :
        (n <= 4194304 ? 4194304 :
        (n <= 8388608 ? 8388608 :
        (n <= 16777216 ? 16777216 :
        (n <= 33554432 ? 33554432 :
        (n <= 67108864 ? 67108864 :
        (n <= 134217728 ? 134217728 :
        (n <= 268435456 ? 268435456 :
        (n <= 536870912 ? 536870912 :
        (n <= 1073741824 ? 1073741824 : 2147483648
        ))))))))))))))))))))))))))))));
}
*/

inline int pow2(int n)
{
    return 1 << n;
}

template<int n>
struct const_pow2
{
    const static int value = 1 << n;
};

template<int n>
struct const_next_pow2
{
    const static int value =
        (n <= 2 ? 2:
        (n <= 4 ? 4 :
        (n <= 8 ? 8 :
        (n <= 16 ? 16 :
        (n <= 32 ? 32 :
        (n <= 64 ? 64 :
        (n <= 128 ? 128 :
        (n <= 256 ? 256 :
        (n <= 512 ? 512 :
        (n <= 1024 ? 1024 :
        (n <= 2048 ? 2048 :
        (n <= 4096 ? 4096 :
        (n <= 8192 ? 8192 :
        (n <= 16384 ? 16384 :
        (n <= 32768 ? 32768 :
        (n <= 65536 ? 65536 :
        (n <= 131072 ? 131072 :
        (n <= 262144 ? 262144 :
        (n <= 524288 ? 524288 :
        (n <= 1048576 ? 1048576 :
        (n <= 2097152 ? 2097152 :
        (n <= 4194304 ? 4194304 :
        (n <= 8388608 ? 8388608 :
        (n <= 16777216 ? 16777216 :
        (n <= 33554432 ? 33554432 :
        (n <= 67108864 ? 67108864 :
        (n <= 134217728 ? 134217728 :
        (n <= 268435456 ? 268435456 :
        (n <= 536870912 ? 536870912 :
        (n <= 1073741824 ? 1073741824 : 2147483648
        ))))))))))))))))))))))))))))));
};

template<typename T>
inline T pow_of10(int n)
{
    switch(n){
    case  0: return 0;
    case  1: return 10;
    case  2: return 100;
    case  3: return 1000;
    case  4: return 10000;
    case  5: return 100000;
    case  6: return 1000000;
    case  7: return 10000000;
    case  8: return 100000000;
    case  9: return 10000000000ull;
    case 10: return 100000000000ull;
    case 11: return 1000000000000ull;
    case 12: return 10000000000000ull;
    case 13: return 100000000000000ull;
    case 14: return 1000000000000000ull;
    case 15: return 10000000000000000ull;
    case 16: return 100000000000000000ull;
    case 17: return 1000000000000000000ull;
    case 18: return 10000000000000000000ull;
    default:
        return 0;
    }
}

//n对齐到align倍
inline int align(int n, int align)
{
    if(is_pow2(align)){
        --align;
        return (n + align) & (~align);
    }
    else{
        return n + (n % align ? align - n % align : 0);
    }
}

//n对齐到m倍的下一个位置
template<typename T>
inline T align_next(T n, T m)
{
    n = n - n % m + m;
    return n;
}

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

//把数值控制在区间
template<typename T>
inline T clamp(T x, T min, T max)
{
    return x < min ? min : (max < x ? max : x);
}

//混合 a = 0 ~ 1
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

//从source递进到dest，步长为speed
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

//greatest common divisor
//最大公约数
int GCD(int x,int y)
{
    while(x != y){
        if(x > y){
            x = x - y;
        }
        else{
            y = y - x;
        }
    }
    return x;
}

//least common multiple
//最小公倍数
int LCM(int x, int y)
{
    return x / GCD(x, y) * y;
}
//---------------------------------------------------------------------------
//浮点数操作

//double round(double n)
//{
//    return std::floor(n + 0.5);
//}

//比较浮点数
int float_equal(float a, float b)
{
    if(is_zero<float>(a - b)){
        return 0;
    }
    else{
        return a < b ? -1 : 1;
    }
}

template <typename destType, typename sourceType>
destType bitCast(const sourceType &source)
{
    size_t copySize = std::min(sizeof(destType), sizeof(sourceType));
    destType output;
    memcpy(&output, &source, copySize);
    return output;
}

// Returns whether the argument is Not a Number.
// IEEE 754 single precision NaN representation: Exponent(8 bits) - 255, Mantissa(23 bits) - non-zero.
inline bool isNaN(float f)
{
    // Exponent mask: ((1u << 8) - 1u) << 23 = 0x7f800000u
    // Mantissa mask: ((1u << 23) - 1u) = 0x7fffffu
    return ((bitCast<uint32_t>(f) & 0x7f800000u) == 0x7f800000u) && (bitCast<uint32_t>(f) & 0x7fffffu);
}

// Returns whether the argument is infinity.
// IEEE 754 single precision infinity representation: Exponent(8 bits) - 255, Mantissa(23 bits) - zero.
inline bool isInf(float f)
{
    // Exponent mask: ((1u << 8) - 1u) << 23 = 0x7f800000u
    // Mantissa mask: ((1u << 23) - 1u) = 0x7fffffu
    return ((bitCast<uint32_t>(f) & 0x7f800000u) == 0x7f800000u) && !(bitCast<uint32_t>(f) & 0x7fffffu);
}

//---------------------------------------------------------------------------
//view angle
//srceen 4:3 is 60 degrees

//float view_angle(float width, float height)
//{
//    return width * 45.0f / height;
//}

//---------------------------------------------------------------------------

//黄金角度，花菜算法
template<typename T>
void golden_angle(std::vector< vec2<T> >& ls,
    size_t size,        //生成数量
    float cx = 0.0f,    //原点
    float cy = 0.0f,
    float scale = 2.0f  //半径缩放
    )
{
    //137.5=(2PI*(1-0.618))
    float a;
    float r;
    float x, y;
    for(size_t i=1; i<size; ++i){
        a = i * 137.5 * M_RD;
        r = sqrt(float(i)) * scale;
        x = cx + std::cos(a) * r;
        y = cy + std::sin(a) * r;
        ls.push_back(vec2<T>(x, y));
    }
}

//---------------------------------------------------------------------------

//获得转向半径
//速度 * 旋转周期 / 2PI
double turn_radius(double speed, double round)
{
    return speed * round / (2 * M_PI);
}

}//end namespace math
}//end namespace cgl

#endif //MATH_H_200606181850

