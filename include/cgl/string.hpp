/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 string.hpp

 2005.12.22.21:55
 2006.06.11 19:24

 config
    CGL_USEING_T_TEXT
    CGL_STRING_CACHE

 class
    stringref

*/
#ifndef CGL_STRING_H_200512222155
#define CGL_STRING_H_200512222155

#include <cgl/public.h>
#include <cgl/algorithm.hpp>
//#include <cgl/string/bstring.hpp>
#include <cgl/string/cstring.hpp>
//#include <cgl/string/stringref.hpp>
#include <cgl/string/stringsplit.hpp>
#include <cgl/string/ecstr.hpp>

#include <cgl/resource/strings.hpp>

namespace cgl{

struct endline_t
{
    //#ifdef CGL_PLATFORM_WINDOWS
    static const char* a_str()
    {
        return "\r\n";
    }

    static const wchar_t* w_str()
    {
        return L"\r\n";
    }

    static const tchar_t* t_str()
    {
        #ifdef UNICODE
            return w_str();
        #else
            return a_str();
        #endif
    }
    //#endif//CGL_PLATFORM_WINDOWS

    template<typename T>
    static const T* c_str()
    {
        return tvalue<T>(a_str, w_str)();
    }

    template<typename T>
    void put(T& s)
    {
        s.push_back('\r');
        s.push_back('\n');
    }
};

const static endline_t endline = endline_t();

//---------------------------------------------------------------------------

//返回空格、制表符、回车换行等不可显示字符
template<typename _char_t>
inline const _char_t* space_str()
{
    //static const _char_t space[] = {' ', '\t', '\r', '\n', '\f', '\v', '\0'};
    return cstring<_char_t>::space();
}

//{{ string trim
// trim() trim_left() trim_right()
// 去除空白字符
template<typename char_type, template<typename> class string_type>
inline void trim_left(
    string_type<char_type>& str,
    const string_type<char_type>& trimmer = space_str<char_type>())
{
    str.erase(0, str.find_first_not_of(trimmer));
}

template<typename char_type, template<typename> class string_type>
inline void trim_right(
    string_type<char_type>& str,
    const string_type<char_type>& trimmer = space_str<char_type>())
{
    str.erase(str.find_last_not_of(trimmer)+1);
}

template<typename char_type, template<typename> class string_type>
inline void trim(string_type<char_type>& str,
    string_type<char_type>& trimmer = space_str<char_type>())
{
    trim_right(str, trimmer);
    trim_left(str, trimmer);
}

//}} string trim

//---------------------------------------------------------------------------
//
// csicmper 忽略大小写的字符串比较器
//
//---------------------------------------------------------------------------

struct csicmper
{
    bool operator()(const char* a, const char* b)const
    {
        return csicmp(a, b, cslen(a) + 1) < 0;//??
    }

    template<typename T>
    bool operator()(const cstring<T>& a, const cstring<T>& b)const
    {
        return a.icompare(b) < 0;
    }
};

//---------------------------------------------------------------------------
//const wchar_t* chsnum_lower = L"〇一二三四五六七八九";
//const wchar_t* chsnum_upper = L"零壹贰叁肆伍陆柒捌玖";
#if 0

class chs_num
{
public:
    static const wchar_t UPPER[];
    static const wchar_t LOWER[];

    //英文中文数字
    wchar_t tolower_chsnum(int n)
    {
        if(n < 10)return LOWER[n];
        return 0;
    }

    wchar_t toupper_chsnum(int n)
    {
        if(n < 10)return UPPER[n];
        return 0;
    }

    int to_int(wchar_t ch)
    {
        for(int i=0; i<10; ++i){
            if(UPPER[i] == ch || LOWER[i] == ch){
                return i;
            }
        }
        return 0;
    }

    wchar_t tolower(wchar_t ch)
    {
        for(int i=0; i<13; ++i){
            if(UPPER[i] == ch)return LOWER[i];
        }
        return 0;
    }

