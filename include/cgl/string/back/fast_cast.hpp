/*

 fast_cast.hpp

 sdragonx 2006-11-23 20:33:31

 14:33 2015/2/23

 ���������и��õ�ʵ�ַ�ʽ����������ĵ��÷����ǣ�
 T1 a = fast_cast<T1>(T2)
 һ���������Ǳ�׼����

 ʹ��ʾ����
 int a = fast_cast<int>("123");     //ʮ����
 int b = fast_cast<int>("FF8", 16); //ʮ������

 string str = fast_cast<string>(123);     //ʮ����
 string str = fast_cast<string>(123, 16); //ʮ������

 string str = fast_cast<string>(123.456);       //Ĭ�ϸ�ʽ
 string str = fast_cast<string>(123.456, 4, 6); //����4λ������6λС����������0

 stl��strstream�����������sprintf��sscanf��sscanf�����ַ���ת��ֵ��ʱ��
 ��������ͷƤ���飬strstream��Ч��Ҳ�ò�����ȥ�����atoiһ��ĺ�����sscanf
 Ч��Ҫ�ߣ�������˵linux��֧�֣���˵�ɣ�crt��Դ�����еģ���ĵ��˿���linux��
 ʱ�򣬸���һ�ݾ����ˣ�������������Ƕ��С�
*/
#ifndef FAST_CAST_HPP_20061123203331
#define FAST_CAST_HPP_20061123203331

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cgl/public.h>
#include <cgl/static_array.hpp>
#include <cgl/string/code_convert.h>
#include <cgl/stdlib.hpp>
//#include "to_string.h"

