/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 rdtsc.hpp

 2018-11-21 20:31:38

 得到CPU自启动以后的运行周期,

*/
#ifndef RDTSC_HPP_20181121203138
#define RDTSC_HPP_20181121203138

#include <cgl/public.h>

namespace cgl{
namespace system{

static uint64_t rdtsc()
{
    #if defined(__GNUC__)
        #if defined(__i386__)
        uint64_t x;
        __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
        return x;
        #elif defined(__x86_64__)
        uint32_t hi, lo;
        __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
        return ((uint64_t)lo) | ((uint64_t)hi << 32);
        #else
            #error Unsupported architecture.
        #endif

    #elif defined(__BORLANDC__)
        asm RDTSC;
    #elif defined(_MSC_VER)
        return __rdtsc();
    #else
        #error Other compilers not supported...
    #endif
}

}//end namespace system
}//end namespace cgl

#endif //RDTSC_HPP_20181121203138
