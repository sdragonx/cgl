/*

 glcontext.hpp

 sdragonx 2015-10-14 03:31:39

*/
#ifndef GL_CONTEXT_HPP_20151014033139
#define GL_CONTEXT_HPP_20151014033139

#pragma hdrstop

#include "gl.hpp"
#include "glvbo.hpp"
#include <cgl/graphics/drawcontext.hpp>

_CGL_BEGIN
//#define OFFSET_ARRAY(p) reinterpret_cast<GLvoid*>(p)

class glimage_t;


template<typename = void>
class gl_context:public draw_context
{
private:
	glvbo m_vbo;
public:
	gl_context();
    ~gl_context();

    void create(bool use_vbo = true);
    void destroy();

	//void clear();   //清空绘制列表
	void update();  //更新数据到vbo
	void paint();   //执行绘制



public://矩阵变换
    void init_matrix();
	//void rotate(float angle, float x, float y, float z);
	//void translate(float x, float y, float z);
	//void scale(float x, float y, float z);
	//void push_matrix();
	//void pop_matrix();
	void model_view();
    void projection_view();
    void matrix_mode(GLenum mode);
    void load_matrix(float* matrix);
protected:
	int init_vbo(GLuint texture);
    bool is_suppert_vbo();
};

typedef gl_context<> gl_renderobj;



template<typename T>
gl_context<T>::gl_context():m_vbo()
    //,m_pen_width(1.0f), m_brush_image(0), m_brush_color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

template<typename T>
gl_context<T>::~gl_context()
{
    
}

//
template<typename T>
void gl_context<T>::create(bool use_vbo)
{
    this->clear_buffer();
	if(use_vbo)m_vbo.create(0, 0, GL_STATIC_DRAW);
}

//
template<typename T>
void gl_context<T>::destroy()
{
    this->clear_buffer();
    m_vbo.destroy();
}

//清空缓冲区
/*
template<typename T>
void gl_context<T>::clear()
{
	m_vb.clear();
	m_rclist.clear();
}
*/

//更新数据到vbo
template<typename T>
void gl_context<T>::update()
{
	if(m_vertexbuffer.empty())return ;
	if(m_vbo.handle())
	{
		m_vbo.bind();
		m_vbo.write(
			sizeof(vertex_t)*m_vertexbuffer.size(),
			OFFSET_ARRAY(&m_vertexbuffer[0]),
			//GL_DYNAMIC_DRAW);
			GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);//必须执行，可能会出现别的问题
	}
}


/*
template<typename T>
void gl_context<T>::rotate(float angle, float x, float y, float z)
{
    DRAW_INSTR rc;
    rc.command = DRAW_INSTR::ROTATE;
    rc.x = x;
    rc.y = y;
    rc.z = z;
    rc.angle = angle;
    m_cmdbuffer.push_back(rc);
}

template<typename T>
void gl_context<T>::translate(float x, float y, float z)
{
    DRAW_INSTR rc;
    rc.command = DRAW_INSTR::TRANSLATE;
    rc.x = x;
    rc.y = y;
    rc.z = z;
    m_cmdbuffer.push_back(rc);
}

template<typename T>
void gl_context<T>::scale(float x, float y, float z)
{
    DRAW_INSTR rc;
    rc.command = DRAW_INSTR::SCALE;
	rc.x = x;
	rc.y = y;
	rc.z = z;
	m_cmdbuffer.push_back(rc);
}

template<typename T>
void gl_context<T>::push_matrix()
{
    DRAW_INSTR rc;
	rc.command = DRAW_INSTR::PUSH_MATRIX;
    m_cmdbuffer.push_back(rc);
}

template<typename T>
void gl_context<T>::pop_matrix()
{
	DRAW_INSTR rc;
	rc.command = DRAW_INSTR::POP_MATRIX;
	m_cmdbuffer.push_back(rc);
}
*/

template<typename T>
bool gl_context<T>::is_suppert_vbo()
{//vbo 比 顶点数组要慢，不知道原因啊
	return opengl.is_suppert_vbo();
}

template<typename T>
int gl_context<T>::paint(const glimage_t& image, int x, int y, int w, int h,
	float u1, float v1, float u2, float v2)
{
	//image.bind();
	opengl.bind_tex2d(image.handle());
	//glxRectf(x, y, x+w, y+h, u1, v1, u2, v2);
	this->rectangle(x, y, w, h, u1, v1, u2, v2);
	return 0;
}

_CGL_END

//#include "glcontext.cxx"

#endif //GL_CONTEXT_HPP_20151014033139
