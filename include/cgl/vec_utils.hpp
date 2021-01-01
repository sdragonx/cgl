/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vec_utils.hpp

 2018-02-18 14:40:55

*/
#ifndef VECU_HPP_20180218144055
#define VECU_HPP_20180218144055

#include <cgl/vec.hpp>
#include <cgl/string.hpp>

namespace cgl{

namespace internal{

template<typename T, typename VEC>
struct vec_param
{
    const static T* value;
};

#define VEC_PARAM_DECLARE(Vec, Param)\
    template<>const char* vec_param<char, Vec>::value = Param;\
    template<>const wchar_t* vec_param<wchar_t, Vec>::value = L##Param;


VEC_PARAM_DECLARE(vec2b,    "%hhd,%hhd")
VEC_PARAM_DECLARE(vec2ub,    "%hhu,%hhu")
VEC_PARAM_DECLARE(vec2s,    "%hd,%hd")
//VEC_PARAM_DECLARE(vec2us,    "%hu,%hu")
VEC_PARAM_DECLARE(vec2i,    "%i,%i")
VEC_PARAM_DECLARE(vec2f,    "%f,%f")

VEC_PARAM_DECLARE(vec3b,    "%hhd,%hhd,%hhd")
VEC_PARAM_DECLARE(vec3ub,    "%hhu,%hhu,%hhu")
VEC_PARAM_DECLARE(vec3s,    "%hd,%hd,%hd")
//VEC_PARAM_DECLARE(vec3us,    "%hu,%hu")
VEC_PARAM_DECLARE(vec3i,    "%i,%i,%i")
VEC_PARAM_DECLARE(vec3f,    "%f,%f,%f")

VEC_PARAM_DECLARE(vec4b,    "%hhd,%hhd,%hhd,%hhd")
VEC_PARAM_DECLARE(vec4ub,    "%hhu,%hhu,%hhu,%hhu")
VEC_PARAM_DECLARE(vec4s,    "%hd,%hd,%hd,%hd")
//VEC_PARAM_DECLARE(vec4us,    "%hu,%hu,%hu,%hu")
VEC_PARAM_DECLARE(vec4i,    "%i,%i,%i,%i")
VEC_PARAM_DECLARE(vec4f,    "%f,%f,%f,%f")

#undef VEC_PARAM_DECLARE

}//end namespace internal

//vec2<T>
template<typename _char_t, typename T>
_char_t* vec2_to(_char_t* buf, size_t size, const vec2<T>& vec)
{
    return csprintf<_char_t>(buf, size, internal::vec_param<_char_t, vec2<T> >::value, vec.x, vec.y);
}

template<typename string_type, typename T>
string_type vec2_to(const vec2<T>& vec)
{
    typedef TYPENAME string_type::value_type char_type;
    char_type buf[64];
    csprintf<char_type>(buf, 64, internal::vec_param<char_type, vec2<T> >::value, vec.x, vec.y);
    return string_type(buf);
}

template<typename T, typename _char_t>
vec2<T> to_vec2(const _char_t* str)
{
    vec2<T> vec;
    if(str)csscanf<_char_t>(str, internal::vec_param<_char_t, vec2<T> >::value, &vec.x, &vec.y);
    return vec;
}

template<typename T, typename CH>
vec2<T> to_vec2(const cstring<CH>& str)
{
    return to_vec2<T, CH>(str.c_str());
}

//vec3<T>
template<typename _char_t, typename T>
_char_t* vec3_to(_char_t* buf, size_t size, const vec3<T>& vec)
{
    return csprintf<_char_t>(buf, size, internal::vec_param<_char_t, vec3<T> >::value, vec.x, vec.y, vec.z);
}

template<typename string_type, typename T>
string_type vec3_to(const vec3<T>& vec)
{
    typedef TYPENAME string_type::value_type char_type;
    char_type buf[96];
    csprintf<char_type>(buf, 96, internal::vec_param<char_type, vec3<T> >::value, vec.x, vec.y, vec.z);
    return string_type(buf);
}

template<typename T, typename _char_t>
vec3<T> to_vec3(const _char_t* str)
{
    vec3<T> vec;
    if(str)csscanf<_char_t>(str, internal::vec_param<_char_t, vec3<T> >::value, &vec.x, &vec.y, &vec.z);
    return vec;
}

template<typename T, typename CH>
vec3<T> to_vec3(const cstring<CH>& str)
{
    return to_vec3<T, CH>(str.c_str());
}

//vec4<T>
template<typename _char_t, typename T>
_char_t* vec4_to(_char_t* buf, size_t size, const vec4<T>& vec)
{
    return csprintf<_char_t>(buf, size, internal::vec_param<_char_t, vec4<T> >::value, vec.x, vec.y, vec.z, vec.w);
}

template<typename string_type, typename T>
string_type vec4_to(const vec4<T>& vec)
{
    typedef TYPENAME string_type::value_type char_type;
    char_type buf[128];
    csprintf<char_type>(buf, 128, internal::vec_param<char_type, vec4<T> >::value, vec.x, vec.y, vec.z, vec.w);
    return string_type(buf);
}

template<typename T, typename _char_t>
vec4<T> to_vec4(const _char_t* str)
{
    vec4<T> vec;
    if(str)csscanf<_char_t>(str, internal::vec_param<_char_t, vec4<T> >::value, &vec.x, &vec.y, &vec.z, &vec.w);
    return vec;
}

template<typename T, typename CH>
vec4<T> to_vec4(const cstring<CH>& str)
{
    return to_vec4<T, CH>(str.c_str());
}

#define to_vec4ub to_vec4<byte_t>

//
// size大小的矩形，根据align对齐方式，计算在矩形中的位置
//

vec4i measure_align(int x, int y, int width, int height, int sx, int sy, int align, bool scale)
{
    if(align == CGL_CLIENT)return vec4i(x, y, width, height);

    int w, h;

    if(scale && (width < sx || height < sy)){//目标矩形比较大，进行等比例缩放
        if(sx < sy){
            w = height * sx / sy;
            h = height;
        }
        else if(sy < sx){
            w = width;
            h = width * sy / sx;
        }
        else{
            w = width;
            h = height;
        }
    }
    else{
        w = sx;
        h = sy;
    }

    if((align & CGL_HORIZONTAL) == CGL_HORIZONTAL){
        x += (width - w) / 2;
    }
    else if(align & CGL_RIGHT){
        x += width - w;
    }

    if((align & CGL_VERTICAL) == CGL_VERTICAL){
        y += (height - h) / 2;
    }
    else if(align & CGL_BOTTOM){
        y += height - h;
    }
    return vec4i(x, y, w, h);
}

//不带缩放版本
vec4i calc_align(vec4i bounds, vec2i size, int align)
{
    vec4i rect;
    if(CGL_CLIENT == align){
        return bounds;
    }

    rect.width = size.width;
    rect.height = size.height;

    if((CGL_HORIZONTAL & align) == CGL_HORIZONTAL){
        rect.x = bounds.x + (bounds.width - rect.width) / 2;
    }
    else if(CGL_RIGHT & align){
        rect.x = bounds.x + bounds.width - rect.width;
    }
    else{
        rect.x = bounds.x;
    }

    if((CGL_VERTICAL & align) == CGL_VERTICAL){
        rect.y = bounds.y + (bounds.height - rect.height) / 2;
    }
    else if(CGL_BOTTOM & align){
        rect.y = bounds.y + bounds.height - rect.height;
    }
    else{
        rect.y = bounds.y;
    }

    return rect;
}


float vec4f_angle(const vec4f& v1, const vec4f& v2)
{
    const float MATH_FLOAT_SMALL = 0.000001f;
    float dx = v1.w * v2.x - v1.x * v2.w - v1.y * v2.z + v1.z * v2.y;
    float dy = v1.w * v2.y - v1.y * v2.w - v1.z * v2.x + v1.x * v2.z;
    float dz = v1.w * v2.z - v1.z * v2.w - v1.x * v2.y + v1.y * v2.x;

    //return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(v1, v2));
    return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, v1.dot(v2));
}



}//end namespace cgl

#endif //VECU_HPP_20180218144055

