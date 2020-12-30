/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 program.hpp

 2018-05-29 01:47:16

 shader精度：lowp medp mediump highp.

*/
#ifndef PROGRAM_HPP_20180529014716
#define PROGRAM_HPP_20180529014716

#include <cgl/public.h>
#include "shader.hpp"
#include "gllight.hpp"
#include "shader/basic.hpp"

#ifdef __GNUC__
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif

#define offset_element(obj,element) ((void*)(offsetof(obj, element)))

namespace cgl{
namespace graphics{

struct glprogram_state
{
    typedef int attribute;
    typedef int uniform;

    struct{
        attribute position;
        attribute uv;
        attribute color;
        attribute normal;
    }vertex;

    uniform mvp;
    uniform modelview;
    uniform camera;

    struct{
        uniform position;
        uniform direction;
        uniform ambient;
        uniform diffuse;
        uniform specular;
        uniform shine;
    }light;

    uniform texture;
    uniform texImage;

    glprogram_state()
    {
        this->init();
    }

    void init()
    {
        memset(this, 0, sizeof(glprogram_state));
    }
};

class glprogram
{
private:
    int m_id;

protected:
    glprogram_state state;

public:
    vec4ub color;

public:
    glprogram() : state(), color(255, 255, 255, 255)
    {
        m_id = 0;
    }

    int handle()const { return m_id; }

    int create(const char* vs = basic_vs, const char* fs = basic_fs);
    void dispose();

    bool is_null()const { return !m_id; }

    void active(bool value) { glUseProgram(value ? m_id : 0); }

    int attribute(const char* name)
    {
        int n = glGetAttribLocation(m_id, name);
        if(n == -1){
            CGL_LOG_DEBUG("glprogram : glGetAttribLocation( %s ) failed.", name);
        }
        return n;
    }

    int uniform(const char* name)
    {
        int n = glGetUniformLocation(m_id, name);
        if (n == -1){
            int err = glGetError();
            //CGL_LOG_ERROR("glprogram : uniform( %s ) failed. %x: %s", name, err, glerror_string(err) );
        }
        return n;
    }

    //set_effect(0)
    //set_effect(1)//灰度化

    //update
    void update(int id, int value)                      { glUniform1i(id, value);          }
    void update(int id, int v1, int v2)                 { glUniform2i(id, v1, v2);         }
    void update(int id, int v1, int v2, int v3)         { glUniform3i(id, v1, v2, v3);     }
    void update(int id, int v1, int v2, int v3, int v4) { glUniform4i(id, v1, v2, v3, v4); }

    void update(int id, float value)                            { glUniform1f(id, value);          }
    void update(int id, float v1, float v2)                     { glUniform2f(id, v1, v2);         }
    void update(int id, float v1, float v2, float v3)           { glUniform3f(id, v1, v2, v3);     }
    void update(int id, float v1, float v2, float v3, float v4) { glUniform4f(id, v1, v2, v3, v4); }

    //update array
    void update(int id, int size, const int* a)   { glUniform1iv(id, size, a); }
    void update(int id, int size, const float* a) { glUniform1fv(id, size, a); }

    void update(int id, const std::vector<vec2i>& ls) { glUniform2iv(id, ls.size(), ls[0].data); }
    void update(int id, const std::vector<vec2f>& ls) { glUniform2fv(id, ls.size(), ls[0].data); }

    void update(int id, const std::vector<vec3i>& ls) { glUniform3iv(id, ls.size(), ls[0].data); }
    void update(int id, const std::vector<vec3f>& ls) { glUniform3fv(id, ls.size(), ls[0].data); }

    void update(int id, const std::vector<vec4i>& ls) { glUniform4iv(id, ls.size(), ls[0].data); }
    void update(int id, const std::vector<vec4f>& ls) { glUniform4fv(id, ls.size(), ls[0].data); }

    //update vector
    void update(int id, const vec2i& v) { glUniform2i(id, v.x, v.y);           }
    void update(int id, const vec3i& v) { glUniform3i(id, v.x, v.y, v.z);      }
    void update(int id, const vec4i& v) { glUniform4i(id, v.x, v.y, v.z, v.w); }

    void update(int id, const vec2f& v) { glUniform2f(id, v.x, v.y);           }
    void update(int id, const vec3f& v) { glUniform3f(id, v.x, v.y, v.z);      }
    void update(int id, const vec4f& v) { glUniform4f(id, v.x, v.y, v.z, v.w); }

    //update matrix
    void update(int id, const mat4f& mat) { glUniformMatrix4fv(id, 1, GL_FALSE, mat.data); }

