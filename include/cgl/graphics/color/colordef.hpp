/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 colordef.hpp

 2020-02-17 17:23:44

*/
#ifndef COLORDEF_HPP_20200217172344
#define COLORDEF_HPP_20200217172344

#include "base.hpp"

namespace cgl{
namespace graphics{

//---------------------------------------------------------------------------
//
// 关于位域
// struct{
//    int n:24;
// };
// sizeof() : vc 4; bcc 3; bcc32x 3; gcc 3
//
// union{
//    int n:24;
// };
// sizeof() : vc 4; bcc 4; bcc32x 3; gcc 3
//
//---------------------------------------------------------------------------
//
//颜色结构体 ABGR格式
//

#pragma pack(push, 1)

struct rgb565_t;
struct rgba4444_t;

struct rgb24_t;
struct rgba32_t;
struct rgb48_t;
struct rgba64_t;

struct bgr24_t;
struct bgra32_t;
struct bgr48_t;
struct bgra64_t;

//
// RGB555
//

struct rgb555_t
{
    const static uint16_t white = 0x00007FFF;
    const static uint16_t black = 0x00000000;
    union{
        byte_t   data[2];
        uint16_t value;
        struct {
            uint16_t r:5;
            uint16_t g:5;
            uint16_t b:5;
            uint16_t a:1;
        };
    };

    void reset() { value = 0; }
    void set(uint16_t rgb) { value = rgb; }
    void set(byte_t bR, byte_t bG, byte_t bB) { r = bR; g = bG; b = bB; a = 0; }

    uint16_t color() { return value; }
};

//
// RGB565
//

struct rgb565_t
{
    const static uint16_t white = 0xFFFF;
    const static uint16_t black = 0x0000;

    union{
        byte_t   data[2];
        uint16_t value;
        struct {
            uint16_t r:5;
            uint16_t g:6;
            uint16_t b:5;
        };
    };

    void reset() { value = 0; }
    void set(uint16_t rgb) { value = rgb; }
    void set(byte_t bR, byte_t bG, byte_t bB) { r = bR; g = bG; b = bB; }

    uint16_t color() { return value; }
};

//
// RGBA4444
//

struct rgba4444_t
{
    const static uint16_t white = 0x7FFF;
    const static uint16_t black = 0x0000;
    union{
        byte_t   data[2];
        uint16_t value;
        struct {
            byte_t r:4;
            byte_t g:4;
            byte_t b:4;
            byte_t a:4;
        };
    };

    void reset() { value = 0; }
    void set(uint16_t rgb) { value = rgb; }
    void set(byte_t bR, byte_t bG, byte_t bB) { r = bR; g = bG; b = bB; a = 0; }

    uint16_t color() { return value; }
};

//
// RGB24
//

struct rgb24_t
{
    const static uint32_t white = 0xFFFFFFFF;
    const static uint32_t black = 0xFF000000;

    union{
        uint8_t data[3];
        struct { byte_t blue, green, red; };
        struct { byte_t r, g, b; };
    };

    void reset() { red = green = blue = 0; }
    void set(uint32_t rgb) { r = RGBA_RED(rgb); g = RGBA_GREEN(rgb); b = RGBA_BLUE(rgb); }
    void set(byte_t bR, byte_t bG, byte_t bB) { r = bR; g = bG; b = bB; }

    uint32_t color()const { return RGBA(r, g, b, 0xFF); }

    bool is_white()const { return !((~red)|(~green)|(~blue)); }
    bool is_black()const { return !(red|green|blue); }
//    const colorBGR& blend(const colorBGR& c, byte_t a)
//    {
//        return operator=(blend32(color(), c.color(), a));
//    }
};

//
//RGBA32
//

struct rgba32_t
{
    const static uint32_t white = 0xFFFFFFFF;
    const static uint32_t black = 0xFF000000;
    union{
        byte_t   data[4];
        uint32_t value;
        struct{ byte_t red, green, blue, alpha; };
        struct{ byte_t r, g, b, a; };
    };

    void reset() { red = green = blue = alpha = 0; }
    void set(uint32_t rgba) { r = RGBA_RED(rgba); g = RGBA_GREEN(rgba); b = RGBA_BLUE(rgba); a = RGBA_ALPHA(rgba); }
    void set(byte_t bR, byte_t bG, byte_t bB, byte_t bA) { r = bR; g = bG; b = bB; a = bA; }

