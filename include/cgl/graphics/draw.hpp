/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dc_draw.hpp

 2019-04-06 20:57:42

*/
#ifndef DC_DRAW_HPP_20190406205742
#define DC_DRAW_HPP_20190406205742

#include <cgl/public.h>
#include <cgl/graphics/draw_context.hpp>
#include <cgl/graphics/teximage.hpp>
#include <cgl/vertex.hpp>

namespace cgl{
namespace graphics{


//绘制正菱形rhomboid
void fill_rhomboid(draw_context& dc, float x, float y, float size, float pen_width)
{
    float s = size / 2;
    if(pen_width > s)pen_width = s;

    draw_context::vertex_type v;

    dc.begin(CGL_TRIANGLE_STRIP);

    v.color = dc.color;
    dc.vertex(x + s, y);
    dc.vertex(x + s, y + pen_width);
    dc.vertex(x, y + s);
    dc.vertex(x + pen_width, y + s);
    dc.vertex(x + s, y + size);
    dc.vertex(x + s, y + size - pen_width);
    dc.vertex(x + size, y + s);
    dc.vertex(x + size - pen_width, y + s);
    dc.vertex(x + s, y);
    dc.vertex(x + s, y + pen_width);

    dc.end();
}

/*
绘制菱形rhombus

   /\
  /  \
  \  /
   \/
*/

void fill_diamond(draw_context& dc, float x, float y, float width, float height, float pen_width)
{
    float w = width / 2.0f;//边长一半
    float h = height / 2.0f;
    float penw, penh;
    if(width < height){//计算画笔偏移量宽度
        if(width < 0.0f) return ;
        if(pen_width > w)pen_width = w;

        penw = pen_width;
        penh = pen_width * height / width;
    }
    else{
        if(height < 0.0f) return ;

        if(pen_width > h)pen_width = h;
        penw = pen_width * width / height;
        penh = pen_width;
    }

    draw_context::vertex_type v;

    dc.begin(CGL_TRIANGLE_STRIP);

    v.color = dc.color;
    dc.vertex(x + w, y);
    dc.vertex(x + w, y + penh);
    dc.vertex(x, y + h);
    dc.vertex(x + penw, y + h);
    dc.vertex(x + w, y + height);
    dc.vertex(x + w, y + height - penh);
    dc.vertex(x + width, y + h);
    dc.vertex(x + width - penw, y + h);
    dc.vertex(x + w, y);
    dc.vertex(x + w, y + penh);

    dc.end();
}

/*
//focus ractangle

|---a----|
|-b-|
  v3_____v1     ___
   / ____|     |___ \
  / /v4  v2        \ \
 / /bevel           \ \
/ /                  \ \
| |                   | |
| |                   | |
|_|v8                 |_|

exsample:

struct focus_rect
{
    int x;
    int y;
    int size;
    int frame;
};

on timer:
    if(f.frame > 0){
        f.frame -= 10;
        //...
    }

on draw:
int size = f.size + f.frame;
draw_focus_rect(dc, f.x - size/2, f.y - size/2, size, size, 50, 0, 10.0);

*/

void draw_focus_rect(draw_context& dc,
    float x, float y, float width, float height,//大小
    float a,    //横边宽度
    float b,    //斜边宽度
    float pen_width = 1.0f
    )
{
    if(a < 0.0f){// || width < 0.0f || height < 0.0f){
        return ;
    }

    draw_context::vertex_type v;
    v.color = dc.color;

    float w = width / 2.0f;
    float h = height / 2.0f;

    float ox = x + w;
    float oy = y + h;

    if(a < pen_width)a = pen_width;//横边宽度不能小于画笔宽度
    //if(a > w)a = w;//越界检测，垂直方面没有判断
    if(a < b + pen_width)b = a - pen_width / 2;//b + pen_width <= a
    float pen_fixed = b < pen_width ? pen_width : b + pen_width / 2;//内角修正宽度

    vec2f vs[8];
    vs[0].x = w - a;
    vs[0].y = h;

    vs[1].x = w - a;
    vs[1].y = h - pen_width;

    vs[2].x = w - b;
    vs[2].y = h;

    vs[3].x = w - pen_fixed;
    vs[3].y = h - pen_width;

    vs[4].x = w;
    vs[4].y = h - b;

    vs[5].x = w - pen_width;
    vs[5].y = h - pen_fixed;

    vs[6].x = w;
    vs[6].y = h - a;

    vs[7].x = w - pen_width;
    vs[7].y = h - a;

    dc.begin(CGL_TRIANGLE_STRIP);
    for(int i=0; i<8; ++i){
        v.x = ox + vs[i].x;
        v.y = oy + vs[i].y;
        dc.vertex(v);
    }
    dc.end();

    dc.begin(CGL_TRIANGLE_STRIP);
    for(int i=0; i<8; ++i){
        v.x = ox + vs[i].x;
        v.y = oy - vs[i].y;
        dc.vertex(v);
    }
    dc.end();

    dc.begin(CGL_TRIANGLE_STRIP);
    for(int i=0; i<8; ++i){
        v.x = ox - vs[i].x;
        v.y = oy + vs[i].y;
        dc.vertex(v);
    }
    dc.end();

    dc.begin(CGL_TRIANGLE_STRIP);
    for(int i=0; i<8; ++i){
        v.x = ox - vs[i].x;
        v.y = oy - vs[i].y;
        dc.vertex(v);
    }
    dc.end();
}


//绘制闪电
//dc.blendfunc(CGL_SRC_ALPHA, CGL_ONE);
void draw_lighting(graphics::draw_context& dc,
    vec2i p1,
    vec2i p2,
    int min_length,//分割的最小长度
    int wave_range,//波动幅度
    int sub_probability)//分支闪电产生几率，百分比
{
    if(min_length < 2)min_length = 2;//最小长度大于等于2
    int w = p2.x - p1.x;
    int h = p2.y - p1.y;
    //插值分割
    if(w * w + h * h <= min_length * min_length){
        /*
        vec2i p = p1 + (p2 - p1) / 2;
        dc.color = vec4ub(255, 255, 255, 255);

        dc.color.alpha = 64;
        //dc.blendfunc(CGL_SRC_COLOR, CGL_ONE);
        dc.blendfunc(CGL_SRC_ALPHA, CGL_ONE);
        dc.draw_point_image(tex_l, p.x, p.y, 32, 32, 0, 0, 1.0f, 1.0f);

        dc.color.alpha = 255;
        int a = 8;// 8 + random(18);
        dc.blendfunc(CGL_ONE, CGL_ONE);
        dc.draw_point_image(tex_l, p.x, p.y, a, a, 0, 0, 1.0f, 1.0f);
        dc.draw_point_image(tex_l, p.x, p.y, a, a, 0, 0, 1.0f, 1.0f);
        dc.draw_point_image(tex_l, p.x, p.y, a, a, 0, 0, 1.0f, 1.0f);

        dc.color.alpha = 255;
        dc.color = vec4ub(255, 255, 0, 255);
        /*/
        dc.draw_line(p1, p2);
        //*/
    }
    else{
        vec2i o, p;
        o.x = p1.x + w / 2;//o为p1,p2的中点
        o.y = p1.y + h / 2;
        //随机中点，进行一些偏移
        p.x = o.x + random(h * wave_range / 100) - random(h * wave_range / 100);
        p.y = o.y + random(w * wave_range / 100) - random(w * wave_range / 100);
        draw_lighting(dc, p1, p, min_length, wave_range, sub_probability);
        draw_lighting(dc, p, p2, min_length, wave_range, sub_probability);
        //闪电分支
        if(sub_probability && random(100) < sub_probability){
            if(w * w + h * h >= min_length * min_length){
                p.x=o.x+random(h) - random(h);
                p.y=o.y+random(w) - random(w);
                draw_lighting(dc, p1, p, min_length, wave_range, sub_probability);
            }
        }//end 闪电分支
    }//end 绘制闪电
}//end


//绘制一个扇形光源
void draw_light(draw_context& dc,
    int x, int y,    //block
    float width, float height,
    float tile_size,    //砖块大小
    float angle_begin,
    float angle_end,
    float step = 10.0f)
{
    draw_context::vertex_type v;
    vec2i block(x, y);
    block /= tile_size;

    v.color = dc.color;
    dc.bind_texture(0);
    dc.begin(CGL_TRIANGLE_FAN);// fillmode == CGL_FILL ? CGL_TRIANGLE_FAN : CGL_LINE_LOOP);
    v.x = x;
    v.y = y;
    dc.vertex(v);//圆心
    v.color.alpha = 0;
    for(float angle=angle_begin; angle<angle_end; angle+=step)
    {
        v.x = x + width * cos(angle*M_RD);
        v.y = y - height * sin(angle*M_RD);
        dc.vertex(v);
    }
    v.x = x + width * cos(angle_end*M_RD);
    v.y = y - height * sin(angle_end*M_RD);
    dc.vertex(v);
    dc.end();
}


//绘制一条彩带
/*
    //ribbons
    s.push(X, Y, 32, color::red, 0, 4);

    //lightning
    s.clear();
    int x = X;
    for(int i=0; i<1000; )
    {
        s.push(x, Y - i, 32, color::white, 1);
        i += 32 + random(128);
        x += math::random(-64, 64);
    }
*/
class ribbons
{
public:
    teximage* image;
    int alpha;
    int value;
    std::vector<vtx3f> vb;

