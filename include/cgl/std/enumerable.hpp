/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 enumerable.hpp

 2020-11-04 02:21:08

*/
#ifndef ENUMERABLE_HPP_20201104022108
#define ENUMERABLE_HPP_20201104022108

#include <cgl/public.h>

namespace cgl{

//
// 可枚举对象基类
//
template<typename T>
class enumerable
{
public:
    virtual int count()
    {
        return 0;
    }

    virtual T items(int i)
    {
        return T();
    }
};

}//end namespace cgl

#endif //ENUMERABLE_HPP_20201104022108