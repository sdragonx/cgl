/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 triangle.hpp

 2018-03-24 08:43:03

*/
#ifndef TRIANGLE_HPP_20180324084303
#define TRIANGLE_HPP_20180324084303

#include "base.hpp"

namespace cgl{
namespace math{

//ȡ�����ε��е�
template<typename T>
vec2<T> midpoint(vec2<T> p1, vec2<T> p2, vec2<T> p3)
{
    return midpoint(midpoint(p1, p2), p3);
}

//���������
template<typename T>
float triangleArea(const vec2<T>& A, const vec2<T>& B, const vec2<T>& C)
{
    //�������������Ĳ�������㣬�ɲο�http://blog.csdn.net/zxj1988/article/details/6260576
    vec2<T> AB = B - A;
    vec2<T> BC = C - B;
    return fabs(AB.crossProduct(BC) / 2.0);
}

//���������S=(1/2)*(x1y2+x2y3+x3y1-x1y3-x2y1-x3y2)
inline int triangle_area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return std::abs(((x1 - x3) * y2 + (x2 - x1) * y3 + (x3 - x2) * y1) / 2);
}

inline float triangle_areaf(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return fabs(((x1 - x3) * y2 + (x2 - x1) * y3 + (x3 - x2) * y1) * 0.5f);
}

inline double triangle_areaf(double x1, double y1, double x2, double y2, double x3, double y3)
{
    return fabs(((x1 - x3) * y2 + (x2 - x1) * y3 + (x3 - x2) * y1) * 0.5);
}


//���uv����
//����㷨�õ�����uv��ȷʵ�������ε�ӳ�䣬��uv�������е��¶���
//��������ת��ʱ��uv��ֵ�仯�е��ϲ�
//�жϵ����������� u + v < 1.0 && u > 0 && v > 0
//P�����������ڣ�����(u, v)������������u �� 0, v �� 0, u + v �� 1��
int get_uvf(float x1, float y1, float x2, float y2, float x3, float y3,
    float x, float y, float& u, float& v)
{
    float cx = x1 - x;
    float cy = y1 - y;

    float n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);

    if(n > -FLT_EPSILON && n < FLT_EPSILON){//is zero
        u = v = 0.0f;
    }

    n = 1.0f / n;
    u = ((x3 - x1) * cy - (y3 - y1) * cx) *  n;
    v = ((x2 - x1) * cy - (y2 - y1) * cx) * -n;
    return 0;
};

//���uvֵ
//float n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
//n = 1.0f / n;

inline float calc_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    return 1.0 / n;
}

inline int get_uvf(
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float n,
    float x, float y,
    float& u, float& v)
{
    float cx = x1 - x;
    float cy = y1 - y;

    u = ((x3 - x1) * cy - (y3 - y1) * cx) *  n;
    v = ((x2 - x1) * cy - (y2 - y1) * cx) * -n;

    //if(n < 0)std::swap(u, v);
//������ѡ��uv�仯�ϲ�
    return 0;
}

inline int calc_area(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
}

//int n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
//ӳ�䵽1024֮��
int get_uv(int x1, int y1, int x2, int y2, int x3, int y3,
    int n,
    int x, int y,
    int& u, int& v)
{
    int cx = x1 - x;
    int cy = y1 - y;

    if(n){
        u = ((x3 - x1) * cy - (y3 - y1) * cx) * 255 /  n;
        v = ((x2 - x1) * cy - (y2 - y1) * cx) * 255 / -n;
    }
    else{
        u = v = 0;
    }
    return 0;
}


//����abc���õ��ǵ�ƽ����
vec2i angular_bisector(const vec2i& a, const vec2i& b, const vec2i& c)
{
    int ab = (a - b).length();//��ab�ĳ���
    vec2i p = b - c;
    int bc = p.length();//��bc�ĳ���
    if(bc == 0){
        return vec2i();
    }

    p.x = ab * p.x / bc;
    p.y = ab * p.y / bc;

    p = b - p;//��c�������b��ab����ĵ�
    p = p + (a - p) / 2;//���a��p���е�

    return p;
}


}//end namespace math
}//end namespace cgl

#endif //TRIANGLE_HPP_20180324084303

/*
//�������������
    double ComputeTriangleArea()
    {
        //�������������Ĳ�������㣬�ɲο�http://blog.csdn.net/zxj1988/article/details/6260576
        Vector2d AB = pointB_.Minus(pointA_);
        Vector2d BC = pointC_.Minus(pointB_);
        return fabs(AB.CrossProduct(BC) / 2.0);

    }

    bool IsPointInTriangle1(const Vector2d pointP)
    {
        double area_ABC = ComputeTriangleArea();
        double area_PAB = Triangle(pointP, pointA_, pointB_).ComputeTriangleArea();
        double area_PAC = Triangle(pointP, pointA_, pointC_).ComputeTriangleArea();
        double area_PBC = Triangle(pointP, pointB_, pointC_).ComputeTriangleArea();

        if(fabs(area_PAB + area_PBC + area_PAC - area_ABC) < 0.000001)
            return true;
        else return false;
    }

    bool IsPointInTriangle2(const Vector2d pointP)
    {
        return Vector2d::IsPointAtSameSideOfLine(pointP, pointA_, pointB_, pointC_) &&
            Vector2d::IsPointAtSameSideOfLine(pointP, pointB_, pointA_, pointC_) &&
            Vector2d::IsPointAtSameSideOfLine(pointP, pointC_, pointA_, pointB_);
    }

    bool IsPointInTriangle3(const Vector2d pointP)
    {
        Vector2d AB = pointB_.Minus(pointA_);
        Vector2d AC = pointC_.Minus(pointA_);
        Vector2d AP = pointP.Minus(pointA_);
        double dot_ac_ac = AC.DotProduct(AC);
        double dot_ac_ab = AC.DotProduct(AB);
        double dot_ac_ap = AC.DotProduct(AP);
        double dot_ab_ab = AB.DotProduct(AB);
        double dot_ab_ap = AB.DotProduct(AP);

        double tmp = 1.0 / (dot_ac_ac * dot_ab_ab - dot_ac_ab * dot_ac_ab);

        double u = (dot_ab_ab * dot_ac_ap - dot_ac_ab * dot_ab_ap) * tmp;
        if(u < 0 || u > 1)
            return false;
        double v = (dot_ac_ac * dot_ab_ap - dot_ac_ab * dot_ac_ap) * tmp;
        if(v < 0 || v > 1)
            return false;

        return u + v <= 1;
    }

    bool IsPointInTriangle4(const Vector2d pointP)
    {
        Vector2d PA = pointA_.Minus(pointP);
        Vector2d PB = pointB_.Minus(pointP);
        Vector2d PC = pointC_.Minus(pointP);
        double t1 = PA.CrossProduct(PB);
        double t2 = PB.CrossProduct(PC);
        double t3 = PC.CrossProduct(PA);
        return t1*t2 >= 0 && t1*t3 >= 0;
    }
*/
