/*

 imagelist.hpp

 sdragonx 2015-04-05 01:46:12

*/
#ifndef GL_IMAGELIST_HPP_20150405014612
#define GL_IMAGELIST_HPP_20150405014612

#include <map>
#include <vector>

namespace cgl{
namespace graphics{

class draw_context;

namespace gl{

class glcontext;

//iconlist<index type, userdata>
template<typename T, typename U = int>
class iconlist
{
public:
	struct icon_type
	{
		teximage image;
		U data;
	};

	typedef const icon_type* icon_ptr;
	typedef typename std::map<T, icon_type> map_type;
	typedef typename map_type::iterator iterator;
	typedef typename map_type::const_iterator const_iterator;
private:
	std::vector<GLuint> m_texlist;//纹理缓存列表
	map_type m_imglist;		//图像信息列表
	GLenum m_format;//纹理格式
	int m_width;	//纹理大小
	int m_height;
	int m_filter;	//纹理过滤方式
	int m_size;		//图像大小，高度
	int m_u, m_v;	//添加位置

	glcontext* m_gl;
public:
	iconlist():m_texlist(), m_imglist(), m_format(GL_RGBA), m_filter(GL_LINEAR),
		m_width(512), m_height(512), m_size(16), m_u(0), m_v(0)
	{
		m_gl = null;
	}

	iconlist(glcontext* gl, int width, int height, int size, GLenum format = GL_RGBA8, GLenum filter = GL_LINEAR)
		:m_texlist(), m_imglist(), m_u(0), m_v(0)
	{
		this->create(gl, width, height, size, format, filter);
	}
	~iconlist()
	{
		this->clear();
	}

	int create(glcontext*gl, size_t width, size_t height, size_t size, GLenum format = GL_RGBA8, GLenum filter = GL_LINEAR)
	{
		this->clear();
		m_gl = gl;
		m_width  = width;
		m_height = height;
		m_format = format,
		m_size   = size;
		m_filter = filter;
		m_u = m_v = 0;
		return 0;
	}

	void clear()
	{
		m_imglist.clear();
		for(size_t i=0; i<m_texlist.size(); ++i)
		{
			m_gl->textures.close(m_texlist[i]);
		}
		m_texlist.clear();
		m_u = m_v = 0;
	}

	size_t size()const
	{
		return m_imglist.size();
	}

	//添加图像
	int insert(const T& index, int width, int height, void* data, GLenum format, const U& userdata)
	{
		position_move(width);
		glBindTexture(GL_TEXTURE_2D, m_texlist.back());
		glTexSubImage2D(GL_TEXTURE_2D, 0, m_u, m_v, width, std::min(m_size, height), format, GL_UNSIGNED_BYTE, data);
		icon_type icon;
		icon.image.image = m_texlist.back();
		icon.image.x = m_u;
		icon.image.y = m_v;
		icon.image.width = width;
		icon.image.height = std::min(height, m_size);
		icon.image.u1 = float(m_u) / m_width;
		icon.image.v1 = float(m_v) / m_height;
		icon.image.u2 = float(m_u+width)/m_width;
		icon.image.v2 = float(m_v+std::min(m_size, height))/m_height;
		icon.data = userdata;
		m_imglist[index] = icon;
		m_u += width+1;//做一个像素的间距
		return index;
	}

	icon_ptr image(const T& index)const
	{
		const_iterator itr = m_imglist.find(index);
		if(itr != m_imglist.end())
		{
			return &itr->second;
		}
		else
		{
			return null;
		}
	}

	bool exist(const T& index)const
	{
		return image(index);
	}

	int paint(const T& index, int x, int y, int width, int height, const vec4ub& color)
	{
		imageblock *image = BindTexture(index);
		assert(image);
		gl.draw_rect(x, y, x+width, y+height, image->u1, image->v1, image->u2, image->v2, color);
		return 0;
	}

	int paint(draw_context& dc, const T& index, int x, int y, int width, int height, const vec4ub& color)
	{
		imageblock *image = BindTexture(index);
		assert(image);
		//glxRectf(x, y, x+width, y+height, image->u1, image->v1, image->u2, image->v2);
		dc.draw_image(image->image, x, y, width, height, image->u1, image->v1, image->u2, image->v2);
		return 0;
	}

	int paint(const T& index, int x, int y, const vec4ub& color)
	{
		teximage *icon = BindTexture(index);
		assert(icon);
		//m_gl->color(color.red/255.0f, color.green/255.0f, color.blue/255.0f, color.alpha/255.0f);
		//m_gl->draw_rect(x, y, x+icon->width, y+icon->height, color, icon->u1, icon->v1, icon->u2, icon->v2);
		return 0;
	}

	int paint(draw_context& dc, const T& index, int x, int y)
	{
		teximage *image = BindTexture(index);
		assert(image);
		dc.draw_image(image->image, x, y, image->width, image->height, image->u1, image->v1, image->u2, image->v2);
		return 0;
	}
private:
	void position_move(int width)
	{
		GLuint tex = 0;
		GLuint srcformat;
		width += 1;//做一个像素的间距
		if(m_u+width > m_width)
		{
			m_u = 0;
			m_v += m_size + 1;
		}

		if(m_texlist.empty() || m_v+m_size>m_height)
		{//创建纹理
			//*
			#ifndef CGL_PLATFORM_GLES2
			switch(m_format)
			{
			case GL_ALPHA8:
				srcformat = GL_ALPHA;
				break;
			case GL_LUMINANCE8:
				srcformat = GL_LUMINANCE;
				break;
			case GL_INTENSITY8:
				srcformat = GL_INTENSITY;
				break;
			case GL_BGR:
			case GL_RGB:
				srcformat = GL_BGR;
				break;
			case GL_RGBA:
			case GL_BGRA:
			case GL_RGBA8:
				srcformat = GL_BGRA;
				break;
			default:
				//assert(NULL);
				break;
			}
			#else
			switch(m_format)
			{
			case GL_ALPHA:
				srcformat = GL_ALPHA;
				break;
			case GL_LUMINANCE:
				srcformat = GL_LUMINANCE;
				break;
			//case GL_INTENSITY:
			//	srcformat = GL_INTENSITY;
			//	break;
			case GL_BGR:
			case GL_RGB:
				srcformat = GL_BGR;
				break;
			case GL_RGBA:
			case GL_BGRA:
			//case GL_RGBA8:
				srcformat = GL_BGRA;
				break;
			default:
				//assert(NULL);
				break;
			}
            #endif
			//*/

			//////////////tex = m_gl->textures.create(m_width, m_height, m_format, srcformat, GL_UNSIGNED_BYTE, 0, m_filter);
			m_texlist.push_back(tex);
			m_u = 0;
			m_v = 0;
		}
	}

	teximage* BindTexture(int index)
	{
		iterator itr = m_imglist.find(index);
		if(itr!=m_imglist.end())
		{
			glBindTexture(GL_TEXTURE_2D, itr->second.image.image);
			return &itr->second.image;
		}
		return NULL;
	}
};


}//end namespace gl
}//end namespace graphics
}//end namespace cgl

#endif //GL_IMAGELIST_HPP_20150405014612
