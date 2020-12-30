/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 mat4.hpp

 2018-04-15 19:45:04

void print(mat4f& m)
{
    CGL_LOG("\r\n"
        "%f, %f, %f, %f\r\n"
        "%f, %f, %f, %f\r\n"
        "%f, %f, %f, %f\r\n"
        "%f, %f, %f, %f",
        m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
        m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
        m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
        m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
}

*/
#ifndef MATRIX_HPP_20180415194504
#define MATRIX_HPP_20180415194504

#include <cgl/math.hpp>
#include <cgl/vec.hpp>
#include <cgl/vec_utils.hpp>

namespace cgl{

template<typename T>
inline void mat4_add(const T* m, T scalar, T* dest)
{
    dest[0]  = m[0]  + scalar;
    dest[1]  = m[1]  + scalar;
    dest[2]  = m[2]  + scalar;
    dest[3]  = m[3]  + scalar;
    dest[4]  = m[4]  + scalar;
    dest[5]  = m[5]  + scalar;
    dest[6]  = m[6]  + scalar;
    dest[7]  = m[7]  + scalar;
    dest[8]  = m[8]  + scalar;
    dest[9]  = m[9]  + scalar;
    dest[10] = m[10] + scalar;
    dest[11] = m[11] + scalar;
    dest[12] = m[12] + scalar;
    dest[13] = m[13] + scalar;
    dest[14] = m[14] + scalar;
    dest[15] = m[15] + scalar;
}

template<typename T>
inline void mat4_add(const T* m1, const T* m2, T* dest)
{
    dest[0]  = m1[0]  + m2[0];
    dest[1]  = m1[1]  + m2[1];
    dest[2]  = m1[2]  + m2[2];
    dest[3]  = m1[3]  + m2[3];
    dest[4]  = m1[4]  + m2[4];
    dest[5]  = m1[5]  + m2[5];
    dest[6]  = m1[6]  + m2[6];
    dest[7]  = m1[7]  + m2[7];
    dest[8]  = m1[8]  + m2[8];
    dest[9]  = m1[9]  + m2[9];
    dest[10] = m1[10] + m2[10];
    dest[11] = m1[11] + m2[11];
    dest[12] = m1[12] + m2[12];
    dest[13] = m1[13] + m2[13];
    dest[14] = m1[14] + m2[14];
    dest[15] = m1[15] + m2[15];
}

template<typename T>
inline void mat4_sub(const T* m1, const T* m2, T* dest)
{
    dest[0]  = m1[0]  - m2[0];
    dest[1]  = m1[1]  - m2[1];
    dest[2]  = m1[2]  - m2[2];
    dest[3]  = m1[3]  - m2[3];
    dest[4]  = m1[4]  - m2[4];
    dest[5]  = m1[5]  - m2[5];
    dest[6]  = m1[6]  - m2[6];
    dest[7]  = m1[7]  - m2[7];
    dest[8]  = m1[8]  - m2[8];
    dest[9]  = m1[9]  - m2[9];
    dest[10] = m1[10] - m2[10];
    dest[11] = m1[11] - m2[11];
    dest[12] = m1[12] - m2[12];
    dest[13] = m1[13] - m2[13];
    dest[14] = m1[14] - m2[14];
    dest[15] = m1[15] - m2[15];
}

template<typename T>
inline void mat4_mul(const T* m, T scalar, T* dest)
{
    dest[0]  = m[0]  * scalar;
    dest[1]  = m[1]  * scalar;
    dest[2]  = m[2]  * scalar;
    dest[3]  = m[3]  * scalar;
    dest[4]  = m[4]  * scalar;
    dest[5]  = m[5]  * scalar;
    dest[6]  = m[6]  * scalar;
    dest[7]  = m[7]  * scalar;
    dest[8]  = m[8]  * scalar;
    dest[9]  = m[9]  * scalar;
    dest[10] = m[10] * scalar;
    dest[11] = m[11] * scalar;
    dest[12] = m[12] * scalar;
    dest[13] = m[13] * scalar;
    dest[14] = m[14] * scalar;
    dest[15] = m[15] * scalar;
}

template<typename T>
inline void mat4_mul(const T* m1, const T* m2, T* dest)
{
    T product[16];

    product[0]  = m1[0] * m2[0]  + m1[4] * m2[1]  + m1[8]  * m2[2]  + m1[12] * m2[3];
    product[1]  = m1[1] * m2[0]  + m1[5] * m2[1]  + m1[9]  * m2[2]  + m1[13] * m2[3];
    product[2]  = m1[2] * m2[0]  + m1[6] * m2[1]  + m1[10] * m2[2]  + m1[14] * m2[3];
    product[3]  = m1[3] * m2[0]  + m1[7] * m2[1]  + m1[11] * m2[2]  + m1[15] * m2[3];

    product[4]  = m1[0] * m2[4]  + m1[4] * m2[5]  + m1[8]  * m2[6]  + m1[12] * m2[7];
    product[5]  = m1[1] * m2[4]  + m1[5] * m2[5]  + m1[9]  * m2[6]  + m1[13] * m2[7];
    product[6]  = m1[2] * m2[4]  + m1[6] * m2[5]  + m1[10] * m2[6]  + m1[14] * m2[7];
    product[7]  = m1[3] * m2[4]  + m1[7] * m2[5]  + m1[11] * m2[6]  + m1[15] * m2[7];

    product[8]  = m1[0] * m2[8]  + m1[4] * m2[9]  + m1[8]  * m2[10] + m1[12] * m2[11];
    product[9]  = m1[1] * m2[8]  + m1[5] * m2[9]  + m1[9]  * m2[10] + m1[13] * m2[11];
    product[10] = m1[2] * m2[8]  + m1[6] * m2[9]  + m1[10] * m2[10] + m1[14] * m2[11];
    product[11] = m1[3] * m2[8]  + m1[7] * m2[9]  + m1[11] * m2[10] + m1[15] * m2[11];

    product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];

    memcpy(dest, product, sizeof(product));
}

template<typename T>
bool mat4_inverse(const T* m, T* dest)
{
    T a0 = m[0]  * m[5]  - m[1]  * m[4];
    T a1 = m[0]  * m[6]  - m[2]  * m[4];
    T a2 = m[0]  * m[7]  - m[3]  * m[4];
    T a3 = m[1]  * m[6]  - m[2]  * m[5];
    T a4 = m[1]  * m[7]  - m[3]  * m[5];
    T a5 = m[2]  * m[7]  - m[3]  * m[6];
    T b0 = m[8]  * m[13] - m[9]  * m[12];
    T b1 = m[8]  * m[14] - m[10] * m[12];
    T b2 = m[8]  * m[15] - m[11] * m[12];
    T b3 = m[9]  * m[14] - m[10] * m[13];
    T b4 = m[9]  * m[15] - m[11] * m[13];
    T b5 = m[10] * m[15] - m[11] * m[14];

    // Calculate the determinant.
    T det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // Close to zero, can't invert.
    if (fabs(det) <= 0)// MATH_TOLERANCE)
        return false;

    // Support the case where m == dest.
    T inverse[16];
    inverse[0]  =  m[5]  * b5 - m[6]  * b4 + m[7]  * b3;
    inverse[1]  = -m[1]  * b5 + m[2]  * b4 - m[3]  * b3;
    inverse[2]  =  m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse[3]  = -m[9]  * a5 + m[10] * a4 - m[11] * a3;

    inverse[4]  = -m[4]  * b5 + m[6]  * b2 - m[7]  * b1;
    inverse[5]  =  m[0]  * b5 - m[2]  * b2 + m[3]  * b1;
    inverse[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse[7]  =  m[8]  * a5 - m[10] * a2 + m[11] * a1;

    inverse[8]  =  m[4]  * b4 - m[5]  * b2 + m[7]  * b0;
    inverse[9]  = -m[0]  * b4 + m[1]  * b2 - m[3]  * b0;
    inverse[10] =  m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse[11] = -m[8]  * a4 + m[9]  * a2 - m[11] * a0;

    inverse[12] = -m[4]  * b3 + m[5]  * b1 - m[6]  * b0;
    inverse[13] =  m[0]  * b3 - m[1]  * b1 + m[2]  * b0;
    inverse[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse[15] =  m[8]  * a3 - m[9]  * a1 + m[10] * a0;

    mat4_mul(inverse, 1.0f / det, dest);

    return true;
}

template<typename T>
inline void mat4_transpose(const T* m, T* dest)
{
    T t[16] = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(dest, t, sizeof(t));
}

inline void transformVec4(const float* m, float x, float y, float z, float w, float* dest)
{
    dest[0] = x * m[0] + y * m[4] + z * m[8]  + w * m[12];
    dest[1] = x * m[1] + y * m[5] + z * m[9]  + w * m[13];
    dest[2] = x * m[2] + y * m[6] + z * m[10] + w * m[14];
}

inline void transformVec4(const float* m, const float* v, float* dest)
{
    // Handle case where v == dest.
    float x = v[0] * m[0] + v[1] * m[4] + v[2] * m[8]  + v[3] * m[12];
    float y = v[0] * m[1] + v[1] * m[5] + v[2] * m[9]  + v[3] * m[13];
    float z = v[0] * m[2] + v[1] * m[6] + v[2] * m[10] + v[3] * m[14];
    float w = v[0] * m[3] + v[1] * m[7] + v[2] * m[11] + v[3] * m[15];

    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    dest[3] = w;
}



template<typename T>
class mat4
{
public:
    typedef mat4<T> this_type;
    typedef T value_type;

    enum{ size = 16 };

    //const static value_type IDENTITY[16];
public:
    union{
        value_type data[size];
        value_type m[4][4];
    };

public:
    mat4()
    {
        memset(data, 0, sizeof(data));
    }

    mat4(
        value_type m00, value_type m01, value_type m02, value_type m03,
        value_type m10, value_type m11, value_type m12, value_type m13,
        value_type m20, value_type m21, value_type m22, value_type m23,
        value_type m30, value_type m31, value_type m32, value_type m33)
    {
        data[0] = m00; data[4] = m01; data[8]  = m02; data[12] = m03;
        data[1] = m10; data[5] = m11; data[9]  = m12; data[13] = m13;
        data[2] = m20; data[6] = m21; data[10] = m22; data[14] = m23;
        data[3] = m30; data[7] = m31; data[11] = m32; data[15] = m33;
    }

    mat4(const this_type& mat)
    {
        memcpy(data, mat.data, sizeof(data));
    }

public://assign
    this_type& assign(const value_type* mat)
    {
        memcpy(data, mat, sizeof(data));
    }

    this_type& assign(
        value_type m00, value_type m01, value_type m02, value_type m03,
        value_type m10, value_type m11, value_type m12, value_type m13,
        value_type m20, value_type m21, value_type m22, value_type m23,
        value_type m30, value_type m31, value_type m32, value_type m33)
    {
        data[0] = m00; data[4] = m01; data[8]  = m02; data[12] = m03;
        data[1] = m10; data[5] = m11; data[9]  = m12; data[13] = m13;
        data[2] = m20; data[6] = m21; data[10] = m22; data[14] = m23;
        data[3] = m30; data[7] = m31; data[11] = m32; data[15] = m33;
        return *this;
    }

    /*
    this_type& assign(
        value_type m00, value_type m01, value_type m02, value_type m03,
        value_type m10, value_type m11, value_type m12, value_type m13,
        value_type m20, value_type m21, value_type m22, value_type m23,
        value_type m30, value_type m31, value_type m32, value_type m33)
    {
        m[0][0] = m00; m[1][0]  = m01; m[2][0] = m02; m[3][0] = m03;
        m[0][1] = m10; m[1][1]  = m11; m[2][1] = m12; m[3][1] = m13;
        m[0][2] = m20; m[1][2]  = m21; m[2][2] = m22; m[3][2] = m23;
        m[0][3] = m30; m[1][3]  = m31; m[2][3] = m32; m[3][3] = m33;
    }
    */

    this_type& operator=(const this_type& mat)
    {
        memcpy(data, mat.data, sizeof(data));
        return *this;
    }

    value_type& operator[](size_t index)
    {
        return data[index];
    }

    const value_type& operator[](size_t index)const
    {
        return data[index];
    }

public://operating
    this_type& add(value_type value)
    {
        mat4_add(data, value, data);
        return *this;
    }

    this_type& add(const this_type& mat)
    {
        mat4_add(data, mat.data, data);
        return *this;
    }

    this_type& multiply(value_type value)
    {
        mat4_mul(data, value, data);
        return *this;
    }

    this_type& multiply(const this_type& mat)
    {
        mat4_mul(data, mat.data, data);
        return *this;
    }

    this_type operator*(value_type value)
    {
        return this_type(*this).multiply(value);
    }

    this_type& operator*=(value_type value)
    {
        return this->multiply(value);
    }

    this_type operator*(const this_type& mat)const
    {
        /*return this_type(
            data[0] * mat.data[0] + data[4] * mat.data[1] + data[8] * mat.data[2] + data[12] * mat.data[3],
            data[0] * mat.data[4] + data[4] * mat.data[5] + data[8] * mat.data[6] + data[12] * mat.data[7],
            data[0] * mat.data[8] + data[4] * mat.data[9] + data[8] * mat.data[10] + data[12] * mat.data[11],
            data[0] * mat.data[12] + data[4] * mat.data[13] + data[8] * mat.data[14] + data[12] * mat.data[15],
            data[1] * mat.data[0] + data[5] * mat.data[1] + data[9] * mat.data[2] + data[13] * mat.data[3],
            data[1] * mat.data[4] + data[5] * mat.data[5] + data[9] * mat.data[6] + data[13] * mat.data[7],
            data[1] * mat.data[8] + data[5] * mat.data[9] + data[9] * mat.data[10] + data[13] * mat.data[11],
            data[1] * mat.data[12] + data[5] * mat.data[13] + data[9] * mat.data[14] + data[13] * mat.data[15],
            data[2] * mat.data[0] + data[6] * mat.data[1] + data[10] * mat.data[2] + data[14] * mat.data[3],
            data[2] * mat.data[4] + data[6] * mat.data[5] + data[10] * mat.data[6] + data[14] * mat.data[7],
            data[2] * mat.data[8] + data[6] * mat.data[9] + data[10] * mat.data[10] + data[14] * mat.data[11],
            data[2] * mat.data[12] + data[6] * mat.data[13] + data[10] * mat.data[14] + data[14] * mat.data[15],
            data[3] * mat.data[0] + data[7] * mat.data[1] + data[11] * mat.data[2] + data[15] * mat.data[3],
            data[3] * mat.data[4] + data[7] * mat.data[5] + data[11] * mat.data[6] + data[15] * mat.data[7],
            data[3] * mat.data[8] + data[7] * mat.data[9] + data[11] * mat.data[10] + data[15] * mat.data[11],
            data[3] * mat.data[12] + data[7] * mat.data[13] + data[11] * mat.data[14] + data[15] * mat.data[15]);
        */
        this_type temp;
        mat4_mul(data, mat.data, temp.data);
        return temp;
    }

    this_type& operator*=(const this_type& mat)
    {
        return this->multiply(mat);
    }

public://equal
    bool operator==(const this_type& mat)
    {
        for(size_t i = 0; i < size; ++i){
            if(!is_equal(data[i], mat.data[i])){
                return false;
            }
        }
        return true;
    }

    bool operator!=(const this_type& mat)
    {
        return !(*this == mat);
    }

public://matrix
    this_type& identity()
    {
        return this->assign(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    this_type& rotate(value_type angle, value_type x, value_type y, value_type z)
    {
        vec3f u(x, y, z);
        u.normalize();
        angle *= M_RD;
        float cosine = cos(angle);
        float sine = sin(angle);

        /*
        float even_cos = 1.0f - cosine;//even function: f(x)=f(-x)

        this_type mat(
            cosine + (u.x * u.x * even_cos), (u.x * u.y * even_cos) - (u.z * sine),
            (u.x * u.z * even_cos) + (u.y * sine), 0.0f,
            (u.y * u.x * even_cos) + (u.z * sine), cosine + (u.y * u.y * even_cos),
            (u.y * u.z * even_cos) - (u.x * sine), 0.0f,
            (u.z * u.x * even_cos) - (u.y * sine), (u.z * u.y * even_cos) + (u.x * sine),
            cosine + (u.z * u.z * even_cos), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
        */

        vec3f t = u * (1.0f - cosine);

        this_type mat(
            cosine + (u.x * t.x), (u.x * t.y) - (u.z * sine), (u.x * t.z) + (u.y * sine), 0.0f,
            (u.y * t.x) + (u.z * sine), cosine + (u.y * t.y), (u.y * t.z) - (u.x * sine), 0.0f,
            (u.z * t.x) - (u.y * sine), (u.z * t.y) + (u.x * sine), cosine + (u.z * t.z), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);

        return this->multiply(mat);
    }

    this_type& rotate(value_type angle, const vec3f& v)
    {
        return this->rotate(angle, v.x, v.y, v.z);
    }

    this_type& translate(value_type x, value_type y, value_type z)
    {
        this_type mat(
            1.0f, 0.0f, 0.0f, x,
            0.0f, 1.0f, 0.0f, y,
            0.0f, 0.0f, 1.0f, z,
            0.0f, 0.0f, 0.0f, 1.0f);
        return this->multiply(mat);
    }

    this_type& translate(const vec3f& v)
    {
        return this->translate(v.x, v.y, v.z);
    }

    this_type& scale(value_type x, value_type y, value_type z)
    {
        this_type mat(
               x, 0.0f, 0.0f, 0.0f,
            0.0f,    y, 0.0f, 0.0f,
            0.0f, 0.0f,    z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
        return this->multiply(mat);
    }

    this_type& scale(const vec3f& v)
    {
        return this->scale(v.x, v.y, v.z);
    }

    void viewport(value_type x, value_type y, value_type w, value_type h)
	{
		#if 1
		w *= 0.5;
		h *= 0.5;
		this->assign(
			w, 0.0, 0.0, x + w,
			0.0, h, 0.0, y + h,
			0.0, 0.0, 0.0, 0.0, //z 1.0
			0.0, 0.0, 0.0, 1.0);
		#else
		this->identity();
		this->translate(x + w * 0.5f, y + h * 0.5f, 0.0);
		this->scale(w *0.5, h*0.5, 1.0);
		#endif
	}

    this_type& frustum(value_type l, value_type r, value_type b, value_type t, value_type n, value_type f)
    {
        this_type mat(
            (2.0f * n) / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
            0.0f, (2.0f * n) / (t - b), (t + b) / (t - b), 0.0f,
            0.0f, 0.0f, -(f + n) / (f - n), -(2.0f * f * n) / (f - n),
            0.0f, 0.0f, -1.0f, 0.0f);
        return this->multiply(mat);
    }

    this_type& perspective(value_type fovY, value_type aspectRatio, value_type nearZ, value_type farZ)
    {
        using namespace std;

        #if 1
        const T frustumHeight = tan(static_cast<T>(fovY * M_RD * 0.5f)) * nearZ;
        const T frustumWidth = frustumHeight * aspectRatio;
        return this->frustum(-frustumWidth, frustumWidth, -frustumHeight, frustumHeight, nearZ, farZ);
        #else
        this->identity();

        /*
        data[0] = 1 / (tan(fovY * M_RD * 0.5f) * aspectRatio);
        data[5] = 1 / tan(fovY * M_RD * 0.5f);
        data[10] = farZ / (farZ - nearZ);
        data[11] = 1.0;
        data[14] = (nearZ * farZ) / (nearZ - farZ);
        /*/
        T rFovy = fovY * M_PI / 180;
        const T tanHalfFovy = tan(static_cast<T>(rFovy*0.5));
        data[0] = 1.0 / (aspectRatio * tanHalfFovy);
        data[5] = 1.0 / (tanHalfFovy);
        data[10] = -(farZ + nearZ) / (farZ - nearZ);
        data[11] = -1.0;
        data[14] = (-2.0 *nearZ*farZ) / (farZ - nearZ);
        //*/

        return *this;
        #endif
    }

    this_type& ortho(value_type left, value_type right, value_type bottom, value_type top,
        value_type zNear, value_type zFar)
    {
        //*
        this_type mat(
            2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
            0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
            0.0f, 0.0f, -2.0f / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
            0.0f, 0.0f, 0.0f, 1.0f);
        /*/
        this_type mat;
        mat.identity();
        mat.m[0][0] = (2.0f) / (right - left);
        mat.m[1][1] = (2.0f) / (top - bottom);
        mat.m[2][2] = -(2.0f) / (zFar - zNear);
        mat.m[3][0] = -(right + left) / (right - left);
        mat.m[3][1] = -(top + bottom) / (top - bottom);
        mat.m[3][2] = -(zFar + zNear) / (zFar - zNear);
        //*/
        return this->multiply(mat);
    }

    this_type transpose(const this_type& mat)const
    {
        return this_type(
            mat.data[0], mat.data[1], mat.data[2], mat.data[3],
            mat.data[4], mat.data[5], mat.data[6], mat.data[7],
            mat.data[8], mat.data[9], mat.data[10], mat.data[11],
            mat.data[12], mat.data[13], mat.data[14], mat.data[15]);
    }

    bool inverse(this_type& mat)const
    {
        return mat4_inverse(data, mat.data);
    }

    vec4f transform(float x, float y, float z, float w = 1.0f)const
    {
        return vec4f(
            data[0] * x + data[4] * y + data[ 8] * z + data[12] * w,
            data[1] * x + data[5] * y + data[ 9] * z + data[13] * w,
            data[2] * x + data[6] * y + data[10] * z + data[14] * w,
            data[3] * x + data[7] * y + data[11] * z + data[15] * w);
    }

    vec4f transform(const vec4f& vec)const
    {
        return this->transform(vec.x, vec.y, vec.z, vec.w);
    }
};

/*
template<typename T>
const T mat4<T>::IDENTITY[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f};
*/

//
// operator
//
/*
template<typename T>
vec4<T> operator*(const vec4<T>& vec, const mat4<T>& mat)
{
    return mat.transform(vec);
}
*/

//
// typedef
//

typedef mat4<float> mat4f;

namespace math {

    //获得摄像机位置
    template<typename T>
    vec4<T> view_position(const mat4<T>& mv)
    {
        vec4<T> p;
        p.x = -(mv[0] * mv[12] + mv[1] * mv[13] + mv[2] * mv[14]);
        p.y = -(mv[4] * mv[12] + mv[5] * mv[13] + mv[6] * mv[14]);
        p.z = -(mv[8] * mv[12] + mv[9] * mv[13] + mv[10] * mv[14]);
        return p;
    }


    int gluProject(
        float objx,
        float objy,
        float objz,
        float modelMatrix[16],
        float projMatrix[16],
        vec4i viewport,
        vec3f& win)
    {
        // Transformation vectors
        float fTempo[8];
        // Modelview transform
        fTempo[0] = modelMatrix[0] * objx + modelMatrix[4] * objy + modelMatrix[8] * objz + modelMatrix[12]; // w is always 1
        fTempo[1] = modelMatrix[1] * objx + modelMatrix[5] * objy + modelMatrix[9] * objz + modelMatrix[13];
        fTempo[2] = modelMatrix[2] * objx + modelMatrix[6] * objy + modelMatrix[10] * objz + modelMatrix[14];
        fTempo[3] = modelMatrix[3] * objx + modelMatrix[7] * objy + modelMatrix[11] * objz + modelMatrix[15];
        // Projection transform, the final row of projection matrix is always [0 0 -1 0]
        // so we optimize for that.
        fTempo[4] = projMatrix[0] * fTempo[0] + projMatrix[4] * fTempo[1] + projMatrix[8] * fTempo[2] + projMatrix[12] * fTempo[3];
        fTempo[5] = projMatrix[1] * fTempo[0] + projMatrix[5] * fTempo[1] + projMatrix[9] * fTempo[2] + projMatrix[13] * fTempo[3];
        fTempo[6] = projMatrix[2] * fTempo[0] + projMatrix[6] * fTempo[1] + projMatrix[10] * fTempo[2] + projMatrix[14] * fTempo[3];
        fTempo[7] = -fTempo[2];
        // The result normalizes between -1 and 1
        if (fTempo[7] == 0.0) // The w value
            return 0;
        fTempo[7] = 1.0 / fTempo[7];
        // Perspective division
        fTempo[4] *= fTempo[7];
        fTempo[5] *= fTempo[7];
        fTempo[6] *= fTempo[7];
        // Window coordinates
        // Map x, y to range 0-1
        win.x = (fTempo[4] * 0.5 + 0.5)*viewport[2] + viewport[0];
        win.y = (fTempo[5] * 0.5 + 0.5)*viewport[3] + viewport[1];
        // This is only correct when glDepthRange(0.0, 1.0)
        win.z = (1.0 + fTempo[6])*0.5;    // Between 0 and 1
        return 1;
    }

    vec4f project(const vec4f& v, const mat4f& modelMatrix, const mat4f& projMatrix, const vec4i& viewport)
    {
        return vec4f();
    }

    template<typename T>
    int unproject(T winx, T winy, T winz, const mat4<T>& modelMatrix, const mat4<T>& projMatrix, const vec4i& viewport, vec4<T>& p)
    {
        mat4<T> m;

        // Calculation for inverting a matrix, compute projection x modelview
        m = projMatrix * modelMatrix;

        // Now compute the inverse of matrix A
        if (!m.inverse(m)) {
            return -1;
        }

        // Transformation of normalized coordinates between -1 and 1
        p.x = (winx - static_cast<T>(viewport.x)) / static_cast<T>(viewport.width) * 2.0f - 1.0f;
        p.y = (winy - static_cast<T>(viewport.y)) / static_cast<T>(viewport.height) * 2.0f - 1.0f;
        p.z = 2.0f * winz - 1.0f;
        p.w = 1.0f;

        // Objects coordinates
        p = m.transform(p);

        if (is_zero(p.w)) {
            return -2;
        }

        //p = p / p.w;
        p.perspective_division();

        return 0;
    }

}//end namespace math
}//end namespace cgl

#endif //MATRIX_HPP_20180415194504

