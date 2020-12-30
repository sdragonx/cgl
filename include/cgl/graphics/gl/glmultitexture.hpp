/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glmultitexture

 2019-05-18 23:58:40

 多重纹理关闭的方法，就是把GL_TEXTURE0激活

*/
#ifndef GLMULTITEXTURE_20190518235840
#define GLMULTITEXTURE_20190518235840

#include "gl.hpp"

namespace cgl{
namespace graphics{
namespace gl{

class gltexunit
{
private:
    int m_id;
public:
    gltexunit() : m_id()
    {
    }

    gltexunit(int id) : m_id(id)
    {
    }

    int sampler2D()const
    {
        return m_id - GL_TEXTURE0;
    }

    void active()
    {
        glActiveTexture(m_id);
    }

    void bind_texture(int tex)
    {
        this->active();
        glBindTexture(GL_TEXTURE_2D, tex);
    }
};

class glmultitexture
{
public:
    int max_units()const
    {
        GLint n;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &n);
        return n;
    }

    static gltexunit texture(int index)
    {
        return gltexunit(GL_TEXTURE0 + index);
    }

    gltexunit operator[](int index)
    {
        return gltexunit(GL_TEXTURE0 + index);
    }

    void bind_texture(GLuint tex0, GLuint tex1 = 0, GLuint tex2 = 0, GLuint tex3 = 0)
    {
        texture(3).bind_texture(tex3);
        texture(2).bind_texture(tex2);
        texture(1).bind_texture(tex1);
        texture(0).bind_texture(tex0);
    }

};

}//end namespace gl
}//end namespace graphics
}//end namespace cgl

#endif //GLMULTITEXTURE_20190518235840
