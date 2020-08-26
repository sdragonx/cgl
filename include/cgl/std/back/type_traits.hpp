/*

 type_traits.hpp

 sdragonx 2018-07-26 13:52:05

*/
#ifndef STL_TYPE_TRAITS_H_20180726135205
#define STL_TYPE_TRAITS_H_20180726135205

#include <algorithm>

namespace cgl{
namespace stdex{

struct empty_t { };

template <class T, T n>
struct constant
{
	typedef constant<T, n> type;
	typedef T value_type;
	static const value_type value;
};

template <class T, T n>
const T constant<T, n>::value = n;

//---------------------------------------------------------------------------
//
// true_type false_type
//
//---------------------------------------------------------------------------

typedef constant<bool, true>	true_type;
typedef constant<bool, false>	false_type;

//---------------------------------------------------------------------------
//
// typesize<T>
//
//---------------------------------------------------------------------------

template<typename T>
struct typesize
{
	typedef T value_type;
	enum{
		value = sizeof(T)
	};
};

//---------------------------------------------------------------------------
//
// is_in_types<T, ...> 判断类型是否在列表里面，列表支持16个
//
//---------------------------------------------------------------------------

template<typename T,
	typename T0 = empty_t, typename T1 = empty_t, typename T2 = empty_t, typename T3 = empty_t,
	typename T4 = empty_t, typename T5 = empty_t, typename T6 = empty_t, typename T7 = empty_t,
	typename T8 = empty_t, typename T9 = empty_t, typename T10 = empty_t, typename T11 = empty_t,
	typename T12 = empty_t, typename T13 = empty_t, typename T14 = empty_t, typename T15 = empty_t>
struct is_in_types : public false_type { };

template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T0, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T1, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T2, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T3, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T4, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T5, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T6, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T7, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T8, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T9, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T10, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T11, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T12, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T13, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T14, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };
template<typename T0, typename T1, typename T2, typename T3,
	typename T4, typename T5, typename T6, typename T7,
	typename T8, typename T9, typename T10, typename T11,
	typename T12, typename T13, typename T14, typename T15>
struct is_in_types<T15, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
	 : public true_type { };

//---------------------------------------------------------------------------
//
// is_same<T> 判断类型是否相同
//
//---------------------------------------------------------------------------

template <class _Tp, class _Up> struct is_same	: public false_type {};
template <class _Tp> struct is_same<_Tp, _Tp>	: public true_type {};

//---------------------------------------------------------------------------
//
// is_ctype<T> 判断类型是否c++原生类型
//
//---------------------------------------------------------------------------

template<class T>
struct is_ctype : public is_in_types<T,
	char, unsigned char,
	short, unsigned short,
	int, unsigned int,
	long, unsigned long,
	long long, unsigned long long,
	float, double, long double
	#ifdef CGL_NATIVE_UCHAR_SUPPERT
	, char16_t, char32_t
	#endif
	>
{ };

//---------------------------------------------------------------------------
//
// signed_type<T>
//
//---------------------------------------------------------------------------

template<typename T>struct signed_type			{ typedef T type; };

template<>struct signed_type<unsigned char>		{ typedef char type; };
template<>struct signed_type<unsigned short>	{ typedef short type; };
template<>struct signed_type<unsigned int>		{ typedef int type; };
template<>struct signed_type<unsigned long>		{ typedef long type; };
template<>struct signed_type<unsigned long long>{ typedef long long type; };

//---------------------------------------------------------------------------
//
// unsigned_type<T>
//
//---------------------------------------------------------------------------

template<typename T>struct unsigned_type	{ typedef T type; };

template<>struct unsigned_type<char>		{ typedef unsigned char		 type; };
template<>struct unsigned_type<short>		{ typedef unsigned short	 type; };
template<>struct unsigned_type<int>			{ typedef unsigned int		 type; };
template<>struct unsigned_type<long>		{ typedef unsigned long		 type; };
template<>struct unsigned_type<long long>	{ typedef unsigned long long type; };

//---------------------------------------------------------------------------
//
// dual_type<T> 包含符号和无符号两种类型
//
//---------------------------------------------------------------------------

template<typename T>
struct dual_type
{
	typedef TYPENAME signed_type<T>::type type;
	typedef TYPENAME unsigned_type<T>::type unsigned_type;
};

//---------------------------------------------------------------------------
//
// is_signed<T>() 判断类型是否有符号
// is_signed<T>(const T&) 判断变量是否有符号
// wchar_t char16_t char32_t is unsigned type
//
//---------------------------------------------------------------------------

template<typename T>
bool is_signed()
{
	return is_in_types<T, char, short, int, long, long long, float, double, long double>::value;
}

template<typename T>
bool is_signed(const T& )
{
	return is_signed<T>();
}

//---------------------------------------------------------------------------
//
// is_float<T> 判断类型是否浮点数
//
//---------------------------------------------------------------------------

template<typename T> struct is_float : public is_in_types<T, float, double, long double> { };

//---------------------------------------------------------------------------
//
// is_reference<T> 判断类型是否引用
//
//---------------------------------------------------------------------------

template <typename T> struct is_reference : public false_type { };
template <typename T> struct is_reference<T&> : public true_type { };

template <typename T> struct is_reference<const T&> :  public true_type { };


template <typename T> struct is_const : public false_type { };

template <typename T> struct is_const<const T> : public true_type { };

template <typename T> struct is_const<const T*> : public true_type { };

template <typename T> struct is_const<const T&> : public true_type { };

template <typename T> struct is_const<const T* &> : public true_type { };


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

//---------------------------------------------------------------------------

template<typename T>
struct type_traits_base
{
	typedef T type;
	type value;
	//inline static bool is_ctype(){ return is_ctype<T>::value; }
	inline static type max() { return std::numeric_limits<T>::max(); }
	inline static type min() { return std::numeric_limits<T>::min(); }
	inline static bool is_signed() { return false; }
};

template<typename T>
class type_traits : public type_traits_base<T>
{
public:
	typedef T type;
public:
	inline static void fill(T* p, size_t size, const T& value)
	{
		std::fill(p, p+size, value);
	}

