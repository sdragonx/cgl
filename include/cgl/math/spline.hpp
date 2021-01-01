/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spline.hpp

 2019-11-25 20:23:22

*/
#ifndef SPLINE_HPP_20191125202322
#define SPLINE_HPP_20191125202322

#include "base.hpp"
#include "vector.hpp"

namespace cgl{
namespace math{

class ctrl_point
{
public:
    float x, y;
    int id;
    ctrl_point() : x(), y(), id() { }
    ctrl_point(int i, float vx, float vy) : x(vx), y(vy), id(i) { }
};

//����4��������ߵ�Ӱ�����
inline void cardinal_spline_linear(float t, float s, float& a, float& b, float& c, float& d)
{
    float t1, t2, t3;
    t1 = t;
    t2 = t1 * t1;
    t3 = t2 * t1;
    a = -s * t3 + 2 * s * t2 - s * t1;
    b = (2 - s) * t3 + (s - 3) * t2 + 1;
    c = (s - 2) * t3 + (3 - 2 * s) * t2 + s * t1;
    d = s * t3 - s * t2;
}

//
// ȡk�������������������ܶȿ��ܲ�̫��ȷ
//
template<typename T>
void cardinal_spline(
    std::vector<ctrl_point>& ls,
    const vec2<T>* points,
    size_t size,
    bool closed,
    float tension = 0.5f,
    float density = 0.05f)
{
    ls.clear();

    if(size == 0){
        return ;
    }
    else if(size == 1){
        ls.push_back(ctrl_point(0, points[0].x, points[0].y));
        return ;
    }
    else if(size == 2){
        ls.push_back(ctrl_point(0, points[0].x, points[0].y));
        ls.push_back(ctrl_point(1, points[1].x, points[1].y));
        return ;
    }

    int x, y, k;
    float a, b, c, d;
    //float s = tension;// (1.0f - tension) / 1.0f;
    float t;
    size_t i = 1;

    //add first point
    ls.push_back(ctrl_point(0, points[0].x, points[0].y));

    //i == 0 first
    k = distance(points[0].x, points[0].y, points[1].x, points[1].y) * density;//���ܶ�
    if(k > 1){
        t = 1.0f / k;
        for(int j=1; j<=k; ++j){
            math::cardinal_spline_linear(j * t, tension, a, b, c, d);
            if(closed){
                x=round(a*points[size-1].x+b*points[0].x+c*points[1].x+d*points[2].x);
                y=round(a*points[size-1].y+b*points[0].y+c*points[1].y+d*points[2].y);
            }
            else{
                x=round(a*points[0].x+b*points[0].x+c*points[1].x+d*points[2].x);
                y=round(a*points[0].y+b*points[0].y+c*points[1].y+d*points[2].y);
            }
            ls.push_back(ctrl_point(i, x, y));
        }
    }

    //center
    for(i=1; i<size-2; ++i){
        k = distance(points[i].x, points[i].y, points[i+1].x, points[i+1].y) * density;//���ܶ�
        if(k < 1){
            continue;
        }

        t = 1.0f / k;
        for(int j=1; j<=k; ++j){
            math::cardinal_spline_linear(j * t, tension, a, b, c, d);
            x=round(a*points[i-1].x+b*points[i].x+c*points[i+1].x+d*points[i+2].x);
            y=round(a*points[i-1].y+b*points[i].y+c*points[i+1].y+d*points[i+2].y);
            ls.push_back(ctrl_point(i, x, y));
        }
    }

    //last size - 2
    k = distance(points[i].x, points[i].y, points[i+1].x, points[i+1].y) * density;//���ܶ�
    if(k > 1){
        t = 1.0f / k;
        for(int j=1; j<=k; ++j){
            math::cardinal_spline_linear(j * t, tension, a, b, c, d);
            if(closed){
                x=round(a*points[i-1].x+b*points[i].x+c*points[i+1].x+d*points[0].x);
                y=round(a*points[i-1].y+b*points[i].y+c*points[i+1].y+d*points[0].y);
            }
            else{
                x=round(a*points[i-1].x+b*points[i].x+c*points[i+1].x+d*points[size-1].x);
                y=round(a*points[i-1].y+b*points[i].y+c*points[i+1].y+d*points[size-1].y);
            }
            ls.push_back(ctrl_point(i, x, y));
        }
    }

    //last size - 1
    if(closed){
        ++i;
        k = distance(points[i].x, points[i].y, points[0].x, points[0].y) * density;//���ܶ�
        if(k > 1){
            t = 1.0f / k;
            for(int j=1; j<=k; ++j){
                math::cardinal_spline_linear(j * t, tension, a, b, c, d);
                x=round(a*points[i-1].x+b*points[i].x+c*points[0].x+d*points[1].x);
                y=round(a*points[i-1].y+b*points[i].y+c*points[0].y+d*points[1].y);
                ls.push_back(ctrl_point(i, x, y));
            }
        }
    }
}

template<typename T>
void cardinal_spline(std::vector<ctrl_point>& ls,
    const std::vector< vec2<T> >& points,
    bool closed = true,
    float tension = 0.5f,
    float density = 0.25f)
{
    return cardinal_spline<T>(ls, &points[0], points.size(), closed, tension, density);
}

int test_nearest_point(std::vector<vec3i> ls, int x, int y)
{
    vec2i p(x, y);
    int index = 0;
    float length, min_length = FLT_MAX;
    for(size_t i=0; i<ls.size(); ++i){
        length = p.distance(vec2i(ls[i]));
        if(length < min_length){
            min_length = length;
            index = ls[i].z;
        }
    }
    return index;
}

class spline
{
public:
    typedef ctrl_point point_type;