    //update light
    void update(const gllight& light)
    {
        this->update(this->uniform("light.position"), light.position);
        this->update(this->uniform("light.ambient"), light.ambient);
        this->update(this->uniform("light.diffuse"), light.diffuse);
        this->update(this->uniform("light.specular"), light.specular);
    }

    void begin(const mat4f& mv, const mat4f& p, int texture);
    void end();

    void vtxptr(const vtx3f* vtx);
    void vtxptr(const vtx3fx* vtx);
    void vtxptr(const vtx4fx* vtx);

    //void vtxptr(int id, void *p);

    int draw_arrays(int shape, const vtx3f* vtx, size_t pos, size_t size);
    int draw_arrays(int shape, const vtx3fx* vtx, size_t pos, size_t size);
    int draw_elements(int shape, const vtx3f* vtx, const size_t* indexbuffer, size_t size);
    int draw_elements(int shape, const vtx3fx* vtx, const size_t* indexbuffer, size_t size);
    int draw_elements(int shape, const vtx4fx* vtx, const size_t* indexbuffer, size_t size);

    void draw_rect(float x1, float y1, float x2, float y2, vec4ub color = color::white);
    void draw_rectXZ(float x1, float y1, float x2, float y2, vec4ub color = color::white);

    int draw_image(
        float x1, float y1, float z1,
        float x2, float y2, float z2,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

    int draw_image(
        float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
};

int glprogram::create(const char* vsSource, const char* fsSource)
{
    this->dispose();

    m_id = compile_program(vsSource, fsSource);

    if(!m_id){
        return CGL_ERROR;
    }

    // bind location attribute and uniform
    state.vertex.position = this->attribute("a_position");
    state.vertex.uv       = this->attribute("a_uv");
    state.vertex.color    = this->attribute("a_color");
    state.vertex.normal   = this->attribute("a_normal");

    state.mvp       = this->uniform("u_mvp");
    state.modelview = this->uniform("u_modelview");
    state.camera    = this->uniform("u_camera");

    state.light.position  = this->uniform("light.position");
    state.light.direction = this->uniform("light.direction");
    state.light.ambient   = this->uniform("light.ambient");
    state.light.diffuse   = this->uniform("light.diffuse");
    state.light.specular  = this->uniform("light.specular");
    state.light.shine     = this->uniform("light.shine");

    // suppert 4 textures
    state.texture  = this->uniform("texIndex");
    state.texImage = this->uniform("texImage");

    return CGL_OK;
}

void glprogram::dispose()
{
    if(m_id){
        glDeleteProgram(m_id);
        m_id = 0;
    }

    const_write(state).init();
}

void glprogram::begin(const mat4f& mv, const mat4f& p, int texture)
{
    const int texImage[] = {0, 1, 2, 3};
    mat4f mvp = p * mv;

    this->active(true);
    this->update(state.mvp, mvp);
    this->update(state.modelview, mv);
    this->update(state.texture, texture);
    this->update(state.texImage, 4, texImage);  //暂时支持4个多纹理映射
    glEnableVertexAttribArray(state.vertex.position);
    glEnableVertexAttribArray(state.vertex.uv);
    glEnableVertexAttribArray(state.vertex.color);
    if(state.vertex.normal>=0)glEnableVertexAttribArray(state.vertex.normal);
}

void glprogram::end()
{
    glDisableVertexAttribArray(state.vertex.position);
    glDisableVertexAttribArray(state.vertex.uv);
    glDisableVertexAttribArray(state.vertex.color);
    glDisableVertexAttribArray(state.vertex.normal);
    this->active(false);
}

void glprogram::vtxptr(const vtx3f* vtx)
{
    if(vtx){
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3f), vtx);
        glVertexAttribPointer(state.vertex.uv, 2, GL_FLOAT, GL_FALSE, sizeof(vtx3f), &vtx->u);
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx3f), vtx->color.data);
    }
    else{
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3f), 0);
        glVertexAttribPointer(state.vertex.uv, 2, GL_FLOAT, GL_FALSE, sizeof(vtx3f), offset_element(vtx3f, u));
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx3f), offset_element(vtx3f, color));
    }
}

void glprogram::vtxptr(const vtx3fx* vtx)
{
    if(vtx){
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), vtx);
        glVertexAttribPointer(state.vertex.uv, 2, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), &vtx->u);
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx3fx), vtx->color.data);
        glVertexAttribPointer(state.vertex.normal, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), vtx->normal.data);
    }
    else{
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), 0);
        glVertexAttribPointer(state.vertex.uv, 2, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), offset_element(vtx3fx, u));
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx3fx), offset_element(vtx3fx, color));
        glVertexAttribPointer(state.vertex.normal, 3, GL_FLOAT, GL_FALSE, sizeof(vtx3fx), offset_element(vtx3fx, normal));
    }
}

