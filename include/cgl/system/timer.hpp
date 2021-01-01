/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 timer.hpp

 2018-01-10 01:26:54

*/
#ifndef TIMER_HPP_20180110012654
#define TIMER_HPP_20180110012654

#include <cgl/public.h>
#include <cgl/delegate.hpp>
#include <cgl/std/map/idmap.hpp>
#include <cgl/system/time.hpp>

namespace cgl{
namespace system{

class timer
{
public:
    typedef delegate<void (*)()> callback_t;
    struct TIMER_THREAD
    {
        int id;
        uint64_t time;
        uint64_t interval;
        bool pause;
        callback_t callback;
        TIMER_THREAD() : id(), time(), interval(), pause(), callback(){ /*void*/ }
    };

private:
    idmap<TIMER_THREAD> m_threads;

public:
    int create(uint64_t interval, callback_t call, bool pause = false)
    {
        TIMER_THREAD t;
        t.time = time::clock_microseconds();
        t.interval = interval;
        t.pause = pause;    //default pause = false
        t.callback = call;
        int id = m_threads.allocate();
        m_threads[id] = t;
        return id;
    }

    int start(int index)
    {
        return resume(index);
    }

    int pause(int index)
    {
        if(m_threads.exists(index)){
            m_threads[index].pause = true;
            return 0;
        }
        return -1;
    }

    int resume(int index)
    {
        if(m_threads.exists(index)){
            m_threads[index].pause = false;
            return 0;
        }
        return -1;
    }

    int kill(int index)
    {
        if(m_threads.exists(index)){
            m_threads.erase(index);
            return 0;
        }
        return -1;
    }

    void update()
    {
        uint64_t t = time::clock_microseconds();
        idmap<TIMER_THREAD>::iterator next;
        for(idmap<TIMER_THREAD>::iterator i=m_threads.begin(); i != m_threads.end(); i = next){
            next = i;
            ++next;
            if(!i->second.pause && (t - i->second.time >= i->second.interval)){
                i->second.time = t;
                if(i->second.callback)i->second.callback();
            }
        }
    }
};


}//end namespace system
}//end namespace cgl

#endif //TIMER_HPP_20180110012654
