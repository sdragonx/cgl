/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 drawbuffer.hpp

 drawbuffer_context

 2019-06-06 21:44:56

 和draw_context的区别是，这个适合绘制单一纹理，顶点数量多的场景

*/
#ifndef DRAWBUFFER_HPP_20190606214456
#define DRAWBUFFER_HPP_20190606214456

#include <cgl/graphics/gl/gl.hpp>

namespace cgl{
namespace graphics{

/*

sizeof(vtx3f) == 24

(1025 * 1025) * 24 = 1050625 * 24 = 25215000 = 24.046898M

//网格绘制需要内存，1个面4个顶点

1024    100663296   100M
512     25165824    24M
256     6291456     6M
128     1572864     1.5M
64      393216      393K


插值边全部绘制，8个面，32个顶点

1024    805306368   800M
512     201326592   200M
256     50331648    50M
128     12582912    12M
64      3145728     3M

*/

//目前只支持四边形绘制
//tilemap_renderer
class drawbuffer
{
public:
    typedef vtx3f vertex_type;
public:
    std::vector<vtx3f> m_vb;    //顶点缓冲
    std::vector<int>   m_ib;    //索引缓冲
    int vertex_offset;
    int index_offset;

public:
    vec4ub color;

    drawbuffer() : m_vb(), m_ib(), color(color::white)
    {

    }

    void init(int width, int height)
    {
        m_vb.resize(width * height * 4);
        m_ib.resize(width * height * 6);
        clear();
    }

    void init(size_t size)
    {
        m_vb.resize(size);
        m_ib.resize(size);
        clear();
    }

    size_t size()const
    {
        return vertex_offset;
    }

    void clear()
    {
        vertex_offset = 0;
        index_offset = 0;
    }

    void paint()
    {
        glVertexPointer(3, GL_FLOAT, sizeof(vertex_type), &m_vb[0]);
        glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_type), &m_vb[0].u);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_type), m_vb[0].color.data);
        //glNormalPointer(GL_FLOAT, sizeof(vertex_type), &vertexBuffer[0].normal.data);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);

        glDrawElements(GL_TRIANGLES, index_offset, GL_UNSIGNED_INT, &m_ib[0]);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        //glDisableClientState(GL_NORMAL_ARRAY);
    }

    void flush()
    {
        if(vertex_offset){
            this->paint();
            this->clear();
        }
    }

    void draw_image(float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        push_rect(x, y, width, height, u1, v1, u2, v2);
    }

    void draw_image(const teximage* image,
        float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        push_rect(x, y, width, height, image->u1, image->v1, image->u2, image->v2);
    }

    void draw_image3d(float x, float y, float width, float height, float z,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        push_rect_3d(x, y, width, height, z, u1, v1, u2, v2);
    }

    void point_image(const teximage* image,
        float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        x -= width;
        y -= height;
        width += width;
        height += height;
        push_rect(x, y, width, height, image->u1, image->v1, image->u2, image->v2);
    }

    //绘制四边形插值边框
    //这个需要再根据周围方块筛选那些需要绘制，进行优化
    //周围相同的tile，虽然绘制了也没有问题
    void draw_frame(float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        const float radio = 0.5f;//边框宽度比例
        float hw = width * radio;
        float hh = height * radio;
        float uw = u2 - u1;
        float vh = v2 - v1;
        float uu = (u2 - u1) * radio;
        float vv = (v2 - v1) * radio;

        int alpha = (color.alpha * 256) >> 8;

        //left
        push_rect(x - hw + 0, y + 0, hw, height,   0, alpha, alpha, 0,   u1 - uu, v1, u1, v2);

        //top
        push_rect(x - 0, y - hh + 0, width, hh,   0, 0, alpha, alpha,   u1, v1 - vv, u2, v1);

        //right
        push_rect(x + width - 0,  y + 0, hw, height,   alpha, 0, 0, alpha,   u2, v1, u2 + uu, v2);

        //bottom
        push_rect(x + 0, y + height - 0, width, hh,   alpha, alpha, 0, 0,   u1, v2, u2, v2 + vv);



        //left top
        push_rectNWSE(x - hw, y - hh, hw, hh,   0, 0, alpha, 0,   u1 - uu, v1 - vv, u1, v1);

        //right top
        push_rect(x + width, y - hh, hw, hh,   0, 0, 0, alpha,   u2, v1 - vv, u2 + uu, v1);

        //left bottom
        push_rect(x - hw, y + height, hw, hh,   0, alpha, 0, 0,   u1 - vv, v2, u1, v2 + vv);

        //right bottom
        push_rectNWSE(x + width, y + height, hw, hh,   alpha, 0, 0, 0,   u2, v2, u2 + uu, v2 + vv);
    }