    std::vector<point_type> points;

public:
    spline() : points()
    {
    }

    template<typename T>
    int assign(const std::vector< vec2<T> >& vs, bool closed = false, float tension = 0.5f, float density = 0.25f)
    {
        cardinal_spline(points, vs, closed, tension, density);
        return points.size();
    }

    void push(vec2f p)
    {
        //points.push_back(p);
    }

    void push(float x, float y)
    {
        this->push(vec2f(x, y));
    }

    void execute(float tension = 0.5f, float density = 0.25f)
    {
    }

    void clear()
    {
        points.clear();
    }

    size_t size()const
    {
        return points.size();
    }

    const point_type& operator[](int i)const
    {
        return points[i];
    }
};


//���α���������
//����ջ���
//2020-02-15 16:28:22
int conic_bezier(
    std::vector<vec2f>& ls,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    int steps = 20)
{
    float t = 0.0;
    float step = 1.0f / steps;

    for(int i=0; i < steps; ++i){
        x1 = (1 - t) * x1 + t * x2; //i=1, i<3; j=0, j<2
        y1 = (1 - t) * y1 + t * y2;

        x2 = (1 - t) * x2 + t * x3; //i=1, i<3; j=1, j<2
        y2 = (1 - t) * y2 + t * y3;

        x1 = (1 - t) * x1 + t * x2; //i=2, i<3; j=0, j<1
        y1 = (1 - t) * y1 + t * y2;

        t += step;

        ls.push_back(vec2f(x1, y1));
    }
    ls.push_back(vec2f(x3, y3));
    return 0;
}

//���α���������
//����ջ���
//2020-02-15 16:28:22
int cubic_bezier(
    std::vector<vec2f>& ls,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float x4, float y4,
    int steps = 20)
{
    float t = 0.0;
    float step = 1.0f / steps;

    for(int i=0; i < steps; ++i){
        x1 = (1 - t) * x1 + t * x2; //i=1, i<4; j=0, j<3
        y1 = (1 - t) * y1 + t * y2;

        x2 = (1 - t) * x2 + t * x3; //i=1, i<4; j=1, j<3
        y2 = (1 - t) * y2 + t * y3;

        x3 = (1 - t) * x3 + t * x4; //i=1, i<4; j=2, j<3
        y3 = (1 - t) * y3 + t * y4;

        x1 = (1 - t) * x1 + t * x2; //i=2, i<4; j=0, j<2
        y1 = (1 - t) * y1 + t * y2;

        x2 = (1 - t) * x2 + t * x3; //i=2, i<4; j=1, j<2
        y2 = (1 - t) * y2 + t * y3;

        x1 = (1 - t) * x1 + t * x2; //i=3, i<4; j=0, j<1
        y1 = (1 - t) * y1 + t * y2;

        t += step;

        ls.push_back(vec2f(x1, y1));
    }
    ls.push_back(vec2f(x4, y4));
    return 0;
}

template<typename T>
int conic_bezier(std::vector<vec2f>& ls, T p1, T p2, T p3, int steps = 20)
{
    return conic_bezier(ls, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, steps);
}

template<typename T>
int cubic_bezier(std::vector<vec2f>& ls, T p1, T p2, T p3, T p4, int steps = 20)
{
    return cubic_bezier(ls, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, steps);
}

//n�ױ���������
//2020-02-15 16:28:22
int bezier_curve(std::vector<vec2f>& ls, const std::vector<vec2f>& p)
{
    std::vector<vec2f> temp(p);
    int max_k = p.size() * 5;//����
    float t = 0.0;
    float step = 1.0f / max_k;//����
    for(int k=0; k<max_k; ++k){
        for (size_t i=1; i<p.size(); ++i) {
            for (size_t j=0; j<p.size()-i ; j++) {
                float x = (1-t) * temp[j].x + t * temp[j+1].x;
                float y = (1-t) * temp[j].y + t * temp[j+1].y;
                temp[j] = vec2f(x, y);
            }
        }
        ls.push_back(temp[0]);
        t += step;
    }
    //ls.push_back(p.back());
    return ls.size();
}

}//end namespace math
}//end namespace cgl

#endif //SPLINE_HPP_20191125202322
