/*

 glcontext.hpp

 sdragonx 2015-10-14 03:31:39

*/
#ifndef GL_CONTEXT_HPP_20151014033139
#define GL_CONTEXT_HPP_20151014033139

#pragma hdrstop

#include "gl.hpp"

_CGL_BEGIN

#define OFFSET_ARRAY(p) reinterpret_cast<GLvoid*>(p)

class glimage_t;

template<typename = void>
class gl_context:public draw_context
{
private:
	GLuint m_vbo;//vbo对象
public:
	gl_context();
    ~gl_context();

    void create(bool use_vbo = true);
    void destroy();

    //void clear();   //清空绘制列表
    void update();  //更新数据到vbo
    void paint();   //执行绘制

	void clear_screen(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);
	//void color(float red, float green, float blue, float alpha = 1.0f);
	//void bind_tex2d(GLuint texture);
	//void bind_tex3d(GLuint texture);

	void flush();


//绘制状态
	void fill_mode(GLenum mode);
	void blend_mode(GLenum src, GLenum dst);
	void blend_func(GLenum m);


//图元绘制
	//void begin(GLenum mode);
	//void end();
	//void push_vertex(const vertex_t& v);
	//void push_vertex(float x, float y, float z, float u, float v);
	//void vertex_array(const vertex_t* va);

	//void draw_point(float x, float y, float z);
	//void draw_line(int x1, int y1, int x2, int y2);
	//void draw_rect(float x, float y, float w, float h,
	//    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

	//void draw_image(GLuint image, float x, float y, float w, float h,
	//    float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

	//void draw_circle(float x, float y, float r, float angle_begin, float angle_end, float step);

	//获得当前绘图颜色
	//inline color4f color()const;
    //绘制点阵字体，需要关闭纹理
	int print(int x, int y, const wchar_t* text, int length = INT_MAX);
	int paint(const glimage_t& image, int x, int y, int w, int h,
		float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);//剪裁

public://矩阵变换
    void init_matrix();
    void rotate(float angle, float x, float y, float z);
    void translate(float x, float y, float z);
    void scale(float x, float y, float z);
    void push_matrix();
    void pop_matrix();
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
gl_context<T>::gl_context():m_vbo(0)
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
    this->clear();
    if(!m_vbo && is_suppert_vbo() && use_vbo)
    {//如果支持vbo，创建vbo对象
        glGenBuffers(1, &m_vbo);
        //glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*VERTEX_BUFFER_SIZE, 0, GL_DYNAMIC_DRAW);
    }
}

//
template<typename T>
void gl_context<T>::destroy()
{
    clear();
    if(m_vbo)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }
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
    if(m_vb.empty())return ;
    if(m_vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        #if 0
        vertex_t* pvtx = (vertex_t*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        memcpy(pvtx, &m_vb[0], sizeof(vertex_t)*m_vb.size());
        glUnmapBuffer(GL_ARRAY_BUFFER);
        #else
        //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t)*m_vb.size(), OFFSET_ARRAY(&m_vb[0]));
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t)*m_vb.size(), OFFSET_ARRAY(&m_vb[0]), GL_DYNAMIC_DRAW);
        #endif
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

