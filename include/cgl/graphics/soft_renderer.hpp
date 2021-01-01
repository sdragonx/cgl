/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 soft_renderer.hpp

 2020-04-03 18:44:25

*/
#ifndef SOFT_RENDERER_HPP_20200403184425
#define SOFT_RENDERER_HPP_20200403184425

#include <cgl/public.h>
#include <cgl/mat2.hpp>
#include <cgl/mat3.hpp>
#include <cgl/stack.hpp>
#include <cgl/vec.hpp>
#include <cgl/vertex.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/scanline.hpp>
#include <cgl/graphics/pixel.hpp>

namespace cgl{
namespace graphics{


struct vtx2f
{
    float x, y;
    float u, v;
    uint32_t color;
};

struct triangle2f
{
    vtx2f a, b, c;
};

inline int round(int a){ return a; }

float slope2D(const vtx2f& a, const vtx2f& b)
{
    return float(b.x - a.x) / float(b.y - a.y);
}

template<typename T>
struct pixel_state
{
    T a1, a2, a3;    //三角形面积占比
    uint32_t color;
};

inline uint32_t calc_color(uint32_t c1, uint32_t c2, uint32_t c3,
    int a1, int a2, int a3)
{
    const uint32_t MASK = 0x00FF00FF;
    uint32_t c = 0;
    c += (c1 & MASK) * a1;
    c += (c2 & MASK) * a2;
    c += (c3 & MASK) * a3;
    c >>= 8;
    c &= MASK;

    uint32_t d = 0;
    d += ((c1 >> 8) & MASK) * a1;
    d += ((c2 >> 8) & MASK) * a2;
    d += ((c3 >> 8) & MASK) * a3;
    d &= ~MASK;

    return c | d;
}

void calc_pixel(pixel_state<float>& ps, float x, float y,
    const vtx2f& v1, const vtx2f& v2, const vtx2f& v3,
    float area)
{
    math::get_uvf(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, area, x, y, ps.a2, ps.a3);
    //ps.a1 = 1.0f - ps.a2 - ps.a3;
    int a2 = round(ps.a2);// * 255.0f);
    int a3 = round(ps.a3);// * 255.0f);
    int a1 = 255 - a2 - a3;
    ps.color = calc_color(v1.color, v2.color, v3.color, a1, a2, a3);
}

//
//
//

class soft_renderer
{
public:
    pixelimage colorbuf;
    value_stack<mat2f> m_mat;
    pixelbuffer<uint32_t> pixelbuf;

    pixelbuffer<uint32_t> imagebuf;

    stencil<char, 1920, 1080> stencilbuf;

    std::vector<triangle2f> triangles;

    uint32_t color;

public:
    void init(TImage* Image);
    void clear(uint32_t c = 0xFFFFFFFF);

    void draw_triangle(const vtx3f& v1, const vtx3f& v2, const vtx3f& v3);

    //绘制一个平底或者平顶三角形
    void draw_triangle(vtx2f x1, vtx2f y1, vtx2f x2, vtx2f y2, uint32_t c);

    void solid_line(int x1, int y1, int x2, int y2, uint32_t color);
    void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
    void draw_polyline(std::vector<vec2i>& points, uint32_t color);

    void fill_path_solid(std::vector<vec2i>& points, uint32_t color);
    void fill_path_aa(std::vector<vec2i>& points, uint32_t color);
    void fill_path(std::vector<vec2i>& points, uint32_t color);

