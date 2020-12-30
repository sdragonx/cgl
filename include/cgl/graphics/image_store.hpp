/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 texture_store.hpp

 atlas

 2018-08-08 06:54:08

 从磁盘读取小图片，整合到大图里面，支持绑定zipstore，从zip文件读取图片
 图片读取顺序：磁盘>zip文件

*/
#ifndef TEXTURE_STORE_HPP_20180808065408
#define TEXTURE_STORE_HPP_20180808065408

#include <cgl/algorithm/rectpacker.hpp>
#include <cgl/graphics/gl/glcontext.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/io/zip_store.hpp>
#include <cgl/string.hpp>
#include <cgl/resource/strings.hpp>

namespace cgl{
namespace graphics{

class image_store
{
public:
    io::zip_store* store;
    string_t base_dir;    //final filename = base_dir + filename + ext
    string_t ext;

private:
    glcontext* m_gl;
    std::vector<GLuint> m_atlas;
    std::map<string_t, teximage> m_images;
    math::rectspaker<int> m_pages;
    teximage m_defimage;
    int TEXTURE_SIZE;// = 4096;

public:
    image_store(glcontext* gl);

    int init();

    teximage* defimage();//don't motify!

    bool exists(const string_t& filename)const;

    //create
    teximage* create(const string_t& filename,
        int width, int height,
        int format,
        const byte_t* data,
        bool mipmaps,
        bool can_pack,
        bool fix = true);

    //load image from zip archive or disk
    teximage* open(const string_t& filename, bool can_pack = true, bool fix = true);

private:
    teximage* open_image(const string_t& filename, bool can_pack, bool fix);
};

//---------------------------------------------------------------------------

//typedef image_store glatlas;

//---------------------------------------------------------------------------

image_store::image_store(glcontext* gl) :
    store(null),
    base_dir(),
    ext(),
    m_gl(gl),
    m_atlas(),
    m_images(),
    m_pages(),
    m_defimage()
{
}

int image_store::init()
{
    this->defimage();
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &TEXTURE_SIZE);
    //TEXTURE_SIZE = 10240 + 1024 + 768 + 16;
    if(TEXTURE_SIZE >= 4096){
        TEXTURE_SIZE = 4096;
    }

