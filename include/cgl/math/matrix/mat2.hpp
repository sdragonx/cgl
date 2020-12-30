/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 mat2.hpp

 2020-03-31 13:14:12

*/
#ifndef MAT2_HPP_20200331131412
#define MAT2_HPP_20200331131412

#include <cgl/public.h>
#include <cgl/vec.hpp>

namespace cgl{

/*
template<typename T>
inline void mat2_mul(const T* m1, const T* m2, T* dest)
{
    T product[4];

    product[0]  = m1[0] * m2[0]  + m1[2] * m2[1];
    product[1]  = m1[1] * m2[0]  + m1[3] * m2[1];

    product[2]  = m1[0] * m2[2]  + m1[2] * m2[3];
    product[3]  = m1[1] * m2[2]  + m1[3] * m2[3];

    memcpy(dest, product, sizeof(product));
}
*/

//
// mat2x3
//

template<typename T>
void mat2x3_mul(const T* m1, const T* m2, T* dest)
{
    T product[6];

    product[0] = m1[0] * m2[0] + m1[1] * m2[2];
    product[1] = m1[0] * m2[1] + m1[1] * m2[3];

    product[2] = m1[2] * m2[0] + m1[3] * m2[2];
    product[3] = m1[2] * m2[1] + m1[3] * m2[3];

    product[4] = m1[4] * m2[0] + m1[5] * m2[2] + m2[4];
    product[5] = m1[4] * m2[1] + m1[5] * m2[3] + m2[5];

    memcpy(dest, product, sizeof(product));
}

template<typename T>
void mat2x3_inverse(const T* m, T* dest)
{
    T product[6];

    T d = m[0] * m[3] - m[1] * m[2];

    if(is_zero(d)){
        return ;
    }

    d = 1.0 / d;

    product[0] =  m[3] * d;
    product[1] = -m[1] * d;
    product[2] = -m[2] * d;
    product[3] =  m[0] * d;
    product[4] = -m[4] * product[0] - m[5] * product[2];
    product[5] = -m[4] * product[1] - m[5] * product[3];

    memcpy(dest, product, sizeof(product));
}

template<typename T>
class mat2x3
{
public:
    typedef mat2x3 this_type;
    typedef T value_type;

    union{
        value_type data[6];
        value_type m[3][2];
    };

public:
    mat2x3()
    {
        this->identity();
    }

    mat2x3(
        value_type m00, value_type m01,
        value_type m10, value_type m11,
        value_type m20, value_type m21)
    {
        this->assign(m00, m01, m10, m11, m20, m21);
    }

    mat2x3(const this_type& mat)
    {
        this->assign(mat);
    }

    this_type& assign(
        value_type m00, value_type m01,
        value_type m10, value_type m11,
        value_type m20, value_type m21)
    {
        m[0][0] = m00; m[0][1] = m01;
        m[1][0] = m10; m[1][1] = m11;
        m[2][0] = m20; m[2][1] = m21;
        return *this;
    }

    this_type& assign(const this_type& mat)
    {
        memcpy(data, mat.data, sizeof(data));
        return *this;
    }

    this_type& operator=(const this_type& mat)
    {
        return this->assign(mat);
    }

    value_type& operator[](int n)
    {
        return data[n];
    }

    const value_type& operator[](int n)const
    {
        return data[n];
    }

    void identity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f;
    }

    this_type& translate(value_type x, value_type y)
    {
        m[2][0] += x;
        m[2][1] += y;
        return *this;
    }

    this_type& scale(value_type x)
    {
        return this->scale(x, x);
    }

    this_type& scale(value_type x, value_type y)
    {
        m[0][0] *= x; m[0][1] *= y;
        m[1][0] *= x; m[1][1] *= y;
        m[2][0] *= x; m[2][1] *= y;
        return *this;
    }

    this_type& rotate(value_type angle)
    {
        value_type cosine = cos(angle * M_RD);
        value_type sine   = sin(angle * M_RD);
        value_type mat[6] = {cosine, sine, -sine, cosine, 0.0f, 0.0f};
        mat2x3_mul(data, mat, data);
        return *this;
    }

    void invert()
    {
        mat2x3_inverse(data, data);
    }

    value_type determinant()
    {
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    }

    void invert2()
    {
        float d = determinant();
        d = 1.0 / d;

        double t0  =  m[1][1]  * d;
        m[1][1] =  m[0][0] * d;
        m[0][1] = -m[0][1] * d;
        m[1][0] = -m[1][0] * d;

        double t4 = -m[2][0] * t0 - m[2][1] * m[1][0];
        m[2][1] = -m[2][0] * m[0][1] - m[2][1] * m[1][1];
        m[0][0] = t0;
        m[2][0] = t4;
    }

    void transform(value_type x, value_type y, value_type& dest_x, value_type& dest_y)
    {
        dest_x = x * m[0][0] + y * m[1][0] + m[2][0];
        dest_y = x * m[0][1] + y * m[1][1] + m[2][1];
    }

    vec2<value_type> transform(value_type x, value_type y)
    {
        value_type dest_x = x * m[0][0] + y * m[1][0] + m[2][0];
        value_type dest_y = x * m[0][1] + y * m[1][1] + m[2][1];
        return vec2f(dest_x, dest_y);
    }

    vec2<value_type> transform(const vec2<value_type>& p)
    {
        return this->transform(p.x, p.y);
    }
};



class fixed_mat
{
public:
    typedef fixed_mat this_type;
    typedef int value_type;

    union{
        value_type data[6];
        value_type m[3][2];
    };

public:
    this_type& assign(const mat2x3<float>& mat)
    {
        m[0][0] = round(mat.m[0][0] * 65536); m[0][1] = round(mat.m[0][1] * 65536);
        m[1][0] = round(mat.m[1][0] * 65536); m[1][1] = round(mat.m[1][1] * 65536);
        m[2][0] = round(mat.m[2][0] * 65536); m[2][1] = round(mat.m[2][1] * 65536);
        return *this;
    }

    void transform(value_type x, value_type y, value_type& dest_x, value_type& dest_y)
    {
        dest_x = x * m[0][0] + y * m[1][0] + m[2][0];
        dest_y = x * m[0][1] + y * m[1][1] + m[2][1];
    }
};


typedef mat2x3<int>   mat2i;
typedef mat2x3<float> mat2f;

}//end namespace cgl

#endif //MAT2_HPP_20200331131412
