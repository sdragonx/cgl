/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 construct.hpp

 2018-10-23 23:43:41

*/
#ifndef CONSTRUCT_HPP_20181023234341
#define CONSTRUCT_HPP_20181023234341

#include <cgl/public.h>
#include <new>

namespace cgl{
namespace memory{

//---------------------------------------------------------------------------
//
// construct
//
//---------------------------------------------------------------------------

template<typename T>
inline T* construct(T* p)
{
    return new (p) T();
}

template<typename T>
inline T* construct(T* p, const T& value)
{
    return new (p) T(value);
}

template<typename T>
inline T* construct(T* begin, T* end)
{
    return new (begin) T[end - begin];
}

template<typename T>
inline T* construct(T* begin, T* end, const T& value)
{
    for(T* p = begin; p != end; ++p){
        new (p) T(value);
    }
    return begin;
}

//---------------------------------------------------------------------------
//
// destruct
//
//---------------------------------------------------------------------------

template<typename T>
inline void destruct(T* p)
{
    p->~T();
}

template<>inline void destruct(char* ) { }
template<>inline void destruct(wchar_t* ) { }

template<typename T>
inline void destruct(T* begin, T* end)
{
    for(T* p = begin; p != end; ++p){
        p->~T();
    }
}

template<>inline void destruct(char* , char* ){ /*void*/ }
template<>inline void destruct(wchar_t* , wchar_t* ){ /*void*/ }

}//end namespace memory
}//end namespace cgl

#endif //CONSTRUCT_HPP_20181023234341
