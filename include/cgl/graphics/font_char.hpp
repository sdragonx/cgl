/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 font_char.hpp

 2018-12-20 12:58:49

 uint32_t 表示一个字符索引，包含各种style

*/
#ifndef FONT_CHAR_HPP_20181220125849
#define FONT_CHAR_HPP_20181220125849

#include <cgl/public.h>

namespace cgl{
namespace graphics{

#pragma pack(push, 1)
class font_char
{
public:
    const static int TT_SIZE_MASK = 0x0FFF;//4095
    const static int TT_SHADOW    = 0x8000;
    const static int TT_OUTLINE   = 0x4000;//bold
    const static int IT_ITALIC    = 0x2000;//italic
    const static int TT_FLUORE    = 0x1000;//fluore
private:
    union{
        uint32_t info;
        struct{
            uint16_t ch;
            uint16_t attrib;
        };
    };
public:
    font_char() : info() { }
    font_char(wchar_t c, int16_t size, uint32_t flag = 0)
    {
        set_value(c, size, flag);
    }

    void set_value(wchar_t c, int16_t size, uint32_t flag = 0)
    {
        ch = c;
        set_attrib(size, flag);
    }

    void set_attrib(int16_t size, uint32_t flag)
    {
        attrib = (size & TT_SIZE_MASK) | flag;
    }

    int value()const
    {
        return ch;
    }

    int size()const
    {
        return attrib & TT_SIZE_MASK;
    }

    //阴影效果
    bool shadow()const
    {
        return attrib & TT_SHADOW;
    }

    //描边效果
    bool outline()const
    {
        return attrib & TT_OUTLINE;
    }

    //霓虹灯效果
    bool fluore()const
    {
        return attrib & TT_FLUORE;
    }

    bool operator<(const font_char& other)const
    {
        return info < other.info;
    }

    bool operator==(const font_char& other)const
    {
        return info == other.info;
    }
};
#pragma pack(pop)

}//end namespace graphics
}//end namespace cgl

#endif //FONT_CHAR_HPP_20181220125849
