/*

 glfont.hpp

 sdragonx 2015-04-05 01:47:10

 abc.ttf-32-1024

*/
#ifndef GLFONT_HPP_20150405014710
#define GLFONT_HPP_20150405014710

#include <cgl/public.h>

#ifdef CGL_USING_FREETYPE
	#include <cgl/graphics/freetype.hpp>
#else
	#include <cgl/graphics/stb_font.hpp>
#endif

#include <cgl/graphics/font.hpp>
#include <cgl/graphics/gl/imagelist.hpp>

namespace cgl{
namespace graphics{

class glfont : public unifont
{
public:
	typedef gl::iconlist<wchar_t, char_info> imagelist_type;

	typedef TYPENAME imagelist_type::icon item;

	const static int SHADOW_SIZE = 2;	//阴影大小
	const static int DEFAULT_TEXTURE_SIZE = 512;

	#ifdef CGL_PLATFORM_GLES
    //GL_ALPHA;//GL_LUMINANCE_ALPHA;
	const static int TEX_FORMAT = GL_RGBA;;
	const static int SRC_FORMAT = GL_LUMINANCE_ALPHA;//GL_LUMINANCE_ALPHA;
	#else
	const static int TEX_FORMAT = CGL_ALPHA8;
	const static int SRC_FORMAT = CGL_ALPHA;
	#endif
private:
	device_context* m_gl;
	#ifdef CGL_USING_FREETYPE
	ftfont m_font;
	#else
	stb_font m_font;
	#endif
	imagelist_type m_imagelist;
	imagelist_type m_shadowlist;
	int m_size;			//字体大小
public:
	glfont(device_context* gl);
	glfont(device_context* gl, const char* font, int size, size_t texture_size = DEFAULT_TEXTURE_SIZE);
	virtual ~glfont();

	int openfont(const char* font, int fontsize, size_t texture_size = DEFAULT_TEXTURE_SIZE);
	int open_memory(const void* data, size_t size, int fontsize, size_t texture_size = DEFAULT_TEXTURE_SIZE);
	virtual void dispose();

	//获得字符宽度
	int char_width(wchar_t ch);

	//获取字符高度
	int char_height();

	//获取字符串宽度
	//int text_width(const wchar_t* text, size_t length);

	item char_item(wchar_t ch)
	{
		if(!m_imagelist.exist(ch)){
			make_char(ch);
		}
		return m_imagelist.image(ch);
	}

	//paint a char
	int put_char(context* dc, int x, int y, wchar_t ch);

	//绘制阴影
	//text_outline
	int draw_shadow(context* dc, int x, int y, const wchar_t* text, size_t length = TEXT_MAX);
private:
	int make_char(wchar_t ch, bool shadow = false);
};



}//end namespace graphics
}//end namespace cgl

//#include <glfont.cpp>

#endif //GLFONT_HPP_20150405014710

