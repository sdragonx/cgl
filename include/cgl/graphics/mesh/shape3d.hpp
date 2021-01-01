/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shape3d.hpp

 2019-05-23 13:17:41

*/
#ifndef SHAPE3D_HPP_20190523131741
#define SHAPE3D_HPP_20190523131741

#include <cgl/graphics/effect/ripple.hpp>
#include <cgl/graphics/gl/glbuffer.hpp>
#include <cgl/graphics/gl/glcontext.hpp>
#include <cgl/graphics/gl/glcamera.hpp>
#include <cgl/vertex.hpp>

namespace cgl{
namespace graphics{

template<typename T>
class gridmesh
{
public:
    typedef T vertex_type;
    typedef vertex_type *pointer;
    typedef const vertex_type *const_pointer;

protected:
    int m_width, m_height;
    float m_size;    //tile size
    std::vector<vertex_type> vertexBuffer;
    std::vector<size_t>   indexBuffer;

    glbuffer vbo;
    glbuffer ido;

public:
    gridmesh() : vbo(), ido()
    {

    }

    ~gridmesh()
    {
        vbo.dispose();
        ido.dispose();
    }

    int init(int w, int h, float tilesize);

    virtual void initVertices(vertex_type* vtx, int width, int height, float size);
    virtual void initIndices(size_t* indices, int width, int height);

    int width()const
    {
        return m_width;
    }

    int height()const
    {
        return m_height;
    }

    pointer operator[](int y)
    {
        return &vertexBuffer[y * m_width];
    }

    const_pointer operator[](int y)const
    {
        return &vertexBuffer[y * m_width];
    }

    vertex_type& vertex(int x, int y)
    {
        return vertexBuffer[y * m_width + x];
    }

    void set_color(const vec4ub& color);

    void update()
    {
        if(vbo.handle()){
            byte_t* p = (byte_t*)&vertexBuffer.front();
            vbo.bind();
            //vbo.write(vertexBuffer.size() * sizeof(vertex_type), p, GL_STATIC_DRAW);
            vbo.write(GL_ARRAY_BUFFER, &vertexBuffer[0], vertexBuffer.size(), GL_STATIC_DRAW);
            vbo.unbind();

            ido.bind();
            //ido.write(indexBuffer.size() * sizeof(size_t), &indexBuffer[0], GL_STATIC_DRAW);
            ido.write(&indexBuffer[0], indexBuffer.size(), GL_STATIC_DRAW);
            ido.unbind();
        }
    }

    template<typename DC>
    void paint(DC& dc)
    {
        //set_color(dc.color);
        if(vbo.handle()){
            vbo.bind();
            ido.bind();
            dc.draw_elements(GL_TRIANGLES, null.as<vertex_type*>(), null, indexBuffer.size());
            //dc.draw_elements(GL_TRIANGLES, null.as<vertex_type*>(), &indexBuffer[0], indexBuffer.size());
            vbo.unbind();
            ido.unbind();

        }
        else{
            dc.draw_elements(GL_TRIANGLES, &vertexBuffer[0], &indexBuffer[0], indexBuffer.size());
        }
    }

private:
    void initVertexBuffer();
    void initIndexBuffer();
};

template<typename T>
int gridmesh<T>::init(int w, int h, float tilesize)
{
    m_width = w;
    m_height = h;
    m_size = tilesize;

    vbo.create();
    ido.create();

    initVertexBuffer();
    initIndexBuffer();

    this->update();

    return 0;
}

template<typename T>
void gridmesh<T>::initVertexBuffer()
{
    int size = m_width * m_height;

    vertexBuffer.resize(size);
    initVertices(&vertexBuffer[0], m_width, m_height, m_size);

    vbo.create(GL_ARRAY_BUFFER, size * sizeof(vertex_type), null, GL_STATIC_DRAW);
    vbo.unbind();
}

template<typename T>
void gridmesh<T>::initVertices(vertex_type* vtx, int width, int height, float size)
{
    float coord_x_step = 1.0f / width;
    float coord_y_step = 1.0f / height;
    float x_step = (size * 2) / width;
    float y_step = (size * 2) / height;

    vec4ub color = graphics::color::white;

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            vtx[x].x = -size + x * x_step;
            vtx[x].y =  size - y * y_step;
            vtx[x].z = 0;
            vtx[x].u = x * coord_x_step;
            vtx[x].v = y * coord_y_step;
            vtx[x].color = color;
        }
        vtx += width;
    }
}

