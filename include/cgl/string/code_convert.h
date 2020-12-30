/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 code_convert.hpp

 2016-07-12 11:34:49

 ConvertUTF 不能计算预分配空间大小

*/
#ifndef CODE_CONVERT_HPP_20160712113449
#define CODE_CONVERT_HPP_20160712113449

#include <cgl/public.h>
#include <cgl/memory/block.hpp>

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option push
    #pragma option -w-8004        //is assigned a value that is never used
#endif

#include <cgl/external/ConvertUTF/ConvertUTF.cpp>

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option pop
#endif

namespace cgl{

template<typename _char_dst, typename _char_src>
size_t code_convert(_char_dst* dest, size_t dest_size, const _char_src* source, size_t source_size);

template<typename _char_t>
size_t code_convert(_char_t* dest, size_t dest_size, const _char_t* source, size_t source_size)
{//copy only
    if(!dest)return source_size + 1;
    dest_size = dest_size < source_size ? dest_size : source_size;
    std::memcpy(dest, source, dest_size);
    return dest_size;
}

template<>//utf16 to utf8
size_t code_convert<char8_t, char16_t>(char8_t* dest, size_t dest_size, const char16_t* source, size_t source_size)
{
    if(!dest)return source_size * 4 + 1;//test

    llvm::UTF8 *dstBegin = static_cast<llvm::UTF8*>(dest);
    llvm::UTF8 *dstEnd = dstBegin + dest_size;
    const llvm::UTF16* srcBegin = reinterpret_cast<const llvm::UTF16*>(source);
    const llvm::UTF16* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF16toUTF8(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return dstBegin-dest;
    }
    return 0;
}

template<>//utf32 to utf8
size_t code_convert<char8_t, char32_t>(char8_t* dest, size_t dest_size, const char32_t* source, size_t source_size)
{
    if(!dest)return source_size * 4 + 1;//test

    llvm::UTF8 *dstBegin = static_cast<llvm::UTF8*>(dest);
    llvm::UTF8 *dstEnd = dstBegin + dest_size;
    const llvm::UTF32* srcBegin = reinterpret_cast<const llvm::UTF32*>(source);
    const llvm::UTF32* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF32toUTF8(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return dstBegin-dest;
    }
    return 0;
}

template<>//utf8 to utf16
size_t code_convert<char16_t, char8_t>(char16_t* dest, size_t dest_size, const char8_t* source, size_t source_size)
{
    if(!dest)return source_size + 1;//test
    
    llvm::UTF16 *dstBegin = reinterpret_cast<llvm::UTF16*>(dest);
    llvm::UTF16 *dstEnd = dstBegin + dest_size;
    const llvm::UTF8* srcBegin = static_cast<const llvm::UTF8*>(source);
    const llvm::UTF8* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF8toUTF16(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return (size_t(dstBegin) - size_t(dest))/sizeof(char16_t);
    }
    return 0;
}

template<>//utf32 to utf16
size_t code_convert<char16_t, char32_t>(char16_t* dest, size_t dest_size, const char32_t* source, size_t source_size)
{
    if(!dest)return source_size * 2 + 1;//test

    llvm::UTF16 *dstBegin = reinterpret_cast<llvm::UTF16*>(dest);
    llvm::UTF16 *dstEnd = dstBegin + dest_size;
    const llvm::UTF32* srcBegin = reinterpret_cast<const llvm::UTF32*>(source);
    const llvm::UTF32* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF32toUTF16(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return (size_t(dstBegin) - size_t(dest)) / sizeof(char16_t);
    }
    return 0;
}

template<>//utf8 to utf32
size_t code_convert<char32_t, char8_t>(char32_t* dest, size_t dest_size, const char8_t* source, size_t source_size)
{
    if(!dest)return source_size;//test
    
    llvm::UTF32 *dstBegin = reinterpret_cast<llvm::UTF32*>(dest);
    llvm::UTF32 *dstEnd = dstBegin + dest_size;
    const llvm::UTF8* srcBegin = reinterpret_cast<const llvm::UTF8*>(source);
    const llvm::UTF8* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF8toUTF32(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return (size_t(dstBegin) - size_t(dest)) / sizeof(char32_t);
    }
    return 0;
}

template<>//utf16 to utf32
size_t code_convert<char32_t, char16_t>(char32_t* dest, size_t dest_size, const char16_t* source, size_t source_size)
{
    if(!dest)return source_size + 1;//test

    llvm::UTF32 *dstBegin = reinterpret_cast<llvm::UTF32*>(dest);
    llvm::UTF32 *dstEnd = dstBegin + dest_size;
    const llvm::UTF16* srcBegin = reinterpret_cast<const llvm::UTF16*>(source);
    const llvm::UTF16* srcEnd = srcBegin + source_size;
    if(llvm::conversionOK == llvm::ConvertUTF16toUTF32(&srcBegin, srcEnd, &dstBegin, dstEnd, llvm::strictConversion))
    {
        return (size_t(dstBegin) - size_t(dest)) / sizeof(char32_t);
    }
    return 0;
}

//---------------------------------------------------------------------------
// char8_t  <=> wchar_t
// char16_t <=> wchar_t
// char32_t <=> wchar_t

#if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T

template<>//wchar_t to utf8
size_t code_convert<char8_t, wchar_t>(char8_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char8_t, char16_t>(dest, dest_size, reinterpret_cast<const char16_t*>(source), source_size);
}
template<>//utf8 to wchar_t
size_t code_convert<wchar_t, char8_t>(wchar_t* dest, size_t dest_size, const char8_t* source, size_t source_size)
{
    return code_convert<char16_t, char8_t>(reinterpret_cast<char16_t*>(dest), dest_size, source, source_size);
}

template<>//wchar_t to utf16
size_t code_convert<char16_t, wchar_t>(char16_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char16_t>(dest, dest_size, reinterpret_cast<const char16_t*>(source), source_size);
}
template<>//utf16 to wchar_t
size_t code_convert<wchar_t, char16_t>(wchar_t* dest, size_t dest_size, const char16_t* source, size_t source_size)
{
    return code_convert<wchar_t>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
}

template<>//wchar_t to utf32
size_t code_convert<char32_t, wchar_t>(char32_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char32_t, char16_t>(dest, dest_size, reinterpret_cast<const char16_t*>(source), source_size);
}
template<>//utf32 to wchar_t
size_t code_convert<wchar_t, char32_t>(wchar_t* dest, size_t dest_size, const char32_t* source, size_t source_size)
{
    return code_convert<char16_t, char32_t>(reinterpret_cast<char16_t*>(dest), dest_size, source, source_size);
}

#else //CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T

template<>//wchar_t to utf8
size_t code_convert<char8_t, wchar_t>(char8_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char8_t, char32_t>(dest, dest_size, reinterpret_cast<const char32_t*>(source), source_size);
}
template<>//utf8 to wchar_t
size_t code_convert<wchar_t, char8_t>(wchar_t* dest, size_t dest_size, const char8_t* source, size_t source_size)
{
    return code_convert<char32_t, char8_t>(reinterpret_cast<char32_t*>(dest), dest_size, source, source_size);
}

template<>//wchar_t to utf16
size_t code_convert<char16_t, wchar_t>(char16_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char16_t, char32_t>(dest, dest_size, reinterpret_cast<const char32_t*>(source), source_size);
}
template<>//utf16 to wchar_t
size_t code_convert<wchar_t, char16_t>(wchar_t* dest, size_t dest_size, const char16_t* source, size_t source_size)
{
    return code_convert<char32_t, char16_t>(reinterpret_cast<char32_t*>(dest), dest_size, source, source_size);
}

template<>//wchar_t to utf32
size_t code_convert<char32_t, wchar_t>(char32_t* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    return code_convert<char32_t>(dest, dest_size, reinterpret_cast<const char32_t*>(source), source_size);
}
template<>//utf32 to wchar_t
size_t code_convert<wchar_t, char32_t>(wchar_t* dest, size_t dest_size, const char32_t* source, size_t source_size)
{
    return code_convert<wchar_t>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
}

#endif //CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T

//---------------------------------------------------------------------------
// char <=> wchar_t

template<>//unicode to ansi
size_t code_convert<char, wchar_t>(char* dest, size_t dest_size, const wchar_t* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<char8_t, wchar_t>((char8_t*)dest, dest_size, source, source_size);
    #else
        #ifdef _WINDOWS_
        return WideCharToMultiByte(CP_ACP, 0, source, source_size, dest, dest_size, NULL, NULL);
        #else//need setlocale(LC_CTYPE, "")
        return wcstombs(dest, source, source_size);
        #endif
    #endif
}

template<>//ansi to unicode
size_t code_convert<wchar_t, char>(wchar_t* dest, size_t dest_size, const char* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<wchar_t, char8_t>(dest, dest_size, (const char8_t*)source, source_size);
    #else
        #ifdef _WINDOWS_
        return MultiByteToWideChar(CP_ACP, 0, source, source_size, dest, dest_size);
        #else//need setlocale(LC_CTYPE, "")
        return mbstowcs(dest, source, source_size);
        #endif
    #endif
}

//---------------------------------------------------------------------------
// char <=> char16_t

template<>//utf16 to ansi
size_t code_convert<char, char16_t>(char* dest, size_t dest_size, const char16_t* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<char8_t, char16_t>((char8_t*)dest, dest_size, source, source_size);
    #else
        if(!dest)return source_size * 2 + 1;

        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
            return code_convert<char>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
        #else
            memblock<wchar_t> buf;
            buf.resize(source_size);
            source_size = code_convert<wchar_t>(buf.data(), source_size, source, source_size);
            return code_convert<char>(dest, dest_size, buf.data(), source_size);
        #endif
    #endif
}

template<>//char to char16_t
size_t code_convert<char16_t, char>(char16_t* dest, size_t dest_size, const char* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<char16_t, char8_t>(dest, dest_size, reinterpret_cast<const char8_t*>(source), source_size);
    #else
        //#error char16_t
        /*
        if(!dest)return source_size * 2 + 1;

        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
            return code_convert<char>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
        #else
            memblock<wchar_t> buf;
            buf.resize(source_size);
            source_size = code_convert<wchar_t>(buf.data(), source_size, source, source_size);
            return code_convert<char>(dest, dest_size, buf.data(), source_size);
        #endif
        */
        return 0;
    #endif
}

//---------------------------------------------------------------------------
// char <=> char32_t

template<>//utf32 to ansi
size_t code_convert<char, char32_t>(char* dest, size_t dest_size, const char32_t* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<char8_t, char32_t>((char8_t*)dest, dest_size, source, source_size);
    #else
        if(!dest)return source_size * 2 + 1;

        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
            memblock<wchar_t> buf;
            buf.resize(source_size);
            source_size = code_convert<wchar_t>(buf.data(), source_size, source, source_size);
            return code_convert<char>(dest, dest_size, buf.data(), source_size);
        #else
            return code_convert<char>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
        #endif
    #endif
}

template<>//char to char32_t
size_t code_convert<char32_t, char>(char32_t* dest, size_t dest_size, const char* source, size_t source_size)
{
    #ifdef CGL_USING_UTF8_CHAR
        return code_convert<char32_t, char8_t>(dest, dest_size, reinterpret_cast<const char8_t*>(source), source_size);
    #else
        //#error char32_t
        /*
        if(!dest)return source_size * 2 + 1;

        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
            return code_convert<char>(dest, dest_size, reinterpret_cast<const wchar_t*>(source), source_size);
        #else
            memblock<wchar_t> buf;
            buf.resize(source_size);
            source_size = code_convert<wchar_t>(buf.data(), source_size, source, source_size);
            return code_convert<char>(dest, dest_size, buf.data(), source_size);
        #endif
        */
        return 0;
    #endif
}

//---------------------------------------------------------------------------
// char <=> char8_t

template<>//utf8 to ansi
size_t code_convert<char, char8_t>(char* dest, size_t dest_size, const char8_t* source, size_t source_size)
{
    using namespace std;
    #ifdef CGL_USING_UTF8_CHAR
        if(!dest)return source_size + 1;
        if(source_size < dest_size)dest_size = source_size;
        memcpy(dest, source, dest_size);
        dest[dest_size] = '\0';
        return dest_size;
    #else
        if(!dest)return source_size + 1;
        memblock<wchar_t> buf;
        buf.resize(source_size);
        /*
        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
          source_size = code_convert<char16_t>(reinterpret_cast<char16_t*>(buf.data()), source_size, source, source_size);
        #else//#elif sizeof(char_t) == 4
          source_size = code_convert<char32_t>(reinterpret_cast<char32_t*>(buf.data()), source_size, source, source_size);
        #endif
        */
        source_size = code_convert<wchar_t, char8_t>(buf.data(), source_size, source, source_size);
        source_size = code_convert<char>(dest, dest_size, buf.data(), source_size);
    #endif
    return source_size;
}

template<>//ansi to utf8
size_t code_convert<char8_t, char>(char8_t* dest, size_t dest_size, const char* source, size_t source_size)
{
    using namespace std;
    #ifdef CGL_USING_UTF8_CHAR
        if(!dest)return source_size + 1;
        if(source_size < dest_size)dest_size = source_size;
        memcpy(dest, source, dest_size);
        dest[dest_size] = '\0';
        return dest_size;
    #else
        if(!dest)return source_size * 4;//??
        memblock<wchar_t> buf;
        buf.resize(source_size);
        source_size = code_convert<wchar_t>(buf.data(), source_size, source, source_size);
        //ansi to unicode
        /*
        #if CGL_SIZEOF_WCHAR_T == CGL_SIZEOF_CHAR16_T
          source_size = code_convert<char8_t>(dest, dest_size, reinterpret_cast<const char16_t*>(buf.data()), source_size);
        #else
          source_size = code_convert<char8_t>(dest, dest_size, reinterpret_cast<const char32_t*>(buf.data()), source_size);
        #endif
        */
        return code_convert<char8_t, wchar_t>(dest, dest_size, buf.data(), source_size);
    #endif
    //return source_size;
}

}//end namespace cgl

