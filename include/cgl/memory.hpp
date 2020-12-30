/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 memory.h

 2010-08-20 00:20:14

*/
#ifndef MEMORY_H_20100820002014
#define MEMORY_H_20100820002014

#include <cgl/memory/allocator.hpp>
#include <cgl/memory/construct.hpp>
#include <cgl/memory/deleter.hpp>

#include <cgl/memory/block.hpp>
#include <cgl/memory/memswap.hpp>
#include <cgl/memory/smart_ptr.hpp>
#include <cgl/memory/shared_ptr.hpp>

namespace cgl{

/*
template<typename T>
class typebuf
{
public:
    typedef typebuf this_type;
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;

private:
    pointer m_ptr;
    size_t  m_size;

public:
    pointer alloc(size_t size)
    {
        this->dispose();
        m_ptr = static_cast<pointer>(malloc(size));
        m_size = size;
    }

    void dispose()
    {
        if(m_ptr){
            free(m_ptr);
            m_ptr = null;
            m_size = 0;
        }
    }

    pointer get()
    {
        return m_ptr;
    }

    const_pointer get()const
    {
        return m_ptr;
    }

    void clear()
    {
        memset(m_ptr, 0, m_size);
    }

private:
    typebuf(const typebuf&);
    this_type& operator=(const this_type&);
};
*/

/*
//memzero ”–Œ Ã‚
template<typename T>
inline void memzero(T* p, size_t size = 1)
{
    memset(p, 0, sizeof(T) * size);
}

template<>inline void memzero(void* p, size_t size)//??
{
    memset(p, 0, size);
}
*/

//alignment mast pow of 2
/*
void* malloc_align(size_t alignment, size_t n)
{
    void *p = std::malloc(alignment + n);
    if(!p)throw std::bad_alloc();
    --alignment;
    p = reinterpret_cast<void*>((reinterpret_cast<intptr_t>(p) + sizeof(intptr_t) + alignment) & (~alignment));
    a
}

void free_align
*/

//---------------------------------------------------------------------------

/*
template<typename _char_t, size_t _bufsize = 0>
class char_buf
{
public:
    typedef T value_type, char_type;
    const static size_t bufsize = _bufsize;
public:
    char_buf()
    {

    }

    size_t size()const
    {
        return MAX_SIZE;
    }

    template<typename str_type>
    operator str_type()const
    {
        return str_type(data);
    }

public:
    char_type data[bufsize];
};

template<typename T>
class char_buf<T, 0>
{
public:
    char_buf():data(null)
    {
    }
    char_buf(size_t size):data(null)
    {
        resize(size);
    }

    ~char_buf()
    {
        dispose();
    }

    void dispose()
    {
        if(data)
        {
            free(data);
            data = null;
        }
    }

    void resize(size_t _size)
    {
        dispose();
        data = (T*)malloc(_size);
    }

    size_t size()const
    {
        return cslen(data);
    }
    template<typename str_type>
    operator str_type()const
    {
        return str_type(data);
    }

    operator char_type*()
    {
        return data;
    }
private:
    size_type m_size;
public:
    char_type* data;
};

*/

}//end namespace cgl


#endif //MEMORY_H_20100820002014
