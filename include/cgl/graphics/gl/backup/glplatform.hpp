#ifndef LSD_OPENGL_H_200510182235
#define LSD_OPENGL_H_200510182235
//---------------------------------------------------------------------------

#include "gl.hpp"
#include "basefont.hpp"
//#include "glcontext.hpp"
#include "glimage.hpp"
//#include <cgl/thread/singleton.h>

_CGL_BEGIN


//直角坐标系
inline void draw_rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
    //
    //x1,y1   x2,y1
    struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
	    GLfloat uv[2];
    }vtxLList[] =
    {
    	{ {x1, y1, 0.0f}, {255, 0, 0, 0}, {u1, v1} },
        { {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v1} },
        { {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v2} },
        { {x2, y2, 0.0f}, {255, 0, 0, 255}, {u2, v2} },
    };

    glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//直角坐标系 GL_TRIANGLES模式
inline void draw_rect2(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
    //
    //x1,y1   x2,y1
    struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
	    GLfloat uv[2];
    }vtxLList[] =
    {
    	{ {x1, y1, 0.0f}, {255, 0, 0, 255}, {u1, v1} },//a
        { {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v1} },//b
        { {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v2} },//c
        { {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v1} },//b
        { {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v2} },//c
        { {x2, y2, 0.0f}, {255, 0, 0, 255}, {u2, v2} },//d
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//windows坐标系
inline void ui_draw_rect(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	return draw_rect2(x1, viewport[3]-y2, x2, viewport[3]-y1, u1, v1, u2, v2);
}


//UI坐标系 CW
inline void draw_rect3(float x1, float y1, float x2, float y2, float u1, float v1, float u2, float v2)
{
    //x1,y2   x2,y2
    //
    //x1,y1   x2,y1
    struct vtx{
    	GLfloat vertices[3];
        GLubyte colours[4];
	    GLfloat uv[2];
    }vtxLList[] =
	{
		{ {x1, y1, 0.0f}, {255, 0, 0, 255}, {u1, v2} },
		{ {x2, y1, 0.0f}, {255, 0, 0, 255}, {u2, v2} },
		{ {x1, y2, 0.0f}, {255, 0, 0, 255}, {u1, v1} },
		{ {x2, y2, 0.0f}, {255, 0, 0, 255}, {u2, v1} },
	};

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(vtx), vtxLList[0].vertices);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx), vtxLList[0].colours);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx), vtxLList[0].uv);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}




class glImageManager;
class glRenderObject;
//class render_object<GL_PLATFORM>
class glROManager;


class glTexState
{
public:
	void enable()const
	{
		glEnable(GL_TEXTURE_2D);
	}
	void disable()const
	{
		glDisable(GL_TEXTURE_2D);
	}
	GLint max_size()
	{//最大纹理大小
		GLint size;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
		return size;
	}
};

class glState
{
public:
	static bool is_suppert_vbo()
	{
		return true;
	}
	static bool is_suppert_fbo()
	{
		return true;
    }
};

struct glstate
{
	int support_vbo:1;
    int support_vao:1;
    int support_fbo:1;
    int support_nopow2:1;
};


//opengl 平台环境
class OpenGL_Platform//:public gl_context<T>
{
private:
	HWND  m_hwnd;
	DWORD m_winstyle;	//保存初始窗口状态
	bool  m_fullscreen;	//true 全屏
	int	  m_renderlevel;
    vec4i m_viewport;

	//DWORD FilterMode;	//纹理渲染的状态 GL_LINEAR
	//DWORD ShadeModel;	//设置着色模式 GL_SMOOTH 逐渐过渡的着色
public:
	static glImageManager images;
	static glTexState texture;
public:
	OpenGL_Platform();
	~OpenGL_Platform();

	int  create(HWND hWnd);
	void shutdown();

	void begin_paint();
	void end_paint();
	void push_2d(int left, int top, int right, int bottom);
	void pop_2d();
	void lookat(float atx, float aty, float atz, float anglex, float angley, float anglez);

	void push_3dview(float left, float top, float right, float bottom)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glFrustum(left, right, bottom, top, 1.0f, 1024);
		//gluPerspective(60.0f, (right-left)/(bottom-top), 1.0, INT_MAX);
		glFrontFace(GL_CCW);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//glDisable(GL_DEPTH_TEST);//关闭深度测试
	}

	void pop_3dview()
	{
        glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopAttrib();
    }


    void paint(glImage& image, int x, int y, int width, int height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        image.bind();
        glxRectf(x, y, x+width, y+height, u1, v1, u2, v2);

    }
    void print(int x, int y, const wchar_t* str, size_t length = -1)
    {
        base_font_print(x, y, str, length);
    }

	//设置渲染效果，0最差，1中等，3最好
	int effect_level(int level)
	{
		switch(level)
		{
		case 0://fast
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_FASTEST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_FASTEST);
			break;
		case 1://medium
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			break;
		case 2:
			break;
		default:
			break;
        }
        return 0;
    }

	const GLubyte* version(){ return glGetString(GL_VERSION); }//得到OGL的版本号

	bool is_enabled()
	{
		return wglGetCurrentContext();
	}

//(char*)glGetString(GL_RENDERER);//  得到显卡品牌
//(char*)glGetString(GL_VENDOR);  //得到显卡制造商
public://窗口相关函数
	int  display_mode(int width, int height, int colorbits, bool fullscreen);
	void viewport(int left, int top, int width, int height);
    vec4i viewport()const
    {
    	return m_viewport;
    }
	GLint width()const
	{
		return m_viewport.width;
	}
	GLuint height()const
	{
		return m_viewport.height;
	}
	//int save_screen(const TCHAR* fname);//保存屏幕截图
private://光标相关
public://绘图相关
	void clear(float r, float g, float b, float alpha)
	{
		glClearColor(r, g, b, alpha);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}
	bool is_suppert_vbo()
    {
        return true;
	}

	int draw(const draw_context* obj);
};

extern OpenGL_Platform opengl;

/*
inline void glxRecti(int x1, int y1, int x2, int y2)
{
	float width  = 0;
	float height = 0;
	float fu1, fv1, fu2, fv2;
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	if(u2 == INT_MAX)u2 = width;
	if(v2 == INT_MAX)v2 = height;
	fu1 = u1/width;
	fv1 = v1/height;
	fu2 = u2/width;
	fv2 = v2/height;
	return glxRectf(x1, y1, x2, y2, fu1, fv1, fu2, fv2);
}
//*/


//检测是否包含扩展
//GL_VENDOR, GL_RENDERER, GL_VERSION, GL_SHADING_LANGUAGE_VERSION, or GL_EXTENSIONS.
//GL_ARB_texture_non_power_of_two

//GL_ARB_draw_buffers
//GL_ARB_pixel_buffer_object
//GL_EXT_framebuffer_object
//GL_ARB_framebuffer_object
//GL_ARB_vertex_buffer_object

bool isGLExtensionSupported(const char *extension);
//使用VBO查询字符串为
//isVBOSupported = isGLExtensionSupported( "GL_ARB_vertex_buffer_object" )

_CGL_END

#include "glplatform.cpp"
#include "glcontext.hpp"
#include "glimage.hpp"


//ShowMessage((char*)glGetString(GL_VERSION));
	//ShowMessage((char*)glGetString(GL_EXTENSIONS));

#endif // LSD_OPENGL_H_200510182235


