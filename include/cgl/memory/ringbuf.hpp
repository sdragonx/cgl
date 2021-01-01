/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ringbuffer.hpp

 2019-06-17 11:18:30

 如何设计是个问题

 ringbuf.push();
 ringbuf.front();
 ringbuf.back();
 ringbuf.size();
 ringbuf[0~size];
 ringbuf::iterator ??

*/
#ifndef RINGBUFFER_HPP_20190617111830
#define RINGBUFFER_HPP_20190617111830

#include <cgl/public.h>

namespace cgl{

template<typename T>
class ringbuffer;

template<typename T>
class ringbuf_iterator
{
public:
    typedef ringbuf_iterator this_type;
    typedef ringbuffer<T> buffer_type;

private:
    buffer_type& buffer;

public:
    ringbuf_iterator(buffer_type& buf)
    {
    }

    ringbuf_iterator(this_type& other)
    {
    }

    ringbuf_iterator operator++()
    {
    }

    ringbuf_iterator operator--()
    {
    }

};

template<typename T>
class ringbuffer
{
public:
    typedef T value_type;
    std::vector<value_type> m_buffer;
    int m_begin;
    //int end;

public:
    ringbuffer() : m_buffer(), m_begin()
    {

    }

    void resize(size_t size)
    {
        m_buffer.resize(size);
        m_begin = 0;
    }

    void push(const value_type& value)
    {
        --m_begin;
        if(m_begin < 0){
            m_begin += m_buffer.size();
        }
        m_buffer[m_begin] = value;
    }

    value_type& operator[](int index)
    {
        index += m_begin;
        if(index >= m_buffer.size()){
            index -= m_buffer.size();
        }
        return m_buffer[index];
    }

    const value_type& operator[](int index)const
    {
        index += m_begin;
        if(index >= m_buffer.size()){
            index -= m_buffer.size();
        }
        return m_buffer[index];
    }

protected:
    //n = -1 | 1
    void shift_pointer(int n)
    {
        if(n < 0){
            --m_begin;
            if(m_begin < 0){
                m_begin += m_buffer.size();
            }
        }
        else{
            ++m_begin;
            if(m_begin >= m_buffer.size()){
                m_begin -= m_buffer.size();
            }

        }
    }

    void move_pointer(int n)
    {
        n = n % m_buffer.size();
        m_begin += n;
        if(m_begin < 0){
            m_begin += m_buffer.size();
        }
        else if(m_begin >= m_buffer.size()){
            m_begin -= m_buffer.size();
        }
    }
};


//一个线程添加，一个线程读取
template<typename T>
class ringbuffer
{
public:
    typedef T value_type;

public:
    value_type m_data[1024*256];
    size_t m_front;
    size_t m_back;

    ringbuffer() :
        m_front(0),
        m_back(0)
    {
        memset(m_data, 0, sizeof(m_data));

    }

    size_t capacity()const
    {
        return 1024 * 256;
    }

    size_t size()
    {
        if(m_front < m_back){
            return m_back - m_front;
        }
        else{
            return m_back + capacity() - m_front;
        }
    }

    value_type& front()
    {
        return m_data[m_front];
    }

    value_type& back()
    {
        return m_data[m_back];
    }

    bool pop()
    {
        if(m_back == m_front){
            return false;
        }
        size_t p = this->step(m_front);
        m_front = p;
        return true;
    }

    bool push(const value_type& value)
    {
        if(m_front == m_back){
            return false;
        }
        size_t p = this->step(m_back);
        m_back = p;
        m_data[m_back] = value;
        return true;
    }

private:
    size_t step(size_t value)
    {
        ++value;
        if(value >= capacity()){
            value = 0;
        }
        return value;
    }
};



template<typename T>
class ring_queue;

template<typename T>
class ring_queue_iterator
{
public:
    typedef ring_queue_iterator this_type;
    typedef T value_type;
    typedef ring_queue<value_type> queue_type;

private:
    queue_type& ring;
    size_t pos;

public:
    ring_queue_iterator(queue_type& q, size_t p) : ring(q), pos(p)
    {
    }

    ring_queue_iterator(const this_type& other) : ring(other.ring), pos(other.pos)
    {
    }

    this_type& operator++()
    {
        ++pos;
        return *this;
    }

    this_type& operator--()
    {
        --pos;
        return *this;
    }

    value_type& operator*()
    {
        return ring[pos];
    }

    bool operator!=(const this_type& other)const
    {
        return pos != other.pos;
    }

};


//2020-03-12 21:35:08
template<typename T>
class ring_queue
{
public:
    typedef T value_type;
    typedef ring_queue_iterator<value_type> iterator;

private:
    std::vector<value_type> m_buffer;
    int m_begin;
    //int end;

public:
    ring_queue() : m_buffer(), m_begin()
    {

    }

    ring_queue(size_t n) : m_buffer(n), m_begin()
    {

    }

    size_t size()const
    {
        return m_buffer.size();
    }

    void resize(size_t size)
    {
        m_buffer.resize(size);
        m_begin = m_begin % size;
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, m_buffer.size());
    }

    value_type& first()
    {
        return m_buffer[m_begin];
    }

    value_type& last()
    {
        int last = m_begin + m_buffer.size() - 1;
        if(last >= m_buffer.size()){
            last -= m_buffer.size();
        }
        return m_buffer[last];
    }

    void push(const value_type& value)
    {
        ++m_begin;
        if(m_begin >= m_buffer.size()){
            m_begin -= m_buffer.size();
        }
        this->last() = value;
    }

    value_type& operator[](int index)
    {
        index += m_begin;
        if(index >= m_buffer.size()){
            index -= m_buffer.size();
        }
//        index %= m_buffer.size();
        return m_buffer[index];
    }

    const value_type& operator[](int index)const
    {
        index += m_begin;
        if(index >= m_buffer.size()){
            index -= m_buffer.size();
        }
//        index %= m_buffer.size();
        return m_buffer[index];
    }
};

}//end namespace cgl

#endif //RINGBUFFER_HPP_20190617111830
