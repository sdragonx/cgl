/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 unit_move.hpp

 2019-05-26 23:36:48

 ��������������ڡ�Ӣ���޵�ex������
 xz λ��
 y  �߶�

*/
#ifndef UNIT_MOVE_HPP_20190526233648
#define UNIT_MOVE_HPP_20190526233648

#include <cgl/public.h>
#include <cgl/vec.hpp>

#ifdef CGL_PLATFORM_VCL
    #include <cgl/vcl.hpp>
#endif

namespace cgl{

//�ж�����λ���Ƿ���ͬ��ֻ�Ƚ�xz
template<typename T>
bool is_equal_position(const vec3<T>& p1, const vec3<T>& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

//��ɫ��ס������������й�
template<>
bool is_equal_position<float>(const vec3f& p1, const vec3f& p2)
{
    float x = p2.x - p1.x;
    float z = p2.z - p1.z;

    return (x * x + z * z) < 16.0f;// 0.0001f;
}

template<typename T1, typename T2>
void position_set(vec3<T1>& p, const vec2<T2>& source)
{
    p.x = source.x;
    p.y = 0;
    p.z = source.y;
}

template<typename T>
vec2<T> position_sub(const vec3<T>& p1, const vec3<T>& p2)
{
    return vec2<T>(p1.x - p2.x, p1.z - p2.z);
}

template<typename T>
T unit_distance(const vec3<T>& p1, const vec3<T>& p2)
{
    T x = p2.x - p1.x;
    T z = p2.z - p1.z;
    return sqrt(x * x + z * z);
}

//��ǰ�ƶ�һ��������ƶ�����С��speed�����ز�ֵ
template<typename T>
T move_step(T& x, T dest, T speed)
{
    if(!is_equal(x, dest)){
        if(x < dest){
            if(dest - x < speed){
                x = dest;
                return speed + x - dest;
            }
            else{
                x += speed;
            }
        }
        else {
            if(x - dest < speed){
                x = dest;
                return speed + x - dest;
            }
            else{
                x -= speed;
            }
        }
    }
    return 0;
}

//�������ȽϺ���
template<typename T>
int unit_move(vec3<T>& position, const vec3<T>& dest, T speed)
{
    move_step(position.y, dest.y, speed);

    if(position.x == dest.x && position.z == dest.z){
        return 0;
    }

    T cx = dest.x - position.x;
    T cz = dest.z - position.z;
    if(is_zero(cz)){
        return move_step(position.x, dest.x, speed);
    }
    else if(is_zero(cx)){
        return move_step(position.z, dest.z, speed);
    }
    else{
        T length = sqrt(float(cx * cx + cz * cz));//�������
        if(length < speed){
            position.x = dest.x;
            position.z = dest.z;
            return speed - length;
        }
        else{
            position.x += speed * cx / length;
            position.z += speed * cz / length;
        }
    }
    return 0;
}


template<typename T>
vec3<T> missle_move(vec3<T>& position, const vec3<T>& dest, T speed)
{
    vec3<T> distance = dest - position;
    T length = distance.length();
    if(length < speed){
        position = dest;
        return ;
    }

    if(!is_zero(distance.x))position.x += speed * distance.x / length;
    if(!is_zero(distance.y))position.y += speed * distance.y / length;
    if(!is_zero(distance.z))position.z += speed * distance.z / length;
    return distance;
}

template<>
vec3f missle_move<float>(vec3f& position, const vec3f& dest, float speed)
{
    vec3f distance = dest - position;
    float length = distance.length();
    if(length < speed){
        position = dest;
        return vec3f();
    }

    length = 1.0f / length;
    length *= speed;
    distance *= length;
    position += distance;

    return distance;
}


//�����ũ�ڵ������ǶȲ����������߶���Ϣ������̸߶ȵ�һЩ��
float connon_angle_step(const vec3f& start, const vec3f& end, float& step, float& height)
{
    float length = vec2f(end.x - start.x, end.z - start.z).length();
    if(!is_zero(length)){
        step = 180.0f / length;
        height = step < 1.0 ? height : height / step;
    }
    return length;
}

vec3f cannonball_move(vec3f& position, const vec3f& dest, float speed, float max_y, float angle_step)
{
    position.y = 0;
    vec3f distance = dest - position;
    float length = distance.length();
    if(length < speed){
        position = dest;
        return vec3f();
    }

    float inv_length = 1.0f / length;
    inv_length *= speed;
    distance *= inv_length;
    position += distance;

    //180�ȷ�Χ���Ҳ���������
    position.y = sin(length * angle_step * M_RD) * max_y;

    return distance;
}

class cannonball
{
public:
    vec3f position;
    vec3f dest;
    float height;    //���и߶�
    float step;

    cannonball()
    {

    }

    void attack(vec3f start, vec3f end)
    {
        position = start;
        dest = end;
        height = 100;//��ʼ�������и߶�
        connon_angle_step(position, dest, step, height);
    }

    void update()
    {
        cannonball_move(position, dest, 10, height, step);
    }
};

#ifdef CGL_PLATFORM_VCL
void draw_cannonball(TCanvas* dc, vec3f position, float size)
{
    //����Ӱ��
    dc->Brush->Color = clBlack;
    dc->Ellipse(position.x - size, position.z - size / 2, position.x + size, position.z + size / 2);

    //�����ڵ�
    dc->Brush->Color = clSilver;
    dc->Ellipse(position.x - size, position.z - size - position.y, position.x + size, position.z + size - position.y);
}
#endif //CGL_PLATFORM_VCL

}//end namespace cgl

#endif //UNIT_MOVE_HPP_20190526233648