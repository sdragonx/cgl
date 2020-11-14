/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 thread.hpp

 2018-11-14 22:45:19

 频繁的create，还是会卡死

*/
#ifndef THREAD_HPP_20181114224519
#define THREAD_HPP_20181114224519

#include <cgl/public.h>
#include <cgl/thread/mutex.hpp>
#include <cgl/thread/processor.hpp>

#ifdef CGL_PLATFORM_WINDOWS

#include <windows.h>
#include <process.h>

#else

//#define __USE_GNU
#include <pthread.h>

static inline unsigned int pthread_id()
{
    #ifdef PTW32_VERSION
    return pthread_getw32threadid_np(pthread_self());
    #else
    return (unsigned int)pthread_self();
    #endif
}

#endif

namespace cgl{

class thread;

typedef int (*thread_function)(thread* instance, void *arg);

class thread
{
public:
    #ifdef CGL_PLATFORM_WINDOWS
        typedef int handle_type;
    #else
        typedef pthread_t handle_type;
    #endif

    struct thread_state
    {
        handle_type handle;       //线程句柄
        thread_function function; //线程函数
        void * arg;               //线程函数参数
        bool running;             //是否正在运行
        bool pause;               //是否暂停

        thread_state() : handle(0), function(0), arg(0), running(false), pause(false)
        {

        }
    };

//private:
public:
    thread_state * state;

public:
    thread() : state(null)
    {
    }

    handle_type handle()const
    {
        return state ? state->handle : 0;
    }

    int create(thread_function function, void * arg)
    {
        if(!this->is_null()){
            this->stop();
            this->wait();
        }

        mutex_t mutex;

        if(this->is_null()){
            auto_lock lock(mutex);
            state = new thread_state;
            state->function = function;
            state->arg = arg;
            state->running = true;
            state->pause = false;
            #ifdef CGL_PLATFORM_WINDOWS
            state->handle = _beginthread(thread_proc, 1024, this);//default 1k
            #else
            pthread_create(&state->handle, 0, thread_proc, this)
            #endif
        }
        return this->is_null() ? -1 : 0;
    }

    bool is_null()const
    {
        return !state;
    }

    bool is_running()const
    {
        return state && state->running;
    }

    bool is_pause()const
    {
        return state && state->pause;
    }

    void pause()
    {
        if(this->is_running()){
            state->pause = true;
        }
    }

    void resume()
    {
        if(this->is_running()){
            state->pause = false;
        }
    }

    void stop()
    {
        if(!this->is_null()){
            state->running = false;
        }
    }

    void wait()const
    {
        while(!this->is_null());
    }

    void setcpu(uint32_t mask)
    {
        #ifdef CGL_PLATFORM_WINDOWS
        SetThreadAffinityMask((HANDLE)handle(), mask);
        #else

        #endif
    }

private:
    #ifdef CGL_PLATFORM_WINDOWS
    static void thread_proc(void * arg)
    #else
    static void* thread_proc(void * arg)
    #endif
    {
        thread* t = (thread*)arg;
        t->state->function(t, t->state->arg);

        if(true){//exit and free state
            mutex_t mutex;
            auto_lock lock(mutex);
            delete t->state;
            t->state = null;
        }

        #ifndef CGL_PLATFORM_WINDOWS
        return null;
        #endif
    }
};

}//end namespace cgl

#endif //THREAD_HPP_20181114224519
