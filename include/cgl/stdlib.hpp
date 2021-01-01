/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stdlib.hpp

 2018-03-27 22:15:03

 2018/7/27 1:11

 parse类函数
 inline const char* to_digit(const char* p, int & n)
{
    for( ; *p; ++p){
        switch (*p){
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n *= 10;
            n += *p - '0';
            break;
        default:
            return p;
        }
    }
    return p;
}

2020-02-08 15:58:45
class typeformat
{
public:
    int alpha;
    int beta;
    int delta;
    int gamma;


    int    : 宽度     进制     是否填0 对齐
    float  : 整形宽度 浮点宽度 是否填0 对齐
    string : 宽度     0        是否填0 对齐


public:
    typeformat(int a, int b, int c, int d) : alpha(a), beta(b), delta(c), gamma(d)
    {
    }
};

*/
#ifndef STDLIB_HPP_20180327221503
#define STDLIB_HPP_20180327221503

#include <cstdlib>

#include <cgl/public.h>
#include <cgl/std/string.hpp>
#include <cgl/std/type_traits.hpp>

namespace cgl{
namespace math{

template<typename T>
extern inline T pow_of10(int n);

}//end namespace math

template <class T, std::size_t N>
std::size_t countof(T const (&) [N])
{
    return N;
}

//---------------------------------------------------------------------------
//
// byte_t char_tobyte(char ch)
//
//---------------------------------------------------------------------------

/*
//转换成功，返回值小于0xF0，返回0xF0表示失败
template<typename _char_t>
inline _char_t char_toint(_char_t ch, int radix = 10)
{
    if(ch>='0' && ch<='9'){
        ch -= '0';
    }
    else if(ch >= 'a' && ch <= 'a'+radix-10){
        ch -= 'a';
        ch += 10;
    }
    else if(ch >= 'A' && ch <= 'A'+radix-10){
        ch -= 'A';
        ch += 10;
    }
    else
    {
        return 0xF0;
    }
    return ch;
}
*/

//这个是最快的算法(选自vc的库)
inline byte_t char_tobyte(char ch)
{
    switch (ch)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return (byte_t) (ch - '0');
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        return (byte_t) (10 + (ch - 'A'));
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        return (byte_t) (10 + (ch - 'a'));
    default:
        break;
    }
    return 0;
}

inline byte_t char_tobyte(char ch, int radix)
{
    switch (ch)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return (byte_t) (ch - '0');
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
    case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
        if(ch <= 'A' + radix - 10){
            return (byte_t) (10 + (ch - 'A'));
        }
        break;
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
    case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
    case 'o': case 'p': case 'q': case 'r': case 's': case 't':
    case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
        if(ch <= 'a' + radix - 10){
            return (byte_t) (10 + (ch - 'A'));
        }
        break;
    default:
        break;
    }
    return 0;
}

//---------------------------------------------------------------------------
//
// char_type tohex(char_type* out, const void* in, size_t size)
//
////---------------------------------------------------------------------------

//二进制转十六进制字符串
inline void tohex8(byte_t buf[2], uint8_t value)
{
    uint8_t n = value >> 4 & 0xF;
    buf[0] = n < 10 ? '0' + n : 'A' + n - 10;
    n = value & 0xF;
    buf[1] = n < 10 ? '0' + n : 'A' + n - 10;
}

inline void tohex16(byte_t buf[4], uint16_t ch)
{
    tohex8(buf, ch >> 8 & 0xFF);
    tohex8(buf + 2, ch & 0xFF);
}

inline void tohex32(byte_t buf[8], uint32_t ch)
{
    tohex16(buf, ch >> 16 & 0xFFFF);
    tohex16(buf + 4, ch & 0xFFFF);
}

inline void tohex64(byte_t buf[16], uint64_t ch)
{
    tohex32(buf, ch >> 32 & 0xFFFFFFFF);
    tohex32(buf + 8, ch & 0xFFFFFFFF);
}

//二进制转十六进制字符串
template<typename _char_t>
_char_t* tohex(_char_t* pout, const void* pin, size_t size)
{
    const byte_t *pdata = reinterpret_cast<const byte_t*>(pin);
    byte_t ch;
    _char_t *p = pout;

    for(; size--; ++pdata)
    {
        ch = (*pdata)>>4 & 0xF;
        *p++ = ch < 10 ? '0' + ch : 'A' + ch - 10;
        ch = (*pdata) & 0xF;
        *p++ = ch < 10 ? '0' + ch : 'A' + ch - 10;
    }
    *p = 0;
    return pout;
}

