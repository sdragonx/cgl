/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gl_atlas.hpp

 2018-12-07 13:52:10

 专门为存放字体图块而写的轻量级图集类

*/
#ifndef GL_ATLAS_HPP_20181207135210
#define GL_ATLAS_HPP_20181207135210

#include <cgl/public.h>
#include <cgl/algorithm.hpp>
#include <cgl/algorithm/rectpacker.hpp>
#include <cgl/graphics/context.hpp>

namespace cgl{
namespace graphics{

template<typename T, typename USERDATA = int>
class glatlas
{
public:
    typedef T index_type;
    struct value_type
    {
        teximage image;
        USERDATA data;
    };

    typedef math::maxrects<int> packer_type;
    typedef TYPENAME packer_type::METHOD METHOD;

private:
    device_context* m_gl;
    int m_texsize;
    int m_format;
    bool m_space;
    bool m_fixed_uv;
    METHOD m_method;
    std::vector<int> m_atlas;
    std::map<index_type, value_type> m_images;
    math::rectspaker<int> m_pages;

public:
    glatlas() :
        m_gl(null),
        m_texsize(1024),
        m_format(CGL_RGBA),
        m_space(true),
        m_fixed_uv(false),
        m_method(packer_type::BEST_AREA_FIT),
        m_atlas(),
        m_images(),
        m_pages(1024, 1024, false)
    {

    }

    //space          图块之间间隔距离（边界扩展没有实现）
    //fixed_uv       图块的uv缩小一个像素
    //pack_method    装箱算法
    void init(device_context* gl, int texsize, int format, bool space, bool fixed_uv = false, METHOD pack_method = packer_type::BEST_AREA_FIT)
    {
        if(m_gl){
            if(!m_atlas.empty()){
                this->dispose();
            }
        }
        m_gl = gl;
        m_texsize = texsize;
        m_format = format;
        m_space = space;
        m_fixed_uv = fixed_uv;
        m_method = pack_method;
        m_pages.init(texsize, texsize, false);
    }

    bool is_init()const
    {
        return m_gl;
    }

    void dispose()
    {
        for(size_t i=0; i < m_atlas.size(); ++i){
            m_gl->delete_texture(m_atlas[i]);
        }
        m_atlas.clear();
        m_images.clear();
        m_pages.init(0, 0, false);
    }

    value_type* create(const index_type& id, int width, int height, int format, const byte_t* data, const USERDATA& userdata);

    bool exist(const index_type& id)
    {
        return is_exists(m_images, id);
    }

    value_type* value(const index_type& id)
    {
        return find_obj(m_images, id);
    }
};

template<typename T, typename U>
TYPENAME glatlas<T, U>::value_type* glatlas<T, U>::create(const T& id, int width, int height, int format, const byte_t* data, const U& userdata)
{
    teximage img;
    int tex = 0;

    //not pack 1024
    if(m_texsize < width || m_texsize < height){
        int tex_format = format;
        if(tex_format == CGL_BGR)tex_format = CGL_RGB;
        if(tex_format == CGL_BGRA)tex_format = CGL_RGBA;
        tex = m_gl->create_texture(width, height, tex_format,
            format, CGL_UNSIGNED_BYTE, data);
        img.id = tex;
        img.width = width;
        img.height = height;
        /*
        img.u1 = 0.0009765625;
        img.v1 = 0.0009765625;
        img.u2 = 0.9990234375f;
        img.v2 = 0.9990234375f;
        */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        value_type val;
        val.image = img;
        val.data = userdata;
        m_images[id] = val;
        return &m_images[id];
    }

    int sw = width;
    int sh = height;
    if(m_space){//增加1个像素单位的边界
        if(sw < m_texsize) ++sw;
        if(sh < m_texsize) ++sh;
    }

    //add rect to pages
    vec2ui page = m_pages.insert(sw, sh, m_method);
    vec4s rc = m_pages[page.x][page.y];
    if(page.x < m_atlas.size()){
        tex = m_atlas[page.x];
    }
    else if(page.x == m_atlas.size()){
        tex = m_gl->create_texture(m_texsize, m_texsize, m_format, CGL_RGBA, CGL_UNSIGNED_BYTE, 0);
        m_atlas.push_back(tex);
    }
    else{
        return null;
    }

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(CGL_TEXTURE_2D, 0, rc.x, rc.y, width, height, format, CGL_UNSIGNED_BYTE, data);
    img.id = tex;
    img.x = rc.x;
    img.y = rc.y;
    img.width = width;
    img.height = height;

    float texsize = m_texsize;
    if(m_fixed_uv){//缩小1个像素单位的uv
        img.u1 = (rc.x + 1) / texsize;
        img.v1 = (rc.y + 1) / texsize;
        img.u2 = (rc.x + width - 2) / texsize;
        img.v2 = (rc.y + height - 2) / texsize;
    }
    else{
        img.u1 = (rc.x ) / texsize;
        img.v1 = (rc.y ) / texsize;
        img.u2 = (rc.x + width) / texsize;
        img.v2 = (rc.y + height) / texsize;
    }

    value_type val;
    val.image = img;
    val.data = userdata;
    m_images[id] = val;
    return &m_images[id];
}

}//end namespace graphics
}//end namespace cgl

#endif //GL_ATLAS_HPP_20181207135210
