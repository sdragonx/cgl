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

//��������cmd������������
template<typename _char_t>
int exec(const _char_t* app, const _char_t* arg, cstring<_char_t>* out);

//���г���
template<typename _char_t>
int shell_exec(const _char_t* bin, const _char_t* arg, bool show = true);

//����һ�����򣬲��ȴ������󷵻�
template<typename _char_t>
int run_wait(const _char_t* app, const _char_t* arg, int64_t time);

//������ݷ�ʽ
template<typename _char_t>
int link(const _char_t* dst, const _char_t* src);

}//end namespace cgl

#endif//CGL_PLATFORM_WINDOWS

#endif //SYSTEM_EXEC_HPP_20200207032134