    m_pages.init(TEXTURE_SIZE, TEXTURE_SIZE, false);
    return 0;
}

teximage* image_store::defimage()//don't motify!
{
    if(!m_defimage.id){
        m_defimage.id = m_gl->textures.defimage();
        m_defimage.x = 0;
        m_defimage.y = 0;
        m_defimage.width = 64;
        m_defimage.height = 64;
        m_images[resource::constant_text<char>::common_image] = m_defimage;
    }
    return &m_defimage;
}

bool image_store::exists(const string_t& filename)const
{
    return m_images.find(filename) != m_images.end();
}


teximage* image_store::create(const string_t& filename,
    int width, int height, int format, const byte_t* data,
    bool mipmaps,
    bool can_pack,
    bool fix)
{
    teximage img;
    int tex;

    //not pack 1024
    if(!can_pack || TEXTURE_SIZE-2 < width || TEXTURE_SIZE-2 < height){
        int tex_format = format;
        if(tex_format == CGL_BGR)tex_format = CGL_RGB;
        if(tex_format == CGL_BGRA)tex_format = CGL_RGBA;
        tex = m_gl->textures.create(width, height, tex_format, format, CGL_UNSIGNED_BYTE, data, mipmaps);
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
        m_images[filename] = img;
        return &m_images[filename];
    }

    #if 0
    //扩张一个像素，对于边缘是透明的图片，有黑边
    pixelimage pic;
    pixelimage temp;
    temp.map(width, height, image_linebytes(width, pfbits(format), 1), format, (void*)data);
    image_expand(pic, temp, 1);

    size_t sw = width + 2;
    size_t sh = height + 2;

    vec2ui page = m_pages.insert(sw, sh, math::maxrects<int>::BEST_AREA_FIT);
    vec4s rc = m_pages[page.x][page.y];
    if(page.x < m_atlas.size()){
        tex = m_atlas[page.x];
    }
    else if(page.x == m_atlas.size()){
        tex = m_gl->textures.create(TEXTURE_SIZE, TEXTURE_SIZE, CGL_RGBA, CGL_RGBA, CGL_UNSIGNED_BYTE, 0, false);
        m_atlas.push_back(tex);
    }
    else{
        return null;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(CGL_TEXTURE_2D, 0, rc.x, rc.y, sw, sh, format, CGL_UNSIGNED_BYTE, pic.data());

    img.image = tex;
    img.x = rc.x + 1;
    img.y = rc.y + 1;
    img.width = width;
    img.height = height;
    //缩小1个像素，不然有裂痕
    img.u1 = (rc.x) / float(TEXTURE_SIZE);
    img.v1 = (rc.y) / float(TEXTURE_SIZE);
    img.u2 = (rc.x + width) / float(TEXTURE_SIZE);
    img.v2 = (rc.y + height) / float(TEXTURE_SIZE);

    #else

    int sw = width;
    int sh = height;
    if(sw < TEXTURE_SIZE) ++sw;
    if(sh < TEXTURE_SIZE) ++sh;

    //add rect to pages
    vec2ui page = m_pages.insert(sw, sh, math::maxrects<int>::BEST_AREA_FIT);
    vec4s rc = m_pages[page.x][page.y];
    if(page.x < m_atlas.size()){
        tex = m_atlas[page.x];
    }
    else if(page.x == m_atlas.size()){
        tex = m_gl->textures.create(TEXTURE_SIZE, TEXTURE_SIZE, CGL_RGBA, CGL_RGBA, CGL_UNSIGNED_BYTE, 0, true);
        m_atlas.push_back(tex);
    }
    else{
        return null;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexSubImage2D(CGL_TEXTURE_2D, 0, rc.x, rc.y, width, height, format, CGL_UNSIGNED_BYTE, data);

    img.id = tex;
    img.x = rc.x;
    img.y = rc.y;
    img.width = width;
    img.height = height;
    //缩小1个像素，不然有裂痕
    if(fix){
        img.u1 = (rc.x + 1) / float(TEXTURE_SIZE);
        img.v1 = (rc.y + 1) / float(TEXTURE_SIZE);
        img.u2 = (rc.x + width - 2) / float(TEXTURE_SIZE);
        img.v2 = (rc.y + height - 2) / float(TEXTURE_SIZE);
    }
    else{
        img.u1 = (rc.x) / float(TEXTURE_SIZE);
        img.v1 = (rc.y) / float(TEXTURE_SIZE);
        img.u2 = (rc.x + width) / float(TEXTURE_SIZE);
        img.v2 = (rc.y + height) / float(TEXTURE_SIZE);
    }
    #endif

    m_images[filename] = img;
    return &m_images[filename];
}

teximage* image_store::open(const string_t& filename, bool can_pack, bool fix)
{
    std::map<string_t, teximage>::iterator itr = m_images.find(filename);
    if(itr != m_images.end()){
        return &itr->second;
    }
    else{
        return open_image(filename, can_pack, fix);
    }
}

teximage* image_store::open_image(const string_t& filename, bool can_pack, bool fix)
{
    pixelimage pic;

    if(store){
        std::vector<byte_t> buf;
        if(store->readfile(buf, filename.c_str())){
            pic.open_memory(&buf[0], buf.size());
        }
        else{
            return null;
        }
    }
    else{
        pic.open(filename.c_str());
        if(pic.is_null()){
            return null;
        }
    }

    return this->create(filename, pic.width(), pic.height(), pic.format(), pic.data(), true, can_pack, fix);
}

}//end namespace graphics
}//end namespace cgl

#endif //TEXTURE_STORE_HPP_20180808065408
