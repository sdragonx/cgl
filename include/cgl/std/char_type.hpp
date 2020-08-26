/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 char_type.hpp

 2015-02-03 02:22:04
 2015-09-01 12:44


*/
#ifndef CHAR_TYPE_HPP_20150203022204
#define CHAR_TYPE_HPP_20150203022204

#include <ctype.h>
#include <cgl/public.h>
#include <cgl/stl/ttype.h>

#ifdef isascii
#undef isascii
#undef iswascii
#endif

template<typename _char_t>
class charinfo
{
public:
    typedef _char_t value_type;

protected:
    value_type value;

public:
    charinfo(const value_type& ch) : value(ch) { /*void*/ }

    int isalnum()const
    {
        return tvalue<value_type>(isalnum, iswalnum)(value);
    }
    int isalpha()const
    {
        return tvalue<value_type>(isalpha, iswalpha)(value);
    }
    int isblank()const
    {
        #if defined CGL_PLATFORM_CB6
        return tvalue<value_type>(isspace, iswspace)(value);
        #else
        return tvalue<value_type>(isblank, iswblank)(value);
        #endif
    }
    int iscntrl()const
    {
        return tvalue<value_type>(iscntrl, iswcntrl)(value);
    }
    int isdigit()const
    {
        return tvalue<value_type>(isdigit, iswdigit)(value);
    }
    int isgraph()const
    {
        return tvalue<value_type>(isgraph, iswgraph)(value);
    }
    int islower()const
    {
        return tvalue<value_type>(islower, iswlower)(value);
    }
    int isprint()const
    {
        return tvalue<value_type>(isprint, iswprint)(value);
    }
    int ispunct()const
    {
        return tvalue<value_type>(ispunct, iswpunct)(value);
    }
    int isspace()const
    {
        return tvalue<value_type>(isspace, iswspace)(value);
    }
    int isupper()const
    {
        return tvalue<value_type>(isupper, iswupper)(value);
    }
    int isxdigit()const
    {
        return tvalue<value_type>(isxdigit, iswxdigit)(value);
    }
    int isascii()const
    {
        return tvalue<value_type>(isascii, iswascii)(value);
    }
};

//template<typename T>
//char_info<T> char_type(const T& ch)
//{
//    return char_info<T>(ch);
//}

#endif //CHAR_TYPE_HPP_20150203022204
