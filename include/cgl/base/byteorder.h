/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 byteorder.h

 2018-07-02 14:06:11
 2018-07-26 23:16

*/
#ifndef BYTE_ORDER_H_20180702140611
#define BYTE_ORDER_H_20180702140611

#ifndef CGL_BYTE_ORDER
    #define CGL_LITTLE_ENDIAN 1234
    #define CGL_BIG_ENDIAN    4321

    #ifdef __linux__
        #include <endian.h>
        #define CGL_BYTE_ORDER __BYTE_ORDER
    #else
        #if defined(__hppa__) || \
            defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
            (defined(__MIPS__) && defined(__MISPEB__)) || \
            defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
            defined(__sparc__)
            #define CGL_BYTE_ORDER CGL_BIG_ENDIAN
        #else
            #define CGL_BYTE_ORDER CGL_LITTLE_ENDIAN
        #endif
    #endif
#endif

#include "int.h"

#if defined(CGL_PLATFORM_MSVC)
#pragma warning(push)
#pragma warning(disable: 4244) // nameless unions are part of C++
#endif

namespace cgl{

//__BYTE_ORDER__                    1234
//__ORDER_LITTLE_ENDIAN__           1234
//__ORDER_BIG_ENDIAN__              4321
//__LITTLE_ENDIAN__                 1

namespace internal {

//little endian (endian_test.s & 0xFF) == 'L'
const union
{
    uint8_t  c[2];
    uint16_t s;

}endian_test = {'L', 'B'};

}//end namespace internal

//¼ì²âcpu±àÂë·½Ê½
inline bool big_endian()
{
    unsigned int v = 0x01;
    return (1 != reinterpret_cast<char*>(&v)[0]);
}

#define CGL_BITSWAP_MACRO
#ifdef CGL_BITSWAP_MACRO

#define bswap8(x)  ((x) & 0xFF)
#define bswap16(x) ((bswap8 (x) << 8 ) | bswap8 ((x) >> 8 ))
#define bswap32(x) ((bswap16(x) << 16) | bswap16((x) >> 16))
#define bswap64(x) ((bswap32(x) << 32) | bswap32((x) >> 32))

#else

inline uint8_t  bswap8 (uint8_t  x){ return x; }
inline uint16_t bswap16(uint16_t x){ return (uint16_t(bswap8 (x)) << 8 ) | bswap8 (x >> 8 ); }
inline uint32_t bswap32(uint32_t x){ return (uint32_t(bswap16(x)) << 16) | bswap16(x >> 16); }
inline uint64_t bswap64(uint64_t x){ return (uint64_t(bswap32(x)) << 32) | bswap32(x >> 32); }

#endif //CGL_BITSWAP_MACRO

inline float bswap32f(float x)
{
    union
    {
        float f;
        uint32_t n;
    }swapper;
    swapper.f = x;
    swapper.n = bswap32(swapper.n);
    return swapper.f;
}

inline float bswap64f(double x)
{
    union
    {
        double f;
        uint64_t n;
    }swapper;
    swapper.f = x;
    swapper.n = bswap64(swapper.n);
    return swapper.f;
}

//allow to big endian
inline uint8_t be_conv8(uint8_t x)
{
    return x;
}

inline uint16_t be_conv16(uint16_t x)
{
    return big_endian() ? x : bswap16(x);
}

inline uint32_t be_conv32(uint32_t x)
{
    return big_endian() ? x : bswap32(x);
}

inline uint64_t be_conv64(uint64_t x)
{
    return big_endian() ? x : bswap64(x);
}

inline float be_conv32f(float x)
{
    return big_endian() ? x : bswap32f(x);
}

inline double be_conv64f(double x)
{
    return big_endian() ? x : bswap64f(x);
}

//allow to little endian
inline uint8_t le_conv8(uint8_t x)
{
    return x;
}

inline uint16_t le_conv16(uint16_t x)
{
    return big_endian() ? bswap16(x) : x;
}

inline uint32_t le_conv32(uint32_t x)
{
    return big_endian() ? bswap32(x) : x;
}

inline uint64_t le_conv64(uint64_t x)
{
    return big_endian() ? bswap64(x) : x;
}

inline float le_conv32f(float x)
{
    return big_endian() ? bswap32f(x) : x;
}

inline double le_conv64f(double x)
{
    return big_endian() ? bswap64f(x) : x;
}

//
// ×Ö·û´®×ª»»
//

void csswap16(void* data, size_t size)
{
    uint16_t* begin = (uint16_t*)(data);
    uint16_t* end = begin + size;
    for( ; begin < end; ++begin){
        *begin = bswap16(*begin);
    }
}

void csswap32(void* data, size_t size)
{
    uint32_t* begin = (uint32_t*)(data);
    uint32_t* end = begin + size;
    for( ; begin < end; ++begin){
        *begin = bswap16(*begin);
    }
}

/*
template<typename T>
inline T byte_swap(const T& value)
{
    return value;
}

template<>
inline char byte_swap<char>(const char& value)
{
    return bswap8(value);
}

template<>
inline char byte_swap<unsigned char>(const char& value)
{
    return bswap8(value);
}

template<>
inline char byte_swap<char>(const char& value)
{
    return bswap8(value);
}
*/

}//end namespace cgl

#if defined(CGL_PLATFORM_MSVC)
#pragma warning(pop)
#endif

#endif //BYTE_ORDER_HPP_20180702140611
