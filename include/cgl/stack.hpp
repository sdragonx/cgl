/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stack.hpp

 2019-07-25 19:25:31

*/
#ifndef STACK_HPP_20190725192531
#define STACK_HPP_20190725192531

#include <algorithm>
#include <vector>
#include <functional> //std::less

namespace cgl{

//¶þ²æ¶Ñ
template<class T, class Comp= std::less<T>, class container = std::vector<T> >
class binstack
{
public:
    typedef container container_type;
    typedef Comp compare_func;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;

protected:
    container_type c;   // the underlying container
    static compare_func comp;  // the comparator functor

public:
    binstack() : c()
    {
        std::make_heap(c.begin(), c.end(), comp);
    }

    bool empty() const
    {
        return c.empty();
    }

    size_type size() const
    {
        return (c.size());
    }

    //std extend
    void reserve(size_t size)
    {
        c.reserve(size);
    }

    //std extend
    void clear()
    {
        c.clear();
    }

    const_reference top() const
    {
        return c.front();
    }

    reference top()
    {
        return c.front();
    }

    void push(const value_type& value)
    {
        c.push_back(value);
        push_heap(c.begin(), c.end(), comp);
    }

    void pop()
    {
        pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
};

template<class T, class F, class C>
F binstack<T, F, C>::comp = F();



//
//value_stack<T>
//
//2018-07-20 03:16:44
//value_stack allow have one value.

template<typename T, typename container = std::vector<T> >
class value_stack
{
public:
    typedef T value_type;
    typedef container container_type;

private:
    container_type m_stack;

public:
    value_type value;

public:
    value_stack() : m_stack(), value()
    {
    }

    ~value_stack()
    {
        this->clear();
    }

    value_type* operator->()
    {
        return &value;
    }

    void push()
    {
        m_stack.push_back(value);
    }

    void push(const value_type& a_value)
    {
        this->push();
        value = a_value;
    }

    void pop()
    {
        if(!m_stack.empty()){
            value = m_stack.back();
            m_stack.pop_back();
        }
    }

    size_t size()const
    {
        return m_stack.size();
    }

    bool empty()const
    {
        return m_stack.empty();
    }

    void clear()
    {
        m_stack.clear();
        value = value_type();
    }
};

}//end namespace cgl

#endif //STACK_HPP_20190725192531