//执行绘制
template<typename T>
void gl_context<T>::paint()
{
    if(m_rclist.empty())return ;
    if(m_vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(0));
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, u)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, color)));
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(&m_vb[0]));
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(&m_vb[0].u));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), OFFSET_ARRAY(&m_vb[0].color));
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    for(size_t i=0; i<m_rclist.size(); ++i)
    {
        switch(m_rclist[i].command)
        {
        case RENDER_COMMAND::CLEAR:
            glClearColor(m_rclist[i].red, m_rclist[i].green, m_rclist[i].blue, m_rclist[i].alpha);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            break;
        case RENDER_COMMAND::POINTS:
            //bind_tex2d(0);
            glDrawArrays(GL_POINTS, m_rclist[i].start, m_rclist[i].size);
            break;
        case RENDER_COMMAND::LINES:
            //bind_tex2d(0);
            glDrawArrays(GL_LINES, m_rclist[i].start, m_rclist[i].size);
            break;
        case RENDER_COMMAND::LINE_STRIP:
            //bind_tex2d(0);
            glDrawArrays(GL_LINE_STRIP, m_rclist[i].start, m_rclist[i].size);
            break;
        case RENDER_COMMAND::QUADS:
            glDrawArrays(GL_QUADS, m_rclist[i].start, m_rclist[i].size);
            break;
        case RENDER_COMMAND::COLOR:
            glColor4f(m_rclist[i].red, m_rclist[i].green, m_rclist[i].blue, m_rclist[i].alpha);
            break;
        case RENDER_COMMAND::TEXTURE_2D:
			glBindTexture(GL_TEXTURE_2D, m_rclist[i].image);
			break;
		case RENDER_COMMAND::TEXTURE_3D:
            glBindTexture(GL_TEXTURE_3D, m_rclist[i].image);
            break;
        case RENDER_COMMAND::PUSH_MATRIX:
            glPushMatrix();
            break;
        case RENDER_COMMAND::POP_MATRIX:
            glPopMatrix();
            break;
        case RENDER_COMMAND::ROTATE:
            glRotatef(m_rclist[i].angle, m_rclist[i].x, m_rclist[i].y, m_rclist[i].z);
            break;
        case RENDER_COMMAND::TRANSLATE:
            glTranslatef(m_rclist[i].x, m_rclist[i].y, m_rclist[i].z);
            break;
        case RENDER_COMMAND::SCALE:
            glScalef(m_rclist[i].x, m_rclist[i].y, m_rclist[i].z);
            break;
        default:
            break;
        }
    }
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    if(m_vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

template<typename T>
void gl_context<T>::flush()
{
    
}

/*
template<typename T>
void gl_context<T>::clear_screen(float red, float green, float blue, float alpha)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::CLEAR;
    rc.red = red;
    rc.green = green;
    rc.blue = blue;
    rc.alpha = alpha;
    m_rclist.push_back(rc);
    this->flush();
}

template<typename T>
void gl_context<T>::color(float red, float green, float blue, float alpha)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::COLOR;
    rc.red = red;
    rc.green = green;
    rc.blue = blue;
    rc.alpha = alpha;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::bind_tex2d(GLuint texture)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::TEXTURE_2D;
    rc.texture = texture;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::bind_tex3d(GLuint texture)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::TEXTURE_3D;
    rc.texture = texture;
    m_rclist.push_back(rc);
}

//图元绘制
template<typename T>
void gl_context<T>::begin(GLenum mode)
{
    RENDER_COMMAND rc;

    switch(mode)
    {
    case GL_POINTS:
        rc.command = RENDER_COMMAND::POINTS;
        break;
    case GL_LINES:
        rc.command = RENDER_COMMAND::LINES;
        break;
    case GL_LINE_STRIP:
        rc.command = RENDER_COMMAND::LINE_STRIP;
        break;
    case GL_QUADS:
        rc.command = RENDER_COMMAND::QUADS;
        break;
    default:
        m_begin_paint = false;
        return ;
    }
    if(m_rclist.empty() || m_rclist.back().command!=rc.command)
    {
        rc.start = m_vb.size();
        rc.size = 0;
        m_rclist.push_back(rc);
    }
    m_begin_paint = true;
}
template<typename T>
void gl_context<T>::end()
{
    m_begin_paint = false;
}

template<typename T>
void gl_context<T>::push_vertex(const vertex_t& v)
{
    assert(m_begin_paint);
    if(m_begin_paint){
        ++m_rclist.back().size;
        m_vb.push_back(v);
    }
}

template<typename T>
void gl_context<T>::push_vertex(float x, float y, float z, float u, float v)
{
    vertex_t vtx = {x, y, z, u, v};
    push_vertex(vtx);
}

template<typename T>
void gl_context<T>::draw_point(float x, float y, float z)
{
    this->begin(GL_POINTS);
    this->push_vertex(x, y, z, 0.0f);
    this->end();
}

template<typename T>
void gl_context<T>::draw_line(int x1, int y1, int x2, int y2)
{
    this->begin(GL_LINES);
    vtx2f v;
    v.x = x1;
    v.y = y1;
    this->push_vertex(v);
    v.x = x2;
    v.y = y2;
    this->push_vertex(v);
    this->end();
}

template<typename T>
void gl_context<T>::draw_rect(float x, float y, float w, float h,
    float u1, float v1, float u2, float v2)
{
    this->begin(GL_QUADS);
    vertex_t v;
    v.color.red = 255;
    v.color.green = 255;
    v.color.blue = 255;
    v.color.alpha = 255;
    v.x = x;
    v.y = y;
    v.u = u1;
    v.v = v2;
    this->push_vertex(v);
    v.x = x+w;
    v.y = y;
    v.u = u2;
    v.v = v2;
    this->push_vertex(v);
    v.x = x+w;
    v.y = y+h;
    v.u = u2;
    v.v = v1;
    this->push_vertex(v);
    v.x = x;
    v.y = y+h;
    v.u = u1;
    v.v = v1;
    this->push_vertex(v);
    this->end();
}

template<typename T>
void gl_context<T>::draw_image(GLuint image, float x, float y, float w, float h,
        float u1, float v1, float u2, float v2)
{
    this->bind_tex2d(image);
    this->draw_rect(x, y, w, h, u1, v1, u2, v2);
}

template<typename T>
void gl_context<T>::draw_circle(float x, float y, float r, float angle_begin, float angle_end, float step)
{
    this->begin(GL_LINE_STRIP);
    vertex_t v;
    for(float angle=angle_begin; angle<angle_end; angle+=step)
    {
        v.x = x+r*cos(angle*M_RD);
        v.y = y-r*sin(angle*M_RD);
        push_vertex(v);
    }
    v.x = x+r*cos(angle_end*M_RD);
    v.y = y-r*sin(angle_end*M_RD);
    push_vertex(v);
    this->end();
}

//获得当前绘图颜色
template<typename T>
inline color4f gl_context<T>::color()const
{
    color4f c;
    glGetFloatv(GL_CURRENT_COLOR, c.data);
    return c;
}
*/

template<typename T>
int gl_context<T>::print(int x, int y, const wchar_t* text, int length)
{//绘制点阵字体，需要关闭纹理
    //bind_tex2d(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    base_font_print(x, y, text, length);
    return 0;
}

template<typename T>
void gl_context<T>::init_matrix()
{
    flush();
    glLoadIdentity();
}

/*
template<typename T>
void gl_context<T>::rotate(float angle, float x, float y, float z)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::ROTATE;
    rc.x = x;
    rc.y = y;
    rc.z = z;
    rc.angle = angle;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::translate(float x, float y, float z)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::TRANSLATE;
    rc.x = x;
    rc.y = y;
    rc.z = z;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::scale(float x, float y, float z)
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::SCALE;
    rc.x = x;
    rc.y = y;
    rc.z = z;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::push_matrix()
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::PUSH_MATRIX;
    m_rclist.push_back(rc);
}

template<typename T>
void gl_context<T>::pop_matrix()
{
    RENDER_COMMAND rc;
    rc.command = RENDER_COMMAND::POP_MATRIX;
    m_rclist.push_back(rc);
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
