/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 skybox.hpp

 2019-05-20 23:04:50

*/
#ifndef SKYBOX_HPP_20190520230450
#define SKYBOX_HPP_20190520230450

#include <cgl/graphics/gl/glcontext.hpp>
#include "glcamera.hpp"

namespace cgl{
namespace graphics{

/*
    ____________
   /   top     /|
  /___________/ |
 |           |  |
 |           |  |right
 |   front   |  |
 |           | /
 |___________|/

 face   index
 front  0
 left   1
 back   2
 right  3
 top    4
 bottom 5

*/

class glskybox
{
private:
    int skytex[6];

public:
    glskybox()
    {
        memset(skytex, 0, sizeof(skytex));
    }

    void set_texture(int front, int back, int left, int right, int top, int bottom);
    int load_texture(glcontext& dc,
        const char* front,
        const char* back,
        const char* left,
        const char* right,
        const char* top,
        const char* bottom);
    void paint(glcontext& dc, glcamera& camera, float size);
};

void glskybox::set_texture(int front, int back, int left, int right, int top, int bottom)
{
    skytex[0] = front;
    skytex[1] = back;
    skytex[2] = left;
    skytex[3] = right;
    skytex[4] = top;
    skytex[5] = bottom;
}

int glskybox::load_texture(glcontext& dc, const char* front, const char* back, const char* left,
    const char* right, const char* top, const char* bottom)
{
    int mode = GL_CLAMP_TO_EDGE;
    skytex[0] = dc.textures.open(front, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    skytex[1] = dc.textures.open(back, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    skytex[2] = dc.textures.open(left, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    skytex[3] = dc.textures.open(right, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    skytex[4] = dc.textures.open(top, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    skytex[5] = dc.textures.open(bottom, GL_RGB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
    return 0;
}

void glskybox::paint(glcontext& dc, glcamera& camera, float size)
{
    typedef vtx3f vertex_type;
    const vec4ub& color = dc.color;
    int fixed = size;
    //size += 2;

    dc.push_matrix();
    dc.identity();
    LookAt(dc, 0, 0, 0, camera.view.x, camera.view.y, camera.view.z);

    //---------------------------------------------------------------------------
    vertex_type front[] = {//front
        vertex_type( size,  size, fixed, 0.0f, 0.0f, color),
        vertex_type( size, -size, fixed, 0.0f, 1.0f, color),
        vertex_type(-size, -size, fixed, 1.0f, 1.0f, color),
        vertex_type(-size,  size, fixed, 1.0f, 0.0f, color)
    };
    dc.bind_texture(skytex[0]);
    dc.draw_arrays(CGL_QUADS, front, 0, 4);

    //---------------------------------------------------------------------------
    vertex_type left[] = {//left
        vertex_type(-fixed,  size,  size, 0.0f, 0.0f, color),
        vertex_type(-fixed, -size,  size, 0.0f, 1.0f, color),
        vertex_type(-fixed, -size, -size, 1.0f, 1.0f, color),
        vertex_type(-fixed,  size, -size, 1.0f, 0.0f, color),
    };
    dc.bind_texture(skytex[1]);
    dc.draw_arrays(CGL_QUADS, left, 0, 4);

    //---------------------------------------------------------------------------
    vertex_type back[] = {//back
        vertex_type(-size,  size, -fixed, 0.0f, 0.0f, color),
        vertex_type(-size, -size, -fixed, 0.0f, 1.0f, color),
        vertex_type( size, -size, -fixed, 1.0f, 1.0f, color),
        vertex_type( size,  size, -fixed, 1.0f, 0.0f, color),

    };
    dc.bind_texture(skytex[2]);
    dc.draw_arrays(CGL_QUADS, back, 0, 4);

    //---------------------------------------------------------------------------
    vertex_type right[] = {//right
        vertex_type( fixed,  size, -size, 0.0f, 0.0f, color),
        vertex_type( fixed, -size, -size, 0.0f, 1.0f, color),
        vertex_type( fixed, -size,  size, 1.0f, 1.0f, color),
        vertex_type( fixed,  size,  size, 1.0f, 0.0f, color),

    };
    dc.bind_texture(skytex[3]);
    dc.draw_arrays(CGL_QUADS, right, 0, 4);

    //---------------------------------------------------------------------------
    vertex_type top[] = {//top
        vertex_type(-size,  fixed,  size, 0.0f, 0.0f, color),
        vertex_type(-size,  fixed, -size, 0.0f, 1.0f, color),
        vertex_type( size,  fixed, -size, 1.0f, 1.0f, color),
        vertex_type( size,  fixed,  size, 1.0f, 0.0f, color),

    };
    dc.bind_texture(skytex[4]);
    dc.draw_arrays(CGL_QUADS, top, 0, 4);

    //---------------------------------------------------------------------------
    vertex_type bottom[] = {//bottom
        vertex_type(-size, -fixed, -size, 0.0f, 0.0f, color),
        vertex_type(-size, -fixed,  size, 0.0f, 1.0f, color),
        vertex_type( size, -fixed,  size, 1.0f, 1.0f, color),
        vertex_type( size, -fixed, -size, 1.0f, 0.0f, color),

    };
    dc.bind_texture(skytex[5]);
    dc.draw_arrays(CGL_QUADS, bottom, 0, 4);
    dc.pop_matrix();
}

}//end namespace graphics
}//end namespace cgl

#endif //SKYBOX_HPP_20190520230450
