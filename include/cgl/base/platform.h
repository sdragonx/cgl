/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 platform.h

 2016-03-21 16:41:47
 2020-02-02 5:47

 */
#ifndef PLATFORM_H_20160321164147
#define PLATFORM_H_20160321164147

#ifndef __cplusplus
#error cgl only support C++.
#endif

//===========================================================================
//
// c++ version
//

#define CGL_CPP98       199711L
#define CGL_CPP98TR1    200311L
#define CGL_CPP11       201103L
#define CGL_CPP14       201402L
#define CGL_CPP17       201703L

//===========================================================================
//
// CPU
//

//x86
#if defined(_M_IX86) || defined(__X86__) || \
    defined(__i386__) || defined(i386) || defined(__386__)

    #define CGL_PLATFORM_X86
#endif

//x64
#if defined(_M_X64) || defined(_M_IA64) || defined(__x86_64__) || defined(__x86_64) ||\
    defined(_M_AMD64) || defined(__amd64__) || defined(__amd64)

    #define CGL_PLATFORM_X64
#endif

//ARM _M_ARM
#if defined(__arm__) || defined(ARM) || defined (_ARM)
    #define CGL_PLATFORM_ARM
#endif

//ARMv6
#if defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) ||\
    defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6T2__)

    #define CGL_PLATFORM_ARMv6
#endif

//ARMv7
#if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) ||\
    defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__)

    #define CGL_PLATFORM_ARMv7
#endif

//===========================================================================
//
// operation system
//

//CGL_PLATFORM
enum {
    CGL_WINDOWS = 1,
    CGL_UNIX,
    CGL_LINUX = CGL_UNIX,
    CGL_APPLE,
    CGL_ANDROID,
    CGL_NOT_IMPLEMENTED_PLATFORM = 0xFF
};

//---------------------------------------------------------------------------
//windows
#if defined(_WIN32) || defined(_WIN64) || defined(__NT__) || defined(_XBOX)

    //windows version
    //Win8                    WINVER >= 0x0602
    //Windows Server 2008 R2  WINVER >= 0x0600
    //Win7                    WINVER >= 0x0601
    //Windows Server 2008     WINVER >= 0x0600
    //Windows Vista           WINVER >= 0x0600
    //Windows Server 2003     WINVER >= 0x0502
    //Windows XP              WINVER >= 0x0501
    //Windows 2000            WINVER >= 0x0500
    //Windows Me              WINVER >= 0x0500
    //Windows 98              WINVER >= 0x0410
    //Windows NT 4.0          WINVER >= 0x0400
    //Windows 95              WINVER >= 0x0400
    //Windows 3.1             WINVER >= 0x030a

    #define CGL_PLATFORM CGL_WINDOWS
    #define CGL_PLATFORM_WINDOWS

    #if (defined(_WIN32) || defined(WIN32))
        #define CGL_PLATFORM_WIN32
    #endif

    #if (defined(_WIN64) || defined(WIN64))
        #define CGL_PLATFORM_WIN64
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #ifndef STRICT
        #define STRICT
    #endif

    #include <windows.h>
    #include <windowsx.h>

    #ifndef WM_MOUSEWHEEL
        #define WM_MOUSEWHEEL 0x020A
    #endif

//---------------------------------------------------------------------------
//android
#elif defined(ANDROID) || defined(__ANDROID__)

    #define CGL_PLATFORM CGL_ANDROID
    #define CGL_PLATFORM_ANDROID

    #include <jni.h>
    #include <android/native_window.h>

//---------------------------------------------------------------------------
//apple
#elif defined(__APPLE__)

    #define CGL_PLATFORM CGL_APPLE
    #define CGL_PLATFORM_APPLE

    // X11 (tentative)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>

//---------------------------------------------------------------------------
//unix & linux
#elif defined(__unix__) || defined(__unix) || defined(unix) ||\
    defined (__linux__) || defined(__linux) ||defined(linux)

    #define CGL_PLATFORM CGL_UNIX
    #define CGL_PLATFORM_UNIX
    #define CGL_PLATFORM_LINUX

    // X11 (tentative)
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>

#endif

//---------------------------------------------------------------------------
//posix
#if defined(CGL_PLATFORM_UNIX) || defined(CGL_PLATFORM_LINUX) || defined(CGL_PLATFORM_APPLE) || defined(CGL_PLATFORM_ANDROID)

    #define CGL_PLATFORM_POSIX

    #include <unistd.h>

#endif

//===========================================================================
//
// compiler GL_COMPILER
//

//---------------------------------------------------------------------------
//clang
#if defined(__clang__)

    #define CGL_PLATFORM_CLANG __clang__

#endif

//---------------------------------------------------------------------------
//__GNU
#if defined (__GNUC__)

    #define CGL_PLATFORM_GNUC __GNUC__
    #if defined(__MINGW32__) || defined(__MINGW64__)
    #define CGL_PLATFORM_MINGW
    #endif

    #define CGL_PUBLIC_DECLARE __attribute__((weak))

    #define STD
    #define TYPENAME typename
    #define CGL_INLINE __attribute__((always_inline))

