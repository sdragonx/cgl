/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 allocate.hpp

 2020-02-14 22:08:03

*/
#ifndef ALLOCATE_HPP_20200214220803
#define ALLOCATE_HPP_20200214220803

#include <cgl/public.h>

namespace cgl{

template<typename T>
inline T* allocT()
{
    return new T;
}

template<typename T>
inline T* allocT(size_t size)
{
    return new T[size];
}

template<typename T>
inline void deleteT(T* &p)
{
    if(p){
        delete p;
        p = null;
    }
}

template<typename T>
inline void deleteT(T* &p, size_t size)
{
    if(p){
        delete []p;
        p = null;
    }
}

}//end namespace cgl

#endif //ALLOCATE_HPP_20200214220803
