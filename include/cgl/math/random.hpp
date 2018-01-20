/*

 random.hpp

 sdragonx 2015-07-28 13:53:18

*/
#ifndef RANDOM_HPP_20150728135318
#define RANDOM_HPP_20150728135318

#include <stdlib.h>
#include <time.h>

namespace cgl{

//const int RANDOM_MAX = RAND_MAX;

//返回 0.0~1.0 之间的数值
float rand_real()
{
	return std::rand() / float(RAND_MAX);
}

//会返回负数
template<typename T>
inline T random(T n)
{
    return n < 0 ? -std::rand()%n : std::rand()%n;
}

template<>
inline float random<float>(float n)
{
    return std::rand() / float(RAND_MAX) * n;
}

template<>
inline double random<double>(double n)
{
    return std::rand() / double(RAND_MAX) * n;
}

template<>
inline long double random(long double n)
{
    return std::rand() / (long double)(RAND_MAX) * n;
}

template<typename T>
inline T random_real(T min, T max) {
	T n = rand_real();
	return min + (max-min)*n;
}

template<typename T>
inline T random(T min, T max) {
	return min + random(max-min);
}

template<>
inline float random(float min, float max) {
	return random_real(min, max);
}

template<>
inline double random(double min, double max) {
	return random_real(min, max);
}

template<>
inline long double random(long double min, long double max) {
    return random_real(min, max);
}

}// end namespace cgl

#endif //RANDOM_HPP_20150728135318
