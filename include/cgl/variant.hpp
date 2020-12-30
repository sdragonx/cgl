/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 variant.hxx

 2016-07-14 21:26:47

 一百年用不着

*/
#ifndef VARIANT_HXX_20160714212647
#define VARIANT_HXX_20160714212647

#include <cgl/public.h>

namespace cgl{

#pragma pack(push, 1)

class variant
{
public:
    enum typelist{
        vt_unknow,
        vt_int32,
        vt_int64,
        vt_float,
        vt_double,
        vt_void,
        vt_bin,
        vt_ansi,
        vt_unicode,
        vt_u8,
        vt_u16,
        vt_u32,
        vt_vec
    };
public:
    typelist type;
    union{
        int32_t  ival32;
        int64_t  ival64;
        float    fval;
        //double   dval;
        void* pval;
        struct{
            union{
                char* str;
                wchar_t* wstr;
                char8_t* utf8;
                char16_t* utf16;
                char32_t* utf32;
                byte_t* binary;
            };
            uint32_t size;
        };
        struct{
            int32_t x, y;
        };
        struct{
            int32_t width, height;
        };
    };
public:
    variant():ival64()
    {
        type = vt_unknow;
    }

    variant(int32_t value):type(vt_int32), ival32(value)
    {
    }

    variant(int64_t value):type(vt_int64), ival64(value)
    {
    }

    variant(void* p):type(vt_void), pval(p)
    {
    }

    variant(const char* s, size32_t len):type(vt_ansi)
    {
        str = new char[len+1];
        cscpy(str, s, len);
        size = len;
    }

    variant(const wchar_t* s, size32_t len):type(vt_unicode), wstr(csdup(s, len)), size(len)
    {
    }

    variant(byte_t* data, size32_t _size):type(vt_bin), binary(data), size(_size)
    {
    }

    variant(int32_t _x, int32_t _y):type(vt_vec), x(_x), y(_y)
    {
    }

    ~variant()
    {
        this->clear();
    }

    void clear()//dispose
    {
        switch(type)
        {
        case vt_ansi:
        case vt_unicode:
        case vt_u8:
        case vt_u16:
        case vt_u32:
            if(str){
                delete(str), str = null;
                size = 0;
            }
            break;
        case vt_bin:
            if(binary){
                delete(binary), binary = null;
                size = 0;
            }
            break;
        }
    }
};

#pragma pack(pop)

}//end namespace cgl

#endif //VARIANT_HXX_20160714212647
