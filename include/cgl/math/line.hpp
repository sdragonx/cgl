/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 line.hpp

 2015-04-03 05:34:53

*/
#ifndef LINE_HPP_20150403053453
#define LINE_HPP_20150403053453

#include "base.hpp"

namespace cgl{
namespace math{

//
// б��
//

inline int fixed_slope(int x1, int y1, int x2, int y2)
{
    if(y1 == y2){
        return 0;
    }
    return (x2 - x1) * 65536 / (y2 - y1);
}

inline float slope(int x1, int y1, int x2, int y2)
{
    return float(x2 - x1) / float(y2 - y1);
}

inline float slope(float x1, float y1, float x2, float y2)
{
    return (x2 - x1) / (y2 - y1);
}

inline double slope(double x1, double y1, double x2, double y2)
{
    return (x2 - x1) / (y2 - y1);
}

//x = line_step(x1, y1, x2, y2, y)
//��ʱ���ã�û��������0���ж�
int line_step_x(int x1, int y1, int x2, int y2, int y)
{
    return y * (x2 - x1) / (y2 - y1);
}

int line_step_y(int x1, int y1, int x2, int y2, int x)
{
    return x * (y2 - y1) / (x2 - x1);
}


//
// line_aabb ���ֱ�ߵİ�Χ��
//

template<typename T>
vec4<T> line_aabb(T x1, T y1, T x2, T y2)
{
    return vec4<T>(std::min<T>(x1, x2), std::min<T>(y1, y2), absT<T>(x2 - x1), absT<T>(y2 - y1));
}

template<typename T>
inline vec4<T> line_aabb(const vec2<T>& p1, const vec2<T>& p2)
{
    return line_aabb<T>(p1.x, p1.y, p2.x, p2.y);
}



//��p��ֱ��p1,p2����һ��
//0ֱ����
template<typename T>
int positional(const vec2<T>& p1, const vec2<T>& p2, const vec2<T>& p3)
{
    //������� S(P1,P2,P3)=|y1 y2 y3|=(x1-x3)*(y2-y3)-(y1-y3)*(x2-x3)
    return (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);

    //�������
    //return ((p2.y - p1.y) * p3.x + (p1.x-p2.x) * p3.y + (p2.x * p1.y - p1.x*p2.y));
}

/*
//�жϵ�M,N�Ƿ���ֱ��AB��ͬһ��
template<typename T>
bool IsPointAtSameSideOfLine(const vec2<T>& M, const vec2<T>& N,
    const vec2<T> &A, const vec2<T> &B)
{
    vec2<T> AB = B - A;
    vec2<T> AM = M - A;
    vec2<T> AN = N - A;

    //����0ʱ��ʾĳ������ֱ����
    return AB.cross_product(AM) * AB.cross_product(AN) >= 0;
}

//�жϵ��Ƿ���ֱ����
bool IsPointOnLineSegment(const LineSeg& ls, const Point& pt)
{
    Rect rc;
    GetLineSegmentRect(ls, rc);
    double cp = CrossProduct(ls.pe.x - ls.ps.x, ls.pe.y - ls.ps.y, pt.x - ls.ps.x, pt.y - ls.ps.y); //������
    return ( (IsPointInRect(rc, pt)) //�ų�ʵ��
    && IsZeroFloatValue(cp) ); //1E-8 ����
}
*/



//
// is_on_line
// �жϽ����Ƿ�ͬʱ����������
//

//�жϵ��Ƿ��ڰ�Χ����
template<typename T>
inline bool is_on_line(const vec2<T>& a, const vec2<T>& b, T x, T y)
{
    return (x - a.x) * (x - b.x) <= 0 && (y - a.y) * (y - b.y) <= 0;
}

template<typename T>
inline bool is_on_line(const vec2<T>& a, const vec2<T>& b, const vec2<T>& p)
{
    return is_on_line(a, b, p.x, p.y);
}

template<typename T>
inline bool is_on_line(T x, T y, const vec2<T>& a, const vec2<T>& b, const vec2<T>& c, const vec2<T>& d)
{
    return is_online(a, b, x, y) && is_on_line(c, d, x, y);
}

//
//�ж������߶��Ƿ��ཻ
//����ֵ 0 ƽ�У�1 ���������ϣ�-1 ���㲻������
//

//������ĵ��ֱ���ཻ��is_on_line�����Ż��ɵ���ֱ�ߵİ�Χ����
//2020-04-01 02:53:33
//ֻ�󽻵�
template<typename T>
bool line_intersect(const vec2<T> &a, const vec2<T> &b, const vec2<T> &c, const vec2<T> &d, vec2<T> &crossPoint)
{
    vec2<T> ab(b.x - a.x, b.y - a.y);
    vec2<T> cd(d.x - c.x, d.y - c.y);

    T denominator = cross(ab, cd);
    if (is_zero(denominator)){//ƽ��
        return 0;
    }

    vec2<T> ac(c.x - a.x, c.y - a.y);
    //na / denominator = [0~1.0] ����������
    T na = cross(ac, cd);
    //T nb = cross(ac, ab);

    crossPoint.x = a.x + na * ab.x / denominator;
    crossPoint.y = a.y + na * ab.y / denominator;

    //return is_on_line(ptNode.x, ptNode.y, pt1, pt2, pt3, pt4);
    //return is_in_rect(p1, p2) && is_in_rect(p3, p4);
    //return na < 0 ? (denominator <= na && na <= 0) : (0 <= na && na <= denominator);//���Ƿ���ab֮��
    //return is_on_line(a, b, crossPoint.x, crossPoint.y) && is_on_line(c, d, crossPoint.x, crossPoint.y);
    return true;
}

/*
�ӿ�һ��ֱ��

     a
c _______ d
 |   |   |
 |   |   |
 |   |   |
 |   |-v-|
 |___|___|
e         f
     b
*/

class frameline
{
public:
    vec2i a, b;
    vec2i v;
    vec2i c, d;
    vec2i e, f;

public:
    void set_point(const vec2i& p1, const vec2i& p2, int pen_width)
    {
        a = p1;
        b = p2;

        vec2i ab = b - a;
        int length = ab.length();

        if(length == 0){
            return ;
        }

        vec2i v;
        v.x = pen_width * ab.y / length;
        v.y = pen_width * ab.x / length;

        c.x = a.x - v.x;
        c.y = a.y + v.y;

        d.x = a.x + v.x;
        d.y = a.y - v.y;

        e.x = b.x - v.x;
        e.y = b.y + v.y;

        f.x = b.x + v.x;
        f.y = b.y - v.y;
    }
};

//
// lien_project(a, b, p)
//
// ֱ��ͶӰ����һ�㣬�õ�ֱ�ߵĴ���
//

//����õ���p���߶�ab�Ĵ��㣬��һ�����߶���
//ab = b - a;
//return a + ab * dot((p - a), ab) / ab.dot();
//
template<typename T>
vec2<T> line_project(const vec2<T>& a, const vec2<T>& b, const vec2<T>& p)
{
    vec2<T> d = b - a;

    if(is_zero(d.x) && is_zero(d.y)){
        return a;
    }

    T n = d.dot();
    T u = dot(p.x - a.x, p.y - a.y, d.x, d.y);
    d *= u;
    d /= n;
    d += a;

    return d;
}

//��άģʽ
template<typename T>
vec3<T> line_project(const vec3<T>& a, const vec3<T>& b, const vec3<T>& p)
{
    vec3<T> d = a - b;

    if(is_zero(d.x) && is_zero(d.y) && is_zero(d.z)){
        return a;
    }

    T n = d.dot();
    //T u = (p.x - a.x) * (a.x - b.x) + (p.y - a.y) * (a.y - b.y) + (p.z - a.z) * (a.z - b.z);
    T u = (p - a).dot(d);
    d *= u;
    d /= n;
    d += a;

    return d;
}


//
// �ж�ֱ���Ƿ�������ཻ
// ����㷨�޷��ж��ཻλ��
//

bool intersect(const vec4i& rect, vec2i p1, vec2i p2)
{
    //ֱ���ھ����ڲ����ཻ
    if(rect.contains(p1) || rect.contains(p2)){
        return true;
    }

    vec4i aabb = math::line_aabb(p1, p2);

    //�ж�ֱ�߰�Χ���Ƿ�;����ཻ��������ཻ������ֱ�ߺ;��β��ཻ
    if(!rect.intersect(aabb)){
        return false;
    }

    //�ĸ��ǵĵ�
    // v1  v2
    // v3  v4
    vec2i v1(rect.x, rect.y);
    vec2i v2(rect.x + rect.width, rect.y);
    vec2i v3(rect.x, rect.y + rect.height);
    vec2i v4(rect.x + rect.width, rect.height);

    //ֻҪ��������Խ��ߺ�ֱ���ཻ���ʹ���ֱ�ߺ;����ཻ
    vec2i p;
    return line_intersect(v1, v4, p1, p2, p) || line_intersect(v2, v3, p1, p2, p);
}

}//end namespace math
}//end namespace cgl

#endif //LINE_HPP_20150403053453