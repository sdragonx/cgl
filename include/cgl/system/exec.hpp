/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system_exec.hpp

 2020-02-07 03:21:34

*/
#ifndef SYSTEM_EXEC_HPP_20200207032134
#define SYSTEM_EXEC_HPP_20200207032134

#include <cgl/public.h>

#ifdef CGL_PLATFORM_WINDOWS

#include <cgl/windows/system/exec.hpp>

#else

namespace cgl{

//返回运行cmd命令，并捕获输出
template<typename _char_t>
int exec(const _char_t* app, const _char_t* arg, cstring<_char_t>* out);

//运行程序
template<typename _char_t>
int shell_exec(const _char_t* bin, const _char_t* arg, bool show = true);

//运行一个程序，并等待结束后返回
template<typename _char_t>
int run_wait(const _char_t* app, const _char_t* arg, int64_t time);

//建立快捷方式
template<typename _char_t>
int link(const _char_t* dst, const _char_t* src);

}//end namespace cgl

#endif//CGL_PLATFORM_WINDOWS

#endif //SYSTEM_EXEC_HPP_20200207032134