    void flush();
    void draw_triangle(const vtx2f& v1, const vtx2f& v2, const vtx2f& v3);
    void AntiAliasing();

public:
    void identity(){ m_mat.value.identity(); }
    void translate(float x, float y) { m_mat.value.translate(x, y); }
    void scale(float x, float y) { m_mat.value.scale(x, y); }
    void rotate(float a) { m_mat.value.rotate(a); }
    void push_matrix() { m_mat.push(); }
    void pop_matrix() { m_mat.pop(); }

private:
    vtx2f transform(const vtx3f& v)
    {
        vec2f p = m_mat.value.transform(v.x, v.y);
        vtx2f v2;
        v2.x = p.x;
        v2.y = p.y;
        v2.u = v.u;
        v2.v = v.v;
        v2.color = RGBA(v.color.r, v.color.g, v.color.b, v.color.a);
        return v2;
    }
};

void soft_renderer::init(TImage* Image)
{
    colorbuf.map(Image->Picture->Bitmap->Handle);
    pixelbuf.assign(Image);//->Picture->Bitmap->Handle);
}

void soft_renderer::clear(uint32_t c)
{
    pixelbuf.fill(c);
    this->stencilbuf.reset();
}

void soft_renderer::solid_line(int x1, int y1, int x2, int y2, uint32_t color)
{
    math::solid_line(pixelbuf, x1, y1, x2, y2, color);
}

void soft_renderer::draw_line(int x1, int y1, int x2, int y2, uint32_t color)
{
    math::smooth_line<uint32_t>(pixelbuf, x1, y1, x2, y2, color);
}

void soft_renderer::draw_polyline(std::vector<vec2i>& points, uint32_t color)
{
    for(size_t i=1; i<points.size(); ++i){
        draw_line(points[i-1].x, points[i-1].y, points[i].x, points[i].y, color);
    }
}

//非抗锯齿扫描，单线模式
void soft_renderer::fill_path_solid(std::vector<vec2i>& points, uint32_t color)
{
    if(points.size() < 2){
        return ;
    }

    scanline solid;    //反锯齿边缘
    scanline::iterator i;
    int x, y;
    int alpha;
    bool fill;

    for(int i=0; i<points.size() - 1; ++i){
        solid.solid_scan(points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }

    if(points.size() > 2){//close_polygon
        solid.solid_scan(points.back().x, points.back().y, points.front().x, points.front().y);
    }

    for(i = solid.lines.begin(); i != solid.lines.end(); ++i){
        scanline::line_type& line = i->second;
        std::sort(line.begin(), line.end());
        y = i->first;
        for(size_t j=0; j<line.size(); ++j){
            if((j & 0x01) == 0){//odd填充
                pixelbuf.blend_hline(line[j].x + 1, line[j + 1].x + 1, y, color);
            }
        }
    }
}

//模板缓冲
//基本效果过得去，但是绘制的细多边形太粗了
void soft_renderer::fill_path_aa(std::vector<vec2i>& ps, uint32_t color)
{
    if(ps.size() < 2){
        return ;
    }

    scanline solid; //背景填充
    scanline aa;    //反锯齿边缘
    scanline::iterator i;
    int x, y;

    for(int i=0; i<ps.size() - 1; ++i){
        solid.solid_scan(ps[i].x, ps[i].y, ps[i+1].x, ps[i+1].y);
        aa.smooth_scan(ps[i].x, ps[i].y, ps[i+1].x, ps[i+1].y);
    }

    if(ps.size() > 2){//close_polygon
        solid.solid_scan(ps.back().x, ps.back().y, ps.front().x, ps.front().y);
        aa.smooth_scan(ps.back().x, ps.back().y, ps.front().x, ps.front().y);
    }

    box2i aabb;
    aabb.assign(ps);
    aabb.expand(2);
    pixelbuf.draw_rect(aabb.min_x, aabb.min_y, aabb.width(), aabb.height(), 0);

    #if 1
    //算法：先填充背景，再绘制反锯齿直线。重要解决问题是，填充重合问题
    //填充背景
    //目前问题是，反锯齿和填充有重合，没法解决
    //这个效果非常好
    stencil<byte_t, 1920, 1080> stlbuf;
    //stlbuf.reset();
    stlbuf.reset(aabb.min_x, aabb.min_y, aabb.width(), aabb.height());

    for(i = solid.lines.begin(); i != solid.lines.end(); ++i){
        scanline::line_type& line = i->second;
        std::sort(line.begin(), line.end());
        y = i->first;
        for(size_t j=0; j<line.size() - 1; j+=1){
            if((j & 0x01) == 0){//odd填充
                pixelbuf.blend_hline(line[j].x + 1, line[j + 1].x + 1, y, color);
                stlbuf.fill(line[j].x + 1, line[j + 1].x + 1, y, 1);
            }
        }
    }

    //绘制反锯齿边缘
    for(scanline::iterator i=aa.lines.begin(); i!=aa.lines.end(); ++i){
        scanline::line_type& line = i->second;
        //std::sort(line.begin(), line.end());
        y = i->first;
        for(size_t j=0; j<line.size(); ++j){
            x = line[j].x;
            int alpha = line[j].alpha;
            if(!stlbuf[y][x]){
                pixelbuf.blend(line[j].x, y, color, alpha);
            }
        }
    }
    #endif

    #if 0
    //算法：反锯齿扫描线两侧添加实心像素，只填充两个相邻实心像素之间的区域
    int x1, x2;
    for(scanline::iterator i= aa.lines.begin(); i!=aa.lines.end(); ++i){
        scanline::line_type& line = i->second;
        std::sort(line.begin(), line.end());
        int n = 0;
        y = i->first;
        for(size_t j=0; j<line.size()-0; ++j){
            int alpha = line[j].alpha;
            if(alpha == -1){
                //pixelbuf.set_pixel(line[j].x, y, 0);
                //continue;
                x1 = line[j].x;
                for( ++j; j < line.size(); ++j){
                    if(line[j].alpha < 0){
                        ++n;
//                        if(n & 1)
                        pixelbuf.blend_hline(x1 + 0, line[j].x - 0, y, color);
                        break;
                    }
                }
            }
            else{
                //pixelbuf.blend(line[j].x, y, 0x00FF00, alpha);
                pixelbuf.blend(line[j].x, y, color, alpha);
            }
        }
    }
    #endif
}

}//end namespace graphics
}//end namespace cgl

#endif //SOFT_RENDERER_HPP_20200403184425