//---------------------------------------------------------------------------
//
// void* tobin(void* out, const char_type* in, size_t size)
//
//---------------------------------------------------------------------------

//十六进制字符串转二进制
template<typename _char_t>
void* tobin(void *pout, const _char_t* pin, size_t size)
{
    byte_t* pdata = reinterpret_cast<byte_t*>(pout);
    const _char_t* pend = pin+size;
    byte_t ch1, ch2;
    for(; pin<pend; )
    {
        ch1 = *pin++;
        ch2 = *pin++;
        ch1 = char_tobyte(ch1);
        ch2 = char_tobyte(ch2);

        ch1 <<= 4;
        ch1 |= ch2;
        *pdata++ = ch1;
    }
    return pout;
}

//---------------------------------------------------------------------------
//
// signed int_t cstoint(const char_type* begin, char_type** pend, int radix)
//
//---------------------------------------------------------------------------

//根据linux内核函数修改
template<typename signed_int, typename char_type>
signed_int cstoint(const char_type* begin, char_type** pend, int radix)
{
    if(!begin){
        return 0;
    }
    if(radix < 2 && radix > 36){
        CGL_ASSERT("invalid radix");
    }
    signed_int n = 0;
    //const char_type* end = begin + sizeof(signed_int) * 8 + 1;//000000000000000000000000001
    int digit = 0;
    while(isspace(static_cast<int>(*begin)))++begin;
    char_type sign = *begin;//save sign indication
    if (sign == '-' || sign == '+')++begin;//skip sign
    while(*begin == '0')++begin;//skip '0'
    if((radix == 16) && (((*begin)|0x20) == 'x'))++begin;//skip 'X'

    for( ; *begin; ++begin)
    {
        if (*begin >= '0' && *begin <= '9')
            digit = *begin - '0';
        else if (*begin >= 'A' && *begin < 'A' + radix - 10)
            digit = *begin - 'A' + 10;
        else if (*begin >= 'a' && *begin < 'a' + radix - 10)
            digit = *begin - 'a' + 10;
        else{
            //CGL_ASSERT("cstoint : bad cast.");
            break;//check_error
        }
        n *= radix;
        n += digit;
    }
    //out:
    if (pend)
        *pend = (char_type*)begin;
    return sign == '-' ? -n : n;
}

//---------------------------------------------------------------------------
//
// char_type* tostr(char_type* in begin, char_type* in end,
//        T value, T radix, char_type** out pend = null)
//
//---------------------------------------------------------------------------

//
//buffer_sizeof
//calc numeric to string need buffer size
//

template<typename T>
struct buffer_sizeof
{
    typedef T value_type;
    enum{
        value = sizeof(T) * 8 + 1
    };
};

//end buffer_sizeof

template<typename T>
inline const T* digit_text()
{
    const static T text[] = {//0-9 A-Z
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F', 'G',
        'H', 'I', 'J', 'K', 'L', 'M', 'N',
        'O', 'P', 'Q', 'R', 'S', 'T',
        'U', 'V', 'W', 'X', 'Y', 'Z'};
    return text;
}

template<typename _char_t, typename T>
_char_t * tostr(_char_t * begin, size_t size, T value, T radix, _char_t** pend = null)
{
    _char_t buf [buffer_sizeof<T>::value];
    _char_t *end = begin + size;
    _char_t *p = begin;

    if(value == 0){
        *p++ = '0';
    }
    else if (radix >= 2 && radix <= 36){

        if(value < 0){
            *p++ = '-';
            value = -value;
        }

        _char_t *bufp = buf;
        for (;;){
            *bufp++ = (_char_t)((TYPENAME unsigned_type<T>::type)(value) % radix);
            if ((value = (TYPENAME unsigned_type<T>::type)(value) / radix) == 0)
                break;
        }

        const _char_t* digit = digit_text<_char_t>();
        for(;bufp != buf && p < end;){
            *p++ = digit[*--bufp];
            //n < 10 ? '0' + n : 'A' + n - radix
        }
        #ifdef _DEBUG
        if(p == end && bufp < buf){
            CGL_ASSERT("tostr : buffer eof.");
        }
        #endif
    }
    else{
        CGL_ASSERT("tostr : invalid radix.");
    }

    *p = '\0';
    if(pend)*pend = p;
    return (begin);
}