namespace cgl{

//�ַ���ת��ΪT�����������⣬��������Ϊ char* ���� wchar_t*��ʱ��
//����ֿ����ᷢ��char short int ��ͻ�����
template<typename T>
class fast_stream
{
public:
	T operator()(const char*    str) { return cstoi(str); }
	T operator()(const wchar_t* str)           { return _wtoi(str); }
	T operator()(const char*    str, int radix){ return strtoul(str, NULL, radix); }
	T operator()(const wchar_t* str, int radix){ return wcstoul(str, NULL, radix); }
};


//�����char* wchar_*����
template<>
class fast_stream<char*>
{
public:
	template<typename T>
    char* operator()(T value)
    {
    	return static_cast<char*>(value);
    }
};

template<>
class fast_stream<const char*>
{
public:
	template<typename T>
    const char* operator()(T value)
    {
    	return static_cast<char*>(value);
    }
};

template<>
class fast_stream<wchar_t*>
{
public:
	template<typename T>
    char* operator()(T value)
    {
    	return static_cast<char*>(value);
    }
};

template<>
class fast_stream<const wchar_t*>
{
public:
	template<typename T>
    const char* operator()(T value)
    {
    	return static_cast<char*>(value);
    }
};

//*

template<>long             fast_stream<long>::operator()            (const char*    str){ return string_cast<long>(str);     }
template<>long             fast_stream<long>::operator()            (const wchar_t* str){ return string_cast<long>(str);     }
template<>unsigned long    fast_stream<unsigned long>::operator()   (const char*    str){ return string_cast<long>(str);     }
template<>unsigned long    fast_stream<unsigned long>::operator()   (const wchar_t* str){ return string_cast<long>(str);     }
template<>__int64          fast_stream<__int64>::operator()         (const char*    str){ return string_cast<int64_t>(str);  }
template<>__int64          fast_stream<__int64>::operator()         (const wchar_t* str){ return string_cast<int64_t>(str);  }
template<>unsigned __int64 fast_stream<unsigned __int64>::operator()(const char*    str){ return string_cast<uint64_t>(str); }
template<>unsigned __int64 fast_stream<unsigned __int64>::operator()(const wchar_t* str){ return string_cast<uint64_t>(str); }

template<>
class fast_stream<float>
{
public:
	float operator()(const char*    str){ return string_cast<float>(str); }
	float operator()(const wchar_t* str){ return string_cast<float>(str); }
};

template<>
class fast_stream<double>
{
public:
	double operator()(const char*    str){ return string_cast<double>(str); }
	double operator()(const wchar_t* str){ return string_cast<double>(str); }
};

template<>
class fast_stream<long double>
{
public:
	long double operator()(const char*    str){ return string_cast<long double>(str); }
	long double operator()(const wchar_t* str){ return string_cast<long double>(str); }
};
//*/

//����ת��Ϊ�ַ���
template<class string_type>
class tostring_opset
{
public:
    typedef string_type value_type;
	typedef typename value_type::value_type char_type;
	typedef typename value_type::value_type char_type1;
	typedef typename ttype<char_type, wchar_t, char>::type char_type2;
protected:
	typedef static_array<char_type, 32> buffer;
public:
	value_type operator()(int n, int radix = 10)
	{
		return value_type(tvalue<char_type>(itoa, _itow)(n, buffer().data(), radix), buffer::size());
	}
	value_type operator()(unsigned int n, int radix = 10)
    {
        return value_type(tvalue<char_type>(ultoa, _ultow)(n, buffer().data(), radix), buffer::size());
	}
	value_type operator()(long n, int radix = 10)
    {
        return value_type(tvalue<char_type>(ltoa, _ltow)(n, buffer().data(), radix), buffer::size());
    }
	value_type operator()(unsigned long n, int radix = 10)
    {
        return value_type(tvalue<char_type>(ultoa, _ultow)(n, buffer().data(), radix), buffer::size());
    }
	value_type operator()(__int64 n, int radix = 10)
    {
        return value_type(tvalue<char_type>(_i64toa, _i64tow)(n, buffer().data(), radix), buffer::size());
    }
	value_type operator()(unsigned __int64 n, int radix = 10)
    {
		return value_type(tvalue<char_type>(_ui64toa, _ui64tow)(n, buffer().data(), radix), buffer::size());
    }
	value_type operator()(float       n){ return from_float(n); }
	value_type operator()(double      n){ return from_float(n); }
	value_type operator()(long double n){ return from_float(n); }
	value_type operator()(float       n, int int_length, int dec_length){ return from_float(n, int_length, dec_length); }
	value_type operator()(double      n, int int_length, int dec_length){ return from_float(n, int_length, dec_length); }
	value_type operator()(long double n, int int_length, int dec_length){ return from_float(n, int_length, dec_length); }
	value_type operator()(const char*    str){return from_string(str);}
	value_type operator()(const char*    str, int length){ return from_string(str, length); }
	value_type operator()(const wchar_t* str){return from_string(str); }
	value_type operator()(const wchar_t* str, int length){ return from_string(str, length); }
	value_type operator()(const value_type&  str)            { return operator()(str.c_str(), str.length()); }
	value_type operator()(const value_type&  str, int length){ return operator()(str.c_str(), length); }
	#ifdef INC_VCL
	value_type operator()(const String& str){ return operator()(str.c_str(), str.Length());}
	#endif
private:
	value_type from_string(const char_type1* str, int length = -1)
	{
		if(length == -1)length = cslen(str);
		return value_type(str, static_cast<size_t>(length));
    }
	value_type from_string(const char_type2* str, int length = -1)
	{
		std::vector<char_type> buf;
		int n = 0;
		n = tvalue<char_type1>(code_convert<char, wchar_t>, code_convert<wchar_t, char>)(null, 0, str, length);
		buf.reserve(n);
		n = tvalue<char_type1>(code_convert<char, wchar_t>, code_convert<wchar_t, char>)(&buf[0], n, str, length);
		return value_type(&buf[0], static_cast<size_t>(n));
	}
	value_type from_float(double n, int dec = 0, int precision = 0)
	{
		static_array<char_type, 64> buf;
		int length = dec + precision;
		if(!length)
		{
			format(buf.data(), buf.size(), T_TEXT(char_type, "%.16G"), n);
		}
		else
		{
			format(buf.data(), buf.size(), T_TEXT(char_type, "%0*.*f"), dec, precision, n);
		}
		return value_type(buf.begin(), buf.end());
	}
};

//main function
template<typename result_t, typename arg_t>
result_t fast_cast(arg_t a)
{
    static fast_stream<result_t> fs;
	return fs(a);
}

template<typename result_t, typename arg_t>
result_t fast_cast(arg_t a, int b)
{
    static fast_stream<result_t> fs;
    return fs(a, b);
}

template<typename result_t, typename arg_t>
result_t fast_cast(arg_t a, int b, int c)
{
	static fast_stream<result_t> fs;
	return fs(a, b, c);
}

/*
template<>inline int fast_cast<int, float>(const float& value)
{
	return f_toint(value);
}

template<>inline unsigned int fast_cast<unsigned int, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline char fast_cast<char, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline unsigned char fast_cast<unsigned char, float>(const float& value){	return fast_cast<int, float>(value); }
template<>inline short fast_cast<short, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline unsigned short fast_cast<unsigned short, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline long fast_cast<long, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline unsigned long fast_cast<unsigned long, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline long long fast_cast<long long, float>(const float& value){ return fast_cast<int, float>(value); }
template<>inline unsigned long long fast_cast<unsigned long long, float>(const float& value){ return fast_cast<int, float>(value); }
*/

//�ַ�����չ
#define FAST_CAST_STRING_EXTEND(str_t)\
template<>class fast_stream<str_t>:public tostring_opset<string_type>{ /*void*/ };\
template<typename T>T fast_cast(const string_type& value){ return fast_cast<T>(value.c_str()); }\
template<typename T>T fast_cast(const string_type& value, int a){ return fast_cast<T>(value.c_str(), a); }\
template<typename T>T fast_cast(const string_type& value, int a, int b){ return fast_cast<T>(value.c_str(), a, b); }

#ifdef _STRING_
FAST_CAST_STRING_EXTEND(std::string)
FAST_CAST_STRING_EXTEND(std::wstring)
#endif

#ifdef INC_VCL
template<typename T>
T fast_cast(String str)
{
	return fast_cast<T>(str.c_str(), str.Length());
}
#endif

}//end namespace cgl

#endif //FAST_CAST_HPP_20061123203331
