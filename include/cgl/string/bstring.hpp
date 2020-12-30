/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 block_string.hpp

 2019-05-27 12:47:25

 c style string

 固定长度，可以放到结构体、union中

*/
#ifndef BSTRING_HPP_20190527124725
#define BSTRING_HPP_20190527124725

#include <cgl/public.h>
#include "stringref.hpp"

namespace cgl{

template<typename _char_t, std::size_t _length>
class blockstring
{
public:
    typedef blockstring this_type;
    typedef _char_t char_type;
    typedef char_type* pointer;
    typedef const pointer const_pointer;
    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::size_t size_type;
    const static std::size_t max_size = _length < 4 ? 4 : _length;
private:
    char_type m_data[max_size];
public:
    void clear() { this->endsize(0); }

    this_type& assign(const_pointer str)
    {
        csncpy(m_data, str, max_size);
        return *this;
    }

    this_type& assign(const_pointer str, size_type size)
    {
        if(max_size < size)size = max_size;
        csncpy(m_data, str, size);
        return *this;
    }

    this_type& assign(const this_type& str)
    {
        return this->assign(str.c_str());
    }

    this_type& assign(const this_type& str, size_type size)
    {
        return this->assign(str.c_str(), size);
    }

    const_pointer c_str()const { return m_data; }

    pointer data() { return m_data; }
    const_pointer data()const { return m_data; }

    size_type size()const   { return cslen(m_data); }
    size_type length()const { return cslen(m_data); }

    int compare(const_pointer str)const { return csncmp(m_data, str, max_size); }

    bool operator==(const_pointer str)const    { return this->compare(str) == 0;        }
    bool operator==(const this_type& str)const { return this->compare(str.m_data) == 0; }
    bool operator!=(const_pointer str)const    { return this->compare(str) != 0;        }
    bool operator!=(const this_type& str)const { return this->compare(str.m_data) != 0; }
    bool operator<(const_pointer str)const     { return this->compare(str) < 0;         }
    bool operator<(const this_type& str)const  { return this->compare(str.m_data) < 0;  }

private:
    void endsize(size_t pos) { m_data[pos] = '\0'; }
};

}//end namespace cgl

#endif //BSTRING_HPP_20190527124725
