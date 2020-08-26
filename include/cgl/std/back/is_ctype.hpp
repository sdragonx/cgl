/*

 is_ctype.hpp

 sdragonx 2018-07-26 13:31:40

*/
#ifndef IS_CTYPE_HPP_20180726133140
#define IS_CTYPE_HPP_20180726133140

namespace cgl{

template<typename T>
bool is_ctype()
{
	return false; 
}

template<typename T>
bool is_ctype(const T&)
{
	return is_ctype<T>();
}

template<>bool is_ctype<char>				(){ return true; }
template<>bool is_ctype<unsigned char>		(){ return true; }
template<>bool is_ctype<short>				(){ return true; }
template<>bool is_ctype<unsigned short>		(){ return true; }
template<>bool is_ctype<int>				(){ return true; }
template<>bool is_ctype<unsigned int>		(){ return true; }
template<>bool is_ctype<long>				(){ return true; }
template<>bool is_ctype<unsigned long>		(){ return true; }
template<>bool is_ctype<long long>			(){ return true; }
template<>bool is_ctype<unsigned long long>	(){ return true; }
template<>bool is_ctype<float>				(){ return true; }
template<>bool is_ctype<double>				(){ return true; }
template<>bool is_ctype<long double>		(){ return true; }

}//end namespace cgl

#endif IS_CTYPE_HPP_20180726133140
