/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 size_unit.hpp

 2018-03-05 00:41:38

*/
#ifndef SIZE_UNIT_HPP_20180305004138
#define SIZE_UNIT_HPP_20180305004138

#include <cgl/string.hpp>

namespace cgl{
namespace utils{

enum size_unit
{
    SIZE_BYTE = 0,
    SIZE_KB,
    SIZE_MB,
    SIZE_GB,
    SIZE_TB,
    SIZE_PB,
    SIZE_EB,
    SIZE_ZB,
    SIZE_YB,
    SIZE_BB,
    SIZE_NB,
    SIZE_DB,
    SIZE_ERROR = SIZE_MAX
};

template<typename charT>
const charT* size_unit_name(size_unit u)
{
    switch(u)
    {
    case SIZE_BYTE:
        return T_TEXT(charT, "Byte");
    case SIZE_KB://Kilobyte
        return T_TEXT(charT, "KB");
    case SIZE_MB://Megabyte
        return T_TEXT(charT, "MB");
    case SIZE_GB://Gigabyte
        return T_TEXT(charT, "GB");
    case SIZE_TB://Trillionbyte
        return T_TEXT(charT, "TB");
    case SIZE_PB://Petabyte
        return T_TEXT(charT, "PB");
    case SIZE_EB://Exabyte
        return T_TEXT(charT, "EB");
    case SIZE_ZB://Zettabyte
        return T_TEXT(charT, "ZB");
    case SIZE_YB://YottaByte
        return T_TEXT(charT, "YB");
    case SIZE_BB://Brontobyte
        return T_TEXT(charT, "BB");
    case SIZE_NB://NonaByte
        return T_TEXT(charT, "NB");
    case SIZE_DB://DoggaByte
        return T_TEXT(charT, "DB");
    default:
        return null;
    }
}

template<typename charT>
const charT* size_unit_fullname(size_unit u)
{
    switch(u)
    {
    case SIZE_BYTE:
        return T_TEXT(charT, "Byte");
    case SIZE_KB:
        return T_TEXT(charT, "Kilobyte");
    case SIZE_MB:
        return T_TEXT(charT, "Megabyte");
    case SIZE_GB:
        return T_TEXT(charT, "Gigabyte");
    case SIZE_TB:
        return T_TEXT(charT, "Trillionbyte");
    case SIZE_PB:
        return T_TEXT(charT, "Petabyte");
    case SIZE_EB:
        return T_TEXT(charT, "Exabyte");
    case SIZE_ZB:
        return T_TEXT(charT, "Zettabyte");
    case SIZE_YB:
        return T_TEXT(charT, "YottaByte");
    case SIZE_BB:
        return T_TEXT(charT, "Brontobyte");
    case SIZE_NB:
        return T_TEXT(charT, "NonaByte");
    case SIZE_DB:
        return T_TEXT(charT, "DoggaByte");
    default:
        return null;
    }
}

template<typename charT>
cstring<charT> format_file_size(double size)
{
    int n = 0;
    for( ; size > 1024.0f; ++n){
        size /= 1024.0f;
    }

    const charT* u = size_unit_name<charT>(size_unit(n));

    cstring<charT> s;
    s.format(16, T_TEXT(charT, "%0.2f %s"), size, u);
    return s;
}

}//end namespace utils
}//end namespace cgl

#endif //SIZE_UNIT_HPP_20180305004138