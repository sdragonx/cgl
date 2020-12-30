
#pragma once

#include "glcontext.hpp"

namespace cgl{
namespace graphics{

glcontext::glcontext() : gl::native_glcontext(),
	state(), textures(), fonts(this)
{

}

glcontext::~glcontext()
{
	this->dispose();
}

int glcontext::create(platform::display display, platform::window window)
{
	if(!this->ready()){
		if(gl::native_glcontext::create(display, window) == CGL_ERROR){
			return CGL_ERROR;
		}
	}

	return init_state();
}

int glcontext::init_state()
{
	//��ʼ��glew
	//glewInit();

	get_state();

	//����Ĭ��״̬

	//��������ӳ��
	glEnable(GL_TEXTURE_2D);

	//�򿪻��
	glEnable(GL_BLEND);
	//alpha��Ϻ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//���ü��ò���
	glEnable(GL_SCISSOR_TEST);

	//�Ż�͸��
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	#ifndef CGL_PLATFORM_GLES
	//ƽ������ GL_FLAT GL_SMOOTH
	//glShadeModel(GL_SMOOTH);

	//����� GL_FASTEST GL_NICEST
	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	//���Ƶ�ͼ�λ�������
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	#endif

	//����alpha����
	//glEnable(GL_ALPHA_TEST);

	//��Ȳ���
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);//������Ȳ��Ժ���
	//glDepthFunc(GL_LEQUAL);//default
	//glClearDepth(1.0);//���������Ȼ��������õ�ֵ

	#ifndef CGL_PLATFORM_GLES

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ö������ʾģʽΪ�����涼�������ʾ
	//glPolygonMode(GL_FRONT, GL_FILL);//GL_POINT GL_LINE GL_FILL
	//glPolygonMode(GL_BACK, GL_LINE);//GL_POINT GL_LINE GL_FILL
	#endif

	/*
	//��OpenGLʵ��֧�ֵ���������̶�������������̶Ȳ���
	glTexParameterf(GL_TEXTURE_2D, 0x84FE, 16);
	// ע�⣺����� xxx ���� �������Թ��� ������̶Ȳ���
	// ���������Ϊ��4 , 8 , 16 �ȡ�����Ҳ������ InitGL() ���� OpenGL ������д��
	//int LrgSupAni;
	//glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &LrgSupAni);
	//��ô xxx �͵�С�ڻ���� LrgSupAni��

	GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; //��������� ( ���� )
	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f }; // �������� ( ���� )
	GLfloat LightPosition[]= { 3.0f, 2.0f, 0.0f, 1.0f }; // ��Դλ�� ( ���� )

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); // ���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); // ���������
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // ��Դλ��
	glEnable(GL_LIGHT1); // ����һ�Ź�Դ
	*/