//创建索引
template<typename T>
void gridmesh<T>::initIndexBuffer()
{
    const int TRIANGLE_VERTICES = 6;
    size_t *p;

    int index_width = m_width - 1;
    int index_height = m_height - 1;
    int size = index_width * index_height * TRIANGLE_VERTICES;

    indexBuffer.resize(size);
    initIndices(&indexBuffer[0], index_width, index_height);

    ido.create(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(size_t), &indexBuffer[0], GL_STATIC_DRAW);
    ido.unbind();
}

template<typename T>
void gridmesh<T>::initIndices(size_t* indices, int width, int height)
{
    const int TRIANGLE_VERTICES = 6;

    int k = 0;
    for(int y = 0; y < height; ++y){
        for(int x = 0; x < width; ++x){
            //*
            //逆时针
            indices[0] = k + x;
            indices[1] = k + m_width + x;
            indices[2] = k + m_width + x + 1;

            indices[3] = k + x;
            indices[4] = k + m_width + x + 1;
            indices[5] = k + x + 1;
            /*/
            //顺时针
            p[0] = k + x;
            p[1] = k + x + 1;
            p[2] = k + m_width + x + 1;

            p[3] = k + x;
            p[4] = k + m_width + x + 1;
            p[5] = k + m_width + x;
            //*/

            indices += TRIANGLE_VERTICES;
        }
        k += m_width;
    }
}

template<typename T>
void gridmesh<T>::set_color(const vec4ub& color)
{
    for(size_t i=0; i<vertexBuffer.size(); ++i){
        vertexBuffer[i].color = color;
    }
}
//---------------------------------------------------------------------------

class sphere : public gridmesh<vtx3fx>
{
public:
    typedef vtx3fx vertex_type;

protected:
    void initVertices(vertex_type* vtx, int width, int height, float size);
};

void sphere::initVertices(vtx3fx* vtx, int width, int height, float size)
{
    int vtx_size = width * height;
    //int index;

    float coord_x_step = 1.0f / width;
    float coord_y_step = 1.0f / height;

    float x_step = 360.0f / (width - 1);//经度每份对应度数
    float y_step = 180.0f / (height - 1);//纬度每份对应度数

    float angle_x, angle_y;
    float sine_x, cosine_x;
    float latRadius;

    vec4ub color = graphics::color::white;

    for (int y = 0; y < height; ++y){
        angle_x = (y * y_step) * M_RD;
        sine_x = sin(angle_x);
        cosine_x = cos(angle_x);
        latRadius = sine_x * size;
        for (int x = 0; x < width; ++x)
        {
            angle_y = (x * x_step) * M_RD;
            vtx[x].x = -cos(angle_y) * latRadius;
            vtx[x].y = cosine_x * size;
            vtx[x].z = sin(angle_y) * latRadius;
            vtx[x].u = x * coord_x_step;
            vtx[x].v = y * coord_y_step;
            vtx[x].color = color;
        }
        vtx += width;
    }
}

//---------------------------------------------------------------------------

class gridwave : public gridmesh<vtx3fx>
{
public:
    graphics::ripple<int> rp;

public:
    void update(float t)
    {
        rp.update();

        const double scale = 0.0000152587890625f;
        for (int y = 1; y < height(); ++y){
            for (int x = 1; x < width(); ++x){
                this->vertex(x, y).y = rp.front_buffer(x, y) * scale;
                this->vertex(x, y).normal.x = this->vertex(x, y).y - this->vertex(x-1, y).y;//this->vertex(x, y).x - this->vertex(x-1, y-1).x;
                this->vertex(x, y).normal.y = this->vertex(x, y).z - this->vertex(x-1, y-1).z;
                this->vertex(x, y).normal.z = this->vertex(x, y).y - this->vertex(x, y-1).y;
                this->vertex(x, y).normal.normalize();
            }
        }
        gridmesh<vtx3fx>::update();
    }

    void initVertices(vertex_type* vtx, int width, int height, float size);

    void click(int x, int y)
    {
//        vec2i p = vec2i(world_position.x, world_position.z);
//        p += 512;
//        p /= 1024 / wave_size;
//        wave.rp.set_ripple(p.x, p.y, 4, 32);
    }

    void random_ripple()
    {
        //wave.rp.set_ripple(random(wave_size), random(wave_size), 1, 10);
    }
};

void gridwave::initVertices(vertex_type* vtx, int width, int height, float size)
{
    float coord_x_step = 1.0f / width;
    float coord_y_step = 1.0f / height;
    float x_step = (size * 2) / width;
    float y_step = (size * 2) / height;

    vec4ub color = graphics::color::white;

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            vtx[x].x = -size + x * x_step;
            vtx[x].y = 0;
            vtx[x].z = -size + y * y_step;
            vtx[x].u = x * coord_x_step;
            vtx[x].v = y * coord_y_step;
            vtx[x].color = color;
        }
        vtx += width;
    }
}