#endif //CODE_CONVERT_HPP_20160712113449

/*

union xchar_t
{
    char8_t  u8[4];
    char16_t u16[2];
    char32_t u32;
};


char8_t* UTF32toUTF8(char8_t* u8str, const char32_t u32ch)
{
    //const UTF32 byteMask = 0xBF;
    //const UTF32 byteMark = 0x80;

    if (u32ch < 128){
        *u8str++ = char8_t(u32ch);
    }
    else if(u32ch < 2048){
        *u8str++ = 0xC0 | char(u32ch >> 6);
        *u8str++ = 0x80 | (char(u32ch) & 0x3F);
    }
    else if (u32ch < 65536){
        *u8str++ = 0xE0 | char(u32ch >> 12);
        *u8str++ = 0x80 | (char(u32ch >> 6) & 0x3F);
        *u8str++ = 0x80 | (char(u32ch) & 0x3F);
    }
    else if (u32ch < UNI_MAX_LEGAL_UTF32){
        *u8str++ = 0xF0 | char(u32ch >> 18);
        *u8str++ = 0x80 | (char(u32ch >> 12) & 0x3F);
        *u8str++ = 0x80 | (char(u32ch >> 6) & 0x3F);
        *u8str++ = 0x80 | (char(u32ch) & 0x3F);
    }

    return u8str;
}
*/
