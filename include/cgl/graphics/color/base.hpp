/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 COLOR_BASE.hpp

 2020-02-17 17:44:20

*/
#ifndef COLOR_BASE_HPP_20200217174420
#define COLOR_BASE_HPP_20200217174420

#include <cgl/public.h>
#include <cgl/math.hpp>
#include <cgl/vec.hpp>

namespace cgl{
namespace graphics{

//
// RGBA
//
// format BGR
//
//windows RGB
//#define RGB(r,g,b) ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
//#ifndef CGL_PLATFORM_WINDOWS
//#define RGB(r,g,b) (uint32_t((uint8_t(r)|(uint16_t(uint8_t(g))<<8))|(uint32_t(uint8_t(b))<<16)))
//#endif

#define RGBA(r,g,b,a) (uint32_t((uint8_t(b)|(uint16_t(uint8_t(g))<<8))|(uint32_t(uint8_t(r))<<16)|(uint32_t(uint8_t(a))<<24)))
#define BGRA(r,g,b,a) (uint32_t((uint8_t(r)|(uint16_t(uint8_t(g))<<8))|(uint32_t(uint8_t(b))<<16)|(uint32_t(uint8_t(a))<<24)))

#define RGBASwap(color) bswap32(color)
#define RedBlueSwap(color) ((color & 0xFF00FF00) | ((color >> 16) & 0xFF) | ((color << 16) & 0x00FF0000))

inline byte_t RGBA_RED   (uint32_t color) { return color >> 16; }
inline byte_t RGBA_GREEN (uint32_t color) { return color >> 8;  }
inline byte_t RGBA_BLUE  (uint32_t color) { return color;       }
inline byte_t RGBA_ALPHA (uint32_t color) { return color >> 24; }

inline byte_t BGRA_RED   (uint32_t color) { return color;       }
inline byte_t BGRA_GREEN (uint32_t color) { return color >> 8;  }
inline byte_t BGRA_BLUE  (uint32_t color) { return color >> 16; }
inline byte_t BGRA_ALPHA (uint32_t color) { return color >> 24; }

inline uint16_t RGBA64_RED   (uint64_t color) { return color >> 32; }
inline uint16_t RGBA64_GREEN (uint64_t color) { return color >> 16; }
inline uint16_t RGBA64_BLUE  (uint64_t color) { return color;       }
inline uint16_t RGBA64_ALPHA (uint64_t color) { return color >> 48; }

inline uint16_t BGRA64_RED   (uint64_t color) { return color;       }
inline uint16_t BGRA64_GREEN (uint64_t color) { return color >> 16; }
inline uint16_t BGRA64_BLUE  (uint64_t color) { return color >> 32; }
inline uint16_t BGRA64_ALPHA (uint64_t color) { return color >> 48; }

//
// RGB555 RGB565
//
// format RGB
//

#define COLOR_MASK5 0x001F
#define COLOR_MASK6 0x003F

#define RGB555_MASK_RED   0x7C00
#define RGB555_MASK_GREEN 0x03E0
#define RGB555_MASK_BLUE  0x001F

#define RGB555(r,g,b) (uint16_t(( (r & COLOR_MASK5) << 10) | ( (g & COLOR_MASK5) << 5) | (b & COLOR_MASK5) ))

#define RGB555_RED(c)   (((c) & RGB555_MASK_RED  ) >> 10)    // 取值范围0-31
#define RGB555_GREEN(c) (((c) & RGB555_MASK_GREEN) >>  5)    // 取值范围0-31
#define RGB555_BLUE(c)  ( (c) & RGB555_MASK_BLUE        )    // 取值范围0-31

inline uint16_t RGBA_to_RGB555(byte_t r, byte_t g, byte_t b)
{
    return RGB555(r >> 3, g >> 3, b >> 3);
}

inline uint16_t RGBA_to_RGB555(uint32_t color)
{
    return RGBA_to_RGB555(RGBA_RED(color), RGBA_GREEN(color), RGBA_BLUE(color));
}

inline uint32_t RGB555_to_RGBA(byte_t r, byte_t g, byte_t b)
{
    return RGBA(r << 3, g << 3, b << 3, CGL_ALPHA_OPAQUE);
}

inline uint32_t RGB555_to_RGBA(uint16_t color)
{
    return RGB555_to_RGBA(RGB555_RED(color), RGB555_GREEN(color), RGB555_BLUE(color));
}

#define RGB565_MASK_RED   0xF800
#define RGB565_MASK_GREEN 0x07E0
#define RGB565_MASK_BLUE  0x001F

#define RGB565(r,g,b) (unsigned int)( (r|0x08 << 11) | (g|0x08 << 6) | b|0x08 )

#define RGB565_RED(c)   (((c) & RGB565_MASK_RED  ) >> 11)    // 取值范围0-31
#define RGB565_GREEN(c) (((c) & RGB565_MASK_GREEN) >>  5)    // 取值范围0-63
#define RGB565_BLUE(c)  ( (c) & RGB565_MASK_BLUE        )    // 取值范围0-31

inline uint16_t RGBA_to_RGB565(byte_t r, byte_t g, byte_t b)
{
    return RGB565(r >> 3, g >> 2, b >> 3);
}

inline uint16_t RGBA_to_RGB565(uint32_t color)
{
    return RGBA_to_RGB565(RGBA_RED(color), RGBA_GREEN(color), RGBA_BLUE(color));
}

inline uint32_t RGB565_to_RGBA(byte_t r, byte_t g, byte_t b)
{
    return RGBA(r << 3, g << 2, b << 3, CGL_ALPHA_OPAQUE);
}

inline uint32_t RGB565_to_RGBA(uint16_t color)
{
    return RGB565_to_RGBA(RGB565_RED(color), RGB565_GREEN(color), RGB565_BLUE(color));
}

}//end namespace graphics
}//end namespace cgl

#endif //COLOR_BASE_HPP_20200217174420
