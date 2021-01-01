/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 native_window.hpp

 2018-05-09 14:18:31

*/
#ifndef NATIVE_WINDOW_HPP_20180509141831
#define NATIVE_WINDOW_HPP_20180509141831

#ifdef CGL_PLATFORM_WINDOWS
    #include "win32app.hpp"
#elif defined(CGL_PLATFORM_ANDROID)
    #include "android.hpp"
#else

    #error no platform support.

#endif

#endif //NATIVE_WINDOW_HPP_20180509141831
