/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 thread_base.hpp

 2019-06-14 10:36:55

*/
#ifndef THREAD_BASE_HPP_20190614103655
#define THREAD_BASE_HPP_20190614103655

#include <cgl/public.h>

#if defined(CGL_PLATFORM_WINDOWS) && !defined(CGL_PLATFORM_GNUC)

    #include <windows.h>
    #define CGL_THREAD_WINDOWS

#else

    #include <pthread.h>
    #define CGL_THREAD_POSIX

#endif


#endif //THREAD_BASE_HPP_20190614103655
