/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 SYSTEM_INPUT.hpp

 2019-06-02 09:06:06

 输入法管理

 ImmDisableIME(GetCurrentThreadId());

禁用
 HIMC g_hIMC = NULL；//g_hIMC 用于恢复时使用
g_hIMC = ImmAssociateContext(handle, NULL);

启用
ImmAssociateContext(handle, g_hIMC);


linux
    鼠标
    fd = open( "/dev/input/mice", O_RDONLY );
    char buf[6];
    read(fd, buf, 6)
    printf("Button type = %d, X = %d, Y = %d, Z = %d\n", (buf[0] & 0x07), buf[1], buf[2],   buf[3]);

*/
#ifndef SYSTEM_INPUT_HPP_20190602090606
#define SYSTEM_INPUT_HPP_20190602090606

#include <cgl/public.h>

#include <cgl/windows/system/ime.hpp>

namespace cgl{
namespace system{


}//end namespace system
}//end namespace cgl

#endif //SYSTEM_INPUT_HPP_20190602090606
