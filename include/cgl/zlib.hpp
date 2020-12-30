/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 zlib.hpp

 2018-01-20 23:16:17

 zlib头和压缩级别
 78 01    0 1
 78 5E    2-5
 78 9C    6
 78 DA    7-9

 zlib尾部数据是32位adler32效验码

*/
#ifndef ZLIB_HPP_20180120231617
#define ZLIB_HPP_20180120231617

#include <cgl/public.h>
#include <vector>

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option push
    #pragma option -w-8004        //is assigned a value that is never used
#endif

#if defined(__BORLANDC__) && defined(ZlibHPP) //vcl.zlib
    #pragma message "vcl.zlib"
    //vcl的PngImage和Zlib冲突
#endif

#if defined(CGL_USING_ZLIB) //zlib

    //#define ZLIB_WINAPI
    //#define ZLIB_CONST
    //#define Z_PREFIX

    //#ifndef ZLIB_H

    //namespace zlib{
        #include <zlib.h>
    //}
    #pragma link "zlib.lib"
    //using namespace zlib;

    #ifdef Z_SOLO
        #error zlib solo mode.
    #endif

    #undef Z_NO_FLUSH
    #undef Z_PARTIAL_FLUSH
    #undef Z_SYNC_FLUSH
    #undef Z_FULL_FLUSH
    #undef Z_FINISH
    #undef Z_BLOCK
    #undef Z_OK
    #undef Z_STREAM_END
    #undef Z_NEED_DICT
    #undef Z_ERRNO
    #undef Z_STREAM_ERROR
    #undef Z_DATA_ERROR
    #undef Z_MEM_ERROR
    #undef Z_BUF_ERROR
    #undef Z_VERSION_ERROR
    //Z_PARAM_ERROR = -10000

    #undef Z_NO_COMPRESSION
    #undef Z_BEST_SPEED
    #undef Z_BEST_COMPRESSION
    #undef Z_UBER_COMPRESSION
    #undef Z_DEFAULT_LEVEL
    #undef Z_DEFAULT_COMPRESSION

    #undef Z_DEFAULT_STRATEGY
    #undef Z_FILTERED
    #undef Z_HUFFMAN_ONLY
    #undef Z_RLE
    #undef Z_FIXED

    #undef Z_BINARY
    #undef Z_TEXT
    #undef Z_ASCII
    #undef Z_UNKNOWN
    #undef Z_DEFLATED
    #undef Z_NULL

//    #error zlib

    //#endif//ZLIB_H

#else// defined(MZ_VERSION)//miniz

    #include <assert.h>
    #include <stdint.h>
    #include <stdlib.h>
    #include <string.h>

    #include <stdio.h>

    #include <stddef.h>

    //#include <time.h>   //MINIZ_NO_ARCHIVE_APIS

    #define MINIZ_NO_ZLIB_COMPATIBLE_NAMES //zlib name style
    #define MINIZ_NO_ARCHIVE_APIS
    #define MINIZ_NO_STDIO

    //namespace zlib{

    #include "external/miniz.h"

    typedef mz_stream z_stream;

    inline unsigned long compressBound(unsigned long size) { return mz_compressBound(size); }
    inline int deflateInit(mz_streamp pStream, int level) { return mz_deflateInit(pStream, level); }
    inline int deflateInit2(mz_streamp pStream, int level, int method, int window_bits, int mem_level, int strategy) { return mz_deflateInit2(pStream, level, method, window_bits, mem_level, strategy); }
    inline int deflate(mz_streamp pStream, int flush) { return mz_deflate(pStream, flush); }
    inline int deflateEnd(mz_streamp pStream) { return mz_deflateEnd(pStream); }

    inline int inflateInit(mz_streamp pStream) { return mz_inflateInit(pStream); }
    inline int inflate(mz_streamp pStream, int flush) { return mz_inflate(pStream, flush); }
    inline int inflateEnd(mz_streamp pStream) { return mz_inflateEnd(pStream); }


    //};

    /*
    #include "external/miniz/miniz.h"

    #include "external/miniz/miniz.c"
    #include "external/miniz/miniz_tdef.c"
    #include "external/miniz/miniz_tinfl.c"

    #ifndef MINIZ_NO_ARCHIVE_APIS
        #include "external/miniz/miniz_zip.c"
    #endif
    */

    //#pragma message "miniz"

#endif //zlib

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option pop
#endif

//---------------------------------------------------------------------------
//zlib

#ifndef Z_OK// !defined(ZlibHPP) && !defined(ZLIB_H)

