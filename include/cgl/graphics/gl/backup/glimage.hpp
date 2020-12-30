/*

 glimage.hpp

 glimage 不主动释放纹理句柄，只相当于操作纹理句柄的一个“指针”

 sdragonx 2015-10-14 03:27:32

*/
#ifndef GL_IMAGE_HPP_20151014032732
#define GL_IMAGE_HPP_20151014032732

#include <cgl/graphics/gl/glcontext.hpp>

namespace cgl{
namespace graphics{

class glimage
{
private:
	GLuint m_image;
	GLuint m_fbo;
	int m_width;
	int m_height;
	int m_format;
public:
	glimage():m_image(0), m_fbo(0), m_width(), m_height(), m_format()
	{

	}

	~glimage()
	{
		//close();//会出错
	}

	GLuint handle()const
	{
        return m_image;
	}

	void bind(GLuint image)
	{
		//assert(!m_image);//暂定这个
		m_image = image;
		this->get_info();
	}

	void bind()const
	{
		glBindTexture(GL_TEXTURE_2D, m_image);
	}

	GLuint release()//释放句柄
	{
		GLuint image = m_image;
		m_image = 0;
		m_width = m_height = 0;
		m_format = 0;
		return image;
	}

	#if 0
	int create(glcontext& context,
		GLsizei width,
		GLsizei height,
		GLenum  format,
		GLenum	src_fmt,
		GLenum	src_type,
		const GLubyte* src_data = NULL,
		GLenum filter = GL_LINEAR)
	{
		assert(!m_image);
		m_image = context.textures.create(width, height, format, src_fmt, src_type, src_data, filter);
		this->get_info();
		return m_image;
	}

    /*
    int open(const char_t* file, GLenum format = GL_RGBA8)
    {
    	m_image = glxLoadImage(file, format);
		this->get_info();
		return 0;
    }
	*/

	int open(const tchar_t* file, GLenum format = GL_RGBA8)
	{
		m_image = gl::LoadImage(file, format);
		this->get_info();
		return m_image;
	}

	int open_memory(const tchar_t* name, const byte_t* data, size_t size, GLenum format = GL_RGBA8)
	{
		m_image = gl::LoadImageFromMemory(name, data, size, format);
		this->get_info();
		return m_image;
	}

	int save(const tchar_t* file)
	{
		//err
        return 0;
	}

	void close()
	{
		if(m_image)
		{
			gl::DeleteImage(m_image);
			m_image = 0;
			m_width = m_height = 0;
			m_format = 0;
		}
	}
	#endif

	GLboolean is_open()const
	{
		return m_image;// && glIsTexture(m_image);
	}

	GLint width()const
	{
		return m_width;
	}
	GLint height()const
	{
		return m_height;
	}
	GLint format()const
	{
        return m_format;
	}

	void filter(GLenum filter)
	{
		//设置纹理缩放的插值方式
		//GL_NEAREST GL_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	}

	void mipmapFilter(GLenum Filter)
	{
	//GL_NEAREST_MIPMAP_NEAREST
	//GL_LINEAR_MIPMAP_NEAREST
	//GL_NEAREST_MIPMAP_LINEAR
	//GL_LINEAR_MIPMAP_LINEAR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	void wrap_mode(GLenum mode)
	{
		//this->bind()
		//纹理边界过滤模式
		//GL_REPEAT模式下OpenGL在纹理坐标值超过1.0的方向上进行重复
		//GL_CLAMP所需的纹理单元取自纹理边界或TEXTURE_BORDER_COLOR(glTexParameterfv函数设置的值)
		//GL_CLAMP_TO_EDGE强制对范围外的纹理坐标沿着合法的纹理坐标的最后一行或一列进行渲染
		//GL_CLAMP_TO_BORDER范围之外的纹理坐标使用边界纹理单元
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	}

	#if 0
	int copy_screen()//复制屏幕的一份拷贝
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		if(!m_image)
		{
			GLuint tex = gl::TexImage2D(64, 64, GL_RGBA, GL_RGB, GL_UNSIGNED_BYTE, null, GL_NEAREST);
            //GLuint tex;//不在管理中
            //glGenTextures(1, &tex);
			this->bind(tex);
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 0, 0, viewport[2], viewport[3], 0);
		}
		else
		{
			this->bind();
			glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
				0, 0, std::min(width(), viewport[2]), std::min(height(), viewport[3]));
		}
		return 0;
	}

	int default_image()
	{
		//GLuint tex = glxImage2D(64, 64, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE, ERROR_IMAGE_DATA, GL_LINEAR);
		//this->bind(tex);
		vec3ub data[64*64];
		vec3ub *p = data;
		for(int y=0; y<64; ++y)
		{
			for(int x=0; x<64; ++x)
			{
				if(y<32)
				{
					*p = x<32 ? vec3ub(255, 255, 0) : vec3ub(0, 255, 0);
				}
				else
				{
					*p = x<32 ? vec3ub(255, 0, 0) : vec3ub(0, 0, 255);
				}
				++p;
			}
			//p += 64;
		}
		GLuint tex = gl::TexImage2D(64, 64, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, reinterpret_cast<byte_t*>(&data), GL_NEAREST);
		this->bind(tex);
		return tex;
	}

	#endif
public://fbo
	#if 0 //ndef CGL_PLATFORM_CB6
	int begin_paint()
	{

		assert(!m_image);
		bind();
		//创建帧缓冲区
		glGenFramebuffers(1, &m_fbo);
		//绑定帧缓冲区
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		//绑定纹理
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_image, 0);

		if(glCheckFramebufferStatus(m_fbo))
		{
            //ShowMessage("err");
		}



		//glBindRenderBuffer(GL_RENDERBUFFER, m_fbo);
		return 0;
	}

	int end_paint()
	{
		//glxRecti(100, 100, 200, 200);
        //opengl.
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glxBindImage2D(0);
		if(m_fbo)
		{
			glDeleteFramebuffers(1, &m_fbo);
			m_fbo = 0;
		}
		return 0;
	}
	#endif// CGL_PLATFORM_CB6

private:
	void get_info()
	{
		#ifndef CGL_PLATFORM_GLES
		if(this->is_open()){
			this->bind();
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &m_width);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &m_height);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &m_format);

		}
		#endif
	}
};

}//end namespace graphics
}//end namespace cgl

#endif //GL_IMAGE_HPP_20151014032732
