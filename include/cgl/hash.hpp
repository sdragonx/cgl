/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 hash.h

 2010-08-27 13:43:57

*/
#ifndef HASH_H_20100827134357
#define HASH_H_20100827134357

#include <memory>
#include <cgl/public.h>
#include <cgl/string.hpp>
#include <cgl/hash/base64.hpp>
#include <cgl/hash/crc32.hpp>
#include <cgl/hash/md5.hpp>
#include <cgl/external/hash/MurmurHash3.cpp>
#include <cgl/external/hash/MurmurHash2_64.cpp>

namespace cgl{
namespace hash{

typedef uint32_t hash32_t;
typedef uint64_t hash64_t;
//typedef x128_t   hash128_t;

uint32_t mmhash32(const void* data, int size, uint32_t seed)
{
    uint32_t h;
    MurmurHash3_x86_32(data, size, seed, &h);
    return h;
}

uint64_t mmhash64(const void* data, size_t size, uint32_t seed)
{
    #if defined(CGL_PLATFORM_X86)
    return MurmurHash64B(data, size, seed);
    #else
    return MurmurHash64A(data, size, seed);
    #endif
}

void mmhash128(const void* data, size_t size, uint32_t seed, void* out)
{
    #if defined(CGL_PLATFORM_X86)
    MurmurHash3_x86_128(data, size, seed, out);
    #else
    MurmurHash3_x64_128(data, size, seed, out);
    #endif
}

//{00000000-0000-0000-0000000000000000}
string_t uuid_to_str(const __x128& id)
{
    string_t::value_type buf[38] = {0};
    string_t::value_type* p = buf;
    *(p++)= '{';
    tohex(p, (byte_t*)&id.alpha, sizeof(id.alpha));
    p += sizeof(id.alpha) * 2;
    *(p++)= '-';
    tohex(p, (byte_t*)&id.beta, sizeof(id.beta));
    p += sizeof(id.beta) * 2;
    *(p++)= '-';
    tohex(p, (byte_t*)&id.gamma, sizeof(id.gamma));
    p += sizeof(id.gamma) * 2;
    *(p++)= '-';
    tohex(p, (byte_t*)&id.delta, sizeof(id.delta));
    p += sizeof(id.delta) * 2;
    *(p++)= '}';
    *p = '\0';
    return string_t(buf, 38);
}

class hash128_t
{
public:
    typedef hash128_t this_type;
    typedef __x128 value_type;
    value_type value;
    hash128_t()
    {
        init();
    }

    hash128_t(const this_type& other)
    {
        *this = other;
    }

    hash128_t(uint32_t alpha, uint16_t beta, uint16_t gamma, uint64_t delta)
    {
        value.alpha = alpha;
        value.beta = beta;
        value.gamma = gamma;
        value.delta = delta;
    }

    hash128_t(const byte_t* data, size_t size, uint32_t seed = 0)
    {
        make(data, size, seed);
    }

    hash128_t(const wchar_t* data, size_t size, uint32_t seed = 0)
    {
        make(data, size, seed);
    }

    void init()
    {
        memset(&value, 0, sizeof(value_type));
    }

    int make(const byte_t* data, size_t size, uint32_t seed = 0)
    {
        mmhash128(data, size, seed, &value);
        return 0;
    }

    int make(const wchar_t* data, size_t size, uint32_t seed = 0)
    {
        mmhash128(data, size * 2, seed, &value);
        return 0;
    }

    bool operator<(const hash128_t& hash)const
    {
        return std::memcmp(value.data, hash.value.data, sizeof(value_type));
    }

    bool operator==(const hash128_t& hash)const
    {
        return !std::memcmp(value.data, hash.value.data, sizeof(value_type));
    }

    bool operator!()const
    {
        return !value.i64l && !value.i64h;
    }

    this_type& operator=(const this_type& other)
    {
        memcpy(value.data, other.value.data, sizeof(x128_t));
        return *this;
    }

    string_t str()const
    {
        return uuid_to_str(value);
    }
};


}//end namespace hash
}//end namespace cgl

#endif //HASH_H_20100827134357

/*

union uint1282{
    char buf[16];
    uint64_t data[2];
    struct {uint64_t first, second;};
};


inline uint64_t Uint128Low64(const uint128& x) { return x.first; }
inline uint64_t Uint128High64(const uint128& x) { return x.second; }


inline uint64_t Hash128to642(const uint128& x) {
  // Murmur-inspired hashing.
  const uint64_t kMul = 0x9ddfea08eb382d69ULL;
  uint64_t a = (Uint128Low64(x) ^ Uint128High64(x)) * kMul;
  a ^= (a >> 47);
  uint64_t b = (Uint128High64(x) ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}
*/
