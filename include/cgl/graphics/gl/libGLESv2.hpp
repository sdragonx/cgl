/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 libGLESv2.hpp

 2020-01-28 03:02:59

*/
#ifndef LIBGLESV2_HPP_20200128030259
#define LIBGLESV2_HPP_20200128030259

#include "libEGL.hpp"
#include <cgl/external/glad/libGLESv2.h>

namespace cgl{

//CGL_PUBLIC_DECLARE shared_obj so = shared_obj();

void* libGLESv2_load(const char* fn)
{
    static shared_obj so = shared_obj();
    if(!so.handle){
        #ifdef CGL_PLATFORM_WINDOWS
        so.open("libGLESv2.dll");
        #else
        so.open("libGLESv2.so");
        #endif
    }
    return so.load(fn);
}

int libGLESv2_init()
{
    return gladLoadGLES2Loader(libGLESv2_load);
}

}//end namespace cgl

#endif //LIBGLESV2_HPP_20200128030259
