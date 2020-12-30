/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gltexture.hpp

 2016-03-15 15:57:22


    //各向异性过滤级别，本机16
    //GLfloat fLargest;
    //glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT,&fLargest);
    //glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,fLargest);

    wglShareLists 可以在不同context之间共享 各种缓冲区对象:纹理,vbo对象

*/
#ifndef GLTEXTURE_HPP_20160315155722
#define GLTEXTURE_HPP_20160315155722

#include <vector>
#include <map>

#include <cgl/graphics/sample.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/gl/gl.hpp>
#include <cgl/graphics/gl/glimage.hpp>

#ifndef CGL_MAX_TEXTURE_COUNT
#define CGL_MAX_TEXTURE_COUNT INT16_MAX
#endif

namespace cgl{
namespace graphics{

class gltextures
{
public:
    typedef GLuint texID;
    const static int SAMPLE_TEXTURE = 64;

private:
    typedef std::vector<texID> container_type;
    typedef TYPENAME container_type::iterator iterator;
    typedef TYPENAME container_type::iterator const_iterator;

    container_type m_images;
    GLuint m_defimage;

public:
    gltextures() : m_images(), m_defimage(0)
    {
    }

    ~gltextures()
    {
        this->clear();
    }

    GLuint create(GLsizei width, GLsizei height, GLenum format,
        GLenum src_fmt, GLenum src_type, const GLubyte* pixels, bool mipmaps)
    {
        GLuint tex = this->texImage2D(width, height, format, src_fmt, src_type, pixels, mipmaps);
        if(tex)m_images.push_back(tex);
        return tex;
    }

    GLuint open(const char* fname, GLenum format, bool mipmaps = false)
    {
        GLuint tex = this->texLoadA(fname, format, mipmaps);
        if(tex){
            m_images.push_back(tex);
        }
        return tex;
    }

    GLuint open(const wchar_t* fname, GLenum format, bool mipmaps = false)
    {
        GLuint tex = this->texLoadW(fname, format, mipmaps);
        if(tex){
            m_images.push_back(tex);
        }
        return tex;
    }

    //根据fname判断图片类型
    GLuint open_memory(const wchar_t* fname, const byte_t* data, size_t size, GLenum format, bool mipmaps = false)
    {
        GLuint tex = this->texLoadMemory(data, size, format, mipmaps);
        if(tex)m_images.push_back(tex);
        return tex;
    }

    void close(GLuint image)
    {
        iterator itr = std::find(m_images.begin(), m_images.end(), image);
        if(itr!=m_images.end())
        {
            glDeleteTextures(1, &(*itr));
            m_images.erase(itr);
        }
        if(image == m_defimage){
            m_defimage = 0;
        }
    }

    void clear()
    {
        if(!m_images.empty()){
            glDeleteTextures(m_images.size(), &m_images[0]);
            m_images.clear();
        }
    }

    GLuint operator[](size_t index)
    {
        if(index < m_images.size()){
            return m_images[index];
        }
        else{
            return 0;
        }
    }

    size_t size()const
    {
        return m_images.size();
    }

    void pack_align(int n)
    {
        glPixelStorei(GL_PACK_ALIGNMENT, n);
    }

    void unpack_align(int n)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, n);
    }

    void bind_texture(GLuint image)
    {
        glBindTexture(GL_TEXTURE_2D, image);
    }

    GLuint current()const
    {
        GLint image;
        glGetIntegerv(GL_TEXTURE_2D, &image);
        return image;
    }