//---------------------------------------------------------------------------
//
// double cstof(const char_type* str)
//
//---------------------------------------------------------------------------

template<typename _char_t>
double cstof(const _char_t* str)
{
    double n = 0.0;
    double d = 0.1;
    int sign = 1;

    while(*str == ' ' || *str == '\t'){//skip space
        ++str;
    }

    if(*str == '-'){//get sign
        sign = -1;
        ++str;
    }
 
    if(!(*str >= '0' && *str <= '9')){//is not digits, return
        return n;
    }

    for( ; *str >= '0' && *str <= '9'; ++str){//整数部分
        n = n * 10.0 + *str - '0';
    }

    if(*str == '.'){//小数部分
        ++str;
        for( ; *str>='0' && *str<='9'; ++str){
            n = n + (*str - '0') * d;
            d *= 0.1;
        }
    }

    if(*str == 'e' || *str == 'E'){//exp
        ++str;

        int exp = 0;
        int exp_sign = 1;
        
        if(*str == '-'){
            exp_sign = -1;
            ++str;
        }

        for( ; *str>='0' && *str<='9'; ++str){//指数部分
            exp = exp * 10 + *str - '0';
        }

        if(0 < exp_sign){//pow
            while(exp > 0){
                n *= 10.0;
                --exp;
            }
        }
        else{
            while(exp > 0){
                n *= 0.1;
                --exp;
            }
        }
    }

    return n * sign;
}

//---------------------------------------------------------------------------
//
// format_float(char_type*, float_type, int_digits, dec_digits)
//
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//
// float_traits
//
//---------------------------------------------------------------------------

template<typename T>
struct float_traits
{
    typedef float value_type;
    typedef int32_t int_type;//转换整形需要的类型
    static float max_digits()//最高精度倍数
    {
        //static float digits = 1000000.0f;//6
        //return digits;
        return 1000000.0f;
    }
};

template<>
struct float_traits<float>
{
    typedef float value_type;
    typedef int32_t int_type;//转换整形需要的类型
    static float max_digits()//最高精度倍数
    {
        //static float digits = 1000000.0f;//6
        return 1000000.0f;//digits;
    }
};

template<>
struct float_traits<double>
{
    typedef double value_type;
    typedef int64_t int_type;
    static double max_digits()
    {
        //static double digits = 10000000000000000.0;//16
        return 10000000000000000.0;//digits;
    }
};

template<typename _char_t, typename T>
_char_t* tostr_align_right(_char_t* str, _char_t* buf, _char_t** pend, T n, int width = 0)
{
    _char_t* p = buf;
    _char_t* dest = str;

    for(; n; ++p){
        *p = n % 10;
        n /= 10;
    }
    width -= (p - buf);
    if(buf < p)--p;
    for(int i=0; i<width; ++i){
        ++p;
        *p = '\0';
    }
    for(; p>=buf; --p){
        *dest = (*p) + '0';
        ++dest;
    }
    *dest = '\0';
    if(pend)*pend = dest;
    return str;
}

template<typename _char_t, typename T>
_char_t* tostr_align_left(_char_t* str, _char_t* buf, _char_t** pend, T n, int width = 0)
{
    _char_t* p = buf;
    _char_t* dest = str;

    for(; n; ++p){
        *p = n % 10;
        n /= 10;
    }
    width -= (p - buf);
    if(buf < p)--p;
    for(_char_t* i=p; i>=buf; --i){
        *dest = (*i) + '0';
        ++dest;
    }
    for(int i=0; i < width; ++i){
        *dest = '0';
        ++dest;
    }
    *dest = '\0';
    if(pend)*pend = dest;
    return str;
}

template<typename _char_t, typename _float_t>
_char_t* format_float(_char_t* str, size_t size, _float_t n, int int_length = 0, int dec_length = 6)
{
    typedef TYPENAME float_traits<_float_t>::int_type int_type;
    _char_t buf[64] = {0};
    _char_t* p = str;

    CGL_ASSERT(int_length <64 && dec_length < 64);

    if(n < 0){
        *p = '-';
        ++p;
        n = -n;
    }

    int_type m = std::floor(n);

    tostr_align_right<_char_t, int_type>(p, buf, &p, m, int_length);//整数部分

    n -= m;
    n += 1.0f;
    _float_t digits = math::pow_of10<_float_t>(dec_length);
    if(digits == 0 && dec_length != 0)digits = float_traits<_float_t>::max_digits();
    n *= digits;
    m = n;
    tostr_align_left<_char_t, int_type>(p, buf, null.as<_char_t**>(), m, dec_length);//小数部分

    *p = '.';
    return str;
}