	//glGetIntegerv(GL_VIEWPORT, m_viewport.data);
	return 0;
}

void glcontext::dispose()
{
	//free fonts
	fonts.dispose();

	//free textures
	textures.clear();

	//free context
	gl::native_glcontext::dispose();
}

bool glcontext::IsExtensionSupported(const char *extension)
{
	const char *extensions = 0;
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
	OpenGL extensions string. Don't be fooled by sub-strings,
	etc. */
	extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

	if(!extensions){
		return false;
	}

	start = extensions;
	for (;;)
	{
		where = strstr(start, extension );
		if ( !where ) break;

		terminator = where + strlen(extension);
		if ( (where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0') )
			return true;
		start = terminator;
	}

	return false;
}

void glcontext::get_state()
{
	//GL_ARB_draw_buffers
	//GL_ARB_pixel_buffer_object
	int n;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &n);
	const_write(state).max_image_size = n;
	const_write(state).non_pow_of_two = IsExtensionSupported("GL_ARB_texture_non_power_of_two");
	const_write(state).vbo = IsExtensionSupported("GL_ARB_vertex_buffer_object");
	const_write(state).fbo = IsExtensionSupported("GL_ARB_framebuffer_object") || IsExtensionSupported("GL_EXT_framebuffer_object");
}

void glcontext::viewport(int left, int top, int width, int height)
{
	glViewport(left, top, width, height);
	glScissor (left, top, width, height);
	/*
	m_viewport.x = left;
	m_viewport.y = top;
	m_viewport.width = width;
	m_viewport.height = height;
	*/
}

vec4i glcontext::viewport()const
{
	//return m_viewport;
	return vec4i();
}

//�����
void glcontext::dither(bool enable)
{
	if(enable){
		glEnable(GL_DITHER);
	}
	else{
		glDisable(GL_DITHER);
	}
}

//����������
void cull_face(bool enable)
{
	if(enable){
		glEnable(GL_CULL_FACE);
	}
	else{
		glDisable(GL_CULL_FACE);
	}
	//glCullFace(GL_BACK);
}

void glcontext::blendfunc(int source, int dest)
{
	glBlendFunc(source, dest);
}

void glcontext::cliprect(int x, int y, int width, int height)
{
	glScissor(x, y, width, height);
}
//---------------------------------------------------------------------------

void glcontext::clear(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

//void glColor3f(float red, float green, float blue, float alpha = 1.0f);
void glcontext::push_view2D(int left, int top, int width, int height)
{
	this->matrix_mode(GL_PROJECTION);
	this->push_matrix();
	this->identity();

#if CGL_COORDINATE == CGL_LOWER_LEFT
	//ֱ������ϵ
	this->ortho(left, width-1, top, height-1, 0, INT16_MAX);
	//�����������棬Ĭ��GL_CCW
	glFrontFace(GL_CCW);
#else
	//windows����ϵ
	//��һ�Ǵ���ģ���ͼ����ʧ��
	//this->ortho(left, left+width-1, top+height-1, top, INT16_MIN, INT16_MAX);
	this->ortho(left, left+width, top+height, top, INT16_MIN, INT16_MAX);
	glFrontFace(GL_CW);
#endif

	//ʮ������ϵ
	//glOrtho(-width/2, width/2, -height/2, height/2, 0, INT_MAX);//

	//��ת��Ļ
	//glScalef(1.0f, -1.0f, 1.0f);
	//glTranslatef(0.0f, -height, 0.0f);

	this->matrix_mode(GL_MODELVIEW);
	this->identity();
	this->translate(0.375f, 0.375f, 0.0f);//GL_POINTS and GL_LINES don't touch the right pixels
	glDisable(GL_DEPTH_TEST);//�ر���Ȳ���
	glDisable(GL_CULL_FACE); //�ر����޳�
}

void glcontext::push_view3D(int left, int top, int right, int bottom)
{
	#ifndef CGL_PLATFORM_GLES
	//glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glFrustum(left, right, bottom, top, 1.0f, 1024);
	//gluPerspective(60.0f, (right-left)/(bottom-top), 1.0, INT_MAX);
	glFrontFace(GL_CCW);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glDisable(GL_DEPTH_TEST);//�ر���Ȳ���
	#endif
}

void glcontext::pop_view()
{
	this->matrix_mode(GL_PROJECTION);
	this->pop_matrix();
	this->matrix_mode(GL_MODELVIEW);

	//glPopAttrib();
}

void glcontext::pen_width(float width)
{
	#ifndef CGL_PLATFORM_GLES
	glPointSize(width);
	#endif
	glLineWidth(width);
}

int glcontext::draw_rect(float x1, float y1, float x2, float y2)
{
	vertex_type vtx[] = {
		vertex_type(x1, y1, 0.0f, 0.0f, 0.0f, color),
		vertex_type(x2, y1, 0.0f, 1.0f, 0.0f, color),
		vertex_type(x2, y2, 0.0f, 1.0f, 1.0f, color),
		vertex_type(x1, y2, 0.0f, 0.0f, 1.0f, color)
	};

	//this->bind_texture(0);
	return this->draw_arrays(GL_LINE_LOOP, vtx, 0, 4);
}

int glcontext::fill_rect(float x1, float y1, float x2, float y2)
{
	vertex_type vtx[] = {
		vertex_type(x1, y1, 0.0f, 0.0f, 0.0f, color),
		vertex_type(x2, y1, 0.0f, 0.0f, 0.0f, color),
		vertex_type(x2, y2, 0.0f, 0.0f, 0.0f, color),
		vertex_type(x1, y2, 0.0f, 0.0f, 0.0f, color)
	};

	this->bind_texture(0);
	return this->draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);
}

int glcontext::draw_image(intptr_t image, float x, float y, float width, float height,
	float u1, float v1, float u2, float v2)
{
	vertex_type vtx[] = {
		#if CGL_COORDINATE == CGL_LOWER_LEFT
		vertex_type(x,         y,          0.0f, u1, v2, color),
		vertex_type(x + width, y,          0.0f, u2, v2, color),
		vertex_type(x + width, y + height, 0.0f, u2, v1, color),
		vertex_type(x        , y + height, 0.0f, u1, v1, color)
		#else
		vertex_type(x,         y,          0.0f, u1, v1, color),
		vertex_type(x + width, y,          0.0f, u2, v1, color),
		vertex_type(x + width, y + height, 0.0f, u2, v2, color),
		vertex_type(x,         y + height, 0.0f, u1, v2, color)
		#endif
	};
	this->bind_texture(image);
	return this->draw_arrays(GL_TRIANGLE_FAN, vtx, 0, 4);
}

void glcontext::draw_image(const teximage& image, float x, float y, float width, float height)
{
	this->draw_image(image.image, x, y, width, height, image.u1, image.v1, image.u2, image.v2);
}

int glcontext::print(float x, float y, const wchar_t* text, size_t length)
{
	glfont* font = fonts.load(cstring<char>(), 16, 256);
	return font->tab_print(this, x, y, text, length);
}

int glcontext::draw_text(int left, int top, int width, int height, const wchar_t* text, size_t length, int style)
{
	glfont* font = fonts.load(cstring<char>(), 16, 256);
	return font->draw_text(this, left, top, width, height, text, length, style);
}

//---------------------------------------------------------------------------
//
// texture
//
//---------------------------------------------------------------------------

int glcontext::create_texture(int width, int height, int format,
		int source_format, int source_type, const byte_t* pixels, int filter)
{
	return this->textures.create(width, height, format, source_format, source_type, pixels, filter);
}

int glcontext::load_texture(const wchar_t* filename, int format)
{
	return this->textures.open(filename, format);
}

int glcontext::default_texture()
{
	return this->textures.defimage();
}

void glcontext::delete_texture(int tex)
{
	this->textures.close(tex);
}

//---------------------------------------------------------------------------
//
// draw_context
//
//---------------------------------------------------------------------------

int glcontext::draw(const draw_context& dc)
{
	if(!dc.commands_count())return -1;

	//�����ֳ�
	//this->push_attrib();

	//��ʼ��״̬
	#ifndef CGL_PLATFORM_GLES
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPointSize(1.0f);
	#endif
	glLineWidth(1.0f);
	glBlendFunc(CGL_SRC_ALPHA, CGL_ONE_MINUS_SRC_ALPHA);
	//ֱ������ϵ
	int viewport[4];
	//glGetIntegerv(GL_VIEWPORT, viewport);
	//glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
	glScissor(0, 0, INT16_MAX, INT16_MAX);

	for(size_t i=0; i<dc.commands_count(); ++i)
	{
		const drawcall& c = dc.commands(i);
		switch(c.command)
		{
		case CGL_POINTS:
		case CGL_LINES:
		case CGL_LINE_LOOP:
		case CGL_LINE_STRIP:
		case CGL_TRIANGLE_STRIP:
		case CGL_TRIANGLES:
		case CGL_TRIANGLE_FAN:
		{
			//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
			this->draw_arrays(c.command, dc.vertex_pointer(), c.start, c.size);
			//glPopClientAttrib();
			break;
		}
		case CGL_CLEAR:
			this->clear(c.red/255.0f, c.green/255.0f, c.blue/255.0f, c.alpha/255.0f);
			break;
		case CGL_COLOR:
			//glColor4f(itr->red, itr->green, itr->blue, itr->alpha);
			//color =
			break;
		case CGL_FLUSH:
			//glFlush();
			break;
		case CGL_DRAW_CONTEXT:
			//reinterpret_cast<draw_context*>(itr->object)->paint();
			break;

		case CGL_TEXTURE_1D:
			//glBindTexture(GL_TEXTURE_1D, c.value);
			break;
		case CGL_TEXTURE_2D:
			this->bind_texture(c.value);
			break;
		case CGL_TEXTURE_3D:
			//glBindTexture(GL_TEXTURE_3D, itr->value);
			break;

		case CGL_IDENTITY:
			this->identity();
			break;
		case CGL_PUSH_MATRIX:
			this->push_matrix();
			break;
		case CGL_POP_MATRIX:
			this->pop_matrix();
			break;
		case CGL_ROTATE:
			this->rotate(c.angle, c.x, c.y, c.z);
			break;
		case CGL_TRANSLATE:
			this->translate(c.x, c.y, c.z);
			break;
		case CGL_SCALE:
			this->scale(c.x, c.y, c.z);
			break;

		case CGL_BLEND_FUNC:
			this->blendfunc(c.blend_src, c.blend_dst);
			break;
		case CGL_CLIPRECT:
			#if CGL_COORDINATE == CGL_LOWER_LEFT
				glScissor(c.x, c.y, c.width, c.height);
			#else
				//��תģʽ
				glGetIntegerv(GL_VIEWPORT, viewport);
				glScissor(c.x, viewport[3] - c.y - c.height, c.width, c.height);
			#endif
			break;
		/*
		case CGL_POINT_SIZE:
			glPointSize(c.value);
			break;
		case CGL_LINE_WIDTH:
		*/
		case CGL_PEN_WIDTH:
			this->pen_width(c.width);
			break;
		default:
			break;
		}
	}

	//this->pop_attrib();
	return 0;
}

}//end namespace graphics
}//end namespace cgl

