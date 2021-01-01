/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 public.h

 2015-01-08 15:31:12

 CGL_PUBLIC
 CGL_STATIC
 CGL_INLINE

*/
#ifndef PUBLIC_H_20150108153112
#define PUBLIC_H_20150108153112
#define CGL_PUBLIC_H

#include <cgl/base/typedef.h>
#include <cgl/base/values.h>
#include <cgl/base/stringify.h>
#include <cgl/memory/smart_ptr.hpp>
#include <cgl/std/const_write.hpp>
#include <cgl/std/ttype.hpp>
#include <cgl/std/type_traits.hpp>

namespace cgl{
    //extern void show_message(...)
    extern void __cgl_print(const char* text);
    //extern void __cgl_print(const wchar_t* text);

    void __cgl_log_vprint(const char* title, const char* param, va_list body)
    {
        using namespace std;
        char buf[1024] = {0};
        size_t size = strlen(title);
        strncpy(buf, title, 1024);
        buf[size++] = ' ';
        buf[size++] = '>';
        buf[size++] = ' ';
        vsnprintf(buf + size, 1024 - size, param, body);
        __cgl_print(buf);
    }

    void __cgl_log_print(const char* title, ...)
    {
        va_list body;
        va_start(body, title);
        const char* param = va_arg(body, const char*);
        __cgl_log_vprint(title, param, body);
        va_end(body);
    }

}//end namespace cgl

//CGL_LOG
#ifdef CGL_PLATFORM_ANDROID

#include <android/log.h>

#define CGL_LOG(...)       ((void)__android_log_print(ANDROID_LOG_INFO, "CGL", __VA_ARGS__))
#define CGL_LOG_ERROR(...) ((void)__android_log_print(ANDROID_LOG_WARN, "CGL", __VA_ARGS__))

#else//CGL_PLATFORM_ANDROID

#ifdef CGL_SUPPORT_VA_ARGS
    #define CGL_LOG(...)       (cgl::__cgl_log_print("CGL", __VA_ARGS__))
    #define CGL_LOG_ERROR(...) ((void)cgl::__cgl_log_print("CGL Error", __VA_ARGS__))
#else//CGL_SUPPORT_VA_ARGS
    void CGL_LOG(const char* param, ...)
    {
        va_list body;
        va_start(body, param);
        cgl::__cgl_log_vprint("CGL", param, body);
        va_end(body);
    }

    void CGL_LOG_ERROR(const char* param, ...)
    {
        va_list body;
        va_start(body, param);
        cgl::__cgl_log_vprint("CGL ERROR", param, body);
        va_end(body);
    }
#endif//CGL_SUPPORT_VA_ARGS

#endif//CGL_PLATFORM_ANDROID

//CGL_LOG_DEBUG
#ifdef _DEBUG
    #ifdef CGL_PLATFORM_CB6
    #   define CGL_LOG_DEBUG CGL_LOG
    #else
    #   define CGL_LOG_DEBUG(...) CGL_LOG(__VA_ARGS__)
    #endif
#else
    #ifdef CGL_PLATFORM_CB6
        inline void CGL_LOG_DEBUG(...){/*void*/}
    #else
        #define CGL_LOG_DEBUG(...) ((void)0)
    #endif
#endif

//CGL_THROW
#ifndef CGL_THROW
  #ifdef CGL_PLATFORM_ANDROID
    #define CGL_THROW(err) ((void)0) //CGL_LOG_ERROR(err)
  #else
    #define CGL_THROW(err) throw(err)
  #endif
#endif

//CGL_ASSERT
#ifdef _DEBUG
    #define CGL_ASSERT(x) if(!(x))CGL_THROW(#x)
#else
    #define CGL_ASSERT(x)
#endif

#endif //PUBLIC_H_20150108153112
