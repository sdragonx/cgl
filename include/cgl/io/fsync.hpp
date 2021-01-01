/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fsync.hpp

 2018-02-08 23:17:47

 将文件修改刷新到磁盘

*/
#ifndef FSYNC_HPP_20180208231747
#define FSYNC_HPP_20180208231747

#ifdef CGL_PLATFORM_WINDOWS

#include <io.h>
#include <windows.h>

inline void fsync(int file)
{
    FlushFileBuffers((HANDLE)_get_osfhandle(file));
}

inline void fdatasync(int file)
{
    fsync(file);
}

inline void fsync(std::FILE* f)
{
    fsync(fileno(f));
}

#elif defined(CGL_PLATFORM_POSIX)
    #include <unistd.h>
#endif //CGL_PLATFORM_WINDOWS

#endif //FSYNC_HPP_20180208231747