    ribbons() :
        image(), vb()
    {

    }

    void paint(graphics::device_context& dc, int texture = 0)
    {
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        dc.bind_texture(texture);
        if(!vb.empty()){
            dc.draw_arrays(CGL_TRIANGLE_STRIP, &vb[0], 0, vb.size());
        }
    }

    void push(int x, int y,
        int pen_width,    //宽度
        vec4ub color,    //颜色
        int style = 0,    //0彩带 1闪电
        int decay = 0,    //alpha衰减
        size_t max_size = 64,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)//最大长度
    {
        vec2i c(x, y), d(x, y);

        if(style == 1){
            c.x -= pen_width;
            d.x += pen_width;
        }
        else if(style == 2){
            c.y -= pen_width;
            d.y += pen_width;
        }

        if(vb.size()){
            vec2i a = vec2i(x, y);
            vec2i b = last();

            vec2i ab = b - a;
            int length = ab.length();

            if(length == 0){
                return ;
            }

            vec2i v;
            v.x = pen_width * ab.y / length;
            v.y = pen_width * ab.x / length;

            c.x = a.x - v.x;
            c.y = a.y + v.y;

            d.x = a.x + v.x;
            d.y = a.y - v.y;
        }

        static bool r = false;
        r = !r;

        vtx3f vtx;
        vtx.x = c.x;
        vtx.y = c.y;
        if(r){
            vtx.u = u1;
            vtx.v = v1;
        }
        else{
            vtx.u = u1;
            vtx.v = v2;
        }
        vtx.color = color;
        vb.insert(vb.begin(), vtx);

        vtx.x = d.x;
        vtx.y = d.y;
        if(r){
            vtx.u = u2;
            vtx.v = v1;
        }
        else{
            vtx.u = u2;
            vtx.v = v2;
        }
        vtx.color = color;
        vb.insert(vb.begin(), vtx);

        if(vb.size() > max_size * 2){
            vb.pop_back();
            vb.pop_back();
            if(style == 100){//彩带
                c = last();
                vb[vb.size()-1].x = c.x;
                vb[vb.size()-1].y = c.y;
                vb[vb.size()-2].x = c.x;
                vb[vb.size()-2].y = c.y;
            }
        }
        if(decay){//alpha
            vtx3f* pv = &vb[2];
            for(size_t i=2; i<vb.size(); i+=2){
                pv[0].color.alpha -= decay;
                pv[1].color.alpha -= decay;
                pv += 2;
            }
        }
    }

    vec2i last()
    {
        int x = vb[vb.size()-1].x + vb[vb.size()-2].x;
        int y = vb[vb.size()-1].y + vb[vb.size()-2].y;
        return vec2i(x / 2, y / 2);
    }

    void clear()
    {
        vb.clear();
    }
};


}//end namespace graphics
}//end namespace cgl

#endif //DC_DRAW_HPP_20190406205742
