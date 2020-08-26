/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 xtype.hpp

 2018-01-14 12:57:43

*/
#ifndef XTYPE_HPP_20180114125743
#define XTYPE_HPP_20180114125743

#include "int.h"

namespace cgl{

#pragma pack(push, 1)

typedef union __x16
{
    uint8_t  data[2];
    uint16_t value;
    struct{
        uint8_t a, b;
    };
}x16_t;

typedef union __x32
{
    uint8_t  data[4];
    uint32_t value;
    float     real;

    struct{
        uint16_t sa, sb;
    };
    struct{
        uint8_t a, b, c, d;
    };

}x32_t;

typedef union __x64
{
    uint8_t data[8];
    uint64_t value;
    double fullreal;
    double f_64[1];

    struct{
        float fa, fb;
    };

    struct{
        uint32_t ia, ib;
    };

    struct{
        uint16_t sa, sb, sc, sd;
    };

    struct{
        uint8_t a, b, c, d, e, f, g;
    };

    //4个16位寄存器
    struct
    {
        uint16_t ax, bx, cx, dx;
    };

    //8个8位寄存器
    struct{
        uint8_t al, ah, bl, bh, cl, ch, dl, dh;
    };
}x64_t;

typedef union __x128
{
    uint8_t data[16];
    uint32_t idata[4];
    float fdata[4];

    struct{
        double da, db;
    };

    struct{
        __x32 x32a, x32b, x32c, x32d;
    };
    struct{
        __x64 x64a, x64b;
    };

    struct {
        uint64_t i64l, i64h;
    };

    //4个32位寄存器
    struct{
        float fax, fbx, fcx, fdx;
    };
    struct{
        uint32_t eax, ebx, ecx, edx;
    };
    struct{
        uint16_t ax, ay, bx, by, cx, cy, dx, dy;
    };
    struct{
        uint8_t al, ah, ayl, ayh, bl, bh, byl, byh, cl, ch, cyl, cyh, dl, dh, dyl, dyh;
    };

    //UUID
    struct{
        uint32_t alpha;
        uint16_t beta;
        uint16_t gamma;
        uint64_t delta;
    };
}x128_t;

typedef union __x256{
    uint8_t  data[32];
    uint32_t idata[8];
    float    fdata[8];

    struct{
        double mmx0, mmx1, mmx2, mmx3;
    };

    struct{
        float fp0, fp1, fp2, fp3, fp4, fp5, fp6, fp7;
    };

    //8个32位寄存器
    struct{
        uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp;
    };

    struct{
        uint16_t ax, ay, bx, by, cx, cy, dx, dy, si, sj, di, dj, sp, sq, bp, bq;
    };

    struct{
        uint8_t al, ah, ayl, ayh, bl, bh, byl, byh, cl, ch, cyl, cyh, dl, dh, dyl, dyh;
    };
}x256_t;


#pragma pack(pop)

}//end namespace cgl

#endif //XTYPE_HPP_20180114125743