/*
//
// format float
// float    precision 6~7 digits
// double    precision 15~16 digits
// long double ??
//
template<typename _char_t>
_char_t* format_float(_char_t* buf, size_t size, float value)
{
    return csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%.8g"), value);
}

template<typename _char_t>
_char_t* format_float(_char_t* buf, size_t size, float value, int dec, int precision)
{
    int length = dec + precision;
    ++length;
    return csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%0*.*f"), length, precision, value);
}

template<typename _char_t>
_char_t* format_float(_char_t* buf, size_t size, double value)
{
    return csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%.16g"), value);
}

template<typename _char_t>
_char_t* format_float(_char_t* buf, size_t size, double value, int dec, int precision)
{
    int length = dec + precision;
    ++length;
    return csprintf<_char_t>(buf, size, T_TEXT(_char_t, "%0*.*f"), length, precision, value);
}
*/

//---------------------------------------------------------------------------
//string to int

inline int32_t atoi32(const char* str)
{
    using namespace std;
    return atoi(str);
}

inline int32_t wtoi32(const wchar_t* str)
{
    using namespace std;
    #ifdef CGL_PLATFORM_WINDOWS
    return _wtoi(str);
    #else//linux
    return wcstol(str, null, 10);
    #endif
}

template<typename _char_t>
inline int32_t cstoi(const _char_t* str)
{
    return tvalue<_char_t>(atoi32, wtoi32)(str);
}

inline int64_t atoi64(const char* str)
{
    using namespace std;
    #ifdef CGL_PLATFORM_WINDOWS
    return _atoi64(str);
    #else//linux
    return atoll(str);
    #endif
}

inline int64_t wtoi64(const wchar_t* str)
{
    using namespace std;
    #ifdef CGL_PLATFORM_WINDOWS
    return _wtoi64(str);
    #else//linux
    return wcstoll(str, null, 10);
    #endif
}

template<typename _char_t>
inline int64_t cstoi64(const _char_t* str)
{
    return tvalue<_char_t>(atoi64, wtoi64)(str);
}

template<typename _char_t>
inline int32_t cstol(const _char_t* begin, _char_t** pend, int radix = 10)
{
    using namespace std;
    return tvalue<_char_t>(strtol, wcstol)(begin, pend, radix);
}

template<typename _char_t>
inline uint32_t cstoul(const _char_t* begin, _char_t** pend, int radix = 10)
{
    using namespace std;
    return tvalue<_char_t>(strtoul, wcstoul)(begin, pend, radix);
}

template<typename _char_t>
inline int64_t cstoll(const _char_t* begin, _char_t** pend, int radix = 10)
{
    using namespace std;
    #ifdef CGL_PLATFORM_CB6
    return cstoint<int64_t>(begin, pend, radix);
    #else
    return tvalue<_char_t>(strtoll, wcstoll)(begin, pend, radix);
    #endif
}

template<typename _char_t>
inline uint64_t cstoull(const _char_t* begin, _char_t** pend, int radix = 10)
{
    using namespace std;
    #ifdef CGL_PLATFORM_CB6
    return cstoint<int64_t>(begin, pend, radix);
    #else
    return tvalue<_char_t>(strtoull, wcstoull)(begin, pend, radix);
    #endif
}

//---------------------------------------------------------------------------
// cstof

/*
template<typename _char_t>
inline double cstof(const _char_t* str)
{
    return tvalue<_char_t>(STD::strtod, STD::wcstod)(str, null);
}

 template<>inline double cstof<char8_t>(const char8_t* str)
{
    return STD::strtod(reinterpret_cast<const char*>(str), null);
}
*/

template<typename _char_t>
inline long double cstold(const _char_t* str)
{
    #if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)
    return tvalue<_char_t>(STD::_strtold, STD::_wcstold)(str, null);
    #else
    return tvalue<_char_t>(STD::strtold, STD::wcstold)(str, null);
    #endif
}
//---------------------------------------------------------------------------
// cstobool

