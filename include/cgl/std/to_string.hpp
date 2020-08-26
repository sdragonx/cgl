/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 to_string.h

 2018-07-27 01:16:07

*/
#ifndef TO_STRING_H_20180727011607
#define TO_STRING_H_20180727011607

#include <cstdlib>
#include <cgl/stl/ttype.h>

namespace cgl{

//
//bufsize_of
//calc numeric to string need buffer size

template<typename T, size_t n>
struct bufsize_base
{
    const static size_t value = n;
};

template<typename T>
struct bufsize_of : public bufsize_base<T, sizeof(T)*8+1> { };

//end bufsize_of

template<typename _char_t>
_char_t* to_string(_char_t* buf, int value, int radix = 10)
{
    return tvalue<_char_t>(itoa, _itow)(value, buf, radix);
}

template<typename string_type>
string_type to_string(int value)
{
    STL_TYPENAME string_type::value_type buf[33];
    to_string(buf, value, 10);
    return string_type(buf);
}


template<typename _char_t>
_char_t* to_string(_char_t* buf, int64_t value, int radix = 10)
{
    return tvalue<_char_t>(_ui64toa, _ui64tow)(value, buf, radix);
}

template<typename string_type>
string_type to_string(int64_t value)
{
    STL_TYPENAME string_type::value_type buf[65];
    to_string(buf, value, 10);
    return string_type(buf);
}

template<typename _char_t>
const _char_t* to_string(_char_t* buf, bool value)
{
    return cscpy<_char_t>(buf, value ? T_TEXT(_char_t, "true") : T_TEXT(_char_t, "false"));
}

template<typename _char_t>
_char_t* format_float(_char_t* buf, size_t size, double value, int dec = 0, int precision = 0)
{
    int length = dec + precision +1;
    if(!length){
        csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%0.16G"), value);
    }
    else{
        csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%0*.*f"), length, precision, value);
    }
    return buf;
}

}//end namespace cgl

#endif //TO_STRING_H_20180727011607
