#pragma once

#include <cgl/graphics/gl/imagelist.hpp>

#ifndef CGL_USING_FREETYPE
	#include <cgl/graphics/gaussblur.h>
#endif

#include "glfont.hpp"
//#include <cgl/log.hpp>

namespace cgl{
namespace graphics{

//m_size
glfont::glfont(device_context* gl) : m_gl(gl),
	m_font(), m_imagelist(gl), m_shadowlist(gl)
{
	shadow_color = vec4ub(0, 0, 0, 255);
	CGL_LOG("glfont create %p", this);
}

glfont::glfont(device_context* gl, const char* font, int size, size_t texture_size) :
	m_gl(gl), m_font(), m_imagelist(gl), m_shadowlist(gl)
{
	this->openfont(font, size, texture_size);
}

glfont::~glfont()
{
	this->dispose();
}

int glfont::openfont(const char* font, int fontsize, size_t texture_size)
{
	this->dispose();

	if(m_font.open(font) < 0){
		return -1;
	}
	m_size = fontsize;
	m_font.set_size(fontsize);

	color = vec4ub(255, 255, 255, 255);
	shadow_color = vec4ub(0, 0, 0, 255);

	m_imagelist.create(texture_size, texture_size, fontsize, TEX_FORMAT, CGL_NEAREST);
	m_shadowlist.create(texture_size, texture_size, fontsize + SHADOW_SIZE * 2, TEX_FORMAT);

	return 0;
}

int glfont::open_memory(const void* data, size_t size, int fontsize, size_t texture_size)
{
	m_size = fontsize;
	m_imagelist.create(texture_size, texture_size, fontsize, TEX_FORMAT, GL_NEAREST);
	m_shadowlist.create(texture_size, texture_size, fontsize + SHADOW_SIZE * 2, TEX_FORMAT);

	m_font.open_memory(data, size);
	m_font.set_size(fontsize);
	return 0;
}

void glfont::dispose()
{
	m_imagelist.dispose();
	m_shadowlist.dispose();
	m_font.dispose();
}

//获得字符宽度
int glfont::char_width(wchar_t ch)
{
	imagelist_type::icon icon;
	if(!m_imagelist.exist(ch)){
		make_char(ch);
	}
	icon = m_imagelist.image(ch);
	return icon ? icon->data.next_x : 0;
}

//获取字符高度
int glfont::char_height()
{
	return m_size;
}

int glfont::put_char(context* dc, int x, int y, wchar_t ch)
{
	if(!m_gl->ready()){
		return 0;
	}

	if(ch == '\r' || ch == '\n' || ch == '\t'){
		return 0;
	}
	if(!m_imagelist.exist(ch)){
		make_char(ch);
	}
	imagelist_type::icon icon = m_imagelist.image(ch);
	dc->draw_image(icon->image.image,
		x + icon->data.x,
		y + (m_size - icon->data.y),
		icon->image.width,
		icon->image.height,
		icon->image.u1, icon->image.v1, icon->image.u2, icon->image.v2);
	return icon->data.next_x;
}

//绘制阴影
int glfont::draw_shadow(context* dc, int x, int y, const wchar_t* text, size_t length)
{
	imagelist_type::icon icon;
	if(length == TEXT_MAX)length = wcslen(text);
	x += -SHADOW_SIZE + 1;
	y += - SHADOW_SIZE + 1;
	for(size_t i=0; i<length; ++i)
	{
		if(!m_shadowlist.exist(text[i])){
			int n = make_char(text[i], true);
			if(n < 0)continue;
		}
		icon = m_shadowlist.image(text[i]);
		//阴影坐标默认右下移动一个像素
		//m_shadowlist.paint(text[i], x + icon->data.x, y - icon->data.y, shadow_color);
		dc->draw_image(icon->image.image,
			x + icon->data.x,
			y + (m_size - icon->data.y),
			icon->image.width,
			icon->image.height,
			icon->image.u1, icon->image.v1, icon->image.u2, icon->image.v2);
		x += icon->data.next_x;
	}
	return 0;
}

int glfont::make_char(wchar_t ch, bool shadow)
{
	if(!m_gl->ready()){
		return 0;
	}

	pixelimage image;
	char_info info;

	if(ch == 0x20){//空格
		m_font.render_font(image, info, ch);
		image.create(info.next_x, 1, 8);
		image.clear();
		m_imagelist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
		m_shadowlist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
		return 0;
	}

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	if(!shadow)
	{
		if(m_font.render_font(image, info, ch)){
			CGL_LOG_ERROR("glfont : char render faild.");
			return -1;
		}
		//CGL_LOG("rander char : %C %d, %d", ch, image.width(), image.height());

		/*
		mmlog.filename = "log.cpp";
		mmlog<<"render char : "<<ch<<" "<<image.width()<<","<<image.height()<<endline;

		char buf[1024];
		for(size_t y=0; y<image.height(); ++y){
			tohex(buf, image.scanline(y), image.width());
			for(int i=0; i<1024; ++i)if(buf[i] == '0')buf[i] = ' ';
			mmlog<<buf<<endline;
		}
		mmlog<<endline;
        //*/

		/*
		//加深一点阴影
		byte_t* pdata = image.data();

		int n = 0;
		for(size_t y=0; y<image.height(); ++y){
			for(size_t x=0; x<image.width(); ++x){
				n = pdata[x]/2;
				if(n)n += 32;
				if(n>255)n = 255;
				//pdata[x] = n;//pdata[x]>=225 ? 255 : pdata[x];
				//pdata[x] = 255-pdata[x];
			}
			pdata += image.linebytes();
		}
		//*/

		#ifndef CGL_PLATFORM_GLES
		m_imagelist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
		image.dispose();
		#else
			/*
			m_imagelist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
			image.dispose();
			*/
			/*
			//GL_LUMINANCE_ALPHA
			pixelimage la;
			la.create(image.width(), image.height(), 16);
			byte_t* pdata = image.data();
			byte_t* pi = la.data();
			for(size_t y=0; y<image.height(); ++y)
			{
				for(size_t x=0; x<image.width(); ++x)
				{
					pi[x*2] = 255;
					pi[x*2+1] = pdata[x];
				}
				pdata += image.linebytes();
				pi += la.linebytes();
			}

			m_imagelist.insert(ch, image.width(), image.height(), la.data(), SRC_FORMAT, info);
			image.dispose();
			la.dispose();
			/*/
            //GL_RGBA
			pixelimage source;
			source.create(image.width(), image.height(), 32);
			byte_t* pdata = image.data();
			for(size_t y=0; y<image.height(); ++y){
				vec4ub* pi = (vec4ub*)source.scanline(y);
				for(size_t x=0; x<image.width(); ++x){
					pi[x].red = pi[x].green = pi[x].blue = 255;
					pi[x].alpha = pdata[x];
				}
				pdata += image.linebytes();
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
			m_imagelist.insert(ch, image.width(), image.height(), source.data(), CGL_RGBA, info);
			image.dispose();
			source.dispose();
            //*/
		#endif
	}
	else//创建阴影图片
	#ifdef CGL_USING_FREETYPE
	{
		int n = m_font.render_outline(image, info,
			ch, 1.0f,
			vec4ub(255, 255, 255, 255),     //字体颜色，可以使用glColor替换
			vec4ub(0, 0, 0, 255));   //描边颜色
		if(n){
			print_message(L"font renderer faild.");
			return -1;
		}
		m_shadowlist.insert(ch, image.width(), image.height(), image.data(), SRC_FORMAT, info);
		image.dispose();
	}
	#else //CGL_USING_FREETYPE
	{//制作阴影贴图
		#ifdef CGL_PLATFORM_GLES
			//#error
		#endif
		if(m_font.render_font(image, info, ch)){
			print_message(L"font renderer faild.");
			return -1;
		}

		pixelimage shadow;
		shadow.create(image.width() + SHADOW_SIZE * 2, image.height() + SHADOW_SIZE * 2, 8);
		shadow.clear();
		image_blt(shadow, SHADOW_SIZE, SHADOW_SIZE, image);
		gauss_blur(shadow.data(), shadow.width(), shadow.height(), 1, shadow.linebytes(), 2.0f);//shadow_size/2.0f);
		byte_t* pdata = shadow.data();
		for(size_t y = 0; y<shadow.height(); ++y)//加深一点阴影
		{
			for(size_t x = 0; x<shadow.width(); ++x)
			{
//				pdata[x] = pdata[x]>=128 ? 255 : pdata[x]*2;
//				pdata[x] = 255-pdata[x];
//				pdata[x] = pdata[x] < 223 ? 255 : pdata[x]+32;
				if(pdata[x] > 8){
					//pdata[x] = byte_edge(pdata[x] + 25);
				}
			}
			pdata += shadow.linebytes();
		}
		m_shadowlist.insert(ch, shadow.width(), shadow.height(), shadow.data(), SRC_FORMAT, info);
	}
	#endif //CGL_USING_FREETYPE
	return 0;
}

}//end namespace graphics
}//end namespace cgl