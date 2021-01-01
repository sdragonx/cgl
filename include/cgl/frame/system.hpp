/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system.hpp

 2018-06-25 21:59:17

*/
#ifndef CORE_SYSTEM_HPP_20180625215917
#define CORE_SYSTEM_HPP_20180625215917

#include <cgl/system/time.hpp>
#include <cgl/system/sleep.hpp>
#include <cgl/system/shutdown.hpp>

namespace cgl{
namespace frame{

class native_system
{
public:
    class native_time
    {
    public:
        int64_t now()
        {
            return cgl::system::time::now();
        }

        int64_t clock()
        {
            return cgl::system::time::clock_milliseconds();
        }
    };

    void sleep(int t)
    {
        cgl::system::sleep(t);
    }

    void pause()
    {
        std::system("pause");
    }

    void logout()
    {
        cgl::system::logout();
    }

    void reboot()
    {
        cgl::system::reboot();
    }

    void halt()
    {
        cgl::system::halt();
    }
};

}//end namespace frame
}//end namespace cgl

#endif //CORE_SYSTEM_HPP_20180625215917
