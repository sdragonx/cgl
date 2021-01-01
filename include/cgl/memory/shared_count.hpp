/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shared_count.h

 2010-09-18 14:21:55

*/
#ifndef SHARED_COUNT_H_20100918142155
#define SHARED_COUNT_H_20100918142155

#include <cgl/public.h>
#include <utility>    //std::swap
#include <cgl/thread/atom.hpp>

namespace cgl{

class shared_count
{
private:
    long *m_ref_count;
public:
    shared_count(void):m_ref_count(NULL) { }
    shared_count(long n):m_ref_count(new long(n)) { }
    shared_count(const shared_count& other):m_ref_count(other.m_ref_count)
    {
        if (m_ref_count){////引用计数加1
            atom_add(m_ref_count);
        }
    }
    ~shared_count(void)
    {
        if(m_ref_count)
        {//引用计数减1
            long n = atom_sub(m_ref_count);
            if (!n){
                delete m_ref_count;
                m_ref_count = NULL;
            }
        }
    }

    const shared_count& operator=(const shared_count & other)
    {
        shared_count(other).swap(*this);
        return *this;
    }

    bool unique()const
    {
        return this->use_count() == 1;
    }

    long use_count()const
    {
        return m_ref_count ? *m_ref_count : 0;
    }

    void swap(shared_count & other)
    {
        using std::swap;
        swap(m_ref_count, other.m_ref_count);
    }
};

}//end namespace cgl

#endif //SHARED_COUNT_H_20100918142155