//---------------------------------------------------------------------------
//c++builder
#elif defined(__BORLANDC__)

    #define CGL_PLATFORM_CBUILDER __BORLANDC__

    //C++ Builder versions
    #define CB_VERSION_60       0x0560
    #define CB_VERSION_64up4    0x0564
    #define CB_VERSION_2006     0x0570
    #define CB_VERSION_2007     0x0590
    #define CB_VERSION_2009     0x0610
    #define CB_VERSION_2010     0x0620    //2010 and update 1
    #define CB_VERSION_2010up2  0x0621
    #define CB_VERSION_XE       0x0630
    #define CB_VERSION_XE2      0x0640
    #define CB_VERSION_XE3      0x0650
    #define CB_VERSION_XE4      0x0660
    #define CB_VERSION_XE5      0x0670
    #define CB_VERSION_XE6      0x0680
    #define CB_VERSION_XE7      0x0690
    #define CB_VERSION_XE8      0x0700
    #define CB_VERSION_XE10     0x0710

    #if (__BORLANDC__ >= 0x0560) && (__BORLANDC__ < 0x0570)
        #define CGL_PLATFORM_CB6 __BORLANDC__   //c++builder6.0 update4
    #elif __BORLANDC__ >= 0x0620 && __BORLANDC__ < 0x0630
        #define CGL_PLATFORM_CB2010 __BORLANDC__//c++builder2010 update2
        #define CGL_SUPPORT_VA_ARGS
    #elif __BORLANDC__ < 0x0640
        #define CGL_PLATFORM_CBXE __BORLANDC__
    #elif __BORLANDC__ < 0x0650
        #define CGL_PLATFORM_CBXE2 __BORLANDC__
    #else
        #define CGL_PLATFROM_CBXEx __BORLANDC__
    #endif

    #ifdef INC_VCL
        #define CGL_PLATFORM_VCL
        #include <vcl.h>
    #endif

    #define CGL_PUBLIC_DECLARE __declspec(selectany)

    #define STD std
    #if defined(CGL_PLATFORM_CLANG)
        #define TYPENAME typename
    #else
        #define TYPENAME
    #endif
    #define CGL_INLINE

    #pragma option -vu  //Dont't prefix underbars to exported symbols

//---------------------------------------------------------------------------
//MSVC
#elif defined (_MSC_VER)

    //MS VC++ 15.0 _MSC_VER = 1910 (Visual Studio 2017)
    //MS VC++ 14.0 _MSC_VER = 1900 (Visual Studio 2015)
    //MS VC++ 12.0 _MSC_VER = 1800 (Visual Studio 2013)
    //MS VC++ 11.0 _MSC_VER = 1700 (Visual Studio 2012)
    //MS VC++ 10.0 _MSC_VER = 1600 (Visual Studio 2010)//__VA_ARGS__
    //MS VC++ 9.0  _MSC_VER = 1500 (Visual Studio 2008)
    //MS VC++ 8.0  _MSC_VER = 1400 (Visual Studio 2005)
    //MS VC++ 7.1  _MSC_VER = 1310 (Visual Studio 2003)
    //MS VC++ 7.0  _MSC_VER = 1300 (Visual Studio .NET)
    //MS VC++ 6.0  _MSC_VER = 1200 (Visual Studio 98)
    //MS VC++ 5.0  _MSC_VER = 1100 (Visual Studio 97)

    #define CGL_PLATFORM_MSVC _MSC_VER

    #define CGL_PUBLIC_DECLARE __declspec(selectany)

    #define STD
    #define TYPENAME typename
    #define CGL_INLINE __forceinline

	#define _CRT_SECURE_NO_WARNINGS	//不显示CRT安全函数警告

	#undef min
	#undef max

#else

    #error cgl not supported compiler.

#endif


//===========================================================================

#if !defined(CGL_PLATFORM_GNUC)

    #define __attribute__(a)    //cancle __attribute__

#endif

//===========================================================================
//
// cpp11 check
//

#ifndef __has_feature
    #define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

// Any compiler claiming C++11 supports, Visual C++ 2015 and Clang version supporting constexpr
#if ((__cplusplus >= 201103L) || (_MSC_VER >= 1900) || (__has_feature(cxx_constexpr))) // C++ 11 implementation
    #define CGL_NATIVE_CPP11_SUPPORT
    #define CGL_NATIVE_CPP11_TYPES_SUPPORT
#endif

#if !defined(CGL_NATIVE_CPP11_TYPES_SUPPORT)
    #if ((__cplusplus > 199711L) || defined(__CODEGEARC__))
        #define CGL_NATIVE_CPP11_TYPES_SUPPORT
    #endif
#endif

#if ((!defined(_MSC_VER) || _MSC_VER < 1600) && !defined(CGL_NATIVE_CPP11_TYPES_SUPPORT))
    #define CGL_IMPLEMENTS_NULLPTR_SUPPORT
#else
    #define CGL_NATIVE_NULLPTR_SUPPORT
#endif

#if (_MSC_VER >= 1600)
    #ifndef CGL_NATIVE_CPP11_TYPES_SUPPORT
        #define CGL_NATIVE_CPP11_TYPES_SUPPORT
    #endif
#endif

//===========================================================================
//
// project
//

#if defined(__DLL__)// || defined(_DLL)
    #define CGL_PROJECT_SHARE
#else
    #define CGL_PROJECT_APP
#endif

//===========================================================================
#endif //PLATFORM_H_20160321164147
