/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 atom.hpp

 2019-06-14 10:26:13

*/
#ifndef ATOM_HPP_20190614102613
#define ATOM_HPP_20190614102613

#if defined(CGL_PLATFORM_WINDOWS) && !defined(CGL_PLATFORM_GNUC)

    #include <windows.h>

#else

    #include <pthread.h>

#endif

namespace cgl{

//set and return last value
inline long atom_set(long volatile *atom, long value)
{
    #if defined(CGL_PLATFORM_WINDOWS)
    return InterlockedExchange(atom, value);
    #elif defined(CGL_PLATFORM_POSIX)
    return __sync_lock_test_and_set(atom, value);
    #endif
}

//++i
inline long atom_add(long volatile *atom)
{
    #if defined(CGL_PLATFORM_WINDOWS)
    return InterlockedIncrement(atom);
    #elif defined(CGL_PLATFORM_POSIX)
    return __sync_add_and_fetch(atom, 1);
    #endif
}

//--i
inline long atom_sub(long volatile *atom)
{
    #if defined(CGL_PLATFORM_WINDOWS)
    return InterlockedDecrement(atom);
    #elif defined(CGL_PLATFORM_POSIX)
    return __sync_sub_and_fetch(atom, 1);
    #endif
}

//+=
inline long atom_add(long volatile *atom, long value)
{
    #if defined(CGL_PLATFORM_WINDOWS)
    return InterlockedExchangeAdd(atom, value);
    #elif defined(CGL_PLATFORM_POSIX)
    return __sync_sub_and_fetch(atom, value);
    #endif
}

//-=
inline long atom_sub(long volatile *atom, long value)
{
    #if defined(CGL_PLATFORM_WINDOWS)
    return InterlockedExchangeAdd(atom, -value);
    #elif defined(CGL_PLATFORM_POSIX)
    return __sync_sub_and_fetch(atom, value);
    #endif
}

class atom_t
{
protected:
    long m_value;

public:
    atom_t() : m_value()
    {
    }

    long set(long value)
    {
        return atom_set(&m_value, value);
    }

    long get()const
    {
        return m_value;
    }

    long add()
    {
        return atom_add(&m_value);
    }

    long sub()
    {
        return atom_sub(&m_value);
    }

    long add(long value)
    {
        return atom_add(&m_value, value);
    }

    long sub(long value)
    {
        return atom_sub(&m_value, value);
    }

    long operator++()
    {
        return atom_add(&m_value);
    }

    long operator--()
    {
        return atom_sub(&m_value);
    }

    //这个不准确
    long operator++(int)
    {
        long value = m_value;
        atom_add(&m_value);
        return value;
    }

    long operator--(int)
    {
        long value = m_value;
        atom_sub(&m_value);
        return value;
    }
};


}//end namespace cgl

#endif //ATOM_HPP_20190614102613
