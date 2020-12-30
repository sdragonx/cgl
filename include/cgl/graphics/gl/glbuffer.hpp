/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vertexbuffer.h

 2015-10-27 22:18:52

 OpenGL顶点数据传输速度优化
 https://www.cnblogs.com/ygxsk/p/7693974.html

#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9

*/
#ifndef GL_BUFFER_H_20151027221852
#define GL_BUFFER_H_20151027221852

#include "gl.hpp"

namespace cgl{

//#ifdef CGL_USING_BUFFER_OBJECT

class glbuffer
{
private:
    GLenum m_type;
    GLuint m_id;

public:
    glbuffer() : m_type(), m_id()
    {
    }

    ~glbuffer()
    {
        this->dispose();
    }

    GLuint handle()const
    {
        return m_id;
    }

    int create()
    {
        if(!m_id){
            glGenBuffers(1, &m_id);
        }
        return 0;
    }

    template<typename T>
    int write(GLenum type, const T* data, size_t size, GLenum usage = GL_DYNAMIC_DRAW)
    {
        m_type = type;
        glBindBuffer(m_type, m_id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), data, usage);
        glBindBuffer(m_type, 0);
        return 0;
    }

    int write(const vtx3f* data, size_t size, GLenum usage = GL_DYNAMIC_DRAW)
    {
        m_type = GL_ARRAY_BUFFER;
        glBindBuffer(m_type, m_id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(vtx3f), data, usage);
        glBindBuffer(m_type, 0);
        return 0;
    }

    int write(const vtx3fx* data, size_t size, GLenum usage = GL_DYNAMIC_DRAW)
    {
        m_type = GL_ARRAY_BUFFER;
        glBindBuffer(m_type, m_id);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(vtx3fx), data, usage);
        glBindBuffer(m_type, 0);
        return 0;
    }

    int write(const GLuint* data, size_t size, GLenum usage = GL_DYNAMIC_DRAW)
    {
        m_type = GL_ELEMENT_ARRAY_BUFFER;
        glBindBuffer(m_type, m_id);
        glBufferData(m_type, size * sizeof(GLuint), data, usage);

        return 0;
    }

    //GL_ELEMENT_ARRAY_BUFFER GL_ARRAY_BUFFER
    int create(GLenum type, GLsizeiptr size, GLvoid* data, GLenum usage = GL_DYNAMIC_DRAW)
    {
        this->dispose();
        m_type = type;
        glGenBuffers(1, &m_id);
        glBindBuffer(m_type, m_id);
        glBufferData(m_type, size, data, usage);
        return m_id;
    }

    int vertex_buffer(GLsizeiptr size, GLvoid* data, GLenum usage = GL_DYNAMIC_DRAW)
    {
        this->dispose();
        m_type = GL_ARRAY_BUFFER;
        glGenBuffers(1, &m_id);
        glBindBuffer(m_type, m_id);
        glBufferData(m_type, size, data, usage);
        return m_id;
    }

    int index_buffer(GLsizeiptr size, GLvoid* data, GLenum usage = GL_DYNAMIC_DRAW)
    {
        this->dispose();
        m_type = GL_ELEMENT_ARRAY_BUFFER;
        glGenBuffers(1, &m_id);
        glBindBuffer(m_type, m_id);
        glBufferData(m_type, size, data, usage);
        return m_id;
    }

    void dispose()
    {
        if(m_id){
            //glBindBuffer(m_type, m_id);
            //glBufferData(m_type, 0, 0, GL_DYNAMIC_DRAW);
            glDeleteBuffers(1, &m_id);
            m_id = 0;
        }
        m_type = 0;
    }

    void bind()const
    {
        glBindBuffer(m_type, m_id);
    }

    void unbind()const
    {
        glBindBuffer(m_type, 0);
    }

    GLint size()const
    {
        GLint n = 0;
        this->bind();
        glGetBufferParameteriv(m_type, GL_BUFFER_SIZE, &n);
        return n;
    }
    /*
    GLsizeiptr resize(GLsizeiptr size, GLenum usage = GL_STATIC_COPY)
    {
        //glCopyBufferSubData
        GLsizeiptr n = this->size();
        if(size > n)
        {
            GLuint vbotemp = 0;
            glGenBuffers(1, &vbotemp);
            glBindBuffer(GL_COPY_READ_BUFFER, vbotemp);
            glBufferData(GL_COPY_READ_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
            //copy data from m_id to it
            glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, n);
            destroy();
            m_id = vbotemp;
        }
        return 0;
    }
    */

    //GL_STREAM_DRAW GL_DYNAMIC_DRAW GL_STATIC_DRAW
    int write(GLsizeiptr size, GLvoid* data, GLenum usage = GL_STATIC_DRAW)
    {
        assert(m_id);
        glBufferData(m_type, size, data, usage);
        return 0;
    }

    //glBufferSubData会堵塞DMA
    int write(GLintptr offset, GLsizeiptr size, const void* data)
    {
        assert(m_id);
        glBufferSubData(m_type, offset, size, data);
        return 0;
    }

    #ifndef CGL_PLATFORM_GLES
    //GL_READ_ONLY GL_WRITE_ONLY GL_READ_WRITE
    GLvoid* lock(GLenum access = GL_READ_WRITE)
    {
        this->bind();
        return glMapBuffer(m_type, access);
    }

    void unlock()
    {
        glUnmapBuffer(m_type);
    }
    #endif
};

// glvertexbuffer
// glindexbuffer
// gl...buffer


class glstatic_buffer
{
private:
    std::vector<byte_t> m_buffer;

public:
    glstatic_buffer() : m_buffer()
    {
    }

    ~glstatic_buffer()
    {
        this->dispose();
    }

    GLuint handle()const
    {
        return 0;
    }

    int create(GLsizeiptr size, GLvoid* data, GLenum usage = GL_DYNAMIC_DRAW)
    {
        m_buffer.resize(size);
        if(data){
            memcpy(&m_buffer[0], data, size);
        }
        return 0;
    }

    void dispose()
    {
        m_buffer.clear();
    }

    void bind()const
    {
    }

    GLint size()const
    {
        return m_buffer.size();
    }


    /*
    GLsizeiptr resize(GLsizeiptr size, GLenum usage = GL_STATIC_COPY)
    {
        //glCopyBufferSubData
        GLsizeiptr n = this->size();
        if(size > n)
        {
            GLuint vbotemp = 0;
            glGenBuffers(1, &vbotemp);
            glBindBuffer(GL_COPY_READ_BUFFER, vbotemp);
            glBufferData(GL_COPY_READ_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
            //copy data from m_id to it
            glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_READ_BUFFER, 0, 0, n);
            destroy();
            m_id = vbotemp;
        }
        return 0;
    }
    */

    //这个应该自动大小
    int write(GLsizeiptr size, GLvoid* data, GLenum usage = 0)
    {
        m_buffer.resize(size);
        memcpy(&m_buffer[0], data, size);
        return 0;
    }

    int write(GLintptr offset, GLsizeiptr size, const void* data)
    {
        m_buffer.resize(offset + size);
        memcpy(&m_buffer[offset], data, size);
        return 0;
    }

    GLvoid* lock(GLenum access = 0)
    {
        return &m_buffer[0];
    }

    void unlock()
    {

    }
};


}//end namespace cgl

#endif //GL_BUFFER_H_20151027221852