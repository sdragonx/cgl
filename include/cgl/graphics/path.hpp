/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 path.hpp

 2020-03-25 14:58:41

*/
#ifndef PATH_HPP_20200325145841
#define PATH_HPP_20200325145841

#include <cgl/public.h>
#include <cgl/vec.hpp>

namespace cgl{
namespace graphics{

//
// path_command
//

class path_command
{
public:
    std::vector<int> commands;
    std::vector<vec2f> points;

    bool empty()const
    {
        return commands.empty();
    }

    void clear()
    {
        commands.clear();
        points.clear();
    }

    template<typename T>
    void move_to(const T& p)
    {
        commands.push_back(CGL_MOVE_TO);
        points.push_back(vec2f(p.x, p.y));
    }

    template<typename T>
    void move_to(T x, T y)
    {
        this->move_to(vec2f(x, y));
    }

    template<typename T>
    void line_to(const T& p)
    {
        commands.push_back(CGL_LINE_TO);
        points.push_back(vec2f(p.x, p.y));
    }

    template<typename T>
    void line_to(T x, T y)
    {
        this->line_to(vec2f(x, y));
    }

    template<typename T>
    void arc_to(const T& p1, const T& p2)
    {
        commands.push_back(CGL_ARC_TO);
        points.push_back(vec2f(p1.x, p1.y));
        points.push_back(vec2f(p2.x, p2.y));
    }

    /*
    void arc_to(float x1, float y1, float rx, float ry, float angle, int size, int sweep, float x2, float y2)
    {
        commands.push_back(CGL_ARC_TO);
        points.push_back(vec2f(x1, y1));
        points.push_back(vec2f(rx, ry));
        points.push_back(vec2f(angle, 0));
        points.push_back(vec2f(size, sweep));
        points.push_back(vec2f(x2, y2));
    }
    */

    template<typename T>
    void conic_curve_to(T x2, T y2, T x3, T y3)
    {
        commands.push_back(CGL_CONIC_CURVE_TO);
        points.push_back(vec2f(x2, y2));
        points.push_back(vec2f(x3, y3));
    }

    template<typename T>
    void conic_curve_to(const T& p2, const T& p3)
    {
        commands.push_back(CGL_CONIC_CURVE);
        points.push_back(vec2f(p2.x, p2.y));
        points.push_back(vec2f(p3.x, p3.y));
    }

    template<typename T>
    void conic_curve(T x1, T y1, T x2, T y2, T x3, T y3)
    {
        commands.push_back(CGL_CONIC_CURVE);
        points.push_back(vec2f(x1, y1));
        points.push_back(vec2f(x2, y2));
        points.push_back(vec2f(x3, y3));
    }

    template<typename T>
    void conic_curve(const T& p1, const T& p2, const T& p3)
    {
        commands.push_back(CGL_CONIC_CURVE);
        points.push_back(vec2f(p1.x, p1.y));
        points.push_back(vec2f(p2.x, p2.y));
        points.push_back(vec2f(p3.x, p3.y));
    }

    template<typename T>
    void cubic_curve_to(T x2, T y2, T x3, T y3, T x4, T y4)
    {
        commands.push_back(CGL_CUBIC_CURVE_TO);
        points.push_back(vec2f(x2, y2));
        points.push_back(vec2f(x3, y3));
        points.push_back(vec2f(x4, y4));
    }

    template<typename T>
    void cubic_curve_to(const T& p2, const T& p3, const T& p4)
    {
        commands.push_back(CGL_CUBIC_CURVE);
        points.push_back(vec2f(p2.x, p2.y));
        points.push_back(vec2f(p3.x, p3.y));
        points.push_back(vec2f(p4.x, p4.y));
    }

    template<typename T>
    void cubic_curve(T x1, T y1, T x2, T y2, T x3, T y3, T x4, T y4)
    {
        commands.push_back(CGL_CUBIC_CURVE);
        points.push_back(vec2f(x1, y1));
        points.push_back(vec2f(x2, y2));
        points.push_back(vec2f(x3, y3));
        points.push_back(vec2f(x4, y4));
    }

    template<typename T>
    void cubic_curve(const T& p1, const T& p2, const T& p3, const T& p4)
    {
        commands.push_back(CGL_CUBIC_CURVE);
        points.push_back(vec2f(p1.x, p1.y));
        points.push_back(vec2f(p2.x, p2.y));
        points.push_back(vec2f(p3.x, p3.y));
        points.push_back(vec2f(p4.x, p4.y));
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //PATH_HPP_20200325145841
