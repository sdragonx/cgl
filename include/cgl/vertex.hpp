/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vertex.hpp

 2017-02-02 15:13:01

*/
#ifndef VERTEX_HPP_20170202151301
#define VERTEX_HPP_20170202151301

#include <cgl/matrix.hpp>
#include <cgl/vec.hpp>

namespace cgl{

template<typename VT, typename TT, typename CT>
struct vtx3t2c4
{
    typedef vtx3t2c4 this_type;
    typedef vec4<CT> color_type;

    VT x, y, z;
    TT u, v;
    color_type color;

    vtx3t2c4() : x(), y(), z(), u(), v(), color(){/*void*/}
    vtx3t2c4(const vec3<VT>& vtx, TT tu, TT tv, const color_type& cc) :
        x(vtx.x), y(vtx.y), z(vtx.z), u(tu), v(tv), color(cc) { /*void*/ }

    vtx3t2c4(VT vx, VT vy, VT vz, TT vu, TT vv, const color_type& cc) :
        x(vx), y(vy), z(vz), u(vu), v(vv), color(cc) { /*void*/ }

    this_type& operator=(const vec3<VT>& p)
    {
        x = p.x;
        y = p.y;
        z = p.z;
        return *this;
    }

//    inline float slope2D(const this_type& v)
//    {
//        return float(v.x - x) / float(v.y - y);
//    }
};

template<typename VT, typename TT, typename CT>
struct vtx4t2c4
{
    typedef vtx4t2c4 this_type;
    typedef vec4<CT> color_type;

    VT x, y, z, w;
    TT u, v;
    color_type color;

    vtx4t2c4() : x(), y(), z(), w(), u(), v(), color(){/*void*/}
    vtx4t2c4(const vec4<VT>& vtx, TT tu, TT tv, const color_type& cc) :
        x(v.x), y(v.y), z(v.z), w(v.w), u(tu), v(tv), color(cc) { /*void*/ }

    vtx4t2c4(VT vx, VT vy, VT vz, VT vw, TT vu, TT vv, const color_type& cc) :
        x(vx), y(vy), z(vz), w(vw), u(vu), v(vv), color(cc) { /*void*/ }
};

typedef vtx3t2c4<int,   int,   uint8_t> VERTEX_V3I_T2I_C4UB;
typedef vtx3t2c4<float, float, uint8_t> VERTEX_V3F_T2F_C4UB;
typedef vtx3t2c4<float, float, float>   VERTEX_V3F_T2F_C4F;

typedef vtx4t2c4<int,   int,   uint8_t> VERTEX_V4I_T2I_C4UB;
typedef vtx4t2c4<float, float, uint8_t> VERTEX_V4F_T2F_C4UB;
typedef vtx4t2c4<float, float, float>   VERTEX_V4F_T2F_C4F;

typedef VERTEX_V3I_T2I_C4UB vtx3i;
typedef VERTEX_V3F_T2F_C4UB vtx3f;

typedef VERTEX_V4I_T2I_C4UB vtx4i;
typedef VERTEX_V4F_T2F_C4UB vtx4f;

//---------------------------------------------------------------------------

template<typename VT, typename TT, typename CT>
struct vtx3T2C4N3 : private vec3<VT>
{
    typedef vtx3T2C4N3 this_type;
    typedef vec3<VT> vector_type;
    typedef vec4<CT> color_type;
    typedef vec3<float> normal_type;

    using vector_type::x;
    using vector_type::y;
    using vector_type::z;

    TT u, v;

    vec4<CT> color;
    normal_type normal;

    vtx3T2C4N3() : vector_type(), u(), v(), color(), normal(){/*void*/}

    vtx3T2C4N3(const vector_type& vtx, TT tu, TT tv, const color_type& cc, const normal_type& nn) :
        vector_type(vtx), u(tu), v(tv), color(cc), normal(nn) { /*void*/ }

    vtx3T2C4N3(const vector_type& vtx, TT tu, TT tv, const color_type& cc, float nx, float ny, float nz) :
        vector_type(vtx), u(tu), v(tv), color(cc), normal(nx, ny, nz) { /*void*/ }

    vtx3T2C4N3(VT vx, VT vy, VT vz, TT vu, TT vv, const color_type& cc, const vec3f& nn) :
        vector_type(vx, vy, vz), u(vu), v(vv), color(cc), normal(nn) { /*void*/ }

    vtx3T2C4N3(VT vx, VT vy, VT vz, TT vu, TT vv, const color_type& cc, float nx, float ny, float nz) :
        vector_type(vx, vy, vz), u(vu), v(vv), color(cc), normal(nx, ny, nz) { /*void*/ }

    inline float slope2D(const this_type& v)
    {
        return float(v.x - x) / float(v.y - y);
    }
};

template<typename VT, typename TT, typename CT>
struct vtx4T2C4N3 : private vec4<VT>
{
    typedef vtx4T2C4N3 this_type;
    typedef vec4<VT> vector_type;
    typedef vec4<CT> color_type;
    typedef vec3<float> normal_type;

    using vector_type::x;
    using vector_type::y;
    using vector_type::z;
    using vector_type::w;

    TT u, v, t;

    vec4<CT> color;
    normal_type normal;

    vtx4T2C4N3() : vector_type(), u(), v(), color(), normal(){/*void*/}

    vtx4T2C4N3(const vector_type& vtx, TT tu, TT tv, const color_type& cc, const normal_type& nn) :
        vector_type(vtx), u(tu), v(tv), color(cc), normal(nn) { /*void*/ }

