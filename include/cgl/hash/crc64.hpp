/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 crc64.hpp

 2020-05-09 11:43:18

*/
#ifndef CRC64_HPP_20200509114318
#define CRC64_HPP_20200509114318

#include <cgl/public.h>

namespace cgl{
namespace hash{

const uint64_t CRC64_POLY = 0xC96C5795D7870F42ULL;
const int CRC64_NUM_TABLES = 256;

void crc64_gentable(uint64_t table[CRC64_NUM_TABLES])
{
    uint32_t i;
    for (i = 0; i < 256; ++i){
        uint64_t r = i;
        unsigned j;
        for (j = 0; j < 8; j++){
            r = (r >> 1) ^ (CRC64_POLY & ((uint64_t)0 - (r & 1)));
        }
        table[i] = r;
    }
    for (i = 256; i < 256 * CRC64_NUM_TABLES; i++){
        uint64_t r = table[(size_t)i - 256];
        table[i] = table[r & 0xFF] ^ (r >> 8);
    }
}

uint64_t crc64(uint64_t v, const void *data, size_t size, const uint64_t *table)
{
  const byte_t *p = (const byte_t *)data;
  for (; size > 0 && ((unsigned)(ptrdiff_t)p & 3) != 0; size--, p++)
    v = CRC64_UPDATE_BYTE_2(v, *p);
  for (; size >= 4; size -= 4, p += 4)
  {
    uint32_t d = (uint32_t)v ^ *(const uint32_t *)p;
    v = (v >> 32)
        ^ (table + 0x300)[((d      ) & 0xFF)]
        ^ (table + 0x200)[((d >>  8) & 0xFF)]
        ^ (table + 0x100)[((d >> 16) & 0xFF)]
        ^ (table + 0x000)[((d >> 24))];
  }
  for (; size > 0; size--, p++)
    v = CRC64_UPDATE_BYTE_2(v, *p);
  return v;
}

}//end namespace hash
}//end namespace cgl

#endif //CRC64_HPP_20200509114318
