/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 palette.hpp

 2017-01-31 12:48:16

*/
#ifndef PALETTE_HPP_20170131124816
#define PALETTE_HPP_20170131124816

#include <cgl/public.h>
#include <cgl/graphics/color.hpp>

namespace cgl{
namespace graphics{

typedef vec4ub PALCOLOR;

inline vec4ub PAL_RGBA(int red, int green, int blue, int alpha)
{
    return vec4ub(red, green, blue, alpha);
}

const int PALETTE_MAX = 256;

typedef PALCOLOR PALETTE16[16];
typedef PALCOLOR PALETTE256[256];

//�ڰ׵�ɫ��
const PALCOLOR PALETTE_MONO[] = {PAL_RGBA(0x00, 0x00, 0x00, 0xFF), PAL_RGBA(0xFF, 0xFF, 0xFF, 0xFF)};

//BGRA

//windows 16ɫ��ɫ��
const PALETTE16 PALETTE_WIN16 = {
    PAL_RGBA(0x00,0x00,0x00,0xFF), PAL_RGBA(0x80,0x00,0x00,0xFF), PAL_RGBA(0x00,0x80,0x00,0xFF), PAL_RGBA(0x80,0x80,0x00,0xFF),
    PAL_RGBA(0x00,0x00,0x80,0xFF), PAL_RGBA(0x80,0x00,0x80,0xFF), PAL_RGBA(0x00,0x80,0x80,0xFF), PAL_RGBA(0x80,0x80,0x80,0xFF),
    PAL_RGBA(0xC0,0xC0,0xC0,0xFF), PAL_RGBA(0xFF,0x00,0x00,0xFF), PAL_RGBA(0x00,0xFF,0x00,0xFF), PAL_RGBA(0xFF,0xFF,0x00,0xFF),
    PAL_RGBA(0x00,0x00,0xFF,0xFF), PAL_RGBA(0xFF,0x00,0xFF,0xFF), PAL_RGBA(0x00,0xFF,0xFF,0xFF), PAL_RGBA(0xFF,0xFF,0xFF,0xFF)
};

//windows 256ɫ��ɫ��
const PALETTE256 PALETTE_WIN256 = {
    PAL_RGBA(0x00,0x00,0x00,0xFF), PAL_RGBA(0x80,0x00,0x00,0xFF), PAL_RGBA(0x00,0x80,0x00,0xFF), PAL_RGBA(0x80,0x80,0x00,0xFF),
    PAL_RGBA(0x00,0x00,0x80,0xFF), PAL_RGBA(0x80,0x00,0x80,0xFF), PAL_RGBA(0x00,0x80,0x80,0xFF), PAL_RGBA(0xC0,0xC0,0xC0,0xFF),
    PAL_RGBA(0xC0,0xDC,0xC0,0xFF), PAL_RGBA(0xA6,0xCA,0xF0,0xFF), PAL_RGBA(0x40,0x20,0x00,0xFF), PAL_RGBA(0x60,0x20,0x00,0xFF),
    PAL_RGBA(0x80,0x20,0x00,0xFF), PAL_RGBA(0xA0,0x20,0x00,0xFF), PAL_RGBA(0xC0,0x20,0x00,0xFF), PAL_RGBA(0xE0,0x20,0x00,0xFF),
    PAL_RGBA(0x00,0x40,0x00,0xFF), PAL_RGBA(0x20,0x40,0x00,0xFF), PAL_RGBA(0x40,0x40,0x00,0xFF), PAL_RGBA(0x60,0x40,0x00,0xFF),
    PAL_RGBA(0x80,0x40,0x00,0xFF), PAL_RGBA(0xA0,0x40,0x00,0xFF), PAL_RGBA(0xC0,0x40,0x00,0xFF), PAL_RGBA(0xE0,0x40,0x00,0xFF),
    PAL_RGBA(0x00,0x60,0x00,0xFF), PAL_RGBA(0x20,0x60,0x00,0xFF), PAL_RGBA(0x40,0x60,0x00,0xFF), PAL_RGBA(0x60,0x60,0x00,0xFF),
    PAL_RGBA(0x80,0x60,0x00,0xFF), PAL_RGBA(0xA0,0x60,0x00,0xFF), PAL_RGBA(0xC0,0x60,0x00,0xFF), PAL_RGBA(0xE0,0x60,0x00,0xFF),
    PAL_RGBA(0x00,0x80,0x00,0xFF), PAL_RGBA(0x20,0x80,0x00,0xFF), PAL_RGBA(0x40,0x80,0x00,0xFF), PAL_RGBA(0x60,0x80,0x00,0xFF),
    PAL_RGBA(0x80,0x80,0x00,0xFF), PAL_RGBA(0xA0,0x80,0x00,0xFF), PAL_RGBA(0xC0,0x80,0x00,0xFF), PAL_RGBA(0xE0,0x80,0x00,0xFF),
    PAL_RGBA(0x00,0xA0,0x00,0xFF), PAL_RGBA(0x20,0xA0,0x00,0xFF), PAL_RGBA(0x40,0xA0,0x00,0xFF), PAL_RGBA(0x60,0xA0,0x00,0xFF),
    PAL_RGBA(0x80,0xA0,0x00,0xFF), PAL_RGBA(0xA0,0xA0,0x00,0xFF), PAL_RGBA(0xC0,0xA0,0x00,0xFF), PAL_RGBA(0xE0,0xA0,0x00,0xFF),
    PAL_RGBA(0x00,0xC0,0x00,0xFF), PAL_RGBA(0x20,0xC0,0x00,0xFF), PAL_RGBA(0x40,0xC0,0x00,0xFF), PAL_RGBA(0x60,0xC0,0x00,0xFF),
    PAL_RGBA(0x80,0xC0,0x00,0xFF), PAL_RGBA(0xA0,0xC0,0x00,0xFF), PAL_RGBA(0xC0,0xC0,0x00,0xFF), PAL_RGBA(0xE0,0xC0,0x00,0xFF),
    PAL_RGBA(0x00,0xE0,0x00,0xFF), PAL_RGBA(0x20,0xE0,0x00,0xFF), PAL_RGBA(0x40,0xE0,0x00,0xFF), PAL_RGBA(0x60,0xE0,0x00,0xFF),
    PAL_RGBA(0x80,0xE0,0x00,0xFF), PAL_RGBA(0xA0,0xE0,0x00,0xFF), PAL_RGBA(0xC0,0xE0,0x00,0xFF), PAL_RGBA(0xE0,0xE0,0x00,0xFF),
    PAL_RGBA(0x00,0x00,0x40,0xFF), PAL_RGBA(0x20,0x00,0x40,0xFF), PAL_RGBA(0x40,0x00,0x40,0xFF), PAL_RGBA(0x60,0x00,0x40,0xFF),
    PAL_RGBA(0x80,0x00,0x40,0xFF), PAL_RGBA(0xA0,0x00,0x40,0xFF), PAL_RGBA(0xC0,0x00,0x40,0xFF), PAL_RGBA(0xE0,0x00,0x40,0xFF),
    PAL_RGBA(0x00,0x20,0x40,0xFF), PAL_RGBA(0x20,0x20,0x40,0xFF), PAL_RGBA(0x40,0x20,0x40,0xFF), PAL_RGBA(0x60,0x20,0x40,0xFF),
    PAL_RGBA(0x80,0x20,0x40,0xFF), PAL_RGBA(0xA0,0x20,0x40,0xFF), PAL_RGBA(0xC0,0x20,0x40,0xFF), PAL_RGBA(0xE0,0x20,0x40,0xFF),
    PAL_RGBA(0x00,0x40,0x40,0xFF), PAL_RGBA(0x20,0x40,0x40,0xFF), PAL_RGBA(0x40,0x40,0x40,0xFF), PAL_RGBA(0x60,0x40,0x40,0xFF),
    PAL_RGBA(0x80,0x40,0x40,0xFF), PAL_RGBA(0xA0,0x40,0x40,0xFF), PAL_RGBA(0xC0,0x40,0x40,0xFF), PAL_RGBA(0xE0,0x40,0x40,0xFF),
    PAL_RGBA(0x00,0x60,0x40,0xFF), PAL_RGBA(0x20,0x60,0x40,0xFF), PAL_RGBA(0x40,0x60,0x40,0xFF), PAL_RGBA(0x60,0x60,0x40,0xFF),
    PAL_RGBA(0x80,0x60,0x40,0xFF), PAL_RGBA(0xA0,0x60,0x40,0xFF), PAL_RGBA(0xC0,0x60,0x40,0xFF), PAL_RGBA(0xE0,0x60,0x40,0xFF),
    PAL_RGBA(0x00,0x80,0x40,0xFF), PAL_RGBA(0x20,0x80,0x40,0xFF), PAL_RGBA(0x40,0x80,0x40,0xFF), PAL_RGBA(0x60,0x80,0x40,0xFF),
    PAL_RGBA(0x80,0x80,0x40,0xFF), PAL_RGBA(0xA0,0x80,0x40,0xFF), PAL_RGBA(0xC0,0x80,0x40,0xFF), PAL_RGBA(0xE0,0x80,0x40,0xFF),
    PAL_RGBA(0x00,0xA0,0x40,0xFF), PAL_RGBA(0x20,0xA0,0x40,0xFF), PAL_RGBA(0x40,0xA0,0x40,0xFF), PAL_RGBA(0x60,0xA0,0x40,0xFF),
    PAL_RGBA(0x80,0xA0,0x40,0xFF), PAL_RGBA(0xA0,0xA0,0x40,0xFF), PAL_RGBA(0xC0,0xA0,0x40,0xFF), PAL_RGBA(0xE0,0xA0,0x40,0xFF),
    PAL_RGBA(0x00,0xC0,0x40,0xFF), PAL_RGBA(0x20,0xC0,0x40,0xFF), PAL_RGBA(0x40,0xC0,0x40,0xFF), PAL_RGBA(0x60,0xC0,0x40,0xFF),
    PAL_RGBA(0x80,0xC0,0x40,0xFF), PAL_RGBA(0xA0,0xC0,0x40,0xFF), PAL_RGBA(0xC0,0xC0,0x40,0xFF), PAL_RGBA(0xE0,0xC0,0x40,0xFF),
    PAL_RGBA(0x00,0xE0,0x40,0xFF), PAL_RGBA(0x20,0xE0,0x40,0xFF), PAL_RGBA(0x40,0xE0,0x40,0xFF), PAL_RGBA(0x60,0xE0,0x40,0xFF),
    PAL_RGBA(0x80,0xE0,0x40,0xFF), PAL_RGBA(0xA0,0xE0,0x40,0xFF), PAL_RGBA(0xC0,0xE0,0x40,0xFF), PAL_RGBA(0xE0,0xE0,0x40,0xFF),
    PAL_RGBA(0x00,0x00,0x80,0xFF), PAL_RGBA(0x20,0x00,0x80,0xFF), PAL_RGBA(0x40,0x00,0x80,0xFF), PAL_RGBA(0x60,0x00,0x80,0xFF),
    PAL_RGBA(0x80,0x00,0x80,0xFF), PAL_RGBA(0xA0,0x00,0x80,0xFF), PAL_RGBA(0xC0,0x00,0x80,0xFF), PAL_RGBA(0xE0,0x00,0x80,0xFF),
    PAL_RGBA(0x00,0x20,0x80,0xFF), PAL_RGBA(0x20,0x20,0x80,0xFF), PAL_RGBA(0x40,0x20,0x80,0xFF), PAL_RGBA(0x60,0x20,0x80,0xFF),
    PAL_RGBA(0x80,0x20,0x80,0xFF), PAL_RGBA(0xA0,0x20,0x80,0xFF), PAL_RGBA(0xC0,0x20,0x80,0xFF), PAL_RGBA(0xE0,0x20,0x80,0xFF),
    PAL_RGBA(0x00,0x40,0x80,0xFF), PAL_RGBA(0x20,0x40,0x80,0xFF), PAL_RGBA(0x40,0x40,0x80,0xFF), PAL_RGBA(0x60,0x40,0x80,0xFF),
    PAL_RGBA(0x80,0x40,0x80,0xFF), PAL_RGBA(0xA0,0x40,0x80,0xFF), PAL_RGBA(0xC0,0x40,0x80,0xFF), PAL_RGBA(0xE0,0x40,0x80,0xFF),
    PAL_RGBA(0x00,0x60,0x80,0xFF), PAL_RGBA(0x20,0x60,0x80,0xFF), PAL_RGBA(0x40,0x60,0x80,0xFF), PAL_RGBA(0x60,0x60,0x80,0xFF),
    PAL_RGBA(0x80,0x60,0x80,0xFF), PAL_RGBA(0xA0,0x60,0x80,0xFF), PAL_RGBA(0xC0,0x60,0x80,0xFF), PAL_RGBA(0xE0,0x60,0x80,0xFF),
    PAL_RGBA(0x00,0x80,0x80,0xFF), PAL_RGBA(0x20,0x80,0x80,0xFF), PAL_RGBA(0x40,0x80,0x80,0xFF), PAL_RGBA(0x60,0x80,0x80,0xFF),
    PAL_RGBA(0x80,0x80,0x80,0xFF), PAL_RGBA(0xA0,0x80,0x80,0xFF), PAL_RGBA(0xC0,0x80,0x80,0xFF), PAL_RGBA(0xE0,0x80,0x80,0xFF),
    PAL_RGBA(0x00,0xA0,0x80,0xFF), PAL_RGBA(0x20,0xA0,0x80,0xFF), PAL_RGBA(0x40,0xA0,0x80,0xFF), PAL_RGBA(0x60,0xA0,0x80,0xFF),
    PAL_RGBA(0x80,0xA0,0x80,0xFF), PAL_RGBA(0xA0,0xA0,0x80,0xFF), PAL_RGBA(0xC0,0xA0,0x80,0xFF), PAL_RGBA(0xE0,0xA0,0x80,0xFF),
    PAL_RGBA(0x00,0xC0,0x80,0xFF), PAL_RGBA(0x20,0xC0,0x80,0xFF), PAL_RGBA(0x40,0xC0,0x80,0xFF), PAL_RGBA(0x60,0xC0,0x80,0xFF),
    PAL_RGBA(0x80,0xC0,0x80,0xFF), PAL_RGBA(0xA0,0xC0,0x80,0xFF), PAL_RGBA(0xC0,0xC0,0x80,0xFF), PAL_RGBA(0xE0,0xC0,0x80,0xFF),
    PAL_RGBA(0x00,0xE0,0x80,0xFF), PAL_RGBA(0x20,0xE0,0x80,0xFF), PAL_RGBA(0x40,0xE0,0x80,0xFF), PAL_RGBA(0x60,0xE0,0x80,0xFF),
    PAL_RGBA(0x80,0xE0,0x80,0xFF), PAL_RGBA(0xA0,0xE0,0x80,0xFF), PAL_RGBA(0xC0,0xE0,0x80,0xFF), PAL_RGBA(0xE0,0xE0,0x80,0xFF),
    PAL_RGBA(0x00,0x00,0xC0,0xFF), PAL_RGBA(0x20,0x00,0xC0,0xFF), PAL_RGBA(0x40,0x00,0xC0,0xFF), PAL_RGBA(0x60,0x00,0xC0,0xFF),
    PAL_RGBA(0x80,0x00,0xC0,0xFF), PAL_RGBA(0xA0,0x00,0xC0,0xFF), PAL_RGBA(0xC0,0x00,0xC0,0xFF), PAL_RGBA(0xE0,0x00,0xC0,0xFF),
    PAL_RGBA(0x00,0x20,0xC0,0xFF), PAL_RGBA(0x20,0x20,0xC0,0xFF), PAL_RGBA(0x40,0x20,0xC0,0xFF), PAL_RGBA(0x60,0x20,0xC0,0xFF),
    PAL_RGBA(0x80,0x20,0xC0,0xFF), PAL_RGBA(0xA0,0x20,0xC0,0xFF), PAL_RGBA(0xC0,0x20,0xC0,0xFF), PAL_RGBA(0xE0,0x20,0xC0,0xFF),
    PAL_RGBA(0x00,0x40,0xC0,0xFF), PAL_RGBA(0x20,0x40,0xC0,0xFF), PAL_RGBA(0x40,0x40,0xC0,0xFF), PAL_RGBA(0x60,0x40,0xC0,0xFF),
    PAL_RGBA(0x80,0x40,0xC0,0xFF), PAL_RGBA(0xA0,0x40,0xC0,0xFF), PAL_RGBA(0xC0,0x40,0xC0,0xFF), PAL_RGBA(0xE0,0x40,0xC0,0xFF),
    PAL_RGBA(0x00,0x60,0xC0,0xFF), PAL_RGBA(0x20,0x60,0xC0,0xFF), PAL_RGBA(0x40,0x60,0xC0,0xFF), PAL_RGBA(0x60,0x60,0xC0,0xFF),
    PAL_RGBA(0x80,0x60,0xC0,0xFF), PAL_RGBA(0xA0,0x60,0xC0,0xFF), PAL_RGBA(0xC0,0x60,0xC0,0xFF), PAL_RGBA(0xE0,0x60,0xC0,0xFF),
    PAL_RGBA(0x00,0x80,0xC0,0xFF), PAL_RGBA(0x20,0x80,0xC0,0xFF), PAL_RGBA(0x40,0x80,0xC0,0xFF), PAL_RGBA(0x60,0x80,0xC0,0xFF),
    PAL_RGBA(0x80,0x80,0xC0,0xFF), PAL_RGBA(0xA0,0x80,0xC0,0xFF), PAL_RGBA(0xC0,0x80,0xC0,0xFF), PAL_RGBA(0xE0,0x80,0xC0,0xFF),
    PAL_RGBA(0x00,0xA0,0xC0,0xFF), PAL_RGBA(0x20,0xA0,0xC0,0xFF), PAL_RGBA(0x40,0xA0,0xC0,0xFF), PAL_RGBA(0x60,0xA0,0xC0,0xFF),
    PAL_RGBA(0x80,0xA0,0xC0,0xFF), PAL_RGBA(0xA0,0xA0,0xC0,0xFF), PAL_RGBA(0xC0,0xA0,0xC0,0xFF), PAL_RGBA(0xE0,0xA0,0xC0,0xFF),
    PAL_RGBA(0x00,0xC0,0xC0,0xFF), PAL_RGBA(0x20,0xC0,0xC0,0xFF), PAL_RGBA(0x40,0xC0,0xC0,0xFF), PAL_RGBA(0x60,0xC0,0xC0,0xFF),
    PAL_RGBA(0x80,0xC0,0xC0,0xFF), PAL_RGBA(0xA0,0xC0,0xC0,0xFF), PAL_RGBA(0xFF,0xFB,0xF0,0xFF), PAL_RGBA(0xA0,0xA0,0xA4,0xFF),
    PAL_RGBA(0x80,0x80,0x80,0xFF), PAL_RGBA(0xFF,0x00,0x00,0xFF), PAL_RGBA(0x00,0xFF,0x00,0xFF), PAL_RGBA(0xFF,0xFF,0x00,0xFF),
    PAL_RGBA(0x00,0x00,0xFF,0xFF), PAL_RGBA(0xFF,0x00,0xFF,0xFF), PAL_RGBA(0x00,0xFF,0xFF,0xFF), PAL_RGBA(0xFF,0xFF,0xFF,0xFF)
};

//����16ɫ�Ҷȵ�ɫ��
int palette_gray16(PALCOLOR* pal)
{
    int c = 0;
    for(int i=0; i<16; ++i){
        pal[i] = PAL_RGBA(c, c, c, 255);
        c = 15 + 16 * i;
    }
    return 0;
}

//����64�׻Ҷȵ�ɫ��
int palette_gray64(PALETTE256 &pal)
{
    int c;
    for(int i=0; i<64; ++i){
        c = 255 * i / 64;
        pal[i] = PAL_RGBA(c, c, c, 255);
    }
    return 0;
}

//����256�׻Ҷȵ�ɫ��
int palette_gray256(PALETTE256 &pal)
{
    for(int i=0; i<256; ++i){
        pal[i] = PAL_RGBA(i, i, i, i);
    }
    return 0;
}

/*
class pelette
{
public:
    memblock<PALCOLOR> m_paldata;

public:

};
*/

}//end namespace graphcis
}//end namespace cgl

#endif //PALETTE_HPP_20170131124816