/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stringref.hpp

 2016-04-08 00:35:03

 这个字符串类，只是别的字符串的一个引用，自己不负责任何内存分配删除操作

*/
#ifndef STRINGREF_HPP_20160408003503
#define STRINGREF_HPP_20160408003503

#include <cgl/public.h>
#include <cgl/algorithm.hpp>
#include <cgl/memory.hpp>
#include <cgl/stdlib.hpp>
#include <cgl/string/stringsplit.hpp>
#include <cgl/std/char_traits.hpp>

namespace cgl{

template<typename _char_t>
class stringref
{
public:
    typedef stringref<_char_t> this_type;
    typedef _char_t value_type, char_type;
    typedef const char_type* const_pointer;
    typedef const_pointer const_iterator;
    typedef const std::reverse_iterator<char_type> const_reverse_iterator;
    typedef const char_type& const_reference;
    typedef std::size_t size_type;
    typedef char_traits<value_type> traits;
    const static size_type npos = TEXT_MAX;

private:
    memlink<_char_t> m_link;

public:
    stringref() : m_link()
    {
    }

    stringref(const_pointer str) : m_link(str, cslen(str))
    {
    }

    stringref(const_pointer str, size_type size) : m_link(str, size)
    {
    }

    stringref(const_iterator Begin, const_iterator End) : m_link(Begin, End - Begin)
    {

    }

    ~stringref()
    {
        this->dispose();
    }

    this_type& operator=(const_pointer str)
    {
        m_link.link(str, cslen(str));
        return *this;
    }

//iterator and reference
    inline const_pointer c_str()const { return m_link.data(); }
    inline const_pointer data()const { return m_link.data(); }
    inline const_iterator begin()const { return this->data(); }
    inline const_iterator end()const { return this->begin()+this->size(); }
    inline const_reverse_iterator rbegin()const { return const_reverse_iterator(this->end()); }
    inline const_reverse_iterator rend()const { return const_reverse_iterator(this->begin()); }
    inline const_reference operator[](size_type pos)const { return this->data()[pos]; }
    const_reference front()const { return *this->begin(); }
    const_reference back()const { return *(this->end()-1); }

    void dispose()
    {
        m_link.dispose();
    }

    bool empty()const
    {
        return m_link.empty();
    }

    size_t size()const
    {
        return m_link.size();
    }

    size_t length()const
    {
        return this->size();//cslen(m_str);
    }

//find
    size_type find(const char_type& ch)const
    {
        return std::find(this->begin(), this->end(), ch);
    }
    size_type find(const char_type* _str)
    {
        const_pointer p = csstr(this->m_str(), _str);
        return p ? p-this->begin() : npos;
    }
    size_type find(const char_type& ch)
    {
        const_pointer p = cschr(data(), ch);
        return p ? p-this->begin() : npos;
    }

    size_type rfind(const_pointer str, size_t len = npos)
    {
        if(len == npos)len = traits::length(str);
        const_pointer itr = this->end()-len-1;
        for( ; this->begin()<itr; --itr)
        {
            if(!csncmp(itr, str, len))
            {
                return itr-this->begin();
            }
        }
        return npos;
    }

    size_type find_first_of(const_pointer _str)
    {
        //return cspbrk(data(), _str);
        const_iterator itr = std::find_first_of(begin(), end(), _str, _str + cslen(_str));
        return itr != this->end() ? itr - this->begin() : npos;
    }
    size_type find_last_of(const_reference ch)const
    {
        const_iterator itr = cgl::find_last_of(this->begin(), this->end(), ch);
        return itr != this->end() ? itr - this->begin() : npos;
    }
    size_type find_last_of(const_pointer _str)const
    {
        //const_iterator itr = find_last_of(begin(), end(), _str, _str+stringx_opset<char_type>::length(_str));
        //return itr != this->end() ? itr - this->begin() : npos;
        return 0;
    }

    size_type find_first_not_of(const_iterator find_begin, const_iterator find_end)const
    {
        const_iterator itr = cgl::find_first_not_of(this->begin(), this->end(), find_begin, find_end);
        return itr != this->end() ? itr-this->begin() : npos;
    }
    size_type find_first_not_of(const_pointer str, size_type len = npos)const
    {
        if(len == npos)len = cslen(str);
        return this->find_first_not_of(str, str+len);
    }
    size_type find_first_not_of(const_reference value)const
    {
        //const_iterator itr = find_first_not_of(this->begin(), this->end(), find_begin, find_end);
        //return itr != this->end() ? itr-this->begin() : npos;
        return 0;
    }

//operator <
    bool operator<(const char_type* str)const
    {
        //return compare(str);
        return cscmp(this->data(), str)<0;
    }
    bool operator<(const this_type& str)const
    {
        //return compare(str);
        return cscmp(this->data(), str.data())<0;
    }
//operator ==
    bool operator==(const char_type* str)const
    {
        return !compare(str);
    }
    bool operator==(const this_type& str)const
    {
        return !compare(str);
    }
//compare
    int compare(const char_type* str)const
    {
        return csncmp(this->data(), str, this->size());
    }
    int compare(const char_type* str, size_type count)const
    {
        return csncmp(this->data(), str, count);
    }
    int compare(const this_type& str)const
    {
        return csncmp(this->data(), str.c_str(), str.length());
    }
    int compare(const this_type& str, size_type count)const
    {
        return csncmp(this->data(), str.c_str(), count);
    }

    int icompare(const char_type* str)const
    {
        return csicmp(data(), str, this->size());
    }
    int icompare(const char_type* str, size_type count)const
    {
        return csnicmp(data(), str, count);
    }
    int icompare(const this_type& str)const
    {
        return csicmp(data(), str.c_str());
    }
    int icompare(const this_type& str, size_type count)const
    {
        return csnicmp(data(), str.c_str(), count);
    }

//to numeric
    int        to_int(int radix = 10)const        { return cstoint<int32_t, char_type>(this->c_str(), null, radix); }
    int        to_int64(int radix = 10)const    { return cstoint<int64_t, char_type>(this->c_str(), null, radix); }
    double    to_decimal()const                { return cstof(this->c_str()); }
    bool    to_boolean()const                { return cstobool(this->c_str()); }
};

}//end namespace cgl

#endif //STRINGREF_HPP_20160408003503
