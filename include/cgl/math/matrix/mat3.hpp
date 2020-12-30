/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 mat3.hpp

 2019-05-30 02:06:08

*/
#ifndef MAT3_HPP_20190530020608
#define MAT3_HPP_20190530020608

#include <cgl/public.h>
#include <cgl/vec.hpp>
#include "base.hpp"

namespace cgl{

//
// mat3x3
//

template<typename T>
inline void mat3_mul(const T* m1, const T* m2, T* dest)
{
    T product[9];

    product[0]  = m1[0] * m2[0]  + m1[3] * m2[1] + m1[6] * m2[2];
    product[1]  = m1[1] * m2[0]  + m1[4] * m2[1] + m1[7] * m2[2];
    product[2]  = m1[2] * m2[0]  + m1[5] * m2[1] + m1[8] * m2[2];

    product[3]  = m1[0] * m2[3]  + m1[3] * m2[4] + m1[6] * m2[5];
    product[4]  = m1[1] * m2[3]  + m1[4] * m2[4] + m1[7] * m2[5];
    product[5]  = m1[2] * m2[3]  + m1[5] * m2[4] + m1[8] * m2[5];

    product[6]  = m1[0] * m2[6]  + m1[3] * m2[7] + m1[6] * m2[8];
    product[7]  = m1[1] * m2[6]  + m1[4] * m2[7] + m1[7] * m2[8];
    product[8]  = m1[2] * m2[6]  + m1[5] * m2[7] + m1[8] * m2[8];

    memcpy(dest, product, sizeof(product));
}



template<typename T>
class mat3// : public matrix<T, 3, 3>
{
public:
public:
    typedef mat3 this_type;
    typedef float value_type;

    enum{ size = 9 };
public:
    ///*
    union{
        value_type data[size];
        value_type m[3][3];
    };
    //*/

public:
    mat3(){ /*void*/ }
    mat3(value_type m00, value_type m01, value_type m02,
        value_type m10, value_type m11, value_type m12,
        value_type m20, value_type m21, value_type m22)
    {
        this->assign(m00, m01, m02, m10, m11, m12, m20, m21, m22);
    }

    T* operator[](size_t i)
    {
        return m[i];
    }

    T& operator()(size_t x, size_t y)
    {
        return m[y][x];
    }

    const T& operator()(size_t x, size_t y)const
    {
        return m[y][x];
    }

    this_type& assign(
        value_type m00, value_type m01, value_type m02,
        value_type m10, value_type m11, value_type m12,
        value_type m20, value_type m21, value_type m22)
    {
        data[0] = m00; data[3] = m01; data[6] = m02;
        data[1] = m10; data[4] = m11; data[7] = m12;
        data[2] = m20; data[5] = m21; data[8] = m22;

        return *this;
    }

    void identity()
    {
        this->assign(
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
            );
    }

    this_type& mul(const this_type& mat)
    {
        mat3_mul(data, mat.data, data);
        return *this;
    }

    this_type& translate(value_type x, value_type y, value_type z)
    {

    }

    this_type& scale(value_type x, value_type y, value_type z)
    {

    }

    this_type& rotate(value_type angle, value_type x, value_type y, value_type z)
    {
        vec3f u(x, y, z);
        u.normalize();
        float theta = static_cast<value_type>(angle * (M_PI / 180.0f));
        float cos_t = cos(theta);
        float sin_t = sin(theta);
        float even_cos = 1.0f - cos_t;//even function: f(x)=f(-x)

        this_type mat(
            cos_t + (u.x * u.x * even_cos),
            (u.x * u.y * even_cos) - (u.z * sin_t),
            (u.x * u.z * even_cos) + (u.y * sin_t),

            (u.y * u.x * even_cos) + (u.z * sin_t),
            cos_t + (u.y * u.y * even_cos),
            (u.y * u.z * even_cos) - (u.x * sin_t),

            (u.z * u.x * even_cos) - (u.y * sin_t),
            (u.z * u.y * even_cos) + (u.x * sin_t),
            cos_t + (u.z * u.z * even_cos));

        return this->mul(mat);
    }

    value_type determinant()const
    {
        return 
            m[0][0] * m[1][1] * m[2][2] -
            m[0][0] * m[1][2] * m[2][1] - 
            m[0][1] * m[1][0] * m[2][2] + 
            m[0][1] * m[1][2] * m[0][2] +
            m[0][2] * m[1][0] * m[2][1] -
            m[0][2] * m[1][1] * m[2][0];
    }

    this_type inverse()
    {
        T d = determinant();
        this_type temp;
        if(is_zero(d)){
            return temp;
        }

        temp.m[0][0] =  m[2][2]*m[1][1]-m[2][1]*m[1][2];
        temp.m[0][1] = -m[2][2]*m[0][2]-m[2][1]*m[0][2];
        temp.m[0][2] =  m[1][2]*m[0][2]-m[1][1]*m[0][2];
        temp.m[1][0] = -m[2][2]*m[1][0]-m[2][0]*m[1][2];
        temp.m[1][1] =  m[2][2]*m[0][0]-m[2][0]*m[0][2];
        temp.m[1][2] = -m[1][2]*m[0][0]-m[1][0]*m[0][2];
        temp.m[2][0] =  m[2][1]*m[1][0]-m[2][0]*m[1][1];
        temp.m[2][1] = -m[2][1]*m[0][0]-m[2][0]*m[0][1];
        temp.m[2][2] =  m[1][1]*m[0][0]-m[1][0]*m[0][1];

        temp /= d;

        return temp;
    }

    vec3f transform(float x, float y, float z)const
    {
        return vec3f(
            data[0] * x + data[3] * y + data[6] * z,
            data[1] * x + data[4] * y + data[7] * z,
            data[2] * x + data[5] * y + data[8] * z);
    }
};

typedef mat3<int>   mat3i;
typedef mat3<float> mat3f;

}//end namespace cgl

#endif //MAT3_HPP_20190530020608
