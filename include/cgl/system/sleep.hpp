/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sleep.hpp

 2018-05-12 04:43:35

 use millisecond

*/
#ifndef SLEEP_HPP_20180512044335
#define SLEEP_HPP_20180512044335

#ifdef CGL_PLATFORM_WINDOWS
#include <windows.h>

namespace cgl{
namespace system{

void sleep(int millisecond)
{
    Sleep(millisecond);
}

}//end namespace system
}//end namespace cgl

#else//elif defined(CGL_PLATFORM_POSIX)
#include <unistd.h>

namespace cgl{
namespace system{

void sleep(int millisecond)
{
    usleep(millisecond * 1000);
}

}//end namespace system
}//end namespace cgl

#endif

#endif //SLEEP_HPP_20180512044335
