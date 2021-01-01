/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 singleton.h

 sdragonx ‎2015.01.31 1:49:42

*/

#ifndef SINGLETON_H_20150131014903
#define SINGLETON_H_20150131014903

#include <stdlib.h>
#include "mutex.hpp"

namespace cgl{

#if 0

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

#else

template<class T>
class singleton
{
public:
    static T& instance()// Unique point of access
    {
        if (0 == m_instance){
            auto_lock lock(m_mutex);
            if (0 == m_instance){
                m_instance = new T();
                atexit(destroy);
            }
        }
        return *m_instance;
    }
protected:
    singleton(){}
    ~singleton(){}
private:
    singleton(const singleton&);
    singleton& operator=(const singleton&);
    static void destroy()// Destroy the only instance
    {
        if ( m_instance != 0 ){
            delete m_instance;
            m_instance = 0;
        }
    }
    static mutex_t m_mutex;
    static T * volatile m_instance; // The one and only instance
};

template<class T>
mutex_t singleton<T>::m_mutex;

template<class T>
T * volatile singleton<T>::m_instance = 0;

#endif

}// end namespace cgl

#endif //SINGLETON_H_20150131014903
