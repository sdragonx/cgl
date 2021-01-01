/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 string.h

 2015-02-03 08:51:58

 2018/7/27 1:06

//strrchr   返回find_last           指针
//strspn    返回find_fist_not_of    位置
//strcspn   返回find_first_of   返回位置
//strpbrk   返回find_first_of   返回指针

*/
#ifndef CGL_STR_HPP_20150203085158
#define CGL_STR_HPP_20150203085158

#include <cgl/public.h>
#include <cgl/memory.hpp>
#include <cstdio>
#include <cstring>

namespace cgl{
namespace internal{ namespace string{

template<typename T>
inline size_t cslen(const T* str)
{
    const T* p =  str;
    for(; *p++ ;);
    return (--p) - str;
}

template<typename T>
inline T* cscpy(T* begin, T* end, const T* str)
{
    T* p = begin;
    for(; *str && (p < end); ){
        *p++ = *str++;
    }
    *p = '\0';
    return begin;
}

/*
//gcc 下这个很快
template<typename T>
inline T* cscpy(T* begin, const T* str, size_t size)
{
    size_t n = cslen(str);
    if(n < size)size = n;
    memcpy(begin, str, size * sizeof(T));
    *(begin + size) = 0;

    return begin;
}
*/

}}//end namespace internal::string

//---------------------------------------------------------------------------
//
// size_t cslen(const char_type* str)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline size_t cslen(const _char_t* str)
{
    return internal::string::cslen(str);
}

template<>inline size_t cslen<char>(const char* str) { return strlen(str); }
template<>inline size_t cslen<wchar_t>(const wchar_t* str){ return wcslen(str); }
template<>inline size_t cslen<char8_t>(const char8_t* str){ return strlen((const char*)(str)); }

#if __SIZEOF_WCHAR_T__ == 2
template<>inline size_t cslen<char16_t>(const char16_t* str){ return wcslen((const wchar_t*)(str)); }
template<>inline size_t cslen<char32_t>(const char32_t* str){ return cgl::internal::string::cslen(str); }
#else
template<>size_t cslen<char16_t>(const char16_t* str){ return cgl::internal::string::cslen(str); }
template<>size_t cslen<char32_t>(const char32_t* str){ return wcslen((const wchar_t*)(str)); }
#endif

//---------------------------------------------------------------------------
//
// char_type* csset(char_type* str, char_type ch, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* csset(_char_t* str, _char_t ch, size_t size)
{
    std::fill(str, str + size, ch);
    str[size] = '\0';
    return str;
}

template<>inline char* csset<char>(char* str, char ch, size_t size)
{

    #ifdef CGL_PLATFORM_ANDROID
	memset(str, ch, size);
	str[size] = '\0';
	return str;
	#elif defined(CGL_PLATFORM_MSVC)
    return _strnset(str, ch, size);
    #else
	return strnset(str, ch, size);    
    #endif
}

template<>inline char8_t* csset<char8_t>(char8_t* str, char8_t ch, size_t size)
{
    return (char8_t*)csset<char>((char*)(str), ch, size);
}

//---------------------------------------------------------------------------
//
// char_type* cslwr(char_type* str, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* cslwr(_char_t* str, size_t size)
{
    _char_t* end = str + size;
    for(_char_t* p = str; *p && p < end; ){
        std::tolower(*p++);
    }
}

//---------------------------------------------------------------------------
//
// char_type* csupr(char_type* str, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* csupr(_char_t* str, size_t size)
{
    _char_t* end = str + size;
    for(_char_t* p = str; *p && p < end; ){
        std::toupper(*p++);
    }
}

//---------------------------------------------------------------------------
//
// char_type* cscpy(char_type* dest, const char_type* source, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* cscpy(_char_t* dest, const _char_t* source, size_t size)
{
    return tvalue<_char_t>(strncpy, wcsncpy)(dest, source, size);
}

template<>inline char8_t* cscpy<char8_t>(char8_t* dest, const char8_t* source, size_t size)
{
    return (char8_t*)strncpy((char*)(dest), (const char*)(source), size);
}

#if __SIZEOF_WCHAR_T__ == 2
template<>
inline char16_t* cscpy<char16_t>(char16_t* dest, const char16_t* source, size_t size)
{
    return (char16_t*)wcsncpy((wchar_t*)(dest), (const wchar_t*)(source), size);
}

template<>
inline char32_t* cscpy<char32_t>(char32_t* dest, const char32_t* source, size_t size)
{
    return cgl::internal::string::cscpy(dest, dest + size, source);
}
#else
template<>
char16_t* cscpy<char16_t>(char16_t* dest, const char16_t* source, size_t size)
{
    return cgl::internal::string::cscpy(dest, dest + size, source);
}

template<>
char32_t* cscpy<char32_t>(char32_t* dest, const char32_t* source, size_t size)
{
    return (char32_t*)wcsncpy((wchar_t*)(dest), (const wchar_t*)(source), size);
}
#endif

//---------------------------------------------------------------------------
//
// char_type* cscat(char_type* dest, const char_type* source, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* cscat(_char_t* dest, const _char_t* source, size_t size)
{
    size_t n = cslen(dest);
    _char_t* p = dest + n;
    if(n < size){
        cscpy(p, source, size - n);
    }
    return dest;
}

//---------------------------------------------------------------------------
//
// int cscmp(const char_type* str1, const char_type* str2, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline int cscmp(const _char_t* s1, const _char_t* s2, size_t size)
{
    //++size;//size+1，尾部0判断 strncmp不包括尾部\0
    for(; 0 < size; ++s1, ++s2, --size){
        if(*s1 != *s2){
            return (static_cast<unsigned>(*s1) < static_cast<unsigned>(*s2)) ? -1 : 1;
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
//
// int csicmp(const char_type* str1, const char_type* str2, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
int csicmp(const _char_t* str1, const _char_t* str2, size_t size)
{
    _char_t c1, c2;
    for(; size > 0; --size){
        c1 = *str1++;
        c2 = *str2++;
        //tolower
        if ( (c1 >= 'A') && (c1 <= 'Z') )
            c1 += 0x20;
        if ( (c2 >= 'A') && (c2 <= 'Z') )
            c2 += 0x20;

        if(c1 != c2){
            return (static_cast<unsigned>(c1) < static_cast<unsigned>(c2)) ? -1 : 1;
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
//
// char_type *csdup(const char_type *str, size_t n)
//
//---------------------------------------------------------------------------

//copy string, need to release memory with std::free()
template<typename _char_t>
_char_t *csdup(const _char_t *str, size_t size)
{
    _char_t *buf = (_char_t*)std::malloc((size + 1) * sizeof(_char_t));
    memcpy(buf, str, size * sizeof(_char_t));
    buf[size] = 0;
    return buf;
};

//---------------------------------------------------------------------------
//
// char_type* csrev(char_type* str, size_t size)
//
//---------------------------------------------------------------------------

template<typename _char_t>
inline _char_t* csrev(_char_t* str, size_t size)
{
    return std::reverse(str, str + size);
}

//---------------------------------------------------------------------------
//
// find
//
//---------------------------------------------------------------------------

template<typename _char_t>
const _char_t* csfind(const _char_t* str, size_t size, _char_t ch)
{
    return std::find(str, str + size, ch);
}

template<typename _char_t>
const _char_t* csfind(const _char_t* str, size_t size, const _char_t* strfind, size_t sizefind)
{
    return std::search(str, str + size, strfind, strfind + sizefind);
}

template<typename _char_t>
size_t find_string(const _char_t* text, size_t length,
    const _char_t* find, size_t find_length,
    bool case_sensitive = true)
{
    for(size_t i=0; i<length - find_length; ++i){
        if(case_sensitive){
            if(cscmp(text+i, find, find_length) == 0){
                return i;
            }
        }
        else{
            if(csicmp(text+i, find, find_length) == 0){
                return i;
            }
        }
    }
    return -1;
}

template<typename _char_t>
int rfind_string(const _char_t* text, size_t length,
    _char_t* find, size_t find_length, bool case_sensitive = true)
{
    for(size_t i=length - find_length - 1; i<length - find_length; --i){
        if(case_sensitive){
            if(cscmp(text+i, find, find_length) == 0){
                return i;
            }
        }
        else{
            if(csicmp(text+i, find, find_length) == 0){
                return i;
            }
        }
    }
    return -1;
}

//---------------------------------------------------------------------------
//
// char_type* csprintf(char_type* buf, size_t size, const char_type* param, ...)
//
//---------------------------------------------------------------------------

template<typename _char_t>
_char_t* csprintf(_char_t* buf, size_t size, const _char_t* param, ...)
{
    using namespace std;
    va_list body;
    va_start(body, param);
    #ifndef CGL_PLATFORM_ANDROID
    tvalue<_char_t>(vsnprintf, vsnwprintf)(buf, size, param, body);
    #else
    vsnprintf(buf, size, param, body);
    #endif
    va_end(body);
    return buf;
}

#ifdef CGL_PLATFORM_ANDROID
template<>
wchar_t* csprintf<wchar_t>(wchar_t* buf, size_t size, const wchar_t* param, ...)
{
    CGL_ASSERT(0);//android using utf8
    using namespace std;
    va_list body;
    va_start(body, param);
    char* astr = (char*)malloc(size * 4);//测试，不一定正确
    wcstombs(astr, buf, size);
    char a_param[1024];
    wcstombs(a_param, param, wcslen(param));
    vsnprintf(astr, size, a_param, body);
    free(buf);
    va_end(body);
    return buf;
}
#endif

//---------------------------------------------------------------------------
//
// int csscanf(const char_type* text, const char_type* param, ...)
//
//---------------------------------------------------------------------------

template<typename _char_t>
int csscanf(const _char_t* text, const _char_t* param, ...)
{
    using namespace std;
    va_list body;
    va_start(body, param);
    int n = tvalue<_char_t>(vsscanf, vswscanf)(text, param, body);
    va_end(body);
    return n;
}

//---------------------------------------------------------------------------

}//end namespace cgl

#endif //CGL_STR_HPP_20150203085158
