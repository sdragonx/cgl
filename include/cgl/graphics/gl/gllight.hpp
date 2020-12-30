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

GLfloat LightAmbient[]= { 0.20f, 0.20f, 0.20f, 1.0f };    //���������
GLfloat LightDiffuse[]= { 0.50f, 0.50f, 0.50f, 1.0f };    //��������
GLfloat planet_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };    //��������
GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f };    //��Դλ��

#define GL_CONSTANT_ATTENUATION        //����˥�� 1.0
#define GL_LINEAR_ATTENUATION        //����˥�� 0.0
#define GL_QUADRATIC_ATTENUATION    //����˥�� 0.0

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; //��������� ( ���� )
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // �������� ( ���� )
GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f }; // ��Դλ�� ( ���� )

glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // ���û�����
glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // ���������
glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // ��Դλ��
glEnable(GL_LIGHT1); // ����һ�Ź�Դ

//��������
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

    //���ù�Դλ��
    //���һ������Ϊ0��˵���Ƿ����Թ�Դ����0��Ϊλ���Թ�Դ
    //default = vec4(0.0, 0.0, 1.0, 0.0)
    void set_position(float x, float y, float z, float w = 1.0f)
    {
        position = vec4f(x, y, z, w);
        glLightfv(m_id, GL_POSITION, position.data); // ��Դλ��
    }

    //���û�����
    //default = vec4(0.0, 0.0, 0.0, 0.0)
    void set_ambient(float r, float g, float b, float a)
    {
        ambient = vec4f(r, g, b, a);
        glLightfv(m_id, GL_AMBIENT, ambient.data);
    }

    //���������
    //Light0 default = vec4(1.0, 1.0, 1.0, 1.0)
    void set_diffuse(float r, float g, float b, float a)
    {
        diffuse = vec4f(r, g, b, a);
        glLightfv(m_id, GL_DIFFUSE, diffuse.data);
    }

    //���þ����
    //default = vec4(1.0, 1.0, 1.0, 1.0)
    void set_specular(float r, float g, float b, float a)
    {
        specular = vec4f(r, g, b, a);
        glLightfv(m_id, GL_SPECULAR, specular.data);
    }

    //ָ���۹�Ƶķ��� default = vec3(0.0, 0.0, -1.0)
    void spot_direction(float x, float y, float z)
    {
        float vec[] = {x, y, z};
        glLightfv(m_id, GL_SPOT_DIRECTION, vec);
    }

    //���þ۹�ƵĽǶ�
    //default = 180.0
    void spot_cutoff(float angle = 180.0f)
    {
        glLightf(m_id,GL_SPOT_CUTOFF, angle);
    }

    //�۹�ָ�� default = 0.0
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

    //�Ƿ������� default=disabled
    static void enable(bool value)
    {
        if(value){
            glEnable(GL_LIGHTING);
        }
        else{
            glDisable(GL_LIGHTING);
        }
    }

    //����ȫ�ֹ��� default=vec4f(0.2f, 0.2f, 0.2f, 1.0f)
    static void global_ambient(float r = 0.2f, float g = 0.2f, float b = 0.2f, float a = 1.0f)
    {
        float globalAmbient[4] = { r, g, b, a };
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    }

    //�Ƿ���˫����� default=GL_FALSE
    static void tow_side(bool enable)
    {
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE , enable ? GL_TRUE : GL_FALSE);
    }

    //��ɫ����
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

    //���淴��Ƕ���μ��� default=GL_FALSE
    static void local_viewer(bool value)
    {
        //������Զ�Ĺ۲���Ϊ�ֲ��۲��
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, value ? GL_TRUE : GL_FALSE);
    }

    //���渨����ɫ default=GL_SINGLE_COLOR
    //ִ��������ͼ�ǲ����ã���ʹЧ�����棬��ԭ���ǣ��Ѿ���������ɫ�г��������
    //��������ͼ���ٰѾ����ӽ�ȥ
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

