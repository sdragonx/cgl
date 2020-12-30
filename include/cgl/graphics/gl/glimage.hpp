/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glimage.hpp

 glimage 不主动释放纹理句柄，只相当于操作纹理句柄的一个“指针”

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

    GLuint release();    //释放句柄

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

GLuint glimage::release()//释放句柄
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
    //设置纹理缩放的插值方式
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
    //纹理边界过滤模式
    //GL_REPEAT模式下OpenGL在纹理坐标值超过1.0的方向上进行重复
    //GL_CLAMP所需的纹理单元取自纹理边界或TEXTURE_BORDER_COLOR(glTexParameterfv函数设置的值)
    //GL_CLAMP_TO_EDGE强制对范围外的纹理坐标沿着合法的纹理坐标的最后一行或一列进行渲染
    //GL_CLAMP_TO_BORDER范围之外的纹理坐标使用边界纹理单元
    this->bind_always();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
}

#if 0
int copy_screen()//复制屏幕的一份拷贝
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    if(!m_image)
    {
        GLuint tex = gl::TexImage2D(64, 64, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE, null, GL_NEAREST);
        //GLuint tex;//不在管理中
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
