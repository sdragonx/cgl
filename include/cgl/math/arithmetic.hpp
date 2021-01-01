/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 arithmetic.hpp

 2020-02-22 14:06:22

 ���ϵ�����

*/
#ifndef ARITHMETIC_HPP_20200222140622
#define ARITHMETIC_HPP_20200222140622

#include <cgl/public.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric>
#include <vector>

namespace cgl{
namespace math{

//
// ��������
//

template<typename T, typename iterator>
void add(iterator begin, iterator end, const T& value)
{
    for( ; begin != end; ++begin){
        (*begin) += value;
    }
}

template<typename T, typename iterator>
void sub(iterator begin, iterator end, const T& value)
{
    for( ; begin != end; ++begin){
        (*begin) -= value;
    }
}

template<typename T, typename iterator>
void mul(iterator begin, iterator end, const T& value)
{
    for( ; begin != end; ++begin){
        (*begin) *= value;
    }
}

template<typename T, typename iterator>
void div(iterator begin, iterator end, const T& value)
{
    for( ; begin != end; ++begin){
        (*begin) /= value;
    }
}

template<typename T, typename container>
void add(container& ls, const T& value)
{
    add(ls.begin(), ls.end(), value);
}

template<typename T, typename container>
void sub(container& ls, const T& value)
{
    sub(ls.begin(), ls.end(), value);
}

template<typename T, typename container>
void mul(container& ls, const T& value)
{
    mul(ls.begin(), ls.end(), value);
}

template<typename T, typename container>
void div(container& ls, const T& value)
{
    div(ls.begin(), ls.end(), value);
}

//�����
template<typename T, typename iterator>
T total(iterator begin, iterator end)
{
    T n = T();
    for(; begin != end; ++begin){
        n += *begin;
    }
    return n;
}

//����һά�����
template<typename T>
T total(const T* array, size_t size)
{
    T n = T();
    for(size_t i=0; i<size; ++i){
        n += array[i];
    }
    return n;
}

//�����ά�����
template<typename T, size_t CX, size_t CY>
T total(T mat[CY][CX])
{
    T n = T();
    for(size_t y=0; y < CY; ++y)
        for(size_t x=0; x < CX; ++x)
            n += mat[y][x];

    return n;
}

//ƽ��ֵ
template<typename T, typename iterator>
T mean(iterator begin, iterator end)
{
    T n = total(begin, end);
    return n / T(end - begin);
}

template<typename T>
T mean(const T* array, size_t size)
{
    T n = total(array, size);
    return n / T(size);
}

//�����е���Сֵ
template<typename T, typename iterator>
T min_element(iterator begin, iterator end)
{
    return *std::min_element(begin, end);
}

//�����е����ֵ
template<typename T, typename iterator>
T max_element(iterator begin, iterator end)
{
    return *std::max_element(begin, end);
}

//
// ͼ����
//

//ֱ��ͼ
template<typename T, typename iterator>
void histogram(std::map<T, int>& data, iterator begin, iterator end)
{
    for( ; begin != end; ++begin){
        ++data[*begin];
    }
}

//���ݷ��ʽ���㷽��
//data  ֱ��ͼ
//mean  ƽ��ֵ
double variance(std::map<int, int>& data, double mean)
{
    std::map<int, int>::iterator itr;
    double total = 0.0;
    int count = 0;
    double n;
    for(itr = data.begin(); itr != data.end(); ++itr){
        n = (itr->first - mean);
        n *= n;
        n *= itr->second;
        total += n;
        count += itr->second;   //�������
    }

    if(count < 1){
        return 0.0;
    }

    return sqrt(total / (count - 1));
}

}//end namespace math
}//end namespace cgl

#endif //ARITHMETIC_HPP_20200222140622