/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glu.hpp

 2019-12-24 23:30:28

*/
#ifndef GLU_HPP_20191224233028
#define GLU_HPP_20191224233028

#include "gl.hpp"

namespace cgl{
namespace graphics{

//---------------------------------------------------------------------------
//
// draw_cube
//

static GLuint CUBE_INDICES[36] = {
    0,1,2,0,2,3,  4,5,6,4,6,7,  8,9,10, 8,10,11,  12,13,14,12,14,15,  16,17,18,16,18,19,  20,21,22,20,22,23
};

template<typename CONTEXT>
void draw_cube(CONTEXT& dc, float x, float y, float z, float size)
{
    typedef vtx3fx vertex_type;
    const vec4ub& color = dc.color;

    size *= 0.5f;

    //face draw order
    // 0  3
    // 1  2

    vertex_type vtx[] = {
        //front
        vertex_type(x - size, y + size, z + size, 0.0f, 0.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x - size, y - size, z + size, 0.0f, 1.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x + size, y - size, z + size, 1.0f, 1.0f, color, 0.0f, 0.0f, 1.0f),
        vertex_type(x + size, y + size, z + size, 1.0f, 0.0f, color, 0.0f, 0.0f, 1.0f),

        //back
        vertex_type(x + size, y + size, z - size, 0.0f, 0.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x + size, y - size, z - size, 0.0f, 1.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x - size, y - size, z - size, 1.0f, 1.0f, color, 0.0f, 0.0f, -1.0f),
        vertex_type(x - size, y + size, z - size, 1.0f, 0.0f, color, 0.0f, 0.0f, -1.0f),

        //left
        vertex_type(x - size, y + size, z - size, 0.0f, 0.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y - size, z - size, 0.0f, 1.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y - size, z + size, 1.0f, 1.0f, color, -1.0f, 0.0f, 0.0f),
        vertex_type(x - size, y + size, z + size, 1.0f, 0.0f, color, -1.0f, 0.0f, 0.0f),

        //right
        vertex_type(x + size, y + size, z + size, 0.0f, 0.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y - size, z + size, 0.0f, 1.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y - size, z - size, 1.0f, 1.0f, color, 1.0f, 0.0f, 0.0f),
        vertex_type(x + size, y + size, z - size, 1.0f, 0.0f, color, 1.0f, 0.0f, 0.0f),

        //top
        vertex_type(x - size, y + size, z - size, 0.0f, 0.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x - size, y + size, z + size, 0.0f, 1.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x + size, y + size, z + size, 1.0f, 1.0f, color, 0.0f, 1.0f, 0.0f),
        vertex_type(x + size, y + size, z - size, 1.0f, 0.0f, color, 0.0f, 1.0f, 0.0f),

        //bottom
        vertex_type(x - size, y - size, z + size, 0.0f, 0.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x - size, y - size, z - size, 0.0f, 1.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x + size, y - size, z - size, 1.0f, 1.0f, color, 0.0f, -1.0f, 0.0f),
        vertex_type(x + size, y - size, z + size, 1.0f, 0.0f, color, 0.0f, -1.0f, 0.0f),
    };

    dc.draw_elements(GL_TRIANGLES, vtx, CUBE_INDICES, 36u);
}

//ªÊ÷∆Ã›–Œ
template<typename CONTEXT>
void draw_trapzium(CONTEXT& dc, float x, float y, float z, float top, float bottom, float height)
{
    typedef vtx3fx vertex_type;
    const vec4ub& color = dc.color;
    float u0 = bottom - top;
    u0 *= 0.5f;
    u0 /= bottom;
    float u1 = 1.0f - u0;

    vec3f vs[] = {
        vec3f(x - top, y + height, z - top),
        vec3f(x - top, y + height, z + top),
        vec3f(x + top, y + height, z + top),
        vec3f(x + top, y + height, z - top),

        vec3f(x - bottom, y - height, z + bottom),
        vec3f(x - bottom, y - height, z - bottom),
        vec3f(x + bottom, y - height, z - bottom),
        vec3f(x + bottom, y - height, z + bottom)
    };

    vec3f normals[6] = {
        vec3f(0.0f,    u0,   u1),
        vec3f(0.0f,   -u0,  -u1),
        vec3f( -u1,    u0, 0.0f),
        vec3f(  u1,    u0, 0.0f),
        vec3f(0.0f,  1.0f, 0.0f),
        vec3f(0.0f, -1.0f, 0.0f),
    };

    vertex_type vtx[] = {
        //front
        vertex_type(vs[1],   u0, 0.0f, color, normals[0]),
        vertex_type(vs[4], 0.0f, 1.0f, color, normals[0]),
        vertex_type(vs[7], 1.0f, 1.0f, color, normals[0]),
        vertex_type(vs[2],   u1, 0.0f, color, normals[0]),

        //back
        vertex_type(vs[3],   u0, 0.0f, color, normals[1]),
        vertex_type(vs[6], 0.0f, 1.0f, color, normals[1]),
        vertex_type(vs[5], 1.0f, 1.0f, color, normals[1]),
        vertex_type(vs[0],   u1, 0.0f, color, normals[1]),

        //left
        vertex_type(vs[0],   u0, 0.0f, color, normals[2]),
        vertex_type(vs[5], 0.0f, 1.0f, color, normals[2]),
        vertex_type(vs[4], 1.0f, 1.0f, color, normals[2]),
        vertex_type(vs[1],   u1, 0.0f, color, normals[2]),

        //right
        vertex_type(vs[2],   u0, 0.0f, color, normals[3]),
        vertex_type(vs[7], 0.0f, 1.0f, color, normals[3]),
        vertex_type(vs[6], 1.0f, 1.0f, color, normals[3]),
        vertex_type(vs[3],   u1, 0.0f, color, normals[3]),

        //top
        vertex_type(vs[0], 0.0f, 0.0f, color, normals[4]),
        vertex_type(vs[1], 0.0f, 1.0f, color, normals[4]),
        vertex_type(vs[2], 1.0f, 1.0f, color, normals[4]),
        vertex_type(vs[3], 1.0f, 0.0f, color, normals[4]),

        //bottom
        vertex_type(vs[4], 0.0f, 0.0f, color, normals[5]),
        vertex_type(vs[5], 0.0f, 1.0f, color, normals[5]),
        vertex_type(vs[6], 1.0f, 1.0f, color, normals[5]),
        vertex_type(vs[7], 1.0f, 0.0f, color, normals[5]),
    };

    dc.draw_elements(GL_TRIANGLES, vtx, graphics::CUBE_INDICES, 36);
}

//---------------------------------------------------------------------------
//32
void draw_circle(float x, float y, float r = 1.0f)
{
    float data[] = {
        1.000000f * r,
        0.980785f * r,
        0.923880f * r,
        0.831470f * r,
        0.707107f * r,
        0.555570f * r,
        0.382683f * r,
        0.195090f * r,
        0.0f
    };

    float buf[66] = {
        data[0] + x, data[8] + y,
        data[1] + x, data[7] + y,
        data[2] + x, data[6] + y,
        data[3] + x, data[5] + y,
        data[4] + x, data[4] + y,
        data[5] + x, data[3] + y,
        data[6] + x, data[2] + y,
        data[7] + x, data[1] + y,

        -data[8] + x, data[0] + y,
        -data[7] + x, data[1] + y,
        -data[6] + x, data[2] + y,
        -data[5] + x, data[3] + y,
        -data[4] + x, data[4] + y,
        -data[3] + x, data[5] + y,
        -data[2] + x, data[6] + y,
        -data[1] + x, data[7] + y,

        -data[0] + x, -data[8] + y,
        -data[1] + x, -data[7] + y,
        -data[2] + x, -data[6] + y,
        -data[3] + x, -data[5] + y,
        -data[4] + x, -data[4] + y,
        -data[5] + x, -data[3] + y,
        -data[6] + x, -data[2] + y,
        -data[7] + x, -data[1] + y,

        data[8] + x, -data[0] + y,
        data[7] + x, -data[1] + y,
        data[6] + x, -data[2] + y,
        data[5] + x, -data[3] + y,
        data[4] + x, -data[4] + y,
        data[3] + x, -data[5] + y,
        data[2] + x, -data[6] + y,
        data[1] + x, -data[7] + y,

        data[0] + x, data[8] + y,
    };

    graphics::draw_arrays(CGL_LINE_STRIP, (vec2f*)buf, 33);
}

}//end namespace graphics
}//end namespace cgl

#endif //GLU_HPP_20191224233028
