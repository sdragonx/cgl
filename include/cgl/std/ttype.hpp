/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ttype.h

 2015-02-18 18:32:43
 2018-07-26 23:20

 �������ģ�溯����Ϊ�˴���ansi��unicode��ʹ�ַ���ֵ�����ַ��������ܹ���ģ����ʹ��

 ttype<char, char, wchar_t>::type        //type is char
 ttype<wchar_t, char, wchar_t>::type    //type is wchar_t

 char* str = tvalue<char>("ansi", "wide");            //return "ansi"
 wchar_t* str = tvalue<wchar_t>("ansi", "wide");    //return "wide"

 template<typename T>
 size_t cslen(const T* str)//�������������ģ���̻᷽��ܶ�
 {
    return tvalue<TCHAR>(strlen, wcslen)("abc");
 }

 TCHAR* s = T_TEXT(TCHAR, "abc");    //if TCHAR is char, s is "abc"; else s is L"abc"

*/
#ifndef STL_TTYPE_H_20150218183243
#define STL_TTYPE_H_20150218183243

namespace cgl{

//
// ttype
//

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

//
// tvalue
//

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

}//end namespace cgl

#define T_TEXT(T, S) cgl::tvalue<T>(S, L##S)

#endif//STL_TTYPE_H_20150218183243
