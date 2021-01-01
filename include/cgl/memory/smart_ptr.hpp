/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 smart_ptr.hpp

 2006-06-09 18:26
 2018-10-13 12:49

*/
#ifndef CGL_SMART_PTR_HPP_200606091826
#define CGL_SMART_PTR_HPP_200606091826

namespace cgl{

/*
 alone_ptr

 can not construct and copy

*/
template<typename T>
class alone_ptr
{
public:
    typedef T value_type;
    typedef T* pointer;

private:
    pointer m_ptr;

public:
    alone_ptr( pointer p )
    {
        m_ptr = p;
    }

    ~alone_ptr()
    {
        free();
    }

    bool is_null()const
    {
        return !m_ptr;
    }

    const pointer get()const
    {
        return m_ptr;
    }
    
    pointer get()
    {
        return m_ptr;
    }

    void reset(pointer ptr)
    {
        this->free();
        m_ptr = ptr;
    }

    pointer operator->()
    {
        return m_ptr; 
    }

    const pointer operator->()const
    {
        return m_ptr; 
    }

    void free()
    {
        if(m_ptr){
            delete m_ptr;
            m_ptr = NULL;
        }
    }

    pointer release()
    {
        pointer temp = m_ptr;
        m_ptr = NULL;
        return temp;
    }

    bool operator==(const pointer pt){ return m_ptr == pt; }
    bool operator!=(const pointer pt){ return m_ptr != pt; }

private:
    alone_ptr( const alone_ptr& );
    pointer operator=( const alone_ptr& );
    pointer operator=( pointer p );
};

}//end namespace cgl

#endif //CGL_SMART_PTR_HPP_200606091826