template<typename T>
const T* bool_name(bool value, bool numeric = false)
{
    const static T strTrue [] = {'t', 'r', 'u', 'e', '\0'};
    const static T strFalse[] = {'f', 'a', 'l', 's', 'e', '\0'};
    const static T numTrue [] = {'1', '\0'};
    const static T numFalse[] = {'0', '\0'};
    return value ? (numeric ? numTrue : strTrue) : (numeric ? numFalse : strFalse);
}

//default true == ["true" || !"0"]
template<typename _char_t>
bool cstobool(const _char_t* str, const _char_t * real = null)
{
    if(!str)return false;
    if(!real)real = bool_name<_char_t>(true, false);
    if(!csicmp(str, real, 6) || cstoi(str)){
        return true;
    }
    return false;
}

//default true = "true"
template<typename _char_t>
_char_t* booltostr(_char_t* buf, bool value, bool numeric = false)
{
    return cscpy<_char_t>(buf, bool_name<_char_t>(value, numeric), 6);
}
//---------------------------------------------------------------------------
// string_cast

template<typename T, typename _char_t>
T string_cast(const _char_t* str)
{
    return T(str);
}

template<>inline bool string_cast<bool, char>(const char* str)
{
    return cstobool<char>(str);
}

template<>inline bool string_cast<bool, wchar_t>(const wchar_t* str)
{
    return cstobool<wchar_t>(str);
}

template<>inline int string_cast<int, char>(const char* str)
{
    return cstoi(str);
}
template<>inline int string_cast<int, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline unsigned int string_cast<unsigned int, char>(const char* str)
{
    return cstoi(str);
}
template<>inline unsigned int string_cast<unsigned int, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline char string_cast<char, char>(const char* str)
{
    return cstoi(str);
}

template<>inline wchar_t string_cast<wchar_t, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline unsigned char string_cast<unsigned char, char>(const char* str)
{
    return cstoi(str);
}
template<>inline unsigned char string_cast<unsigned char, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline short string_cast<short, char>(const char* str)
{
    return cstoi(str);
}
template<>inline short string_cast<short, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline unsigned short string_cast<unsigned short, char>(const char* str)
{
    return cstoi(str);
}
template<>inline unsigned short string_cast<unsigned short, wchar_t>(const wchar_t* str)
{
    return cstoi(str);
}

template<>inline long string_cast<long, char>(const char* str)
{
    return cstol<char>(str, null, 10);
}
template<>inline long string_cast<long, wchar_t>(const wchar_t* str)
{
    return cstol<wchar_t>(str, null, 10);
}

template<>inline unsigned long string_cast<unsigned long, char>(const char* str)
{
    return cstoul<char>(str, null, 10);
}
template<>inline unsigned long string_cast<unsigned long, wchar_t>(const wchar_t* str)
{
    return cstoul<wchar_t>(str, null, 10);
}

template<>inline int64_t string_cast<int64_t, char>(const char* str)
{
    return cstoi64(str);
}
template<>inline int64_t string_cast<int64_t, wchar_t>(const wchar_t* str)
{
    return cstoi64(str);
}

template<>inline unsigned long long string_cast<unsigned long long, char>(const char* str)
{
    return cstoull<char>(str, null, 10);
}
template<>inline unsigned long long string_cast<unsigned long long, wchar_t>(const wchar_t* str)
{
    return cstoull<wchar_t>(str, null, 10);
}

template<>inline double string_cast<double, char>(const char* str)
{
    return cstof(str);
}
template<>inline double string_cast<double, wchar_t>(const wchar_t* str)
{
    return cstof(str);
}

template<>inline float string_cast<float, char>(const char* str)
{
    return (float)cstof(str);
}
template<>inline float string_cast<float, wchar_t>(const wchar_t* str)
{
    return (float)cstof(str);
}

template<>inline long double string_cast<long double, char>(const char* str)
{
    return cstof(str);
}
template<>inline long double string_cast<long double, wchar_t>(const wchar_t* str)
{
    return cstof(str);
}

//---------------------------------------------------------------------------
//to string

/*
template<typename string_type, typename T>
string_type tostr(const T& value)
{
    TYPENAME string_type::value_type buf[buffer_sizeof<T>::value];
    tostr(buf, sizeof(buf), value, 10);
    return string_type(buf);
}
*/

}//end namespace cgl

#endif //STDLIB_HPP_20180327221503
