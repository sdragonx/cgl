/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 imagelist.hpp

 2015-04-05 01:46:12

*/
#ifndef GL_IMAGELIST_HPP_20150405014612
#define GL_IMAGELIST_HPP_20150405014612

#include <cgl/graphics/gl/base.hpp>
#include <cgl/graphics/teximage.hpp>
#include <map>
#include <vector>

namespace cgl{
namespace graphics{
namespace gl{

template<typename T, typename USERDATA>
class iconlist
{
public:
    struct value_type
    {
        teximage image;
        USERDATA data;
    };

    typedef const value_type* icon;
    typedef typename std::map<T, value_type> map_type;
    typedef typename map_type::iterator iterator;
    typedef typename map_type::const_iterator const_iterator;
private:
    device_context* m_gl;
    std::vector<intptr_t> m_texlist;//纹理缓存列表
    map_type m_imglist;        //图像信息列表
    int m_format;   //纹理格式
    int m_filter;   //纹理过滤方式
    int m_width;    //纹理大小
    int m_height;
    int m_size;     //图像大小，高度
    int m_u, m_v;   //添加位置

public:
    iconlist(device_context *g) : m_gl(g),
        m_texlist(), m_imglist(),
        m_format(), m_filter(CGL_LINEAR),
        m_width(512), m_height(512), m_size(16), m_u(0), m_v(0)
    {
    }

    iconlist(device_context* g, int width, int height, int size, int format, int filter = CGL_LINEAR)
        : m_gl(g), m_texlist(), m_imglist(), m_u(0), m_v(0)
    {
        this->create(width, height, size, format, filter);
    }

    ~iconlist()
    {
        this->dispose();
    }

    int create(size_t width, size_t height, size_t size, int format, int filter = CGL_LINEAR)
    {
        this->dispose();
        m_width  = width;
        m_height = height;
        m_format = format,
        m_size   = size;
        m_filter = filter;
        m_u = m_v = 0;
        //CGL_LOG("imagelist create");
        return 0;
    }

    void dispose()
    {
        m_imglist.clear();
        for(size_t i=0; i<m_texlist.size(); ++i)
        {
            m_gl->delete_texture(m_texlist[i]);
        }
        m_texlist.clear();
        m_u = m_v = 0;
    }

    size_t size()const
    {
        return m_imglist.size();
    }

    //添加图像
    int insert(const T& index, int width, int height, void* data, int format, const USERDATA& userdata)
    {
        if(!m_gl->ready())return -1;

        if(m_size < height)height = m_size;
        position_move(width);
        m_gl->bind_texture(m_texlist.back());
        glTexSubImage2D(CGL_TEXTURE_2D, 0, m_u, m_v, width, height, format, CGL_UNSIGNED_BYTE, data);
        value_type icon;
        icon.image.image = m_texlist.back();
        icon.image.x = m_u;
        icon.image.y = m_v;
        icon.image.width = width;
        icon.image.height = height;
        icon.image.u1 = float(m_u) / m_width;
        icon.image.v1 = float(m_v) / m_height;
        icon.image.u2 = float(m_u + width) / m_width;
        icon.image.v2 = float(m_v + height) / m_height;
        icon.data = userdata;
        m_imglist[index] = icon;
        m_u += width;
        ++m_u;//做一个像素的间距
        return index;
    }

    icon image(const T& index)const
    {
        const_iterator itr = m_imglist.find(index);
        if(itr != m_imglist.end()){
            return &itr->second;
        }
        else{
            return null;
        }
    }

    bool exist(const T& index)const
    {
        return image(index);
    }

private:
    void position_move(int width)
    {
        int tex = 0;
        //width += 1;//做一个像素的间距
        if(m_u + width > m_width){
            m_u = 0;
            m_v += m_size;
            ++m_v;//行距一个像素
        }

        //添加纹理页
        if(m_texlist.empty() || m_v + m_size > m_height){
            tex = m_gl->create_texture(m_width, m_height, m_format, CGL_RGBA, CGL_UNSIGNED_BYTE, null, m_filter);
            //CGL_LOG("add page %p", this);
            m_texlist.push_back(tex);
            m_u = 0;
            m_v = 0;
        }
    }

    teximage* BindTexture(int index)
    {
        iterator itr = m_imglist.find(index);
        if(itr!=m_imglist.end()){
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

    /*
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
    */
