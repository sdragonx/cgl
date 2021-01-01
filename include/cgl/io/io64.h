/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 io64.h

 2014-07-11 12:24:52

 2018/9/24 14:21

*/
#ifndef IO64_H_20140711122452
#define IO64_H_20140711122452

#if defined(__linux__) || defined(__unix__) || defined(__android__)
    //#define _FILE_OFFSET_BITS 64
    //#define _LARGEFILE64_SOURCE

    #if _FILE_OFFSET_BITS < 64
        //#error _FILE_OFFSET_BITS less than 64
    #endif

    #ifndef _LARGEFILE64_SOURCE

        //#error _LARGEFILE64_SOURCE undefined
    #endif

    #include <unistd.h>    //ftruncate64
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__NT__) || defined(_XBOX)
    #ifndef CGL_PLATFORM_WINDOWS
        #define CGL_PLATFORM_WINDOWS
    #endif
    #include <windows.h>
#endif

#include <cstdio>
#ifndef CGL_PLATFORM_ANDROID
#include <io.h>
#endif

#include <cgl/base/int.h>

namespace cgl{
namespace io{

//windows file io
#ifdef CGL_PLATFORM_WINDOWS

inline int64_t FileSeek(HANDLE hFile, int64_t pos, int mthd)
{
    LARGE_INTEGER LI = {0};
    LI.QuadPart = pos;
    SetFilePointerEx(hFile, LI, &LI, mthd);
    return LI.QuadPart;
}

inline int64_t FileTell(HANDLE hFile)
{
    LARGE_INTEGER LI = {0};
    SetFilePointerEx(hFile, LI, &LI, FILE_CURRENT);
    return LI.QuadPart;
}

inline int64_t FileLength(HANDLE hFile)
{
    LARGE_INTEGER seek = {0};
    GetFileSizeEx(hFile, &seek);
    return seek.QuadPart;
}

inline int64_t SetFileLength(HANDLE hFile, int64_t size)
{
    size = FileSeek(hFile, size, FILE_BEGIN);
    SetEndOfFile(hFile);
    return size;
}

#endif//CGL_PLATFORM_WINDOWS

//---------------------------------------------------------------------------
//
// os file
//

inline int64_t seeki64(int file, int64_t pos, int mthd)
{
    #ifdef CGL_PLATFORM_WINDOWS
        #if (_MSC_VER >= 1400) || (__BORLANDC__ >= 0x0610)//vc8.0(2005) cb2009
            return _lseeki64(file, pos, mthd);
        #else
            return FileSeek((handle_t)_get_osfhandle(file), pos, mthd);
        #endif
    #else//POSIX
        return lseek64(file, pos, mthd);
    #endif
}

inline int64_t telli64(int file)
{
    #ifdef CGL_PLATFORM_WINDOWS
        #if (_MSC_VER >= 1400) || (__BORLANDC__ >= 0x0610)
            return _telli64(file);
        #else
            return FileTell((handle_t)_get_osfhandle(file));
        #endif
    #else//POSIX
        return lseek64(file, 0, SEEK_CUR);
    #endif
}

inline int64_t filelengthi64(int file)
{
    #ifdef CGL_PLATFORM_WINDOWS
        return FileLength((handle_t)_get_osfhandle(file));
    #else
        int64_t pos = telli64(file);
        seeki64(file, 0, SEEK_END);
        int64_t n = telli64(file);
        seeki64(file, pos, SEEK_SET);
        return n;
    #endif
}

inline int64_t chsizei64(int file, int64_t size)
{
    #ifdef CGL_PLATFORM_WINDOWS
    return SetFileLength((handle_t)_get_osfhandle(file), size);
    #else//POSIX
        //using namespace std;
        //#ifdef __android__
        //return ftruncate(file, size);
        //#else
        //
        //#endif
        #ifdef CGL_PLATFORM_ANDROID
            return 0;
        #else
            return ftruncate64(file, size);
        #endif
    #endif
}

//---------------------------------------------------------------------------
//
// FILE std c io
//

inline int64_t fseeki64(std::FILE* file, int64_t pos, int mthd)
{
    return seeki64(fileno(file), pos, mthd);
}

inline int64_t ftelli64(std::FILE* file)
{
    return telli64(fileno(file));
}

inline int64_t flength(std::FILE* file)
{
    #ifdef CGL_PLATFORM_ANDROID
    return filelengthi64(fileno(file));
    #else
    return filelength(fileno(file));
    #endif
}

inline int64_t fchsizei64(std::FILE* file, int64_t size)
{
    fflush(file);
    return chsizei64(fileno(file), size);
}

}//end namespace io
}//end namespace cgl

#endif //IO64_H_20140711122452