void glprogram::vtxptr(const vtx4fx* vtx)
{
    if(vtx){
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), vtx);
        glVertexAttribPointer(state.vertex.uv, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), &vtx->u);
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx4fx), vtx->color.data);
        glVertexAttribPointer(state.vertex.normal, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), vtx->normal.data);
    }
    else{
        glVertexAttribPointer(state.vertex.position, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), 0);
        glVertexAttribPointer(state.vertex.uv, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), offset_element(vtx4fx, u));
        glVertexAttribPointer(state.vertex.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(vtx4fx), offset_element(vtx4fx, color));
        glVertexAttribPointer(state.vertex.normal, 3, GL_FLOAT, GL_FALSE, sizeof(vtx4fx), offset_element(vtx4fx, normal));
    }
}

int glprogram::draw_arrays(int shape, const vtx3f* vtx, size_t pos, size_t size)
{
    this->vtxptr(vtx);
    glDrawArrays(shape, pos, size);
    return 0;
}

int glprogram::draw_arrays(int shape, const vtx3fx* vtx, size_t pos, size_t size)
{
    this->vtxptr(vtx);
    glDrawArrays(shape, pos, size);
    return 0;
}

int glprogram::draw_elements(int shape, const vtx3f* vtx, const size_t* indexbuffer, size_t size)
{
    this->vtxptr(vtx);
    glDrawElements(shape, size, GL_UNSIGNED_INT, indexbuffer);
    return 0;
}

int glprogram::draw_elements(int shape, const vtx3fx* vtx, const size_t* indexbuffer, size_t size)
{
    this->vtxptr(vtx);
    glDrawElements(shape, size, GL_UNSIGNED_INT, indexbuffer);
    return 0;
}

int glprogram::draw_elements(int shape, const vtx4fx* vtx, const size_t* indexbuffer, size_t size)
{
    this->vtxptr(vtx);
    glDrawElements(shape, size, GL_UNSIGNED_INT, indexbuffer);
    return 0;
}

void glprogram::draw_rect(float x1, float y1, float x2, float y2, vec4ub color)
{
    typedef vtx3f vertex_type;
    vertex_type vtx[] = {
        vertex_type(x1, y1, 0.0f, 0.0f, 0.0f, color),
        vertex_type(x2, y1, 0.0f, 1.0f, 0.0f, color),
        vertex_type(x2, y2, 0.0f, 1.0f, 1.0f, color),
        vertex_type(x1, y2, 0.0f, 0.0f, 1.0f, color)
    };

    this->draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);
}

void glprogram::draw_rectXZ(float x1, float y1, float x2, float y2, vec4ub color)
{
    typedef vtx3f vertex_type;
    vertex_type vtx[] = {
        vertex_type(x1, 0.0f, y1, 0.0f, 0.0f, color),
        vertex_type(x2, 0.0f, y1, 1.0f, 0.0f, color),
        vertex_type(x2, 0.0f, y2, 1.0f, 1.0f, color),
        vertex_type(x1, 0.0f, y2, 0.0f, 1.0f, color)
    };

    this->draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);
}

int glprogram::draw_image(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float u1, float v1, float u2, float v2)
{
    vtx3f vtx[] = {
        #if CGL_COORDINATE == CGL_LOWER_LEFT
        vtx3f(x1, y1, z1, u1, v2, color),
        vtx3f(x2, y1, z1, u2, v2, color),
        vtx3f(x2, y2, z2, u2, v1, color),
        vtx3f(x1, y2, z2, u1, v1, color)
        #else
        vtx3f(x1, y1, z1, u1, v1, color),
        vtx3f(x2, y1, z1, u2, v1, color),
        vtx3f(x2, y2, z2, u2, v2, color),
        vtx3f(x1, y2, z2, u1, v2, color)
        #endif
    };
    return this->draw_arrays(CGL_QUADS, vtx, 0, 4);
}

int glprogram::draw_image(float x, float y, float width, float height,
    float u1, float v1, float u2, float v2)
{
    return this->draw_image(x, y, 0.0f, x + width, y + height, 0.0f, u1, v1, u2, v2);
}

}//end namespace graphics
}//end namespace cgl

#ifdef __GNUC__
    #pragma GCC diagnostic pop
#endif

#endif //PROGRAM_HPP_20180529014716
