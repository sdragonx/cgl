/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system_version.hpp

 2018-02-19 11:30:07

 这个应该在os下

*/
#ifndef SYSTEM_VERSION_HPP_20180219113007
#define SYSTEM_VERSION_HPP_20180219113007

#include <cgl/public.h>

namespace cgl{
namespace system{

struct version_t
{
    int major;
    int minor;
    int release;
    int build;    
};

version_t version()
{
    version_t vi = {0};
#ifdef CGL_PLATFORM_WINDOWS
    OSVERSIONINFO osver = {sizeof(osver)};
    GetVersionEx(&osver);
    vi.major = osver.dwMajorVersion;
    vi.minor = osver.dwMinorVersion;
    vi.build = osver.dwBuildNumber;
#else

#endif
    return vi;
}

}//end namespace system
}//end namespace cgl

#endif //SYSTEM_VERSION_HPP_20180219113007