//---------------------------------------------------------------------------
private:
    void push_rect(float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        vertex_type* va = &m_vb[vertex_offset];

        va[0].x = x;
        va[0].y = y;
        va[0].color = color;
        va[0].u = u1;
        va[0].v = v1;

        va[1].x = x + width;
        va[1].y = y;
        va[1].color = color;
        va[1].u = u2;
        va[1].v = v1;

        va[2].x = x + width;
        va[2].y = y + height;
        va[2].color = color;
        va[2].u = u2;
        va[2].v = v2;

        va[3].x = x;
        va[3].y = y + height;
        va[3].color = color;
        va[3].u = u1;
        va[3].v = v2;

        push_rect_index();

        vertex_offset += 4;
    }

    void push_rect_3d(float x, float y, float width, float height, float z,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        vertex_type* va = &m_vb[vertex_offset];

        va[0].x = x;
        va[0].y = y;
        va[0].z = z;
        va[0].color = color;
        va[0].u = u1;
        va[0].v = v1;

        va[1].x = x + width;
        va[1].y = y;
        va[1].z = z;
        va[1].color = color;
        va[1].u = u2;
        va[1].v = v1;

        va[2].x = x + width;
        va[2].y = y + height;
        va[2].z = z;
        va[2].color = color;
        va[2].u = u2;
        va[2].v = v2;

        va[3].x = x;
        va[3].y = y + height;
        va[3].z = z;
        va[3].color = color;
        va[3].u = u1;
        va[3].v = v2;

        push_rect_index();

        vertex_offset += 4;
    }


    //填充一个矩形，并指定四个顶点的alpha
    //NESW
    void push_rect(float x, float y, float width, float height,
        int a1 = 255, int a2 = 255, int a3 = 255, int a4 = 255,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        vertex_type* va = &m_vb[vertex_offset];

        va[0].x = x;
        va[0].y = y;
        va[0].color = color;
        va[0].color.alpha = a1;
        va[0].u = u1;
        va[0].v = v1;

        va[1].x = x + width;
        va[1].y = y;
        va[1].color = color;
        va[1].color.alpha = a2;
        va[1].u = u2;
        va[1].v = v1;

        va[2].x = x + width;
        va[2].y = y + height;
        va[2].color = color;
        va[2].color.alpha = a3;
        va[2].u = u2;
        va[2].v = v2;

        va[3].x = x;
        va[3].y = y + height;
        va[3].color = color;
        va[3].color.alpha = a4;
        va[3].u = u1;
        va[3].v = v2;

        push_rect_index();

        vertex_offset += 4;
    }

    void push_rectNWSE(float x, float y, float width, float height,
        int a1 = 255, int a2 = 255, int a3 = 255, int a4 = 255,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        vertex_type* va = &m_vb[vertex_offset];

        va[0].x = x + width;
        va[0].y = y;
        va[0].color = color;
        va[0].color.alpha = a2;
        va[0].u = u2;
        va[0].v = v1;

        va[1].x = x + width;
        va[1].y = y + height;
        va[1].color = color;
        va[1].color.alpha = a3;
        va[1].u = u2;
        va[1].v = v2;

        va[2].x = x;
        va[2].y = y + height;
        va[2].color = color;
        va[2].color.alpha = a4;
        va[2].u = u1;
        va[2].v = v2;

        va[3].x = x;
        va[3].y = y;
        va[3].color = color;
        va[3].color.alpha = a1;
        va[3].u = u1;
        va[3].v = v1;

        push_rect_index();

        vertex_offset += 4;
    }

    //压入矩形索引
    void push_rect_index()
    {
        int *p = &m_ib[index_offset];
        p[0] = vertex_offset + 0;
        p[1] = vertex_offset + 1;
        p[2] = vertex_offset + 2;
        p[3] = vertex_offset + 0;
        p[4] = vertex_offset + 2;
        p[5] = vertex_offset + 3;
        index_offset += 6;
    }

};

}//end namespace graphics
}//end namespace cgl

#endif //DRAWBUFFER_HPP_20190606214456