//---------------------------------------------------------------------------
// 2 * n + 1 地图大小
//
//

class gridmap : public gridmesh<vtx4fx>
{
public:
    gridmap() : gridmesh<vtx4fx>()
    {
    }

    void initVertices(vertex_type* vtx, int width, int height, float size);
    void initIndices(size_t* indices, int width, int height);

    float get_height(float x, float z)
    {
        return tilemap_height(*this, m_width / 2, m_height / 2, x, z);
    }

    void set(int x, int y)
    {
        vec2i p(x, y);
        p += 1;
        if(p.inside(0, 0, m_width, m_height)){
            p.x &= ~1;
            p.y &= ~1;
            this->vertex(p.x, p.y).y = 2.0;

            if(this->vertex(p.x - 2, p.y).y > 0){
                internal_set(p.x - 1, p.y);
            }
            if(this->vertex(p.x + 2, p.y).y > 0){
                internal_set(p.x + 1, p.y);
            }
            if(this->vertex(p.x, p.y - 2).y > 0){
                internal_set(p.x, p.y - 1);
            }
            if(this->vertex(p.x, p.y + 2).y > 0){
                internal_set(p.x, p.y + 1);
            }

            if(this->vertex(p.x - 2, p.y - 2).y > 0){
                internal_set(p.x - 1, p.y - 1, 1.5 + math::random(10) / 5.0f);
            }
            if(this->vertex(p.x + 2, p.y - 2).y > 0){
                internal_set(p.x + 1, p.y - 1, 1.5 + math::random(10) / 5.0f);
            }
            if(this->vertex(p.x - 2, p.y + 2).y > 0){
                internal_set(p.x - 1, p.y + 1, 1.5 + math::random(10) / 5.0f);
            }
            if(this->vertex(p.x + 2, p.y + 2).y > 0){
                internal_set(p.x + 1, p.y + 1, 1.5 + math::random(10) / 5.0f);
            }
        }
    }

    void reset(int x, int y)
    {
        vec2i p(x, y);
        p += 1;
        if(p.inside(0, 0, m_width, m_height)){
            p.x &= ~1;
            p.y &= ~1;
            internal_reset(p.x, p.y);

            internal_reset(p.x-1, p.y);
            internal_reset(p.x, p.y-1);
            internal_reset(p.x+1, p.y);
            internal_reset(p.x, p.y+1);
            internal_reset(p.x-1, p.y-1);
            internal_reset(p.x+1, p.y-1);
            internal_reset(p.x-1, p.y+1);
            internal_reset(p.x+1, p.y+1);
        }
    }

private:
    void internal_set(int x, int y, float value = 2.0f)
    {
        this->vertex(x, y).y = value;
        this->vertex(x, y).color = graphics::color::white;
    }

    void internal_reset(int x, int y)
    {
        this->vertex(x, y).y = 0;
        this->vertex(x, y).color = graphics::color::green;
    }

};

void gridmap::initVertices(vertex_type* vtx, int width, int height, float size)
{
    float coord_x_step = 1.0f / width;
    float coord_y_step = 1.0f / height;
    float x_step = size / width;
    float y_step = size / height;

    vec4ub color = graphics::color::white;

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            vtx[x].x = x * x_step;
            vtx[x].y = y * y_step;
            vtx[x].z = 0;
            vtx[x].u = x * coord_x_step;
            vtx[x].v = y * coord_y_step;
            vtx[x].color = color;
        }
        vtx += width;
    }
}

void gridmap::initIndices(size_t* indices, int width, int height)
{
    const int TRIANGLE_VERTICES = 6;

    int k = 0;
    for(int y = 0; y < width; ++y){
        for(int x = 0; x < width; ++x){
            //*
            //逆时针
            if((x + y) & 1){
                indices[0] = k + x;
                indices[1] = k + m_width + x;
                indices[2] = k + m_width + x + 1;

                indices[3] = k + x;
                indices[4] = k + m_width + x + 1;
                indices[5] = k + x + 1;
            }
            else{
                indices[0] = k + x + 1;
                indices[1] = k + x;
                indices[2] = k + m_width + x;

                indices[3] = k + x + 1;
                indices[4] = k + m_width + x;
                indices[5] = k + m_width + x + 1;
            }

            /*/
            //顺时针
            p[0] = k + x;
            p[1] = k + x + 1;
            p[2] = k + m_width + x + 1;

            p[3] = k + x;
            p[4] = k + m_width + x + 1;
            p[5] = k + m_width + x;
            //*/

            indices += TRIANGLE_VERTICES;
        }
        k += m_width;
    }
}

}//end namespace graphics
}//end namespace cgl

#endif //SHAPE3D_HPP_20190523131741
