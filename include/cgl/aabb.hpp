/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 aabb.hpp

 2020-03-30 03:35:56

 bounding box

*/
#ifndef MATH_AABB_HPP_20200330033556
#define MATH_AABB_HPP_20200330033556

#include <cgl/public.h>
#include <cgl/math.hpp>
#include <cgl/vec.hpp>

#include <limits>

namespace cgl{

template<typename T>
class aabb
{
public:
    typedef aabb this_type;
    typedef T value_type;

protected:
    value_type m_min;
    value_type m_max;

public:
    aabb()
    {
        this->reset();
    }

    value_type min()const
    {
        return m_min;
    }

    value_type max()const
    {
        return m_max;
    }

    void reset()
    {
        m_min = std::numeric_limits<value_type>::max();
        m_max = std::numeric_limits<value_type>::min();
    }

    void append(value_type value)
    {
        if(value < m_min) m_min = value;
        if(value > m_max) m_max = value;
    }

    value_type size()const
    {
        return m_max - m_min;
    }

    void expand(value_type value)
    {
        m_min -= value;
        m_max += value;
    }
};


template<typename T>
class aabb2
{
public:
    typedef aabb2 this_type;
    typedef T value_type;

public:
    T min_x, min_y;
    T max_x, max_y;

public:
    aabb2()
    {
        reset();
    }

    aabb2(T x1, T y1, T x2, T y2)
    {
        reset();
        this->assign(x1, y1, x2, y2);
    }

    aabb2(const std::vector< vec2<T> >& points)
    {
        reset();
        this->assign(points);
    }

    T width()const
    {
        return max_x - min_x;
    }

    T height()const
    {
        return max_y - min_y;
    }

    void reset()
    {
        min_x = std::numeric_limits<value_type>::max();
        max_x = std::numeric_limits<value_type>::min();
    }

    bool is_valid()const
    {
        return min_x < max_x && min_y < max_y;
    }

    //assign rect
    void assign(T x1, T y1, T x2, T y2)
    {
        if(x1 < x2){
            min_x = x1;
            max_x = x2;
        }
        else{
            min_x = x2;
            max_x = x1;
        }
        if(y1 < y2){
            min_y = y1;
            max_y = y2;
        }
        else{
            min_y = y2;
            max_y = y1;
        }
    }

    void assign(const std::vector< vec2<T> >& points)
    {
        reset();
        for(size_t i=0; i<points.size(); ++i){
            this->append(points[i].x, points[i].y);
        }
    }

    void append(T x, T y)
    {
        if(x < min_x) min_x = x;
        if(x > max_x) max_x = x;
        if(y < min_y) min_y = y;
        if(y > max_y) max_y = y;
    }

    void append(const vec2<T>& p)
    {
        this->append(p.x, p.y);
    }

    bool contains(T x, T y)
    {
        return math::between(x, min_x, max_x) && math::between(y, min_y, max_y);
    }

    //判断是否相交, 边界重合
    bool intersect(const this_type& other)
    {
        return !(min_x >= other.max_x || min_y >= other.max_y || max_x <= other.min_x || max_y <= other.min_y);
    }

    this_type& operator&=(const this_type& other)
    {
        range_and(min_x, max_x, other.min_x, other.max_x);
        range_and(min_y, max_y, other.min_y, other.max_y);
        if(max_x < min_x)max_x = min_x;//test inverse
        if(max_y < min_y)max_y = min_y;
        return *this;
    }

    this_type& operator|=(const this_type& other)
    {
        range_or(min_x, max_x, other.min_x, other.max_x);
        range_or(min_y, max_y, other.min_y, other.max_y);
        return *this;
    }

    void expand(T n)
    {
        min_x -= n;
        min_y -= n;
        max_x += n;
        max_y += n;
    }

protected:
    void range_and(T& min1, T& max1, const T& min2, const T& max2)
    {
        min1 = std::max(min1, min2);
        max1 = std::min(max1, max2);
    }

    void range_or(T& min1, T& max1, const T& min2, const T& max2)
    {
        min1 = std::min(min1, min2);
        max1 = std::max(max1, max2);
    }

};

template<typename T>
class aabb3 : public aabb2<T>
{
public:
    typedef aabb3 this_type;
    typedef T value_type;

public:
    T min_z, max_z;

public:
    aabb3() : aabb2<T>()
    {

    }

    T depth()const
    {
        return max_z - min_z;
    }

    void reset()
    {
        aabb2<T>::reset();
        min_z = std::numeric_limits<int>::max();
        max_z = std::numeric_limits<int>::min();
    }

    bool is_valid()const
    {
        return aabb2<T>::is_valid() && min_z < max_z;
    }

    //assign box
    void assign(T x1, T y1, T x2, T y2, T z1, T z2)
    {
        aabb2<T>::assign(x1, y1, x2, y2);
        if(z1 < z2){
            min_z = z1;
            max_z = z2;
        }
        else{
            min_z = z2;
            max_z = z1;
        }
    }

    void assign(const std::vector< vec3<T> >& points)
    {
        reset();
        for(size_t i=0; i<points.size(); ++i){
            this->append(points[i].x, points[i].y, points[i].z);
        }
    }

    void append(T x, T y, T z)
    {
        aabb2<T>::append(x, y);
        if(z < min_z) min_z = z;
        if(z > max_z) max_z = z;
    }

    void expand(T n)
    {
        aabb2<T>::expand(n);
        min_z -= n;
        max_z += n;
    }
};

typedef aabb2<int>   box2i;
typedef aabb2<float> box2f;

typedef aabb3<int>   box3i;
typedef aabb3<float> box3f;

}//end namespace cgl

#endif //MATH_AABB_HPP_20200330033556
