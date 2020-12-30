/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sqrt.hpp

 2018-07-31 01:31:39

 //1024^2 * 2 = 2097152

*/
#ifndef SQRT_HPP_20180731013139
#define SQRT_HPP_20180731013139

#include <cgl/public.h>

namespace cgl{
namespace math{


//
// T sqrt(value)
//

//开方
template<typename T>
inline T sqrt(T value)
{
    using namespace std;
    return ::sqrt(value);
}

template<>inline float sqrt<float>(float value)
{
    using namespace std;
    return ::sqrtf(value);
}

#if 0

//QUAKE里面的sqrt函数(bast magic number is 0x5f375a86)
float InvSqrt (float x)
{
    float xhalf = 0.5f * x;
    int i = *(int*) & x;
    i = 0x5f3759df - (i >> 1);      //计算第一个近似根
    x = *(float*) & i;
    x = x*(1.5f - xhalf * x * x);   // 牛顿迭代法
    return x;
}

//bast magic number is 0x5fe6eb50c7aa19f9
double InvSqrtD(double number)
{
      double x2, y;
      const double threehalfs = 1.5F;
      union
      {
            double d;
            __int64 i;
      }d;
      x2 = number * 0.5F;
      y = number;
      d.d =  y;
      d.i = 0x5fe6ec85e7de30da - (d.i >> 1);
      y = d.d;
      y = y * (threehalfs - (x2 * y * y)); //1stiteration
      y = y * (threehalfs - (x2 * y * y)); //2nditeration, this can be removed
      return y;
}

float SquareRootFloat(float number)
{
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}
#endif



//
// Carmack在QUAKE3中使用的计算平方根的函数
//
float CarmSqrt(float x){
    union{
        int intPart;
        float floatPart;
    } convertor;
    union{
        int intPart;
        float floatPart;
    } convertor2;
    convertor.floatPart = x;
    convertor2.floatPart = x;
    convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
    convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
    return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
}



//fastsqrt 782    std::sqrt 546
/*
float fastsqrt(float val) {
    int tmp = *(int *)&val;
    tmp -= 1<<23;
    tmp = tmp >> 1;
    tmp += 1<<29;
    return *(float *)&tmp;
}
*/



//
//
//    Integer
//
//


/*
sqrt(f) < 1024 (2097152) 以内的开方表，占用4,194,304内存

template<typename T>
class sqrt1024
{
public:
    const static T data[2097152];

    static T init()
    {
        for(int i=0; i<2097153; ++i){
            const_write(data[i]) = std::sqrt((float)i);
        }
        return 0;
    }
};

template<typename T>
const T sqrt1024<T>::data[2097152] = { sqrt1024<T>::init() };
*/


//sqrti16 16位开方
//sqrti16 563    std::sqrt 547
/*
const uint16_t SQRT_I16_TABLE[] = {
    0, 1, 4, 9, 16, 25, 36, 49,
    64, 81, 100, 121, 144, 169, 196, 225,
    256, 289, 324, 361, 400, 441, 484, 529,
    576, 625, 676, 729, 784, 841, 900, 961,
    1024, 1089, 1156, 1225, 1296, 1369, 1444, 1521,
    1600, 1681, 1764, 1849, 1936, 2025, 2116, 2209,
    2304, 2401, 2500, 2601, 2704, 2809, 2916, 3025,
    3136, 3249, 3364, 3481, 3600, 3721, 3844, 3969,
    4096, 4225, 4356, 4489, 4624, 4761, 4900, 5041,
    5184, 5329, 5476, 5625, 5776, 5929, 6084, 6241,
    6400, 6561, 6724, 6889, 7056, 7225, 7396, 7569,
    7744, 7921, 8100, 8281, 8464, 8649, 8836, 9025,
    9216, 9409, 9604, 9801, 10000, 10201, 10404, 10609,
    10816, 11025, 11236, 11449, 11664, 11881, 12100, 12321,
    12544, 12769, 12996, 13225, 13456, 13689, 13924, 14161,
    14400, 14641, 14884, 15129, 15376, 15625, 15876, 16129,
    16384, 16641, 16900, 17161, 17424, 17689, 17956, 18225,
    18496, 18769, 19044, 19321, 19600, 19881, 20164, 20449,
    20736, 21025, 21316, 21609, 21904, 22201, 22500, 22801,
    23104, 23409, 23716, 24025, 24336, 24649, 24964, 25281,
    25600, 25921, 26244, 26569, 26896, 27225, 27556, 27889,
    28224, 28561, 28900, 29241, 29584, 29929, 30276, 30625,
    30976, 31329, 31684, 32041, 32400, 32761, 33124, 33489,
    33856, 34225, 34596, 34969, 35344, 35721, 36100, 36481,
    36864, 37249, 37636, 38025, 38416, 38809, 39204, 39601,
    40000, 40401, 40804, 41209, 41616, 42025, 42436, 42849,
    43264, 43681, 44100, 44521, 44944, 45369, 45796, 46225,
    46656, 47089, 47524, 47961, 48400, 48841, 49284, 49729,
    50176, 50625, 51076, 51529, 51984, 52441, 52900, 53361,
    53824, 54289, 54756, 55225, 55696, 56169, 56644, 57121,
    57600, 58081, 58564, 59049, 59536, 60025, 60516, 61009,
    61504, 62001, 62500, 63001, 63504, 64009, 64516, 65025
};

//这个效率和PC库函数不相上下
inline int sqrti16(uint16_t x) {
    const uint16_t *p = SQRT_I16_TABLE;

    if (p[128] <= x) p += 128;
    if (p[ 64] <= x) p +=  64;
    if (p[ 32] <= x) p +=  32;
    if (p[ 16] <= x) p +=  16;
    if (p[  8] <= x) p +=   8;
    if (p[  4] <= x) p +=   4;
    if (p[  2] <= x) p +=   2;
    if (p[  1] <= x) p +=   1;

    return p - SQRT_I16_TABLE;
}
*/



// 32位整形查表开方
//fastsqrti32 200        std::sqrt 540

template<typename T>
class sqrti32_table
{
public:
    const static uint16_t table[65536];
    const static bool is_init;

public:
    static bool initTable()
    {
        for (int x=0; x<65536; ++x){
            double v = x / 65535.0;
            v = std::sqrt(v);
            int y = int(v * 65535.0 + 0.999);
            const_cast<uint16_t&>(table[x]) = y;
        }
        return true;
    }
};

template<typename T>
const uint16_t sqrti32_table<T>::table[65536] = { 0 };

template<typename T>
const bool sqrti32_table<T>::is_init = sqrti32_table<T>::initTable();

inline uint16_t sqrti32(uint32_t x)
{
    const uint32_t m1 = 0xff000000;
    const uint32_t m2 = 0x00ff0000;
    if (x & m1) {
        return sqrti32_table<int>::table[x>>16];
    }
    else if (x & m2) {
        return sqrti32_table<int>::table[x>>8]>>4;
    }
    else {
        return sqrti32_table<int>::table[x]>>8;
    }
}



/*
// mysqrt 1203  std::sqrt 547
unsigned int mysqrt(unsigned int x)
{
    unsigned int y;
    //初始估计值
    if(x>16384)y=192;
    else if(x>4096)y=96;
    else if(x>1024)y=48;
    else if(x>256)y=24;
    else if(x>64)y=12;
    else y=6;
    y=(y+x/y)>>1;
    y=(y+x/y)>>1;
    return y;
}

//但最慢的算法
//1 + 3 + 5 ... + (2n - 1) = n * n
unsigned linear_search(unsigned long x)
{
    unsigned long sum_n = 1;
    unsigned n = 1;

    if(x <= 1){
        return x;
    }

    while(sum_n <= x){
        n++;
        sum_n += (n<<1) - 1;
    }

    return (n-1);
}

//逐比特确认法 James Ulery  论文:Computing Integer Square Roots
unsigned bitwise_verification(unsigned long x)
{
   unsigned long temp = 0;
   unsigned v_bit = 15;
   unsigned n = 0;
   unsigned b = 0x8000;

   if (x <= 1)
       return x;

   do{
       temp = ((n << 1) + b) << (v_bit--);
       if (x >= temp){
           n += b;
           x -= temp;
       }
   }while (b >>= 1);

   return n;
}

*/

}//end namespace math
}//end namespace cgl

#endif //SQRT_HPP_20180731013139
