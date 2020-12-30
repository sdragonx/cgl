/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 line_collision.hpp

 2019-06-03 16:22:06

*/
#ifndef COLLISION_HPP_20190603162206
#define COLLISION_HPP_20190603162206

#include <cgl/public.h>

namespace cgl{
namespace math{

//线段和未旋转的矩形碰撞
//返回碰撞边，0为未碰撞，负值为发射点在矩形内部
//两点完全在矩形内部未检测
//  1
//8   2
//  4
//如果p1在矩形内部，……

template<typename T>
int line_collision(const vec4<T>& rc, const vec2<T>& p1, const vec2<T>& p2, vec2<T>& p)
{
    T x1 = rc.x;
    T x2 = rc.x + rc.width;
    T y1 = rc.y;
    T y2 = rc.y + rc.height;

    T x, y;
    T w = p2.x - p1.x;
    T h = p2.y - p1.y;
    T n = 0;

//    if(p1.x > x1 && p1.y > y1 && p1.x < x2 && p1.y < y2){//发射点在矩形内部
//        if(p2.x > x1 && p2.y > y1 && p2.x < x2 && p2.y < y2){//两点都在矩形内部
//            return -15;
//        }
//    }

    //左右两边检测
    if(p1.x < p2.x){//从左到右
        if(p1.x < x1){
            if(p2.x < x1){//直线在矩形左侧
                return 0;
            }
            x = x1 - p1.x;
            n = 8;
        }
        else if(p1.y > y1 && p1.y < y2){//内部右边检测
            if(p1.x > x2){
                return 0;
            }
            x = x2 - p1.x;
            n = -2;
        }
    }
    else{//从右到左
        if(p1.x > x2){
            if(p2.x > x2){//直线在矩形右侧
                return 0;
            }
            x = x2 - p1.x;
            n = 2;
        }
        else if(p1.y > y1 && p1.y < y2){//内部左边检测
            if(p1.x < x1){
                return 0;
            }
            x = x1 - p1.x;
            n = -8;
        }
    }

    if(n){
        y = w == 0 ? x : x * h / w;
        x += p1.x;
        y += p1.y;
        if(y > y1 && y < y2){
            p = vec2i(x, y);
            return n;
        }
    }

    //上下两边检测
    if(p1.y < p2.y){
        if(p1.y < y1){
            if(p2.y < y1){//矩形上方
            return 0;
            }
            y = y1 - p1.y;
            n = 1;
        }
        else{//p1在矩形内部
            if(p1.y > y2){
                return 0;
            }
            y = y2 - p1.y;
            n = -4;
        }
    }
    else{//从下向上检测
        if(p1.y > y2){
            if(p2.y > y2){//矩形下方
                return 0;
            }
            y = y2 - p1.y;
            n = 4;
        }
        else{//p1在矩形内部
            y = y1 - p1.y;
            n = -1;
        }
    }
    x = h == 0 ? y : y * w / h;
    x += p1.x;
    y += p1.y;
    if(x > x1 && x < x2){
        p = vec2i(x, y);
        return n;
    }

    return 0;
}

//
// 从p1向p2发射一条射线，检测射线和矩形碰撞
//

template<typename T>
int ray_collision(const vec4<T>& rc, const vec2<T>& p1, const vec2<T>& p2, vec2<T>& p)
{
    T x1 = rc.x;
    T x2 = rc.x + rc.width;
    T y1 = rc.y;
    T y2 = rc.y + rc.height;

    T x, y;
    T w = p2.x - p1.x;
    T h = p2.y - p1.y;
    T n = 0;

//    if(p1.x > x1 && p1.y > y1 && p1.x < x2 && p1.y < y2){//发射点在矩形内部
//        if(p2.x > x1 && p2.y > y1 && p2.x < x2 && p2.y < y2){//两点都在矩形内部
//            return -15;
//        }
//    }

    //左右两边检测
    if(p1.x < p2.x){//从左到右
        if(p1.x < x1){
            if(p2.x < x1){//直线在矩形左侧
                //return 0;
            }
            x = x1 - p1.x;
            n = 8;
        }
        else if(p1.y > y1 && p1.y < y2){//内部右边检测
            if(p1.x > x2){
                return 0;
            }
            x = x2 - p1.x;
            n = -2;
        }
    }
    else{//从右到左
        if(p1.x > x2){
            if(p2.x > x2){//直线在矩形右侧
                //return 0;
            }
            x = x2 - p1.x;
            n = 2;
        }
        else if(p1.y > y1 && p1.y < y2){//内部左边检测
            if(p1.x < x1){
                return 0;
            }
            x = x1 - p1.x;
            n = -8;
        }
    }

    if(n){
        y = w == 0 ? x : x * h / w;
        x += p1.x;
        y += p1.y;
        if(y > y1 && y < y2){
            p = vec2i(x, y);
            return n;
        }
    }

    //上下两边检测
    if(p1.y < p2.y){
        if(p1.y < y1){
            if(p2.y < y1){//矩形上方
                //return 0;
            }
            y = y1 - p1.y;
            n = 1;
        }
        else{//p1在矩形内部
            if(p1.y > y2){
                return 0;
            }
            y = y2 - p1.y;
            n = -4;
        }
    }
    else{//从下向上检测
        if(p1.y > y2){
            if(p2.y > y2){//矩形下方
                //return 0;
            }
            y = y2 - p1.y;
            n = 4;
        }
        else{//p1在矩形内部
            y = y1 - p1.y;
            n = -1;
        }
    }
    x = h == 0 ? y : y * w / h;
    x += p1.x;
    y += p1.y;
    if(x > x1 && x < x2){
        p = vec2i(x, y);
        return n;
    }

    return 0;
}


//直线与旋转的矩形碰撞
template<typename T>
bool ray_collision(
    vec4<T> rc,        //矩形
    float rotate,    //旋转角度
    vec2<T> p1,        //直线
    vec2<T> p2,
    vec2<T>& p)        //返回第一个交点
{
    vec2i center = rc.center();
    p1 -= rc.center();
    p2 -= rc.center();

    rc.offset(-center);

    p1.rotate(-rotate);//运动矢量
    p2.rotate(-rotate);

    bool b = math::ray_collision(rc, p1, p2, p);

    p.rotate(rotate);
    p += center;

    return b;
}

}//end namespace math
}//end namespace cgl

#endif //COLLISION_HPP_20190603162206
