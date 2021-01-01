/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pool.hpp

 2018-10-22 14:29:23

 struct element
 {
    pointer p;
    size_t size;
    pointer next;
 }

*/
#ifndef POOL_ALLOCATOR_20181022142923
#define POOL_ALLOCATOR_20181022142923

#include <cgl/public.h>
//#include <cgl/std/dynamic_array.hpp>
#include <cgl/memory/block.hpp>

namespace cgl{


//
// pool
//
// ������һ���͵����ݣ�ÿҳ�̶�SIZE��������ҳ��Сһ��
// һ��ֻ�����뵥һ����
// ������ڴ棬��poolδ���ͷ�֮ǰ���ڴ��ַ�ǲ����
//

template <typename T, size_t PAGE>
class pool
{
public:
    typedef pool              this_type;
    typedef T                 value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    const static size_t PAGE_SIZE = PAGE;

private:
    struct node{
        node* next;
        value_type value;
    };

    //�������������ռ�
    struct buffer_type
    {
        void* list[PAGE_SIZE];
        value_type data[PAGE_SIZE];
    };

    node* entry;    //free space entry
    std::vector<void*> poolbuf;

public:
    pool() : entry()
    {
    }

    pool(const this_type&) : entry()
    {
    }

    ~pool()
    {
        this->dispose();
    }

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

    pointer allocate(size_type size = 1, const_pointer = 0)
    {
        CGL_ASSERT(size == 1);
        if(!entry){
            node* buf = allocate_buffer();
            entry = buf;
        }
        node* n = entry;
        entry = entry->next;
        return this->address(n);
    }

    pointer allocate(size_type n, size_type alignment, const_pointer = 0)
    {
        CGL_ASSERT(false);
        return null;
    }

    void deallocate(pointer p, size_type = 0)
    {
        node * n = node_pointer(p);
        n->next = entry;
        entry = n;
    }

    pointer reallocate(pointer ptr, size_type n)
    {
        CGL_ASSERT(false);
        return null;
    }

    void dispose()
    {
        for(size_type i = 0; i < poolbuf.size(); ++i)
        {
            deallocate_buffer(poolbuf[i]);
        }
        poolbuf.clear();
//        poolbuf.dispose();
        entry = null;
    }

    size_type max_size()const
    {
        return static_cast<size_type>(-1)/sizeof(T);
    }

    size_type size()const
    {
        return poolbuf.size() * PAGE_SIZE;
    }

    //ͳ�����ɿռ��С
    size_type free_size()const
    {
        size_type n = 0;
        node* p = entry;
        while(p){
            ++n;
            p = p->next;
        }
        return n;
    }

    void construct(pointer p, const value_type& x)
    {
        new(p) value_type(x);
    }

    void construct(pointer p)
    {
        new (p) value_type();
    }

    void destroy(pointer p)
    {
        p->~T();
    }
private:
    node* allocate_buffer()
    {
//        node* buf = new node[PAGE_SIZE];
        node* buf = static_cast<node*>(malloc(PAGE_SIZE * sizeof(node)));

        //init page
        for(size_type i=0; i<PAGE_SIZE; ++i){
            buf[i].next = buf + i + 1;
        }
        buf[PAGE_SIZE - 1].next = null;
        poolbuf.push_back(buf);
        return buf;
    }

    void deallocate_buffer(void* &buf)
    {
        node* p = static_cast<node*>(buf);
        //delete []p;
        free(p);
        buf = null;
    }

    pointer address(node* n)
    {
        return &n->value;
    }

    node* node_pointer(void* p)
    {
        return reinterpret_cast<node*>(reinterpret_cast<byte_t*>(p) - sizeof(node*));
    }
};

}//end namespace cgl

#endif //POOL_ALLOCATOR_20181022142923

/*
    typedef string_t type;
    pool_allocator<type, 64> pool;

    type* pi[2048];

    int t = std::clock();
    for(int i=0; i<10000; ++i)
    {
        for(int j=0; j<2048; ++j){
            pi[j] = new type;
        }
        for(int j=0; j<2048; ++j){
            delete pi[j];
        }
    }
    print(std::clock() - t);

    t = std::clock();
    for(int i=0; i<10000; ++i)
    {
        for(int j=0; j<2048; ++j){
            pi[j] = pool.allocate();
            //pool.construct(pi[j]);//malloc�汾��Ҫ�ֶ�����
        }
        for(int j=0; j<2048; ++j){
            //pool.destroy(pi[j]);//malloc�汾��Ҫ�ֶ�����
            pool.deallocate(pi[j]);
        }
    }
    print(std::clock() - t);
    print(pool.size());
*/