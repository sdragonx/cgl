/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fast_cast.hpp

 2018-10-31 11:17:42

 64ŒªŒ¥ µœ÷

*/
#ifndef FAST_CAST_HPP_20181031111742
#define FAST_CAST_HPP_20181031111742

#include <cgl/base/float.h>

namespace cgl{

template<typename Tx, typename Ty>
inline Tx fast_cast(const Ty& value)
{
    return Tx(value);
}
//---------------------------------------------------------------------------
// float to int

template<>inline
int fast_cast<int>(const float& value)
{
    return round(value);
}

template<>inline
int fast_cast<int>(const double& value)
{
    return round(value);
}

template<>inline
int fast_cast<int>(const long double& value)
{
    return round(value);
}

template<>inline unsigned int       fast_cast<unsigned int>      (const float& value){ return fast_cast<int>(value); }
template<>inline char               fast_cast<char>              (const float& value){ return fast_cast<int>(value); }
template<>inline wchar_t            fast_cast<wchar_t>           (const float& value){ return fast_cast<int>(value); }
template<>inline unsigned char      fast_cast<unsigned char>     (const float& value){ return fast_cast<int>(value); }
template<>inline short              fast_cast<short>             (const float& value){ return fast_cast<int>(value); }
template<>inline unsigned short     fast_cast<unsigned short>    (const float& value){ return fast_cast<int>(value); }
template<>inline long               fast_cast<long>              (const float& value){ return fast_cast<int>(value); }
template<>inline unsigned long      fast_cast<unsigned long>     (const float& value){ return fast_cast<int>(value); }
template<>inline long long          fast_cast<long long>         (const float& value){ return fast_cast<int>(value); }
template<>inline unsigned long long fast_cast<unsigned long long>(const float& value){ return fast_cast<int>(value); }

template<>inline unsigned int       fast_cast<unsigned int>      (const double& value){ return fast_cast<int>(value); }
template<>inline char               fast_cast<char>              (const double& value){ return fast_cast<int>(value); }
template<>inline wchar_t            fast_cast<wchar_t>           (const double& value){ return fast_cast<int>(value); }
template<>inline unsigned char      fast_cast<unsigned char>     (const double& value){ return fast_cast<int>(value); }
template<>inline short              fast_cast<short>             (const double& value){ return fast_cast<int>(value); }
template<>inline unsigned short     fast_cast<unsigned short>    (const double& value){ return fast_cast<int>(value); }
template<>inline long               fast_cast<long>              (const double& value){ return fast_cast<int>(value); }
template<>inline unsigned long      fast_cast<unsigned long>     (const double& value){ return fast_cast<int>(value); }
//template<>inline long long          fast_cast<long long>         (const double& value){ return fast_cast<int>(value); }
//template<>inline unsigned long long fast_cast<unsigned long long>(const double& value){ return fast_cast<int>(value); }

template<>inline unsigned int       fast_cast<unsigned int>      (const long double& value){ return fast_cast<int>(value); }
template<>inline char               fast_cast<char>              (const long double& value){ return fast_cast<int>(value); }
template<>inline wchar_t            fast_cast<wchar_t>           (const long double& value){ return fast_cast<int>(value); }
template<>inline unsigned char      fast_cast<unsigned char>     (const long double& value){ return fast_cast<int>(value); }
template<>inline short              fast_cast<short>             (const long double& value){ return fast_cast<int>(value); }
template<>inline unsigned short     fast_cast<unsigned short>    (const long double& value){ return fast_cast<int>(value); }
template<>inline long               fast_cast<long>              (const long double& value){ return fast_cast<int>(value); }
template<>inline unsigned long      fast_cast<unsigned long>     (const long double& value){ return fast_cast<int>(value); }
//template<>inline long long          fast_cast<long long>         (const long double& value){ return fast_cast<int>(value); }
//template<>inline unsigned long long fast_cast<unsigned long long>(const long double& value){ return fast_cast<int>(value); }

}//end namespace cgl

#endif //FAST_CAST_HPP_20181031111742

/*

template<typename T>
inline T fast_cast(const char& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const wchar_t& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const unsigned char& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const short& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const unsigned short& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const int& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const unsigned int& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const long& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const unsigned long& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const long long& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const unsigned long long& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const float& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const double& value)
{
    return T(value);
}

template<typename T>
inline T fast_cast(const long double& value)
{
    return T(value);
}

template<typename Tx>
inline Tx fast_cast(const char* value)
{
    return Tx(value);
}

template<typename Tx>
inline Tx fast_cast(const wchar_t* value)
{
    return Tx(value);
}

template<typename Tx>
inline Tx fast_cast(const char8_t* value)
{
    return Tx(value);
}

template<typename Tx>
inline Tx fast_cast(const char16_t* value)
{
    return Tx(value);
}

template<typename Tx>
inline Tx fast_cast(const char32_t* value)
{
    return Tx(value);
}
*/
