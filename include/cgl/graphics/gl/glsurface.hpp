/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glsurface.hpp

 2019-12-25 07:22:41

*/
#ifndef GLSURFACE_HPP_20191225072241
#define GLSURFACE_HPP_20191225072241

#include "glcontext.hpp"

namespace cgl{
namespace graphics{

class glsurface
{
public:
    glcontext& m_gl;
    GLuint m_image;
    GLuint m_fbo;
    vec2i  m_bufsize;
    vec2i  m_size;
    vec4i  m_vp;    //����viewport

public:
    glsurface(glcontext& dc) : m_gl(dc), m_image(), m_fbo(), m_bufsize(), m_size(), m_vp()
    {
    }

    ~glsurface()
    {
        this->dispose();
    }

    int create(int width, int height)
    {
        this->dispose();

        //����֡������
        glGenFramebuffers(1, &m_fbo);
        //��֡������
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        return this->resize(width, height);
    }

    int resize(int width, int height)
    {
        //���ı��С
        if((width <= m_bufsize.width) && (height <= m_bufsize.height)){
            m_size = vec2i(width, height);
            return 0;
        }

        //ɾ������
        if(m_image){
            m_gl.textures.close(m_image);
            m_image = 0;
        }

        //��������
        m_image = m_gl.textures.create(width, height, CGL_RGBA, CGL_RGBA, GL_UNSIGNED_BYTE, null, false);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //������
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_image, 0);

        if(glCheckFramebufferStatus(m_fbo)){
            CGL_LOG("error");
            return -1;
        }

        m_size = m_bufsize = vec2i(width, height);
        return 0;
    }

    void dispose()
    {
        if(m_image){
            m_gl.textures.close(m_image);
            m_image = 0;
        }
        if(m_fbo){
            glDeleteFramebuffers(1, &m_fbo);
            m_fbo = 0;
        }
        m_size = m_bufsize = vec2i();
    }

    int begin_paint()
    {
        glGetIntegerv(GL_VIEWPORT, m_vp.data);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
        //glBindRenderBuffer(GL_RENDERBUFFER, m_fbo);

        m_gl.viewport(0, 0, m_size.width, m_size.height);
        m_gl.push_view2D(0, 0, m_size.width, m_size.height);
        return 0;
    }

    void end_paint()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//        glDrawBuffer(GL_BACK);
//        glBindTexture(GL_TEXTURE_2D, 0);
        m_gl.pop_view();
        m_gl.viewport(m_vp.x, m_vp.y, m_vp.width, m_vp.height);
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //GLSURFACE_HPP_20191225072241
