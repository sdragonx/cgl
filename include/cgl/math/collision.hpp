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

//�߶κ�δ��ת�ľ�����ײ
//������ײ�ߣ�0Ϊδ��ײ����ֵΪ������ھ����ڲ�
//������ȫ�ھ����ڲ�δ���
//  1
//8   2
//  4
//���p1�ھ����ڲ�������

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

//    if(p1.x > x1 && p1.y > y1 && p1.x < x2 && p1.y < y2){//������ھ����ڲ�
//        if(p2.x > x1 && p2.y > y1 && p2.x < x2 && p2.y < y2){//���㶼�ھ����ڲ�
//            return -15;
//        }
//    }

    //�������߼��
    if(p1.x < p2.x){//������
        if(p1.x < x1){
            if(p2.x < x1){//ֱ���ھ������
                return 0;
            }
            x = x1 - p1.x;
            n = 8;
        }
        else if(p1.y > y1 && p1.y < y2){//�ڲ��ұ߼��
            if(p1.x > x2){
                return 0;
            }
            x = x2 - p1.x;
            n = -2;
        }
    }
    else{//���ҵ���
        if(p1.x > x2){
            if(p2.x > x2){//ֱ���ھ����Ҳ�
                return 0;
            }
            x = x2 - p1.x;
            n = 2;
        }
        else if(p1.y > y1 && p1.y < y2){//�ڲ���߼��
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

    //�������߼��
    if(p1.y < p2.y){
        if(p1.y < y1){
            if(p2.y < y1){//�����Ϸ�
            return 0;
            }
            y = y1 - p1.y;
            n = 1;
        }
        else{//p1�ھ����ڲ�
            if(p1.y > y2){
                return 0;
            }
            y = y2 - p1.y;
            n = -4;
        }
    }
    else{//�������ϼ��
        if(p1.y > y2){
            if(p2.y > y2){//�����·�
                return 0;
            }
            y = y2 - p1.y;
            n = 4;
        }
        else{//p1�ھ����ڲ�
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
// ��p1��p2����һ�����ߣ�������ߺ;�����ײ
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

//    if(p1.x > x1 && p1.y > y1 && p1.x < x2 && p1.y < y2){//������ھ����ڲ�
//        if(p2.x > x1 && p2.y > y1 && p2.x < x2 && p2.y < y2){//���㶼�ھ����ڲ�
//            return -15;
//        }
//    }

    //�������߼��
    if(p1.x < p2.x){//������
        if(p1.x < x1){
            if(p2.x < x1){//ֱ���ھ������
                //return 0;
            }
            x = x1 - p1.x;
            n = 8;
        }
        else if(p1.y > y1 && p1.y < y2){//�ڲ��ұ߼��
            if(p1.x > x2){
                return 0;
            }
            x = x2 - p1.x;
            n = -2;
        }
    }
    else{//���ҵ���
        if(p1.x > x2){
            if(p2.x > x2){//ֱ���ھ����Ҳ�
                //return 0;
            }
            x = x2 - p1.x;
            n = 2;
        }
        else if(p1.y > y1 && p1.y < y2){//�ڲ���߼��
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

    //�������߼��
    if(p1.y < p2.y){
        if(p1.y < y1){
            if(p2.y < y1){//�����Ϸ�
                //return 0;
            }
            y = y1 - p1.y;
            n = 1;
        }
        else{//p1�ھ����ڲ�
            if(p1.y > y2){
                return 0;
            }
            y = y2 - p1.y;
            n = -4;
        }
    }
    else{//�������ϼ��
        if(p1.y > y2){
            if(p2.y > y2){//�����·�
                //return 0;
            }
            y = y2 - p1.y;
            n = 4;
        }
        else{//p1�ھ����ڲ�
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


//ֱ������ת�ľ�����ײ
template<typename T>
bool ray_collision(
    vec4<T> rc,        //����
    float rotate,    //��ת�Ƕ�
    vec2<T> p1,        //ֱ��
    vec2<T> p2,
    vec2<T>& p)        //���ص�һ������
{
    vec2i center = rc.center();
    p1 -= rc.center();
    p2 -= rc.center();

    rc.offset(-center);

    p1.rotate(-rotate);//�˶�ʸ��
    p2.rotate(-rotate);

    bool b = math::ray_collision(rc, p1, p2, p);

    p.rotate(rotate);
    p += center;

    return b;
}

}//end namespace math
}//end namespace cgl

#endif //COLLISION_HPP_20190603162206
