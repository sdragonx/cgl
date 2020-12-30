/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 lookup_table.hpp

 2020-11-28 08:54:58

*/
#ifndef LOOKUP_TABLE_HPP_20201128085458
#define LOOKUP_TABLE_HPP_20201128085458

#include <cgl/public.h>

namespace cgl{
namespace graphics{

//
// 查表对颜色进行过滤
//

template<typename T>
inline void lookup_table(T& c, byte_t table[256])
{
    c.r = table[c.r];
    c.g = table[c.g];
    c.b = table[c.b];
}

//BGRA
template<>
inline void lookup_table<uint32_t>(uint32_t& c, byte_t table[256])
{
    byte_t r = table[BGRA_RED(c)];
    byte_t g = table[BGRA_GREEN(c)];
    byte_t b = table[BGRA_BLUE(c)];
    byte_t a = BGRA_RED(c);

    c = BGRA(r, g, b, a);
}

}//end namespace graphics
}//end namespace cgl

#endif //LOOKUP_TABLE_HPP_20201128085458
