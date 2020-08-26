/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stringify.h

 2018-04-14 23:31:13
 2018-07-26 23:17
 2018-08-30 14:39    add T type

*/
#ifndef STRINGIFY_H_20180414233113
#define STRINGIFY_H_20180414233113

#ifndef __stringify

#define __stringify_ansi(x) x
#define __stringify_unicode(x) L##x

#ifndef UNICODE
  #define __stringify_text(x) __stringify_ansi(x)
#else
  #define __stringify_text(x) __stringify_unicode(x)
#endif

#if defined(CGL_PLATFORM_CB6) || defined(CGL_PLATFORM_MSVC)
    #define __stringify(x) __stringify_ansi(#x)
    #define __wstringify(x) __stringify_unicode(#x)
    #define __tstringify(x) __stringify_text(#x)
#else
    #define __stringify_1(x...) __stringify_ansi(#x)
    #define __stringify(x...) __stringify_1(x)

    #define __wstringify_1(x...) __stringify_unicode(#x)
    #define __wstringify(x...) __wstringify_1(x)

    #define __tstringify_1(x...) __stringify_text(#x)
    #define __tstringify(x...) __tstringify_1(x)
#endif

#endif//__stringify

#endif //STRINGIFY_H_20180414233113
