/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 char.h

 2018-07-27 00:16:41

 CGL_ANSI
 CGL_UNICODE
 CGL_NATIVE_UCHAR_SUPPERT char16_t char32_t

*/
#ifndef CHAR_H_20180727001641
#define CHAR_H_20180727001641

#include <cctype>
#include <cwchar>
#include "int.h"

//---------------------------------------------------------------------------
//wchar_t
/*
#ifndef __GNUC__
    #ifndef __SIZEOF_WCHAR_T__
        #define __SIZEOF_WCHAR_T__ sizeof(wchar_t)
    #endif
#endif
*/

#if defined(__SIZE_OF_WCHAR_T__)
    #define CGL_SIZEOF_WCHAR_T __SIZE_OF_WCHAR_T__
#elif defined(_MSC_VER) && ( _MSC_VER <= 1310 || defined(UNDER_CE) && _MSC_VER <= 1500 )
    // MSVC 7.1 and MSVC 8 (arm) define WCHAR_MAX to a value not suitable for constant expressions
    #define __SIZE_OF_WCHAR_T__ 2
#elif (WCHAR_MAX + 0) == 0xff || (WCHAR_MAX + 0) == 0x7f
    #define __SIZE_OF_WCHAR_T__ 1
#elif (WCHAR_MAX + 0) == 0xffff || (WCHAR_MAX + 0) == 0x7fff
    #define __SIZE_OF_WCHAR_T__ 2
#elif (WCHAR_MAX + 0) == 0xffffffff || (WCHAR_MAX + 0) == 0x7fffffff
    #define __SIZE_OF_WCHAR_T__ 4
#elif (WCHAR_MAX + 0) == UINT64_C(0xffffffffffffffff) || (WCHAR_MAX + 0) == INT64_C(0x7fffffffffffffff)
    #define __SIZE_OF_WCHAR_T__ 8
#elif
    #define __SIZE_OF_WCHAR_T__ sizeof(wchar_t)
#endif

#define CGL_SIZEOF_WCHAR_T __SIZEOF_WCHAR_T__

//---------------------------------------------------------------------------
//char8_t char16_t char32_t

#ifdef __BORLANDC__
    #ifdef __CODEGEARC__
        #define CGL_NATIVE_UCHAR_SUPPERT
    #endif
#elif defined (__GNUC__)
    #include <uchar.h>
    #if !(defined(__cplusplus) && defined(__GXX_EXPERIMENTAL_CXX0X__)) || !defined(__GNUC__) ||\
        (!defined(__clang__) && (__GNUC__ < 4 || (__GNUC__ == 4 &&    __GNUC_MINOR__ < 4)))
        //typedef uint_least16_t char16_t;
        //typedef uint_least32_t char32_t;
        #define CGL_IMPLEMENTS_UCHAR_SUPPERT
    #else
        #define CGL_NATIVE_UCHAR_SUPPERT
    #endif
#elif defined (_MSC_VER)
    #if (_MSC_VER >= 1600)
        #define CGL_NATIVE_UCHAR_SUPPERT
    #endif
#elif defined (__clang__)
    #if __has_feature(cxx_constexpr)
        #define CGL_NATIVE_UCHAR_SUPPERT
    #endif
#endif

typedef uint8_t char8_t;

#if !defined(CGL_NATIVE_UCHAR_SUPPERT) && !defined(CGL_IMPLEMENTS_UCHAR_SUPPERT)
typedef uint_least16_t char16_t;
typedef uint_least32_t char32_t;
#endif

#ifndef __STDC_UTF_16__
#define __STDC_UTF_16__ 1
#endif

#ifndef __STDC_UTF_32__
#define __STDC_UTF_32__ 1
#endif

#define CGL_SIZEOF_CHAR8_T  1
#define CGL_SIZEOF_CHAR16_T 2
#define CGL_SIZEOF_CHAR32_T 4

namespace cgl{

//char_type
#if defined(UNICODE) || defined(_UNICODE)
    typedef wchar_t tchar_t;
#else
    typedef char    tchar_t;
#endif

#ifdef CGL_PLATFORM_WINDOWS
    typedef wchar_t  unichar_t;
#else
    typedef char16_t unichar_t;
#endif

}

#endif //CHAR_H_20180727001641
