/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 geometry.hpp

 2018-10-28 21:54:09

 ͨ�ü��κ���

 circumference  �ܳ�

*/
#ifndef GEOMETRY_HPP_20181028215409
#define GEOMETRY_HPP_20181028215409

#include <cgl/math/base.hpp>
#include "model/line.hpp"
#include "model/circle.hpp"

namespace cgl{
namespace math{

//
// ������
//

//����������
template<typename T>
T cube_surface_area(T length, T width, T height)
{
    return (length * width + length * height + width * height) * 2;
}

template<typename T>
T cube_surface_area(T size)
{
    return size * size * 6;
}

//���������
template<typename T>
T cube_volume(T length, T width, T height)
{
    return length * width * height;
}

template<typename T>
T cube_volume(T size)
{
    return size * size * size;
}

//
// Բ
//

//�ܳ� = PI * 2R
template<typename T>
T circle_perimeter(T r)
{
    return M_PI * r * 2;
}

//��� = PI * R^2
template<typename T>
T circle_area(T r)
{
    return M_PI * r * r;
}

//
// ��Բ
//

//��� S = PI * A * B
template<typename T>
T ellipse_area(T a, T b)
{
    return M_PI * a * b;
}

//
// Բ����
//

//�����
template<typename T>
T cyllinder_surface_area(T r, T height)
{
    return circle_perimeter(r) * height;
}

//��� = �����*��
template<typename T>
T cyllinder_volume(T r, T height)
{
    return circle_area(r) * height;
}

//
// Բ׶��
//

//�����
//PI * R * L + PI * R^2
//PI * R * (R + L)
//L = sqrt(R^2 + H^2)
template<typename T>
T cone_surface_area(T r, T height)
{
    T L = sqrt(r * r + height * height);
    return M_PI * r * (r + L);
}

//��� = �����*��/3
template<typename T>
T cone_volume(T r, T height)
{
    return circle_area(r) * height / 3;
}

//
// ����
//

//�������ʽ S = 4 * PI * R^2

template<typename T>
T sphere_surface_area(T r)
{
    return 4 * M_PI * r * r;
}

//��� V = 4/3 * PI * R^3
//V=(4/3)��R^3
template<typename T>
T sphere_volume(T r)
{
    return r * r * r * M_PI * 3 / 4;
}

//---------------------------------------------------------------------------
//p1(x1, y1)Ϊԭ�㣬����p2(x2, y2)��λ��
//ʵ��Ӧ�ã�������ֱ�ߵ�ʱ�򣬰���shift����������������λ�õ���
//��˸�����ֻ�Ǵ��Լ��㣬��eight_dir���Ȳ�ͬ

void shift_line_point(int x1, int y1, int& x2, int& y2)
{
    using namespace std;
    int w = x2 - x1;
    int h = y2 - y1;
    int sign;

    if(abs(w) < abs(h / 2)){//����
        x2 = x1;
    }
    else if(abs(h) < abs(w / 2)){//����
        y2 = y1;
    }
    else{//б45���ߣ�ȡ�����̵Ľ��в���
        if(abs(w) < abs(h)){
            sign = w < 0 ? -1 : 1;
            x2 = x1 + abs(h) * sign;
        }
        else{
            sign = h < 0 ? -1 : 1;
            y2 = y1 + abs(w) * sign;
        }
    }
}

/*
��ð˸�����
7 0 1
6   2
5 4 3

22.5��
1.0/(x/y) =   0.414213592529297
256         106.038679687500032
1024        424.154718750000128
*/
int eight_dir(int x, int y)
{
    using namespace std;
    int w = abs(x);
    int h = abs(y);
    int n;
    if(w < h){
        n = h * 106 / 256;
        if(w < n){
            return y < 0 ? 0 : 4;
        }
        else{
            if(x < 0){
                return y < 0 ? 7 : 5;
            }
            else{
                return y < 0 ? 1 : 3;
            }
        }
    }
    else{
        n = w * 106 / 255;
        if(h < n){
            return x < 0 ? 6 : 2;
        }
        else{
            if(x < 0){
                return y < 0 ? 7 : 5;
            }
            else{
                return y < 0 ? 1 : 3;
            }
        }
    }
}


//���κ�Բ��ײ��������ײ��
//Բ�ھ����ڲ������ص�Ϊ0�����ص�ֻ��Բ��������������
template<typename T>
bool circle_collision(T x, T y, T width, T height, T cx, T cy, T cr, T& dx, T& dy)
{
    width /= 2;
    height /= 2;

    x += width;
    y += height;

    T rx = cx - x;
    T ry = cy - y;

    dx  = std::min(rx,  width);
    dy  = std::min(ry,  height);
    dx = std::max(dx, -width);
    dy = std::max(dy, -height);

    dx = dx - rx;//����Բ������ĵ�
    dy = dy - ry;

    return dx * dx + dy * dy <= cr * cr;
}

template<typename T>
bool circle_collision(const vec4<T>& rect, const vec2<T>& c, T r, vec2<T>& p)
{
    return circle_collision(rect.x, rect.y, rect.width, rect.height, c.x, c.y, r, p.x, p.y);
}


//�ڲ�Ҳ����
bool get_vec(vec4i rect, vec2i c, int r, vec2i& d)
{
    using namespace std;

    int width = rect.width / 2;
    int height = rect.height / 2;

    int x = rect.x + width;  //��������
    int y = rect.y + height;

    int rx = c.x - x;
    int ry = c.y - y;

    int dx, dy;

    #if 1
    //Բ���ڲ������������ײ
    if(rx > -width && rx < width && ry > -height && ry < height){
        if(abs(rx) < abs(ry)){
            dx = rx;
            //dy = ry < 0 ? -height - height - ry : height + height -ry;
            dy = ry < 0 ? -height : height;
        }
        else{
            //dx = rx < 0 ? - width - width - rx : width + width  - rx;
            dx = rx < 0 ? -width : width;
            dy = ry;
        }
    }
    else{
        dx  = std::min(rx,  width);
        dy  = std::min(ry,  height);
        dx = std::max(dx, -width);
        dy = std::max(dy, -height);
    }
    #endif

    dx = dx - rx;//����Բ������ĵ�
    dy = dy - ry;

    d.x = dx;
    d.y = dy;

    return dx * dx + dy * dy <= r * r;
}

template<typename T>
bool point_in_polygon(vec2<T> *points, int size, T x, T y)
{
    bool c = 0;
    for(int i = 0, j = size-1; i < size; j = i++){
        if ( ((points[i].y > y) != (points[j].y > y)) &&
            (x < (points[j].x - points[i].x) * (y - points[i].y) / (points[j].y - points[i].y) + points[i].x) )
            c = !c;
    }
    return c;
}

}//end namespace math
}//end namespace cgl

#endif //GEOMETRY_HPP_20181028215409