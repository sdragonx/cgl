/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 is_equal.h

 2020-09-25 02:23:43

*/
#ifndef IS_EQUAL_H_20200925022343
#define IS_EQUAL_H_20200925022343

namespace cgl{

template<typename T>bool is_zero(const T& n);

#ifndef CGL_IS_EQUAL
#define CGL_IS_EQUAL

template<typename T>
inline bool is_equal(const T& a, const T& b)
{
    return a == b;
}

template<> inline bool is_equal<float>(const float& a, const float& b) { return is_zero(b - a); }
template<> inline bool is_equal<double>(const double& a, const double& b) { return is_zero(b - a); }
template<> inline bool is_equal<long double>(const long double& a, const long double& b) { return is_zero(b - a); }

//template<> inline bool is_equal<char*>(const long double& a, const long double& b) { return is_zero(b - a); }

template<typename T>
inline bool is_not_equal(const T& a, const T& b)
{
    return !is_equal<T>(a, b);
}

#endif

}//end namespace cgl

#endif //IS_EQUAL_H_20200925022343