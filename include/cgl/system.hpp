/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system.hpp

 2010-08-29 15:40:28

*/
#ifndef SYSTEM_HPP_20100829154028
#define SYSTEM_HPP_20100829154028

#include <cgl/public.h>
#include <cgl/string.hpp>
#include <cgl/system/base.hpp>
#include <cgl/system/cursor.hpp>
#include <cgl/system/exec.hpp>
#include <cgl/system/keyboard.hpp>
#include <cgl/system/shutdown.hpp>
#include <cgl/system/sleep.hpp>
#include <cgl/system/time.hpp>
#include <cgl/system/timer.hpp>
#include <cgl/system/version.hpp>

#if defined(CGL_PLATFORM_WINDOWS)
#include <cgl/windows/system/ime.hpp>

/*
int n = 10000;
int64_t t = cpu_tick();
for(int i=0; i<n; ++i)
{
}
print(cpu_tick() - t);

int64_t cpu_tick()
{
    int64_t t;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
    return t;
}
*/

#else

#endif

namespace cgl{
namespace system{
namespace shell{

//操作系统版本
//int version();



}//end namespace shell
}//end namespace system
}//end namespace cgl;

//#undef system
//#define system cgl::__system()

/*
shell.exec(app, arg, ...)
shell.cmd(cmdline)
shell.run_wait(app, arg, time);
shell.create_shortcut
shell.link("/abc", "/aaa");

*/

#endif //SYSTEM_HPP_20100829154028
