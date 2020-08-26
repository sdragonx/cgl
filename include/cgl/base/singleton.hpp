/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 singleton.hpp

 2019-06-14 06:10:06

*/
#ifndef SINGLETON_HPP_20190614061006
#define SINGLETON_HPP_20190614061006

#include <cgl/public.h>

namespace cgl{

//C++0x 线程安全
template<class T>
class singleton
{
public:
    static T& instance()
    {
        static T object;
        return object;
    }

protected:
    singleton() {/*void*/}
    virtual ~singleton() { /*void*/ }

private:
    singleton(const singleton&);
    singleton& operator=(const singleton&);
};

}//end namespace cgl

#endif //SINGLETON_HPP_20190614061006