    wchar_t toupper(wchar_t ch)
    {
        for(int i=0; i<13; ++i){
            if(LOWER[i] == ch)return UPPER[i];
        }
        return 0;
    }
};

const wchar_t chs_num::UPPER[] = L"零壹贰叁肆伍陆柒捌玖拾佰仟";
const wchar_t chs_num::LOWER[] = L"〇一二三四五六七八九十百千";

#endif

//判定unicode字符是否中文（不全面）
bool is_chinese(wchar_t ch)
{
    return ch >= 0x4E00 && ch <= 0x9FA5;
}

//
// 字符串右对齐
//

template<typename T>
T* align_right(T *dst_begin, T* dst_end, const T* src_begin, const T* src_end, const T& fill_ch)
{
    if(src_end - src_begin < dst_end - dst_begin){
        dst_end = std::copy_backward(src_begin, src_end, dst_end);
        std::fill(dst_begin, dst_end, fill_ch);
    }
    else if(dst_begin != src_begin){
        std::copy(src_begin, src_end, dst_begin);
    }
    return dst_begin;
}


template<typename T>
cstring<T> string_align(const cstring<T>& str, size_t width, size_t align, T fill_ch = ' ')
{
    if(width <= str.size()){
        return str;
    }

    cstring<T> temp;
    if(align == CGL_CENTER){
        int n = width - str.size();
        return cstring<T>(fill_ch, n / 2).append(str).append(fill_ch, n - n / 2);
    }
    else if(align == CGL_RIGHT){
        return cstring<T>(fill_ch, width - str.size()).append(str);
    }
    else{//left
        return cstring<T>(str).append(fill_ch, width - str.size());
    }
}

template<typename _char_t>
bool is_inside(const _char_t *text, size_t size, const _char_t* str)
{
    return text <= str && str < text + size;
}

template<typename _char_t>
_char_t* string_align(_char_t *dest, size_t size, const _char_t* source, size_t length, int width, int align, _char_t fill_ch)
{
    if(is_inside(dest, size, source)){
        if(dest == source){
            return dest;
        }
    }

    if(size < length){
        memcpy(dest, source, (size - 1) * sizeof(_char_t));
    }
/*
    if(src_end - src_begin < dst_end - dst_begin){
        dst_end = std::copy_backward(src_begin, src_end, dst_end);
        std::fill(dst_begin, dst_end, fill_ch);
    }
    else if(dst_begin != src_begin){
        std::copy(src_begin, src_end, dst_begin);
    }
    return dest;
*/
}

inline void skip_numeric(const char*& str)
{
    while(*str >='0' && *str <='9')++str;
}

inline void skip_space(const char*& str)
{
    while(isspace(*str))++str;
}

template<typename T>
inline T skip_space(T begin, T end)
{
    for( ; begin < end; ++begin){
        if(*begin != ' ' && *begin != '\t' && *begin != '\r' && *begin != '\n'){
            break;
        }
    }
    return begin;
}

template<typename S, typename T>
T parse_string(S& str, T begin, T end)
{
    for( ; begin < end; ++begin){
        if(isspace(*begin)){
            break;
        }
        else{
            str.push_back(*begin);
        }
    }
    return begin;
}

template<typename _char_t>
void put_tab(cstring<_char_t>& s, int level = 1)
{
    s.append(' ', level * 4);
}

template<typename _char_t>
void put_endl(cstring<_char_t>& s)
{
    s.push_back('\r');
    s.push_back('\n');
}

int hexview(cstring<char>& code, const void* data, size_t size)
{
    const byte_t* pbyte = (const byte_t*)(data);
    byte_t buf[2];
	byte_t ptr[8];
	byte_t hex[16];

    size_t i;

    if(size == 0){
        return 0;
    }

    code = "         |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F |";
    put_endl(code);
    code.append(L'-', 8 + 16 * 3 + 16 + 5);
    put_endl(code);

    memset(ptr, '0', 8);
    code.append(ptr, 8);
    code.append(" | ", 3);

    for(i = 0; i < size; ++i){
        tohex8(buf, pbyte[i]);
        code.push_back(buf[0]);
        code.push_back(buf[1]);

        hex[i & 15] = isprint(pbyte[i]) ? pbyte[i] : '.';

        if((i & 15) == 15){
            code.append(" | ", 3);
            code.append(hex, 16);
            memset(hex, 0, 16);

            put_endl(code);

            tohex32(ptr, i);
            code.append(ptr, 8);
            code.append(" | ", 3);
        }
        else{
            code.push_back(' ');
        }
    }
    if(i &= 15){
        *(code.end() - 2) = ' ';
        code.append(' ', (15 - i) * 3);
        code.append("   | ", 5);
        code.append(hex, i);
        put_endl(code);
    }
    return code.size();
}

int hexview(cstring<char>& code, std::vector<byte_t>& buffer)
{
    return hexview(code, &buffer[0], buffer.size());
}

//
// 数据转c代码
//

int to_source(cstring<char>& code, const char* name, const void* data, size_t size, bool hex_mode = true)
{
    code.clear();

    code = "const unsigned char ";
    code += name;
    code += "[] = {\r\n    ";
    const byte_t* pbyte = (const byte_t*)(data);

    if(hex_mode){//十六进制模式
        char buf[2];
        char hex[16];
        size_t i;
        for(i = 0; i < size; ++i){
            code.push_back('0');
            code.push_back('x');
            tohex8(reinterpret_cast<byte_t*>(buf), pbyte[i]);
            code.push_back(buf[0]);
            code.push_back(buf[1]);
            code.push_back(',');

            hex[i & 15] = isprint(pbyte[i]) ? pbyte[i] : '.';

            if((i & 15) == 15){
                code.append("  //", 4);
                code.append(hex, 16);
                memset(hex, 0, 16);

                put_endl(code);
                put_tab(code);
            }
            else{
                code.push_back(' ');
            }
        }
        if(i &= 15){
            *(code.end() - 2) = ' ';
            code.append(' ', (15 - i) * 6);
            code.append("       //", 9);
            code.append(hex, i);
            put_endl(code);
        }
    }
    else{
        //字符模式
        //可视字符直接以字符形式显示
        //不可打印字符，以十六进制转码形式显示
        for(size_t i = 0; i < size; ++i){
            code.push_back('\'');
            encode_char<char>(code, pbyte[i]);
            code.push_back('\'');
            code.push_back(',');
            if((i & 15) == 15){
                put_endl(code);
            }
            else{
                code.push_back(' ');
            }
        }
    }

    code.push_back('}');
    code.push_back(';');
    return 0;
}

int to_source(cstring<char>& code, const char* name, const std::vector<byte_t>& data, bool hex_mode = true)
{
    return to_source(code, name, &data[0], data.size(), hex_mode);
}

//
// format_int<T>(123456) 123,456
//
template<typename _char_t>
cstring<_char_t> format_int(int n)
{
    cstring<_char_t> s;
    while(n){
        if(!s.empty())s.insert(0, ',');
        s = cstring<_char_t>(n % 1000) + s;
        n /= 1000;
    }
    return s;
}

}//end namespace cgl

#endif //CGL_STRING_H_200512222155
