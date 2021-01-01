/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pkcrypt.hpp

 2020-05-12 16:03:06

 pkzip�����㷨

*/
#ifndef PKCRYPTO_HPP_20200512160306
#define PKCRYPTO_HPP_20200512160306

#include <cgl/public.h>
#include <cgl/hash/crc32.hpp>

namespace cgl{

class pkcrypt
{
public:
    uint32_t keys[3];

    void init(const char* password, size_t size = TEXT_MAX)
    {
        if(size == TEXT_MAX){
            size = strlen(password);
        }

        keys[0] = 305419896;
        keys[1] = 591751049;
        keys[2] = 878082192;

        for(size_t i=0; i<size; ++i){
            update_keys(password[i]);
         }
    }

    void update_keys(byte_t c)
    {
        keys[0]  = hash::crc32_update(keys[0], c);
        keys[1] += keys[0] & 0xFF;
        keys[1]  = keys[1] * 134775813 + 1;
        keys[2]  = hash::crc32_update(keys[2], keys[1] >> 24);
    }

    byte_t encode(byte_t c)
    {
        byte_t t = decrypt_byte(keys);
        update_keys(c);
        return (byte_t)(t ^ c);
    }

    byte_t decode(byte_t c)
    {
        c ^= decrypt_byte(keys);
        update_keys(c);
        return c;
    }

private:
    byte_t decrypt_byte(uint32_t *keys)
    {
        uint32_t temp = keys[2] & 0xFFFFFF;
        temp |= 2;
        temp *= temp ^ 1;
        temp >>= 8;
        return temp & 0xFF;
    }

};

}//end namespace cgl

#endif //PKCRYPTO_HPP_20200512160306