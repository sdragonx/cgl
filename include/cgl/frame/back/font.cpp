/*

 font.cpp

 sdragonx 2018-07-20 00:43:46

*/
#ifndef FONT_CPP_20180720004346
#define FONT_CPP_20180720004346

#include <cgl/frame/core.hpp>

namespace cgl{
namespace graphics{

glfont* font_get_font(const font_t& font)
{
	return CGL_MAIN_FRAME.gl.fonts.get_font(font);
}

int font_char_width(const font_t& font, wchar_t ch)
{
	gl::glfont* f = font_get_font(font);
	if(f){
		return f->char_width(ch);
	}
	return 0;
}

int font_char_height(const font_t& font)
{
	gl::glfont* f = font_get_font(font);
	if(f){
		return f->char_height();
	}
	return 0;
}

int font_text_width(const font_t& font, const wchar_t* text, size_t length)
{
	gl::glfont* f = font_get_font(font);
	if(f){
		return f->text_width(text, length);
	}
	return 0;
}

}//end namespace graphics
}//end namespace cgl

#endif //FONT_CPP_20180720004346

