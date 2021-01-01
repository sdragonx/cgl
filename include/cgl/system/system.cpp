/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system.cpp

 2015-01-08 15:15

*/
#ifndef CGL_SYSTEM_CPP_201501081515
#define CGL_SYSTEM_CPP_201501081515

#pragma hdrstop
#include <cgl/system.hpp>

namespace cgl{

int system_t::version()
{
#ifdef _INC_WINDOWS
    OSVERSIONINFO osver = {sizeof(osver)};
    GetVersionEx(&osver);
    return (osver.dwMajorVersion<<24) | (osver.dwMinorVersion<<16) | (osver.dwBuildNumber);
#else
    return 0;
#endif
}

template<typename _char_t>
handle_t system_t::shell(const _char_t* app, const _char_t* arg, bool show)
{
#ifdef _INC_WINDOWS
    typedef ttype<char_t, STARTUPINFOA, STARTUPINFOW>::type TSTARTUPINFO;
    SECURITY_ATTRIBUTES sa = {0};
    TSTARTUPINFO si = {0};
    PROCESS_INFORMATION  pi = {0};

    si.cb = sizeof(TSTARTUPINFO);
    tvalue<_char_t>(GetStartupInfoA, GetStartupInfoW)(&si);
    si.wShowWindow  = show ? SW_SHOW : SW_HIDE;
    si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    if (!tvalue<_char_t>(CreateProcessA, CreateProcessW)(app, const_cast<_char_t*>(arg),
            NULL, NULL, false, NULL, NULL, "./", &si, &pi))
    {
        return INVALID_HANDLE_VALUE;
    }
    return pi.hProcess;
#else
    return 0;
#endif
}

template<typename _char_t>
int system_t::runwait(const _char_t* app, const _char_t* arg, bool show)
{
#ifdef _INC_WINDOWS
    handle_t h = system.shell<_char_t>(app, arg, show);
    if(h != INVALID_HANDLE_VALUE)
    {
        WaitForSingleObject(h, INFINITE);
        return 0;
    }
    else
        return -1;
#else
    return -1;
#endif
}

template<typename _char_t>
std::basic_string<_char_t> shellcmd(const _char_t* cmd)
{
#ifdef _INC_WINDOWS
    typedef ttype<char_t, STARTUPINFOA, STARTUPINFOW>::type TSTARTUPINFO;
    HANDLE              hRead  = NULL;
    HANDLE              hWrite = NULL;
    SECURITY_ATTRIBUTES sa = {0};
    TSTARTUPINFO        si = {0};
    PROCESS_INFORMATION pi = {0};
    const DWORD         bufsize = 4096;    //»º³åÇø³¤¶È
    _char_t             buffer[bufsize] = {0};
    DWORD               dwRead = 0;
    std::basic_string<_char_t> str = std::basic_string<_char_t>(tvalue<_char_t>("cmd /c ", L"cmd /c")) + cmd;

    sa.nLength              = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle       = TRUE;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
    {
        return tvalue<_char_t>("Error On CreatePipe().", L"Error On CreatePipe().");
    }

    si.cb = sizeof(TSTARTUPINFO);
    tvalue<_char_t>(GetStartupInfoA, GetStartupInfoW)(&si);
    si.hStdError    = hWrite;
    si.hStdOutput   = hWrite;
    si.wShowWindow  = SW_HIDE;
    si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

    if (!tvalue<_char_t>(CreateProcessA, CreateProcessW)(
            NULL, const_cast<_char_t*>(str.c_str()),
            NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
    {
        return tvalue<_char_t>("Error on CreateProcess().", L"Error on CreateProcess().");
    }
    CloseHandle(hWrite);

    str.clear();
    while(ReadFile(hRead, buffer, bufsize, &dwRead, NULL))
    {
        str += std::basic_string<_char_t>(buffer, dwRead/sizeof(_char_t));// + "<CR>";
    }
    CloseHandle(hRead);
    return str;
#else//no _INC_WINDOWS
    return std::basic_string<_char_t>();
#endif
}

} //end namespace cgl

#endif //CGL_SYSTEM_CPP_201501081515