    GLint texture_width(GLuint id)
    {
        GLint n = 0;
        this->bind_texture(id);
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &n);
        return n;
    }

    GLint texture_height(GLuint id)
    {
        GLint n = 0;
        this->bind_texture(id);
        //glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &n);
        return n;
    }

    //返回内置图标64x64
    GLuint defimage()
    {
        if(!m_defimage){
            m_defimage = make_sample();
        }
        return m_defimage;
    }

    int max_texture_size()const
    {
        GLint n;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &n);
        return n;
    }

    #ifndef CGL_PLATFORM_GLES
    int max_texture_units()const
    {
        GLint n;
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, &n);
        return n;
    }
    #endif

    int _count_texture(size_t detect_max = INT16_MAX)
    {
        int n = 0;
        for(size_t i=0; i<detect_max; ++i){
            //CGL_LOG("is tex %i = %i", i, glIsTexture(ls[i]));
            if(glIsTexture(i)){
                ++n;
            }
        }
        return n;
    }

    void _force_clear(size_t detect_max = INT16_MAX)
    {
        for(size_t i=0; i<detect_max; ++i){
            if(glIsTexture(i)){
                glDeleteTextures(1, &i);
            }
        }
    }

    void _list(std::vector<GLuint>& ls, size_t detect_max = INT16_MAX)
    {
        for(size_t i=0; i<detect_max; ++i){
            if(glIsTexture(i)){
                ls.push_back(i);
            }
        }
    }

private:
    GLuint texImage2D(long width, long height, GLenum format,
        GLenum src_format, GLenum src_type, const GLubyte *src_data,
        bool mipmaps);

    GLuint texLoadA(const char* fname, GLenum format, bool mipmaps);
    GLuint texLoadW(const wchar_t* fname, GLenum format, bool mipmaps);
    GLuint texLoadMemory(const byte_t* data, size_t size, GLenum format, bool mipmaps);
    GLuint LoadImage(const pixelimage& image, GLenum format, bool mipmaps);

    texID make_sample()
    {
        if(!m_defimage){
            vec3ub data[SAMPLE_TEXTURE * SAMPLE_TEXTURE];
            sample_image(data, SAMPLE_TEXTURE, SAMPLE_TEXTURE);
            m_defimage = texImage2D(SAMPLE_TEXTURE, SAMPLE_TEXTURE, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<byte_t*>(&data), false);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            m_images.push_back(m_defimage);
            return m_defimage;
        }
        return m_defimage;
    }
};

GLuint gltextures::texImage2D(long width, long height, GLenum format,
    GLenum src_format, GLenum src_type, const GLubyte *src_data,
    bool mipmaps)
{
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    //纹理压缩质量
    //GL_FASTEST GL_NICEST
    //glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, src_format, src_type, src_data);

    if(mipmaps){
        glGenerateMipmap(GL_TEXTURE_2D);
        //gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, src_format, src_type, src_data);

        //这个属性设置到普通纹理上，会显示错误。和普通过滤方式有没有别的冲突还未知
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    //设置纹理缩放的插值方式
    //GL_NEAREST GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //纹理边界过滤模式
    //GL_REPEAT             模式下OpenGL在纹理坐标值超过1.0的方向上进行重复
    //GL_CLAMP              所需的纹理单元取自纹理边界或TEXTURE_BORDER_COLOR(glTexParameterfv函数设置的值)
    //GL_CLAMP_TO_EDGE      强制对范围外的纹理坐标沿着合法的纹理坐标的最后一行或一列进行渲染
    //GL_CLAMP_TO_BORDER    范围之外的纹理坐标使用边界纹理单元
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return tex;
}

GLuint gltextures::LoadImage(const pixelimage& image, GLenum format, bool mipmaps)
{
    if(image.is_null()){
        return 0;
    }

    //几个图像加载库加载的数据，都不是4字节对齐
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint tex = texImage2D(
        image.width(),
        image.height(),
        format,
        image.format(),
        CGL_UNSIGNED_BYTE,
        image.data(),
        mipmaps);

    return tex;
}

GLuint gltextures::texLoadA(const char* fname, GLenum format, bool mipmaps)
{
    pixelimage image;
    image.open(fname);
    GLuint tex = LoadImage(image, format, mipmaps);
    image.dispose();
    return tex;
}

GLuint gltextures::texLoadW(const wchar_t* fname, GLenum format, bool mipmaps)
{
    pixelimage image;
    image.open(fname);
    GLuint tex = LoadImage(image, format, mipmaps);
    image.dispose();
    return tex;
}

GLuint gltextures::texLoadMemory(const byte_t* data, size_t size, GLenum format, bool mipmaps)
{
    pixelimage image;
    image.open_memory(data, size);
    GLuint tex = LoadImage(image, format, mipmaps);
    image.dispose();
    return tex;
}

}//end namespace graphics
}//end namespace cgl

#endif //GLTEXTURE_HPP_20160315155722