    uint32_t color() { return value; }

    bool is_white()const { return !(~(value | 0xFF000000)); }
    bool is_black()const { return !(value & 0x00FFFFFF); }
};

//
//RGBA64
//

struct rgba64_t
{
    union{
        byte_t   data[8];
        uint64_t value;
        struct{ uint16_t red, green, blue, alpha; };
        struct{ uint16_t r, g, b, a; };
    };

    void reset() { red = green = blue = alpha = 0; }
    void set(uint64_t rgba) { r = RGBA64_RED(rgba); g = RGBA64_GREEN(rgba); b = RGBA64_BLUE(rgba); a = RGBA64_ALPHA(rgba); }
    void set(uint16_t bR, uint16_t bG, uint16_t bB, uint16_t bA) { r = bR; g = bG; b = bB; a = bA; }

    uint32_t color() { return value; }
};

//
// BGR24
//

struct bgr24_t
{
    const static uint32_t white = 0xFFFFFFFF;
    const static uint32_t black = 0xFF000000;

    union{
        uint8_t data[3];
        struct { byte_t blue, green, red; };
        struct { byte_t b, g, r; };
    };

    void reset() { red = green = blue = 0; }
    void set(uint32_t rgb) { r = BGRA_RED(rgb); g = BGRA_GREEN(rgb); b = BGRA_BLUE(rgb); }
    void set(byte_t bR, byte_t bG, byte_t bB) { r = bR; g = bG; b = bB; }

    uint32_t color()const { return BGRA(r, g, b, 0xFF); }

    bool is_white()const { return !((~red)|(~green)|(~blue)); }
    bool is_black()const { return !(red|green|blue); }
//    const colorBGR& blend(const colorBGR& c, byte_t a)
//    {
//        return operator=(blend32(color(), c.color(), a));
//    }
};

//
//BGRA32
//

struct bgra32_t
{
    typedef struct colorBGRA this_type;
    const static uint32_t white = 0xFFFFFFFF;
    const static uint32_t black = 0xFF000000;
    union{
        byte_t   data[4];
        uint32_t value;
        struct{ byte_t blue, green, red, alpha; };
        struct{ byte_t b, g, r, a; };
    };

    void reset() { red = green = blue = alpha = 0; }
    void set(uint32_t rgba) { r = BGRA_RED(rgba); g = BGRA_GREEN(rgba); b = BGRA_BLUE(rgba); a = BGRA_ALPHA(rgba); }
    void set(byte_t bR, byte_t bG, byte_t bB, byte_t bA) { r = bR; g = bG; b = bB; a = bA; }

    uint32_t color() { return value; }

    bool is_white()const { return !(~(value | 0xFF000000)); }
    bool is_black()const { return !(value & 0x00FFFFFF); }

//    const this_type& blend(const this_type& c, byte_t a)
//    {
//        color = blend32(color, c.color, a);
//        return *this;
//    }
//    const this_type& blend(const this_type& c)
//    {
//        byte_t a = alpha;
//        color = blend32(color, c.color, c.alpha);
//        alpha = a;
        //alpha = (int(alpha)*alpha + int(c.alpha)*c.alpha)/BYTE_MAX;
//        return *this;
//    }
};

//
// BGRA64
//

struct bgra64_t
{
    union{
        byte_t   data[8];
        uint64_t value;
        struct{ uint16_t blue, green, red, alpha; };
        struct{ uint16_t b, g, r, a; };
    };

    void reset() { red = green = blue = alpha = 0; }
    void set(uint64_t rgba) { r = BGRA64_RED(rgba); g = BGRA64_GREEN(rgba); b = BGRA64_BLUE(rgba); a = BGRA64_ALPHA(rgba); }
    void set(uint16_t bR, uint16_t bG, uint16_t bB, uint16_t bA) { r = bR; g = bG; b = bB; a = bA; }
};

#pragma pack(pop)

typedef rgb565_t color16;
#ifdef CGL_PLATFORM_WINDOWS
typedef bgr24_t  color24;
typedef bgra32_t color32;
typedef bgra64_t color64;
#else
//typedef rgb24_t  color24;
//typedef rgba32_t color32;
//typedef rgba64_t color64;
#endif

}//end namespace graphics
}//end namespace cgl

#endif //COLORDEF_HPP_20200217172344
