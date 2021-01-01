/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shared_ptr.hpp

 2009-08-05 00:39:53

 非全平台

*/
#ifndef SHARED_PTR_HPP_20090805003953
#define SHARED_PTR_HPP_20090805003953

#include "shared_count.hpp"

namespace cgl{

template<typename T>
class shared_ptr
{
private:
    typedef shared_ptr<T> this_type;
public:
    typedef T element_type;
    typedef T value_type;
    typedef T * pointer;
private:
    shared_count pn;
    T *px;
public:
    shared_ptr(void):px(NULL), pn()    { }
    explicit shared_ptr(T *ptr):px(ptr), pn(1) { }
    shared_ptr(const shared_ptr & other):px(other.px), pn(other.pn) { }
    ~shared_ptr(void)
    {
        if(px && pn.unique())
        {
            delete px;
            px = NULL;
        }
    }

    T& operator*(void)
    {
        return *px;
    }

    const T& operator*(void) const
    {
        return *px;
    }

    T *operator->(void)
    {
        return px;
    }

    const T*operator->(void) const
    {
        return px;
    }

    operator bool(void) const
    {
        return px;
    }

    const shared_ptr& operator=(const shared_ptr & other)
    {
        this_type(other).swap(*this);
        return *this;
    }

    T *get(void)
    {
        return px;
    }

    const T *get(void) const
    {
        return px;
    }

    void reset(void)
    {
        this_type().swap(*this);
    }

    void reset(T *ptr)
    {
        this_type(ptr).swap(*this);
    }

    void reset(const shared_ptr & other)
    {
        this_type(other).swap(*this);
    }

    bool unique()const
    {
        return pn.unique();
    }

    long use_count()const
    {
        return pn.use_count();
    }

    void swap(shared_ptr<T> & other)
    {
        std::swap(px, other.px);
        pn.swap(other.pn);
    }
};

}//end namespace cgl

#endif //SHARED_PTR_HPP_20090805003953