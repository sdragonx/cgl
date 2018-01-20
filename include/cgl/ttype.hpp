/*

 ttype.hpp

 sdragonx 2015-02-18 18:32:43

 �������ģ�溯����Ϊ�˴���ansi��unicode��ʹ�ַ���ֵ�����ַ��������ܹ���ģ����ʹ��

*/
#ifndef TTYPE_HPP_20150218183243
#define TTYPE_HPP_20150218183243

#include <tchar.h>

namespace cgl{

//�����ַ����ͣ��Զ�ѡ��TA���ͻ���TW����
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

//�����ַ����ͣ��Զ�ѡ��TAֵ����TWֵ
//char* str = tvalue<char>("abc", L"def");
//�����ַ����ͣ��Զ�ѡ��ansi������������unicode������
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
