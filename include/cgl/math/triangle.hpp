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

//三角形面积
//1:
//依据两个向量的叉乘来计算，可参考http://blog.csdn.net/zxj1988/article/details/6260576
//vec2<T> AB = B - A;
//vec2<T> BC = C - B;
//area = abs(cross(AB, BC) / 2.0);
//2:
//三角形面积S= abs((x1y2+x2y3+x3y1-x1y3-x2y1-x3y2)/2)

template<typename T>
T triangle_area(T x1, T y1, T x2, T y2, T x3, T y3)
{
    return abs(half(cross(x1, y1, x2, y2, x3, y3)));
}

template<typename T>
T triangle_area(const vec2<T>& a, const vec2<T>& b, const vec2<T>& c)
{
    return abs(half(cross(a, b, c)));
}

//获得uv坐标
//这个算法得到的是uv，确实是三角形的映射，但uv的正负有点懵懂。
//三角形旋转的时候，uv的值变化有点叵测
//判断点在三角形内 u + v < 1.0 && u > 0 && v > 0
//P点在三角形内，所以(u, v)必须满足条件u ≥ 0, v ≥ 0, u + v ≤ 1。
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

float float_uv(float x, float y,
    float x1, float y1,
    float x2, float y2,
    float x3, float y3,
    float& u, float& v)
{
    float dx1 = x2 - x1;
    float dx2 = x3 - x2;
    float dy1 = y2 - y1;
    float dy2 = y3 - y2;

    float n = math::cross(dx1, dy1, dx2, dy2);

    if(is_zero(n)){
        u = v = 0.0f;
        return n;
    }

    float dx = x2 - x;
    float dy = y2 - y;

    n = 1.0f / n;
    u = math::cross(dx, dy, dx1, dy1) * n;
    v = math::cross(dx, dy, dx2, dy2) * n;
    return n;
};

//获得uv值
//float n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
//n = 1.0f / n;

inline float calc_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float n = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
    return 1.0f / n;
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
//三角形选择，uv变化叵测
    return 0;
}

//int n = cross(x1, y1, x2, y2, x3, y3)
//映射到1024之间
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


//过角abc，得到角的平分线
vec2i angular_bisector(const vec2i& a, const vec2i& b, const vec2i& c)
{
    int ab = (a - b).length();//边ab的长度
    vec2i p = b - c;
    int bc = p.length();//边bc的长度
    if(bc == 0){
        return vec2i();
    }

    p.x = ab * p.x / bc;
    p.y = ab * p.y / bc;

    p = b - p;//边c上面距离b长ab距离的点
    p = p + (a - p) / 2;//求点a和p的中点

    return p;
}

//背面剪裁
bool face_culling(const vec3f& v1, const vec3f& v2, const vec3f& v3)
{
    vec3f cameraDir(0.0, 0.0, 1.0);
    vec3f n1 = v2 - v1;
    vec3f n2 = v3 - v1;
    vec3f face = n1.cross(n2);
    face.normalize();

    if (face.dot(cameraDir) < 0.0) {
        return true;
    }
    return false;
}

//
// 视口剪裁
//

//所有顶点都在视口内部
bool AllVertexsInside(const vec2f &v1, const vec2f &v2, const vec2f &v3)
{
    if (v1.x > 1 || v1.x < -1)
        return false;
    if (v1.y > 1 || v1.y < -1)
        return false;
    if (v2.x > 1 || v2.x < -1)
        return false;
    if (v2.y > 1 || v2.y < -1)
        return false;
    if (v3.x > 1 || v3.x < -1)
        return false;
    if (v3.y > 1 || v3.y < -1)
        return false;
    return true;
}

bool Inside(const vec3f &line, const vec2f &p)
{
    return line.x * p.x + line.y * p.y + line.z < 0;
}

//交点，通过端点插值
vec2f Intersect(const vec2f &v1, const vec2f &v2, const vec3f &line)
{
    if (line.x == -1) {
        float weight = (-1 - v1.x) / (v2.x - v1.x);
        return math::mix(v1, v2, weight);
    }
    if (line.x == 1) {
        float weight = (1 - v1.x) / (v2.x - v1.x);
        return math::mix(v1, v2, weight);
    }
    if (line.y == -1) {
        float weight = (-1 - v1.y) / (v2.y - v1.y);
        return math::mix(v1, v2, weight);
    }
    if (line.y == 1) {
        float weight = (1 - v1.y) / (v2.y - v1.y);
        return math::mix(v1, v2, weight);
    }

    return vec2f();
}