enum{
    z_no_flush = 0,
    z_partial_flush = 1, /* will be removed, use Z_SYNC_FLUSH instead */
    z_sync_flush = 2,
    z_full_flush = 3,
    z_finish = 4,
    z_block = 5
};

enum{
    z_ok = 0,
    z_stream_end = 1,
    z_need_dict = 2,
    z_errno = -1,
    z_stream_error = -2,
    z_data_error = -3,
    z_mem_error = -4,
    z_buf_error = -5,
    z_version_error = -6,
    //z_param_error = -10000
};

enum{
    z_no_compression = 0,
    z_best_speed = 1,
    z_best_compression = 9,
    z_uber_compression = 10,
    z_default_level = 6,
    z_default_compression = -1
};

enum{
    z_default_strategy = 0,
    z_filtered = 1,
    z_huffman_only = 2,
    z_rle = 3,
    z_fixed = 4
};

enum{
    z_binary = 0,
    z_text = 1,
    z_ascii = z_text, /* for compatibility with 1.2.2 and earlier */
    z_unknown = 2,
    z_deflated = 8,
    z_null = 0
};

#endif//Z_OK

#ifndef Z_VERNUM
    #define Z_VERNUM 0x1230
#endif

namespace cgl{

inline size_t zlib_compressBound(size_t size)
{
    #if defined(ZLIB_H)
    return size;// zlib::compressBound(stream, size);
    #elif defined(MZ_VERSION)
    return mz_compressBound(size);
    #endif
};

inline int zlib_compress(void *dest, size_t *dest_len, const void *source, size_t source_len, int level)
{
    unsigned long size = *dest_len;

    #if defined(ZLIB_H)
        int n = ::compress2(static_cast<byte_t*>(dest), &size, static_cast<const byte_t*>(source), source_len, level);
        //#error "a"
    #else
        int n = mz_compress2(static_cast<byte_t*>(dest), &size, static_cast<const byte_t*>(source), source_len, level);
    #endif

    *dest_len = size;

    return n;
}

inline int zlib_uncompress(void *dest, size_t *dest_len, const void *source, size_t source_len)
{
    unsigned long size = *dest_len;
    #if defined(ZLIB_H)
    int n = ::uncompress(static_cast<Bytef*>(dest), &size, static_cast<const Bytef*>(source), source_len);
    #elif defined(MZ_VERSION)
    int n = mz_uncompress(static_cast<byte_t*>(dest), &size, static_cast<const byte_t*>(source), source_len);
    #endif
    *dest_len = size;
    return n;
}

//inline uint32_t zlib_crc32(mz_ulong crc, const byte_t *ptr, size_t buf_len)
//{
//    return mz_crc32(crc, ptr, buf_len);
//}


class zstream : public z_stream
{
public:
    //size_t out_size;
public:
    int in_data(const void* data, size_t size)
    {
        avail_in = size;
        next_in  = (byte_t*)data;
        return 0;
    }

    int out_data(void* data, size_t size)
    {
        avail_out = size;
        next_out = reinterpret_cast<byte_t*>(data);
        //out_size = size;
        return 0;
    }

    //compressed or uncompressed size
    //size_t size()const { return out_size - avail_out; }

    int deflate_init(int level)
    {
        memset(this, 0, sizeof(zstream));
        return deflateInit(this, level);
    }

    int deflate_init(int level, int method, int window_bits, int mem_level, int strategy)
    {
        return deflateInit2(this, level, method, window_bits, mem_level, strategy);
    }

    int deflate_end()
    {
        return deflateEnd(this);;
    }

    int deflate(int mode)
    {
        int n = ::deflate(this, mode);
        return n;
    }

    bool deflate_complited()
    {
        return avail_out != 0;
    }

    int inflate_init()
    {
        memset(this, 0, sizeof(zstream));
        return inflateInit(this);
    }

    int inflate_end()
    {
        return ::inflateEnd(this);
    }

    int inflate(int mode)
    {
        return ::inflate(this, mode);
    }
};

//
// std::vector<byte_t>
//

int compress(std::vector<byte_t>& buf, const void* data, size_t size, int level = 6)
{
    buf.resize(zlib_compressBound(size));
    size_t buf_size = buf.size();
    zlib_compress(&buf[0], &buf_size, data, size, level);
    buf.resize(buf_size);
    return buf_size;
}

int uncompress(std::vector<byte_t>& buf, size_t source_size, const void* data, size_t size)
{
    buf.resize(source_size);
    zlib_uncompress(&buf[0], &source_size, data, size);
    return source_size;
}

}//end namespace cgl

#endif //ZLIB_HPP_20180120231617
