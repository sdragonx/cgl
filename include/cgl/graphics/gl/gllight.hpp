/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gllight.hpp

 2019-05-18 23:44:51

*/
#ifndef GLLIGHT_HPP_20190518234451
#define GLLIGHT_HPP_20190518234451

#include "gl.hpp"

namespace cgl{
namespace graphics{

/*
#define GL_LIGHT0 0x4000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007

GLfloat LightAmbient[]= { 0.20f, 0.20f, 0.20f, 1.0f };    //环境光参数
GLfloat LightDiffuse[]= { 0.50f, 0.50f, 0.50f, 1.0f };    //漫射光参数
GLfloat planet_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };    //镜面光参数
GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f };    //光源位置

#define GL_CONSTANT_ATTENUATION        //常量衰减 1.0
#define GL_LINEAR_ATTENUATION        //线性衰减 0.0
#define GL_QUADRATIC_ATTENUATION    //二次衰减 0.0

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; //环境光参数 ( 新增 )
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // 漫射光参数 ( 新增 )
GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f }; // 光源位置 ( 新增 )

glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // 设置环境光
glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // 设置漫射光
glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // 光源位置
glEnable(GL_LIGHT1); // 启用一号光源

//材质设置
vec4f m(0.2, 0.2, 0.2, 1.0);
vec4f s(0.4, 0.4, 0.4, 1.0);
glMaterialfv(GL_FRONT, GL_AMBIENT, s.data);
glMaterialfv(GL_FRONT, GL_DIFFUSE, m.data);
glMaterialfv(GL_FRONT, GL_SPECULAR, s.data);
//glMaterialfv(GL_FRONT, GL_EMISSION, m.data);
glMaterialf(GL_FRONT, GL_SHININESS, 16);

*/

class gllight
{
private:
    #ifdef CGL_PLATFORM_GLES
    enum{
        GL_AMBIENT = 0x1200,
        GL_DIFFUSE = 0x1201,
        GL_SPECULAR = 0x1202,
        GL_POSITION = 0x1203,
        GL_SPOT_DIRECTION = 0x1204,
        GL_SPOT_EXPONENT = 0x1205,
        GL_SPOT_CUTOFF = 0x1206
    };
    void glLightf(GLenum light, GLenum pname, GLfloat param)
    {
    }
    void glLightfv(GLenum light, GLenum pname, const GLfloat *params)
    {
    }
    #endif
public:
    int m_id;

    vec4f position;
    vec4f ambient;
    vec4f diffuse;
    vec4f specular;
public:
    gllight() :
        m_id(),
        position(0.0f, 0.0f, 1.0f, 0.0f),
        ambient(),
        diffuse(1.0f, 1.0f, 1.0f, 1.0f),
        specular(1.0f, 1.0f, 1.0f, 1.0f)

    {
    }

    gllight(int id) : m_id(id)
    {
    }

    void bind(int id)
    {
        m_id = id;
    }

    void enable(bool value)
    {
        if(value){
            glEnable(m_id);
        }
        else{
            glDisable(m_id);
        }
    }

    bool is_enable()
    {
        return glIsEnabled(m_id);
    }

    //设置光源位置
    //最后一个参数为0，说明是方向性光源，非0则为位置性光源
    //default = vec4(0.0, 0.0, 1.0, 0.0)
    void set_position(float x, float y, float z, float w = 1.0f)
    {
        position = vec4f(x, y, z, w);
        glLightfv(m_id, GL_POSITION, position.data); // 光源位置
    }

    //设置环境光
    //default = vec4(0.0, 0.0, 0.0, 0.0)
    void set_ambient(float r, float g, float b, float a)
    {
        ambient = vec4f(r, g, b, a);
        glLightfv(m_id, GL_AMBIENT, ambient.data);
    }

    //设置漫射光
    //Light0 default = vec4(1.0, 1.0, 1.0, 1.0)
    void set_diffuse(float r, float g, float b, float a)
    {
        diffuse = vec4f(r, g, b, a);
        glLightfv(m_id, GL_DIFFUSE, diffuse.data);
    }

    //设置镜面光
    //default = vec4(1.0, 1.0, 1.0, 1.0)
    void set_specular(float r, float g, float b, float a)
    {
        specular = vec4f(r, g, b, a);
        glLightfv(m_id, GL_SPECULAR, specular.data);
    }

