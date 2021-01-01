/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 allocator.h

 2016-03-18 15:01:16

 template<typename T>
 class allocator
 allocator<T>        定义名为a的allocator对象，可以分配内存或构造T类型的对象。
 a.allocate(n)       分配原始的构造内存以保存T类型的n个对象.
 a.deallocate(p, n)  释放内存，在名为p的T*指针中包含的地址处保存T类型的n个对象。
 a.construct(p, t)   在T*指针p所指向的内存中构造一个新元素。运行T类型的复制构造函数用t初始化该对象
 a.destroy(p)        运行T*指针p所指向的对象的析构函数。

*/
#ifndef ALLOCATOR_H_20160318150116
#define ALLOCATOR_H_20160318150116

#include <cgl/public.h>
#include <cstdlib>
#include <new>
#include <memory>

namespace cgl{

//use malloc, free
template <typename T>
class malloc_allocator
{
public:
    typedef malloc_allocator  this_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    template<class _Other>
    struct rebind
    {    // convert an allocator<_Ty> to an allocator <_Other>
        typedef malloc_allocator<_Other> other;
    };

public:
    malloc_allocator() { /*void*/ }
    malloc_allocator(const this_type&) { /*void*/ }
    ~malloc_allocator() { /*void*/ }

    this_type& operator=(const this_type&)
    {
        return *this;
    }

    bool operator==(const this_type&)
    {
        return true;
    }

    bool operator!=(const this_type&)
    {
        return false;
    }

    pointer allocate(size_type n, const_pointer = 0)
    {
        void* p = std::malloc(n * sizeof(T));
        if(!p)CGL_THROW( std::bad_alloc() );
        return static_cast<pointer>(p);
    }

    pointer allocate(size_type n, size_type alignment, const_pointer = 0)
    {
        //void* p = malloc_align(alignment, n * sizeof(T));
        //return static_cast<pointer>(p);
        return null;
    }

    void deallocate(pointer p, size_type)
    {
        std::free(p);
    }

    pointer reallocate(pointer ptr, size_type n)
    {
        void *p = static_cast<pointer>(std::realloc(ptr, n * sizeof(T)));
        if(!p)CGL_THROW(std::bad_alloc());
        return static_cast<pointer>(p);
    }

    size_type max_size()const
    {
        return static_cast<size_type>(-1)/sizeof(T);
    }

    void construct(pointer p, const value_type& x)
    {
        new(p) value_type(x);
    }

    void destroy(pointer p)
    {
        p->~value_type();
    }
};

template<>
class malloc_allocator<void>
{
    typedef void        value_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
};



}//end namespace cgl

#endif //ALLOCATOR_H_20160318150116
