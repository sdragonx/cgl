/*

 ttype.hpp

 sdragonx 2015-02-18 18:32:43

 这个几个模版函数是为了处理ansi或unicode，使字符串值或者字符串函数能够在模版中使用

*/
#ifndef TTYPE_HPP_20150218183243
#define TTYPE_HPP_20150218183243

#include <tchar.h>

namespace cgl{

//根据字符类型，自动选择TA类型或者TW类型
template<typename _char_t, typename TA, typename TW>
struct ttype;

template<typename TA, typename TW>
struct ttype<char, TA, TW>
{
	typedef TA type;
};

template<typename TA, typename TW>
struct ttype<wchar_t, TA, TW>
{
	typedef TW type;
};

//根据字符类型，自动选择TA值或者TW值
//char* str = tvalue<char>("abc", L"def");
//根据字符类型，自动选择ansi处理函数，或者unicode处理函数
//size_t n = tvalue<char>(strlen, wcslen)("abc");

template<typename TA,typename TW>
inline typename ttype<char, TA, TW>::type tvalue(char*, TA a, TW w)
{
    return a;
}

template<typename TA,typename TW>
inline typename ttype<wchar_t, TA, TW>::type tvalue(wchar_t*, TA a, TW w)
{
    return w;
}

template<typename _char_t, typename TA,typename TW>
inline typename ttype<_char_t, TA, TW>::type tvalue(TA a, TW w)
{
    return tvalue<TA, TW>(static_cast<_char_t*>(0), a, w);
}

#define tstring(T, S) tvalue<T>(S, L##S)

} //end namespace cgl

#endif //TTYPE_HPP_20150218183243