    vtx4T2C4N3(const vector_type& vtx, TT tu, TT tv, const color_type& cc, float nx, float ny, float nz) :
        vector_type(vtx), u(tu), v(tv), color(cc), normal(nx, ny, nz) { /*void*/ }

    vtx4T2C4N3(VT vx, VT vy, VT vz, TT vu, TT vv, const color_type& cc, const vec3f& nn) :
        vector_type(vx, vy, vz), u(vu), v(vv), color(cc), normal(nn) { /*void*/ }

    vtx4T2C4N3(VT vx, VT vy, VT vz, TT vu, TT vv, const color_type& cc, float nx, float ny, float nz) :
        vector_type(vx, vy, vz), u(vu), v(vv), color(cc), normal(nx, ny, nz) { /*void*/ }

    inline float slope2D(const this_type& v)
    {
        return float(v.x - x) / float(v.y - y);
    }
};

typedef vtx3T2C4N3<float, float, uint8_t> vtx3fx;
typedef vtx4T2C4N3<float, float, uint8_t> vtx4fx;

//---------------------------------------------------------------------------
// transform

template<typename VT, typename TT, typename CT>
inline vtx3t2c4<VT, TT, CT> transform(const vtx3t2c4<VT, TT, CT>& v, const mat4f& mat)
{
    vec4f p = mat.transform(v.x, v.y, v.z);
    return vtx3t2c4<VT, TT, CT>(p.x, p.y, p.z, v.u, v.v, v.color);
}

extern vtx3i transform3i(const vtx3f& v, const mat4f& mat);
extern vtx3f transform3f(const vtx3f& v, const mat4f& mat);

//---------------------------------------------------------------------------
// vertex array filling
//

//填充一个矩形，两个三角形，6个顶点
void va_drawimage(vtx3f* va,
    float x, float y, float width, float height,
    vec4ub color,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
{
    //0 1
    //  2
    va[0].x = x;
    va[0].y = y;
    va[0].color = color;
    va[0].u = u1;
    va[0].v = v1;

    va[1].x = x + width;
    va[1].y = y;
    va[1].color = color;
    va[1].u = u2;
    va[1].v = v1;

    va[2].x = x + width;
    va[2].y = y + height;
    va[2].color = color;
    va[2].u = u2;
    va[2].v = v2;

    //3
    //5 4
    /*
    va[3].x = x;
    va[3].y = y;
    va[3].color = color;
    va[3].u = u1;
    va[3].v = v1;

    va[4].x = x + width;
    va[4].y = y + height;
    va[4].color = color;
    va[4].u = u2;
    va[4].v = v2;
    */

    va[3] = va[0];
    va[4] = va[2];

    va[5].x = x;
    va[5].y = y + height;
    va[5].color = color;
    va[5].u = u1;
    va[5].v = v2;
}

// a1 a2
// a3 a4
void va_rect_set_alpha(vtx3f* va, int a1 = 255, int a2 = 255, int a3 = 255, int a4 = 255)
{
    va[0].color.alpha = a1;
    va[1].color.alpha = a2;
    va[2].color.alpha = a3;
    va[3].color.alpha = a1;
    va[4].color.alpha = a3;
    va[5].color.alpha = a4;
}

//填充一个矩形，并指定四个顶点的alpha
void va_drawimage(vtx3f* va, int x, int y, int width, int height,
    int a1 = 255, int a2 = 255, int a3 = 255, int a4 = 255,
    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
{
    vec4ub color = vec4ub(255, 255, 255, 255);//graphics::color::white;

    //0 1
    //  2
    va[0].x = x;
    va[0].y = y;
    va[0].color = color;
    va[0].color.alpha = a1;
    va[0].u = u1;
    va[0].v = v1;

    va[1].x = x + width;
    va[1].y = y;
    va[1].color = color;
    va[1].color.alpha = a2;
    va[1].u = u2;
    va[1].v = v1;

    va[2].x = x + width;
    va[2].y = y + height;
    va[2].color = color;
    va[2].color.alpha = a3;
    va[2].u = u2;
    va[2].v = v2;

    //3
    //5 4
    /*
    va[3].x = x;
    va[3].y = y;
    va[3].color = color;
    va[3].color.alpha = a1;
    va[3].u = u1;
    va[3].v = v1;

    va[4].x = x + width;
    va[4].y = y + height;
    va[4].color = color;
    va[4].color.alpha = a3;
    va[4].u = u2;
    va[4].v = v2;
    */

    va[3] = va[0];
    va[4] = va[2];

    va[5].x = x;
    va[5].y = y + height;
    va[5].color = color;
    va[5].color.alpha = a4;
    va[5].u = u1;
    va[5].v = v2;
}

//---------------------------------------------------------------------------
//
// 填充一个网络，只能用于单一纹理。然后通过draw_arrays绘制，这个模式非常快速
//
// 需要的顶点数量为width * height * 6
//

void va_draw_grid(vtx3f* va, int width, int height, int tilesize)
{
    //int i;
    int tx, ty;
    for(int y=0; y<height; ++y){
    for(int x=0; x<width; ++x){
        ///i = (y * 512 + x) * 6;
        tx = x * tilesize;
        ty = y * tilesize;
        va_drawimage(va, tx, ty, tilesize, tilesize, vec4ub(255, 255, 255, 255));
        va += 6;
    }
    }
}

//---------------------------------------------------------------------------

}//end namespace cgl

#endif //VERTEX_HPP_20170202151301