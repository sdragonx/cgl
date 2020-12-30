/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 line.hpp

 2019-12-23 08:30:07

*/
#ifndef MODEL_LINE_HPP_20191223083007
#define MODEL_LINE_HPP_20191223083007

#include "base.hpp"
#include "../line.hpp"

namespace cgl{
namespace model{

template<typename T>
class line
{
public:
    typedef line this_type;
    typedef T value_type;
    typedef vec2<T> point_type;

public:
    point_type a, b;

public:
    line() : a(), b() { }
    line(const point_type& p1, const point_type& p2) : a(p1), b(p2) {}
    line(value_type x1, value_type y1, value_type x2, value_type y2) : a(x1, y1), b(x2, y2) {}

    void reset()
    {
        a = b = point_type();
    }

    void set(const point_type& p1, const point_type& p2)
    {
        a = p1;
        b = p2;
    }

    void set(value_type x1, value_type y1, value_type x2, value_type y2)
    {
        a.x = x1; a.y = y1;
        b.x = x2; b.y = y2;
    }

    this_type operator+(const point_type& v) const
    {
        return this_type(a + v, b + v);
    }

    this_type operator-(const point_type &v) const
    {
        return this_type(a - v, b - v);
    }

    aabb2<T> aabb()const
    {
        return aabb2<T>(a.x, a.y, b.x, b.y);
    }

    //直线中点
    point_type center()const
    {
        return a + (b - a) / 2;
    }

    //根据x坐标求y坐标
    //gradient_x(y);
    //gradient_y(x);
    value_type x_of(int y)
    {
        if(a.x == b.x){
            return a.x;
        }
        else{
            int dx = b.x - a.x;
            int dy = b.y - a.y;
            return a.x + (y - a.y) * dx / dy;
        }
    }

    value_type y_of(int x)
    {
        if(a.x == b.x){
            return a.y;
        }
        else{
            int dx = b.x - a.x;
            int dy = b.y - a.y;
            return a.y + (x - a.x) * dy / dx;
        }
    }

    //return The line segment's normal vector.
    point_type normal() const
    {
        point_type dir = b - a;

        // return the direction vector
        // rotated by 90 degrees counter-clockwise
        return point_type(-dir.y, dir.x);
    }

    //直线的方向
    point_type direction()const
    {
        return b - a;
    }

    //线段交点
    //infiniteLines == false 点在线上
    bool intersection(const this_type &other, point_type& cross)const
    {
        if(math::line_intersect(a, b, other.a, other.b, cross)){
            return math::is_on_line(a, b, cross) && math::is_on_line(other.a, other.b, cross);
        }
        return false;
    }

    //点到直线的距离
    value_type distance(const point_type& p)
    {
//        point_type ab = b - a;
//        return cross(ab, p - a) / ab.length();
        if(a == b){
            return math::distance(p, a);
        }
        value_type d = (b - a).length();
        return math::abs( (b - a).cross(p - a) / d );
    }

    //投影；垂线
    point_type project(const point_type& p)
    {
        //point_type ab = b - a;
        //return a + ab * math::dot((p - a), ab) / ab.dot();
        return math::line_project(a, b, p);
    }

//和圆
    //直线和圆是否相交
    bool intersect(const circle<T>& c)
    {
        return this->distance(c.center) < c.r;
    }

};

/*
template<typename T>
class line
{
public:
    vec2f p1, p2;

    

    //与圆的交点
    int crossPoints(const gmCircle& c, vec2f& out1, vec2f& out2)
    {
        if(!this->is_intersect(c)){
            return -1;
        }

        vec2f pr = this->project(c.c);
        vec2f e = (p2 - p1) / (p2 - p1).length();
        double base = std::sqrt(c.r * c.r - (pr - c.c).inner());
        out1 = pr + e * base;
        out2 = pr - e * base;
        return 0;
    }
};
*/

}//end namespace model
}//end namespace cgl

#endif //MODEL_LINE_HPP_20191223083007
