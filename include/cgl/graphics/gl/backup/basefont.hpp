#ifndef FONT_DATA_H
#define FONT_DATA_H
//2015-3-24 21:29
//最基本的OpenGL位图字体，用来显示控制台

#include <algorithm>
#include <cgl/graphics/opengl/gl.hpp>
#include <cgl/graphics/monaco20.h>
#include <cgl/graphics/monaco_ttf.h>

_CGL_BEGIN

int base_font_init()
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//fontOffset = glGenLists (128);
	for(int i=0x20; i < 0x80; ++i)
	{
		glNewList(i, GL_COMPILE);
		//字体宽度，高度
		glBitmap(10, 20, 0.0, 20.0, 8.0, 0.0, monaco[i-0x20]);
		glEndList();
	}
	return 0;
}

int base_font_free()
{
	glDeleteLists(0x20, 0x80-0x20);
	return 0;
}

int base_font_print(int x, int y, const wchar_t* text, size_t length = INT_MAX)
{
	if(length>=INT_MAX)length = wcslen(text);
	//glPushAttrib (GL_LIST_BIT);
	//glListBase(fontOffset);
	glPushAttrib(GL_TEXTURE_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glRasterPos2i(x, y);
	glCallLists(length, GL_SHORT, text);
	glPopAttrib();
	return 0;
}

int base_font_print(int x, int y, const char* text, size_t length = INT_MAX)
{
	if(length>=INT_MAX)length = strlen(text);
	glPushAttrib(GL_TEXTURE_BIT);
	glBindTexture(GL_TEXTURE_2D, 0);
	glRasterPos2i(x, y);
	glCallLists(length, GL_BYTE, text);
	glPopAttrib();
	return 0;
}

_CGL_END

#endif