	inline static void copy(T* dst, const T* src, size_t size)
	{
		if(is_ctype<T>::value){
			memcpy(dst, src, size * sizeof(type));
		}
		else{
			std::copy(dst, src, size);
		}
	}

	inline static void move(T* dst, const T* src, size_t size)
	{
		if(is_ctype<T>::value){
			memmove(dst, src, size * sizeof(type));
		}
		else{
			std::copy(dst, src, size);
		}
		std::copy(dst, src, size);
	}
};

}//end namespace stdex
}//end namespace cgl

#endif //STL_TYPE_TRAITS_H_20180726135205

/*

void make_function()
{
	int n = 16;
	String s;
	String tml;
	String Tx;
	const char* name = "is_in_types";

	mmlog.clear();

	//T0, T1, T2, ...
	for(int i=0; i<n; ++i){
		s = "T" + IntToStr(i);
		if(i != n - 1)s += ", ";
		Tx += s;
	}

	//typename T0 = empty_t, ...
	for(int i=0; i<n; ++i){
		s = "typename T" + IntToStr(i) + " = empty_t";
		if(i != n - 1){
			s += ", ";
			if(i > 0 && (i + 1) % 4 == 0)s += "\r\n\t";
		}
		tml += s;
	}

	mmlog<<"template<typename T, " << tml << ">" << endline;
	mmlog<<"struct " << name << " : public false_type { };" << endline;

	tml = String();
	for(int i=0; i<n; ++i){
		s = "typename T" + IntToStr(i);
		if(i != n - 1){
			s += ", ";
			if(i > 0 && (i + 1) % 4 == 0)s += "\r\n\t";
		}
		tml += s;
	}

	for(int i=0; i<n; ++i){
		s = "T" + IntToStr(i);
		mmlog<<"template<" << tml << ">" << endline;
		mmlog<<"struct " << name << "<" << s << ", " << Tx << ">\r\n\t : public true_type { };" << endline;
	}
}
*/
