/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dynamic_array.hpp

 2015-08-19 23:07:40

 还有bug

*/
#ifndef ARRAY_HPP_20150819230740
#define ARRAY_HPP_20150819230740

#include <utility>    //std::swap
#include <iterator>
#include <memory>
#include <cgl/memory/block.hpp>

namespace cgl{

template<typename T, typename A = std::allocator<T> >
class dynamic_array
{
public:
    typedef dynamic_array this_type;
    typedef T value_type;
    typedef A allocator_type;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::difference_type difference_type;

    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef uint32_t size_type;
    const static size_type npos = UINT32_MAX;
    static allocator_type allocator;

private:
    memblock<T, A> m_buffer;
    size_type   m_size;

public:
    dynamic_array(): m_buffer(), m_size(0)
    {
    }

    dynamic_array(size_type size, const T& value = value_type()) : m_buffer(), m_size(0)
    {
        this->assign(size, value);
    }

    dynamic_array(const_iterator first, const_iterator last) : m_buffer(), m_size(0)
    {
        this->assign(first, last);
    }

    dynamic_array(const value_type* first, size_t count) : m_buffer(), m_size(0)
    {
        this->assign(first, first+count);
    }

    ~dynamic_array()
    {
        this->dispose();
    }

    void assign(size_type size, const value_type& value = value_type())
    {
        erase(begin(), end());
        resize(size, value);
    }

    void assign(const_iterator first, const_iterator last)
    {
        erase(begin(), end());
        insert(begin(), first, last);
    }

    void append(const value_type& value)
    {
        expand_alloc(size()+1);
        m_buffer[m_size++] = value;
    }

    this_type& operator=(const this_type& v)
    {
        assign(v.begin(), v.end());
    }

    void resize(size_type size)
    {
        resize(size, value_type());
    }

    void resize(size_type count, const value_type& value)
    {
        if(empty()){
            m_buffer.resize(count);
            fill(begin(), count, value);
        }
        else{
            if(count <= size()){
                erase(begin()+count, end());
            }
            else{
                insert(end(), count-size(), value);
            }
        }
    }

    size_type size()const
    {
        return m_size;
    }

    size_type reserve(size_type size)
    {
        return expand_alloc(size);
    }

    size_type capacity()const
    {
        return m_buffer.size();
    }

    void clear()
    {
        this->erase(begin(), end());
    }

    bool empty()const
    {
        return 0 == size();
    }

    void swap(this_type& v)
    {
        m_buffer.swap(v.buffer);
        std::swap(m_size, v.m_size);
    }

public://插入删除
    iterator insert(iterator where, const value_type& value)
    {
        return insert(where, 1, value);
    }

    iterator insert(iterator where, size_type count, const value_type& value)
    {
        return begin() + insert(where-begin(), count, value);
    }

    iterator insert(iterator where, const_iterator first, const_iterator last)
    {
        return begin() + insert(where-begin(), first, last);
    }

    //插入相同元素
    size_type insert(size_type where, size_type count, const value_type value)
    {
        expand_alloc(size()+count);
        if(where<size())//中间插入
        {
            std::copy(rbegin(), std::reverse_iterator<iterator>(begin()+where), rbegin()-count);
        }
        fill(begin()+where, count, value);
        m_size += count;
        return where;
    }

    //插入
    size_type insert(size_type where, const_iterator first, const_iterator last)
    {
        size_type count = last - first;
        expand_alloc(size()+count);
        if(where < size())//中间插入
        {
            std::copy(rbegin(), std::reverse_iterator<iterator>(begin()+where), rbegin()-count);
        }
        std::copy(first, last, begin()+where);
        m_size += count;
        return where;
    }

    iterator erase(iterator where)
    {
        assert(where < end());
        std::copy(where + 1, end(), where);
        destroy(begin()+(--m_size));
        return where;
    }

    iterator erase(iterator first, iterator last)
    {
        if(m_buffer.empty()){
            return this->end();
        }

        if(first < this->begin()){
            first = this->begin();
        }

        if(last > this->end()){
            last = this->end();
        }

        if(last < first){
            last = first;
        }

        if(first != last)
        {
            destroy(last, this->end()-last);
            std::copy(last, end(), first);
            m_size -= last-first;
        }
        return first;
    }

public://iterator
    iterator begin() { return m_buffer.begin(); }
    iterator end  () { return m_buffer.end(); }
    const_iterator begin()const { return m_buffer.begin(); }
    const_iterator end  ()const { return m_buffer.end(); }
    reverse_iterator rbegin() { return reverse_iterator(end());   }
    reverse_iterator rend  () { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin()const { return const_reverse_iterator(end());   }
    const_reverse_iterator rend  ()const { return const_reverse_iterator(begin()); }

public://数据操作
    value_type& operator[](size_type index)
    {
        return m_buffer[index];
    }

    const value_type& operator[](size_type index)const
    {
        return m_buffer[index];
    }

    value_type& at(size_type index)
    {
        if(index >= m_size)
        {
            throw "invalid dynamic_array<T> subscript";
        }
        return m_buffer[index];
    }

    const value_type& at(size_type index)const
    {
        if(index >= m_size)
        {
            throw "invalid dynamic_array<T> subscript";
        }
        return m_buffer[index];
    }

    value_type& front()
    {
        return *begin();
    }

    const value_type& front()const
    {
        return *begin();
    }

    value_type& back()
    {
        return *(end()-1);
    }

    const value_type& back()const
    {
        return *(end()-1);
    }

    void push_back(const value_type& value = value_type())
    {
        append(value);
    }

    void pop_back()
    {
        erase(end()-1);
    }

private:
    //扩充分配空间
    size_type expand_alloc(size_type size)
    {
        size_type alloc_size = capacity();
        while(alloc_size < size)
        {
            if(alloc_size < 4){
                ++alloc_size;
            }
            else{
                alloc_size = alloc_size * 2;
            }
        }
        m_buffer.resize(alloc_size);
        return alloc_size;
    }

    void dispose()
    {
        m_buffer.dispose();
        m_size = 0;
    }

    void destroy(iterator element)
    {
        //element->~value_type();
        element->~T();
    }

    void fill(iterator first, size_type count, const value_type& value)
    {
        std::fill_n(first, count, value);
    }
    //初始化n个变量
    void construct(pointer p, size_t n, const value_type& value)
    {
        for(size_t i=0; i<n; ++i){
            allocator.construct(p+i, value);
        }
    }
    void destroy(pointer p, size_t n)
    {
        for(size_t i=0; i<n; ++i)
            allocator.destroy(p);
    }

};

template<typename T, typename A>
A dynamic_array<T, A>::allocator = A();

}//end namespace cgl

#endif //ARRAY_HPP_20150819230740