// Sutherland-Hodgeman三角形裁剪
// 顶点和剪裁的单位坐标都是1
// output   输出裁剪后的顶点组
// v1,v2,v3 输入三个顶点
bool SutherlandHodgeman(
    std::vector<vec2f>& output,
    const vec2f &v1,
    const vec2f &v2,
    const vec2f &v3)
{
    //左上右下
    //四条裁剪框方程
    // Ax + By + C < 0 就在内侧
    static vec3f ViewLines[4] = {
        vec3f(-1,  0, -1),
        vec3f( 0,  1, -1),
        vec3f( 1,  0, -1),
        vec3f( 0, -1, -1)
    };

    if (AllVertexsInside(v1, v2, v3)) {
        return false;
    }

    output.push_back(v1);
    output.push_back(v2);
    output.push_back(v3);

    for (int i = 0; i < 4; ++i) {
        std::vector<vec2f> input(output);
        output.clear();
        for (size_t j = 0; j < input.size(); ++j) {
            vec2f current = input[j];
            vec2f last = input[(j + input.size() - 1) % input.size()];
            if (Inside(ViewLines[i], current)) {
                if (!Inside(ViewLines[i], last)) {
                    vec2f intersecting = Intersect(last, current, ViewLines[i]);
                    output.push_back(intersecting);
                }
                output.push_back(current);
            }
            else if (Inside(ViewLines[i], last)) {
                vec2f intersecting = Intersect(last, current, ViewLines[i]);
                output.push_back(intersecting);
            }
        }
    }
    return true;
}

/* ray_triangle_intersection
 * 射线三角形相交判断
 * org      射线原点
 * dir      射线方向
 * v1,v2,v3 三角形
 * out      交点
 */
bool ray_triangle_intersection(const vec3f& org, const vec3f& dir,
    const vec3f& v1, const vec3f& v2, const vec3f& v3,
    vec3f *out)
{
    //find vectors for two edges sharing v1
    vec3f e1 = v2 - v1;
    vec3f e2 = v3 - v1;

    //begin calculating determinant - also used to calculate u parameter
    vec3f pvec = dir.cross(e2);

    //if determinant is near zero, ray lies in plane of triangle
    float det = e1.dot(pvec);

    //NOT CULLING
    if (is_zero(det)){//det < FLT_EPSILON
        return false;
    }

    float inv_det = 1.0 / det;

    //calculate distance from v1 to ray origin
    vec3f tvec = org - v1;

    //calculate u parameter and test bound
    float u = tvec.dot(pvec) * inv_det;

    //the intersection lies outside of the triangle
    if (u < 0.0 || u > 1.0){
        return false;
    }

    //prepare to test v parameter
    vec3f qvec = tvec.cross(e1);

    //calculate V parameter and test bound
    float v = dir.dot(qvec) * inv_det;

    //the intersection lies outside of the triangle
    if (v < 0.0 || u + v  > 1.0){
        return false;
    }

    float t = e2.dot(qvec) * inv_det;

    //ray intersection
    if (t > FLT_EPSILON){
        *out = org + dir * t;
        return true;
    }

    return false;
}

}//end namespace math
}//end namespace cgl

#endif //TRIANGLE_HPP_20180324084303

/*

//最大32768
//会溢出
int fixed_uv(int x, int y,
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    int& u, int& v)
{
    int dx1 = x2 - x1;
    int dx2 = x3 - x2;
    int dy1 = y2 - y1;
    int dy2 = y3 - y2;

    int n = math::cross(dx1, dy1, dx2, dy2);

    if(is_zero(n)){
        u = v = 0;
        return n;
    }

    int dx = x2 - x;
    int dy = y2 - y;

    u = math::cross(dx, dy, dx1, dy1);// * 65536 /  n;
    v = math::cross(dx, dy, dx2, dy2);// * 65536 / -n;
    return n;
};

vec2<int> fixed_uv(int x, int y, const vec2<int>& a, const vec2<int>& b, const vec2<int>& c)
{
    vec2i coord;
    fixed_uv(x, y, a.x, a.y, b.x, b.y, c.x, c.y, coord.x, coord.y);
    return coord;
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

    
// Determine whether point P in triangle ABC
bool PointinTriangle(vec3f A, vec3f B, vec3f C, vec3f P)
{
    vec3f v0 = C - A ;
    vec3f v1 = B - A ;
    vec3f v2 = P - A ;

    float dot00 = v0.dot(v0) ;
    float dot01 = v0.dot(v1) ;
    float dot02 = v0.dot(v2) ;
    float dot11 = v1.dot(v1) ;
    float dot12 = v1.dot(v2) ;

    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01) ;

    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno ;
    if (u < 0 || u > 1) // if u out of range, return directly
    {
        return false ;
    }

    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno ;
    if (v < 0 || v > 1) // if v out of range, return directly
    {
        return false ;
    }

    return u + v <= 1 ;
}
*/
