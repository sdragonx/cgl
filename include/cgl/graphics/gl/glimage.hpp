/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glimage.hpp

 glimage �������ͷ���������ֻ�൱�ڲ�����������һ����ָ�롱

 2015-10-14 03:27:32

*/
#ifndef GL_IMAGE_HPP_20151014032732
#define GL_IMAGE_HPP_20151014032732

#include <cgl/graphics/image.hpp>
#include <cgl/graphics/gl/gl.hpp>
#include "gltexture.hpp"

namespace cgl{
namespace graphics{

class glimage
{
private:
    GLuint m_image;

public:
    glimage();
    glimage(GLuint tex);
    ~glimage();

    GLuint handle()const { return m_image; }
    bool is_valid()const { return glIsTexture(m_image); }

    glimage& operator=(GLuint tex);
    glimage& operator=(const glimage& image);

    glimage& assign(GLuint image);
    glimage& assign(const glimage& image);
    void bind()const;

    GLuint release();    //�ͷž��

    GLint width()const;
    GLint height()const;
    GLint format()const;

    void filter(GLenum filter);
    void mipmapFilter(GLenum Filter);
    void wrap_mode(GLenum mode);

    void copyscreen();
    int save(const char* filename);
    int save(const wchar_t* filename);

private:
    void bind_always()const
    {
        glBindTexture(GL_TEXTURE_2D, m_image);
    }
};

//
// glimage source
//

glimage::glimage() : m_image()
{
}

glimage::glimage(GLuint tex) : m_image()
{
}

glimage::~glimage()
{
}

glimage& glimage::operator=(GLuint tex)
{
    return this->assign(tex);
}

glimage& glimage::operator=(const glimage& image)
{
    return this->assign(image.m_image);
}

glimage& glimage::assign(GLuint image)
{
    m_image = image;
    return *this;
}

glimage& glimage::assign(const glimage& image)
{
    m_image = image.m_image;
    return *this;
}

void glimage::bind()const
{
    this->bind_always();
}

GLuint glimage::release()//�ͷž��
{
    GLuint image = m_image;
    m_image = 0;
    return image;
}

#if 0
int create(glcontext& context,
    GLsizei width,
    GLsizei height,
    GLenum  format,
    GLenum    src_fmt,
    GLenum    src_type,
    const GLubyte* src_data = NULL,
    GLenum filter = GL_LINEAR)
{
    assert(!m_image);
    m_image = context.textures.create(width, height, format, src_fmt, src_type, src_data, filter);
    this->get_info();
    return m_image;
}

/*
int open(const char_t* file, GLenum format = GL_RGBA8)
{
    m_image = glxLoadImage(file, format);
    this->get_info();
    return 0;
}
*/

int open(const tchar_t* file, GLenum format = GL_RGBA8)
{
    m_image = gl::LoadImage(file, format);
    this->get_info();
    return m_image;
}

int open_memory(const tchar_t* name, const byte_t* data, size_t size, GLenum format = GL_RGBA8)
{
    m_image = gl::LoadImageFromMemory(name, data, size, format);
    this->get_info();
    return m_image;
}

void close()
{
    if(m_image)
    {
        gl::DeleteImage(m_image);
        m_image = 0;
        m_width = m_height = 0;
        m_format = 0;
    }
}
#endif

int glimage::save(const char* file)
{
    pixelimage image;
    image.create(width(), height(), CGL_RGBA, false, null);
    this->bind_always();
    //
    return 0;
}

GLint glimage::width()const
{
    GLint n = 0;
    this->bind_always();
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &n);
    return n;
}

GLint glimage::height()const
{
    GLint n = 0;
    this->bind_always();
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &n);
    return n;
}

GLint glimage::format()const
{
    GLint n = 0;
    this->bind_always();
    //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &n);
    return n;
}

void glimage::filter(GLenum filter)
{
    //�����������ŵĲ�ֵ��ʽ
    //GL_NEAREST GL_LINEAR
    this->bind_always();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void glimage::mipmapFilter(GLenum Filter)
{
    //GL_NEAREST_MIPMAP_NEAREST
    //GL_LINEAR_MIPMAP_NEAREST
    //GL_NEAREST_MIPMAP_LINEAR
    //GL_LINEAR_MIPMAP_LINEAR
    this->bind_always();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void glimage::wrap_mode(GLenum mode)
{
    //this->bind()
    //����߽����ģʽ
    //GL_REPEATģʽ��OpenGL����������ֵ����1.0�ķ����Ͻ����ظ�
    //GL_CLAMP���������Ԫȡ������߽��TEXTURE_BORDER_COLOR(glTexParameterfv�������õ�ֵ)
    //GL_CLAMP_TO_EDGEǿ�ƶԷ�Χ��������������źϷ���������������һ�л�һ�н�����Ⱦ
    //GL_CLAMP_TO_BORDER��Χ֮�����������ʹ�ñ߽�����Ԫ
    this->bind_always();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}

#if 0
int copy_screen()//������Ļ��һ�ݿ���
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if(!m_image)
    {
        GLuint tex = gl::TexImage2D(64, 64, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE, null, GL_NEAREST);
        //GLuint tex;//���ڹ�����
        //glGenTextures(1, &tex);
        this->bind(tex);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, viewport[2], viewport[3], 0);
    }
    else
    {
        this->bind();
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
            0, 0, std::min(width(), viewport[2]), std::min(height(), viewport[3]));
    }
    return 0;
}
#endif

}//end namespace graphics
}//end namespace cgl

#endif //GL_IMAGE_HPP_20151014032732