    //指定聚光灯的方向 default = vec3(0.0, 0.0, -1.0)
    void spot_direction(float x, float y, float z)
    {
        float vec[] = {x, y, z};
        glLightfv(m_id, GL_SPOT_DIRECTION, vec);
    }

    //设置聚光灯的角度
    //default = 180.0
    void spot_cutoff(float angle = 180.0f)
    {
        glLightf(m_id,GL_SPOT_CUTOFF, angle);
    }

    //聚光指数 default = 0.0
    void spot_exponent(float exponent = 0.0f)
    {
        glLightf(m_id, GL_SPOT_EXPONENT, exponent);
    }

    void rotate(float angle, float x, float y, float z = 0.0f)
    {
        vec3f p;
        p.x = cos(angle * M_RD) * 500.0f;
        p.y = -sin(angle * M_RD) * 200.0f;
        this->set_position(p.x, 0, p.y);
    }
};

class gllights
{
public:
    const static int MAX_LIGHTS = 8;

private:
    #ifdef CGL_PLATFORM_GLES
    enum{
        GL_LIGHTING = 0x0B50,
        GL_LIGHT_MODEL_LOCAL_VIEWER = 0x0B51,
        GL_LIGHT_MODEL_TWO_SIDE = 0x0B52,
        GL_LIGHT_MODEL_AMBIENT = 0x0B53,
        GL_COLOR_MATERIAL = 0x0B57,
        GL_MAX_LIGHTS = 0x0D31,
        GL_LIGHT0 = 0x4000,
        GL_LIGHT_MODEL_COLOR_CONTROL = 0x81F8,
        GL_SINGLE_COLOR = 0x81F9,
        GL_SEPARATE_SPECULAR_COLOR = 0x81FA
    };
    static void glLightf(GLenum light, GLenum pname, GLfloat param)
    {
    }
    static void glLightModelfv(GLenum pname, const GLfloat *params)
    {
    }
    static void glLightModeli(GLenum pname, GLint param)
    {
    }
    #endif

    gllight m_lights[MAX_LIGHTS];

public:
    gllights()
    {
        for(int i=0; i<MAX_LIGHTS; ++i){
            m_lights[i].bind(GL_LIGHT0 + i);
        }
    }

    //是否开启光照 default=disabled
    static void enable(bool value)
    {
        if(value){
            glEnable(GL_LIGHTING);
        }
        else{
            glDisable(GL_LIGHTING);
        }
    }

    //开启全局光照 default=vec4f(0.2f, 0.2f, 0.2f, 1.0f)
    static void global_ambient(float r = 0.2f, float g = 0.2f, float b = 0.2f, float a = 1.0f)
    {
        float globalAmbient[4] = { r, g, b, a };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    }

    //是否开启双面光照 default=GL_FALSE
    static void tow_side(bool enable)
    {
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , enable ? GL_TRUE : GL_FALSE);
    }

    //颜色材质
    static void color_material(bool enable)
    {
        if(enable){
            glEnable(GL_COLOR_MATERIAL);
            //glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        }
        else{
            glDisable(GL_COLOR_MATERIAL);
        }
    }

    //镜面反射角度如何计算 default=GL_FALSE
    static void local_viewer(bool value)
    {
        //把无限远的观察点改为局部观察点
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, value ? GL_TRUE : GL_FALSE);
    }

    //镜面辅助颜色 default=GL_SINGLE_COLOR
    //执行纹理贴图是才有用，可使效果逼真，其原理是：把镜面光从主颜色中抽离出来，
    //在纹理贴图后，再把镜面光加进去
    static void color_control(bool value)
    {
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, value ? GL_SEPARATE_SPECULAR_COLOR : GL_SINGLE_COLOR);
    }

    //glMaterialfv(GL_FRONT , GL_SPECULAR ,planet_specular);

    gllight& light(int index)
    {
        return m_lights[index];
    }

    gllight& operator[](int index)
    {
        return m_lights[index];
    }

    static int max_lights()
    {
        GLint n;
        glGetIntegerv(GL_MAX_LIGHTS, &n);
        return n;
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //GLLIGHT_HPP_20190518234451

