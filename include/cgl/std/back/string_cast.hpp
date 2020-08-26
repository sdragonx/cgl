/*

 string_cast.h

 sdragonx 2018-07-26 23:55:45

*/
#ifndef STRING_CAST_H_20180726235545
#define STRING_CAST_H_20180726235545

#include <cstdlib>
#include <cgl/std/ttype.hpp>

namespace cgl{

//根据linux内核函数修改
template<typename singed_int, typename char_type>
singed_int cstoint(const char_type* begin, char_type** pend, int radix)
{
	singed_int n = 0;
	if(!begin){
		return n;
	}
    int digit = 0;
    if(radix<2 && radix>36)return 0;
    while(isspace(static_cast<int>(*begin)))++begin;
    char_type sign = *begin;//save sign indication
	if (sign == '-' || sign == '+')++begin;
    while(*begin == '0')++begin;
    if((radix == 16) && ((*begin)|0x20 == 'x'))++begin;

    for( ; *begin; ++begin)
    {
        if (*begin >= '0' && *begin <= '9')
            digit = *begin - '0';
        else if (*begin >= 'A' && *begin < 'A' + radix - 10)
            digit = *begin - 'A' + 10;
        else if (*begin >= 'a' && *begin < 'a' + radix - 10)
            digit = *begin - 'a' + 10;
        else
			break;
        n *= radix;
        n += digit;
    }
	//out:
	if (pend)
		*pend = (char_type*)begin;
    return sign == '-' ? -n : n;
}

template<typename _char_t>
int32_t cstoi32(const _char_t* begin, int radix = 10)
{
	//return cstoint<int32_t>(begin, null, radix);
	return tvalue<_char_t>(strtoul, wcstoul)(begin, null, radix);
}

template<typename _char_t>
int64_t cstoi64(const _char_t* begin, int radix = 10)
{
	return cstoint<int64_t, _char_t>(begin, null, radix);
}

template<typename _char_t>
long cstol(const _char_t str)
{
    return tvalue<_char_t>(atoi, wcstol)(str);
}

template<typename _char_t>
double cstodec(const _char_t* str)
{
	return tvalue<_char_t>(strtod, wcstod)(str, null);
}

template<>double cstodec<char8_t>(const char8_t* str)
{
	return std::strtod(reinterpret_cast<const char*>(str), null);
}
/*
template<>double cstodec<char16_t>(const char8_t* str)
{
	return strtod(str, null);
}
template<>double cstodec<char32_t>(const char8_t* str) { return strtod(str, null); }
*/


//default true == ["true" || !"0"]
template<typename _char_t>
bool cstobool(const _char_t* str, const _char_t * true_value = null)
{
	using namespace std;
	const static _char_t text[5] = {'t', 'r', 'u', 'e'};
	if(!true_value)true_value = text;
	if(str && (!csicmp(str, true_value, 5) || tvalue<_char_t>(atoi, _wtoi)(str)) )
	{
		return true;
	}
	return false;
}

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
	return std::atoi(str);
}
template<>inline int string_cast<int, wchar_t>(const wchar_t* str)
{
	return std::_wtoi(str);
}

template<>inline unsigned int string_cast<unsigned int, char>(const char* str)
{
	return std::atoi(str);
}
template<>inline unsigned int string_cast<unsigned int, wchar_t>(const wchar_t* str)
{
	return std::_wtoi(str);
}

template<>inline char string_cast<char, char>(const char* str)
{
	return string_cast<int, char>(str);
}
template<>inline wchar_t string_cast<wchar_t, wchar_t>(const wchar_t* str)
{
	return string_cast<int, wchar_t>(str);
}

template<>inline unsigned char string_cast<unsigned char, char>(const char* str)
{
	return string_cast<unsigned int, char>(str);
}
template<>inline unsigned char string_cast<unsigned char, wchar_t>(const wchar_t* str)
{
	return string_cast<unsigned int, wchar_t>(str);
}

template<>inline short string_cast<short, char>(const char* str)
{
	return string_cast<int, char>(str);
}
template<>inline short string_cast<short, wchar_t>(const wchar_t* str)
{
	return string_cast<int, wchar_t>(str);
}

template<>inline unsigned short string_cast<unsigned short, char>(const char* str)
{
	return string_cast<unsigned int, char>(str);
}
template<>inline unsigned short string_cast<unsigned short, wchar_t>(const wchar_t* str)
{
	return string_cast<unsigned int, wchar_t>(str);
}

template<>inline long string_cast<long, char>(const char* str)
{
	return atol(str);
}
template<>inline long string_cast<long, wchar_t>(const wchar_t* str)
{
	return _wtol(str);
}

template<>inline unsigned long string_cast<unsigned long, char>(const char* str)
{
	return atol(str);
}
template<>inline unsigned long string_cast<unsigned long, wchar_t>(const wchar_t* str)
{
	return _wtol(str);
}

template<>inline __int64 string_cast<__int64, char>(const char* str)
{
	//return strtoll(str);// _atoi64(str);
}
template<>inline __int64 string_cast<__int64, wchar_t>(const wchar_t* str)
{
	return _wtoi64(str);
}

template<>inline unsigned long long string_cast<unsigned long long, char>(const char* str)
{
	return _atoi64(str);
}
template<>inline unsigned long long string_cast<unsigned long long, wchar_t>(const wchar_t* str)
{
	return _wtoi64(str);
}

template<>inline double string_cast<double, char>(const char* str)
{
	return atof(str);
}
template<>inline double string_cast<double, wchar_t>(const wchar_t* str)
{
	return _wtof(str);
}

template<>inline float string_cast<float, char>(const char* str)
{
	return string_cast<double, char>(str);
}
template<>inline float string_cast<float, wchar_t>(const wchar_t* str)
{
	return string_cast<double, wchar_t>(str);
}

template<>inline long double string_cast<long double, char>(const char* str)
{
	return string_cast<double, char>(str);
}
template<>inline long double string_cast<long double, wchar_t>(const wchar_t* str)
{
	return string_cast<double, wchar_t>(str);
}

}//end namespace cgl

#endif //STRING_CAST_H_20180726235545