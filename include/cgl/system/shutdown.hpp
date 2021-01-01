/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shutdown.hpp

 2018-06-25 21:44:22

*/
#ifndef SHUTDOWN_HPP_20180625214422
#define SHUTDOWN_HPP_20180625214422

#include <cgl/public.h>

#ifdef CGL_PLATFORM_WINDOWS

#include <cgl/windows/system/shutdown.hpp>

namespace cgl{ namespace system{

inline void logout()
{
    cgl::windows::system::logout();
}

inline void reboot()
{
    cgl::windows::system::reboot();
}

inline void halt()
{
    cgl::windows::system::halt();
}


}}//end namespace cgl

#elif defined (CGL_PLATFORM_POSIX)

#include <unistd.h>
#include <sys/reboot.h>

namespace cgl{ namespace system{

//linux-2.6.21.7/include/linux/reboot.h
#define    LINUX_REBOOT_CMD_RESTART     0x01234567
#define    LINUX_REBOOT_CMD_HALT        0xCDEF0123
#define    LINUX_REBOOT_CMD_CAD_ON      0x89ABCDEF
#define    LINUX_REBOOT_CMD_CAD_OFF     0x00000000
#define    LINUX_REBOOT_CMD_POWER_OFF   0x4321FEDC
#define    LINUX_REBOOT_CMD_RESTART2    0xA1B2C3D4
#define    LINUX_REBOOT_CMD_SW_SUSPEND  0xD000FCE2
#define    LINUX_REBOOT_CMD_KEXEC       0x45584543

void shutdown()
{
    sync();
    reboot(LINUX_REBOOT_CMD_HALT);
}

void logout()
{
    std::system("logout");
}

void reboot()
{
    std::system("reboot");
}

void halt()
{
    std::system("halt");
}

}}//end namespace cgl

#endif

#endif //SHUTDOWN_HPP_20180625214422
