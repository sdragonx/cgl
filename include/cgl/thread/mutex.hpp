/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 mutex.h

 2015.01.31 1:49:42

*/

#ifndef CGL_MUTEX_H_20150131014942
#define CGL_MUTEX_H_20150131014942

#include <cgl/public.h>

//
// mutex_t
//

#if defined(CGL_PLATFORM_WINDOWS) && !defined(CGL_PLATFORM_GNUC)

#include <windows.h>

namespace cgl{

class mutex_t
{
private:
    CRITICAL_SECTION m_cs;

public:
    mutex_t()
    {
        InitializeCriticalSection(&m_cs);
    }
    ~mutex_t()
    {
        DeleteCriticalSection(&m_cs);
    }
    void lock()
    {
        EnterCriticalSection(&m_cs);
    }
    void unlock()
    {
        LeaveCriticalSection(&m_cs);
    }

private://uncopyable
    mutex_t(const mutex_t&);
    mutex_t& operator=(const mutex_t&);
};

}//end namespace cgl

#else//posix

#include <pthread.h>

namespace cgl{

class mutex_t
{
private:
    pthread_mutex_t m_mutex;

public:
    mutex_t()
    {
        pthread_mutex_init(&m_mutex, NULL);
    }
    ~mutex_t()
    {
        pthread_mutex_destroy(&m_mutex);
    }
    void lock()
    {
        pthread_mutex_lock(&m_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

private://uncopyable
    mutex_t(const mutex_t&);
    mutex_t& operator=(const mutex_t&);
    friend class cond_t;
};

}//end namespace cgl

#endif

//
// cond_t
//

#ifdef CGL_PLATFORM_GNUC

#include <pthread.h>

namespace cgl{

class cond_t
{
private:
    pthread_cond_t m_cond;
public:
    cond_t()
    {
        pthread_cond_init(&m_cond, NULL);
    }
    ~cond_t()
    {
        pthread_cond_destroy(&m_cond);
    }

    void wait(mutex_t& mutex)
    {
        pthread_cond_wait(&m_cond, &mutex.m_mutex);
    }

    void broadcast()
    {
        pthread_cond_broadcast(&m_cond);
    }
};

}//end namespace cgl

#endif


//
// auto_lock
//

namespace cgl{

class auto_lock
{
public:
    explicit auto_lock(mutex_t& mutex):m_mutex(mutex)
    {
        m_mutex.lock();
    }
    ~auto_lock()
    {
        m_mutex.unlock();
    }
private:
    mutex_t& m_mutex;
    auto_lock(const auto_lock&);
    auto_lock& operator=(const auto_lock&);
};

}// end namespace cgl

#endif //CGL_MUTEX_H_20150131014942

/*

int n = 10;
mutex_t mutex;
cond_t  cond;

void* thread_proc(void* data)
{
    while(n >=0){
        --n;
        system::sleep(100);
        printf("thread : n = %d\r\n", n);
    }
    printf("thread end\n");
    cond.broadcast();//发送信号解锁
    return NULL;
}

int main(int, char**)
{
    pthread_t t;
    pthread_create(&t, 0, thread_proc, 0);

    //等待线程执行结束
    while(n > 0){
        printf("main : n = %d\r\n", n);
        auto_lock lock(mutex);
        cond.wait(mutex);//等待
    }

    printf("main end\r\n");

    return 0;
}

*/