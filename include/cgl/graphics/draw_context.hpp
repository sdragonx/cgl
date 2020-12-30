/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 drawcontext.hpp

 2015-11-11 12:35:00

*/
#ifndef DRAW_CONTEXT_HPP_20151111123500
#define DRAW_CONTEXT_HPP_20151111123500

#include <vector>
#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/context.hpp>
#include <cgl/graphics/font.hpp>
#include <cgl/math.hpp>
#include <cgl/stack.hpp>
#include <cgl/vertex.hpp>

//#define CGL_SUPPERT_VBO 1

#ifdef CGL_SUPPERT_VBO
#include <cgl/graphics/opengl/glvbo.hpp>
#endif

namespace cgl{
namespace graphics{

#pragma pack(push, 1)
struct drawcall
{
    //变量
    int32_t command;
    union{//16 bytes
        void* object;
        struct{
            int64_t value;
            size32_t offset, size;
        };
        struct {
            int32_t blend_src, blend_dst;
        };
        struct {
            int32_t source, dest;
        };
        struct{
            byte_t red, green, blue, alpha;
        };
        struct{
            byte_t r, g, b, a;
        };
        struct{
            float x, y, z; union{ float angle; float w; };
        };
        struct{
            float _x, _y, width, height;
        };
    };
};
#pragma pack(pop)

//drawcall_buffer draw_buffer

class draw_context : public unicontext
{
public:
    #pragma pack(push, 1)
    class context_state
    {
    public:
        bool drawing;    //是否处在begin() end()之间，以便处理一些不方便执行的命令

        #if 0
        int blend:1;
        int shade_model:1;
        int point_smooth:1;
        int line_smooth:1;
        #endif

        struct {
            int16_t src;
            int16_t dst;
        }blendfunc;
        int16_t logic_op;
        float pen_width;
        int texture;
    public:
        context_state()
        {
            init();
        }

        void init()
        {
            drawing = CGL_FALSE;
            blendfunc.src = CGL_SRC_ALPHA;
            blendfunc.dst = CGL_ONE_MINUS_SRC_ALPHA;
            pen_width = 1.0f;
            texture = -1;
        }
    };
    #pragma pack(pop)

public:
    typedef vtx3f vertex_type;

    typedef std::vector<drawcall>    commandbuf_type;
    typedef std::vector<vertex_type> vertexbuf_type;
    typedef std::vector<size_t>      indexbuf_type;

    const context_state state;
    font_t font;

protected:
    device_context* m_gl;
    commandbuf_type commandBuffer;    //绘图指令缓冲区
    vertexbuf_type  vertexBuffer;    //顶点缓冲区
    indexbuf_type   indexBuffer;    //索引缓冲区
    size_t index_count;
    bool state_changed;

    #ifdef CGL_SUPPERT_VBO
    glvbo m_vbo;
    #endif
    value_stack<vec4s> cliprectStack;

public:
    draw_context(device_context& context) :
        state(),
        font(),
        m_gl(&context),
        commandBuffer(),
        vertexBuffer(),
        #ifdef CGL_SUPPERT_VBO
        m_vbo(),
        #endif
        cliprectStack()
    {
        cliprectStack.value = vec4s(0, 0, INT16_MAX, INT16_MAX);
        color = color::white;
        index_count = 0;
        state_changed = false;
    }

    ~draw_context()
    {
    }

    void init()
    {
        commandBuffer.clear();
        vertexBuffer.clear();
        indexBuffer.clear();
        const_write(state).init();
        color = color::white;
        cliprectStack.value = vec4s(0, 0, INT16_MAX, INT16_MAX);
    }

    bool empty()const
    {
        return commandBuffer.empty();
    }

    size_t size()const
    {
        return commandBuffer.size();
    }

    const drawcall& commands(int index)const
    {
        return commandBuffer[index];
    }

    const vertex_type* vertex_pointer()const
    {
        return &vertexBuffer[0];
    }

    const size_t* index_pointer()const
    {
        return &indexBuffer[0];
    }

    //更新数据到vbo，mode = GL_STREAM_DRAW|GL_STREAM_DRAW|GL_DYNAMIC_DRAW
    void update(int mode);

    //绘制并清空缓冲区 不能用
    void flush()
    {
        this->update(0);
        //this->paint();
        commandBuffer.clear();
        vertexBuffer.clear();
    }
    //void paint();//执行绘制


public://绘图状态
    void cliprect(int x, int y, int width, int height)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_CLIPRECT;
        d.x = x;
        d.y = y;
        if(width<0.0f)width = INT16_MAX;
        if(height<0.0f)height = INT16_MAX;
        d.width = width;
        d.height = height;
        commandBuffer.push_back(d);
        cliprectStack.value = vec4s(x, y, width, height);
    }

    void push_cliprect()
    {
        cliprectStack.push();
    }

    void pop_cliprect()
    {
        cliprectStack.pop();
        unicontext::cliprect(cliprectStack.value);
    }

    void blendfunc(int smode, int dmode)
    {
        CGL_ASSERT(!state.drawing);
        if(state.blendfunc.src != smode && state.blendfunc.dst != dmode){
            drawcall d;
            d.command = CGL_BLEND;
            d.blend_src = smode;
            d.blend_dst = dmode;
            commandBuffer.push_back(d);
            const_write(state).blendfunc.src = smode;
            const_write(state).blendfunc.dst = dmode;
        }
    }

    #if 0//gles nonsuppert
    void fill_mode(int mode)
    {
        drawcall d;
        d.command = CGL_FILL_MODE;
        d.value = mode;
        commandBuffer.push_back(d);
        const_write(state).fill_mode = mode;
    }

    void shade_model(int mode)
    {
        drawcall d;
        d.command = CGL_SHADE_MODEL;
        d.value = mode;
        commandBuffer.push_back(d);
        const_write(state).shade_model = mode;
    }

    void point_size(float size)
    {
        drawcall d;
        d.command = CGL_PEN_WIDTH;
        d.value = size;
        commandBuffer.push_back(d);
        const_write(state).pen_width = size;
    }
    #endif

    void pen_width(float width)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_PEN_WIDTH;
        d.width = width;
        commandBuffer.push_back(d);
        const_write(state).pen_width = width;
    }

    //gles2 貌似不支持
    void logic_op(int op)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_LOGIC_OP;
        d.value = op;
        commandBuffer.push_back(d);
        const_write(state).logic_op = op;
    }



public://纹理
//    intptr_t defimage()
//    {
//        return 0;
//    }

    void bind_texture(int tex)
    {
        CGL_ASSERT(!state.drawing);
        if(state.texture != tex){
            drawcall d;
            d.command = CGL_TEXTURE_2D;
            d.value = tex;
            commandBuffer.push_back(d);

            const_write(state.texture) = tex;
            state_changed = true;
        }
    }

    #if 0
    void bind_image1D(intptr_t image)
    {
        drawcall d;
        d.command = CGL_TEXTURE_2D;
        d.value = image;
        commandBuffer.push_back(d);
    }
    void bind_image2D(intptr_t image)
    {
        drawcall d;
        d.command = CGL_TEXTURE_2D;
        d.value = image;
        commandBuffer.push_back(d);
    }
    void bind_image3D(intptr_t image)
    {
        drawcall d;
        d.command = CGL_TEXTURE_3D;
        d.value = image;
        commandBuffer.push_back(d);
    }
    #endif

    //多纹理启用，未实现
    //active_texture(0)//默认纹理
    //void active_texture(int index);
    //enable_multitexture();
    //disable_multitexture();
    //void bind_texture(int level, intptr_t image, int blendmode, int blendfunc)



public://矩阵变换 模型视图
    void identity()
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_IDENTITY;
        commandBuffer.push_back(d);
    }

    void rotate(float angle, float x, float y, float z)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_ROTATE;
        d.x = x;
        d.y = y;
        d.z = z;
        d.angle = angle;
        commandBuffer.push_back(d);
    }

    void translate(float x, float y, float z = 0.0f)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_TRANSLATE;
        d.x = x;
        d.y = y;
        d.z = z;
        commandBuffer.push_back(d);
    }

    void scale(float x, float y, float z = 1.0f)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_SCALE;
        d.x = x;
        d.y = y;
        d.z = z;
        commandBuffer.push_back(d);
    }

    void push_matrix()
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_PUSH_MATRIX;
        commandBuffer.push_back(d);
    }

    void pop_matrix()
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command = CGL_POP_MATRIX;
        commandBuffer.push_back(d);
    }

    //push_view2D(...) pop_view()
    //void model_view();
    //void projection_view();
    //void matrix_mode(GLenum mode);
    //void load_matrix(float* matrix);



public://vertex
    /*
    //要做到drawarray批量处理，缓存当前绘图模式，如果绘制类型没有改变，直接附加顶点（未完善）
    */
    void begin(int mode)
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        switch(mode)
        {
        case CGL_POINTS:
            d.command = CGL_POINTS;
            break;
        case CGL_LINES:
            d.command = CGL_LINES;
            break;
        case CGL_LINE_LOOP:
            d.command = CGL_LINE_LOOP;
            state_changed = true;
            break;
        case CGL_LINE_STRIP:
            d.command = CGL_LINE_STRIP;
            state_changed = true;
            break;
        case CGL_TRIANGLES:
            d.command = CGL_TRIANGLES;
            break;
        case CGL_TRIANGLE_STRIP:
            d.command = CGL_TRIANGLE_STRIP;
            state_changed = true;
            break;
        case CGL_TRIANGLE_FAN:
            d.command = CGL_TRIANGLE_FAN;
            state_changed = true;
            break;
        case CGL_QUADS: //CGL_RECT
            d.command = CGL_QUADS;
            break;
        default:
            const_write(state).drawing = CGL_FALSE;
            return ;
        }

        if(commandBuffer.empty() || commandBuffer.back().command != d.command){
            state_changed = true;
        }

        if(state_changed){
            if(mode == CGL_QUADS){
                d.offset = indexBuffer.size();
                d.size = 0;
                index_count = 0;
            }
            else{
                d.offset = vertexBuffer.size();
                d.size = 0;
            }
            commandBuffer.push_back(d);
        }
        const_write(state).drawing = CGL_TRUE;
        state_changed = false;
    }

    void end()
    {
        CGL_ASSERT(state.drawing);
        const_write(state).drawing = CGL_FALSE;
    }

    void vertex(const vertex_type& v)
    {
        CGL_ASSERT(state.drawing);
        vertexBuffer.push_back(v);
        if(commandBuffer.back().command != CGL_QUADS){
            ++commandBuffer.back().size;
        }
        else{//CGL_QUADS 转为三角形
            ++index_count;
            if(index_count == 4){
                this->push_index(vertexBuffer.size() - 4);
                index_count = 0;
            }
        }
    }

    void vertex(float x, float y, float z, float u, float v, const vec4ub& c)
    {
        this->vertex(vertex_type(x, y, z, u, v, c));
    }

    void vertex(float x, float y, float z = 0.0f, float u = 0.0f, float v = 0.0f)
    {
        this->vertex(vertex_type(x, y, z, u, v, color));
    }

private:
    //4个顶点索引成2个三角形
    void push_index(size_t offset)
    {
        indexBuffer.push_back(offset + 0);
        indexBuffer.push_back(offset + 1);
        indexBuffer.push_back(offset + 2);
        indexBuffer.push_back(offset + 0);
        indexBuffer.push_back(offset + 2);
        indexBuffer.push_back(offset + 3);

        commandBuffer.back().size += 6;
    }

public://绘图函数
    //清屏
    void clear()
    {
        CGL_ASSERT(!state.drawing);
        drawcall d;
        d.command  = CGL_CLEAR;
        d.red   = color.red;
        d.green = color.green;
        d.blue  = color.blue;
        d.alpha = color.alpha;
        commandBuffer.push_back(d);
    }

    void color4ub(int red, int green, int blue, int alpha = 255)
    {
        color = vec4ub(red, green, blue, alpha);
    }

    void color4ub(const vec4f& c)
    {
        color = c;
    }

    void color4f(float red, float green, float blue, float alpha = 1.0f)
    {
        color = vec4ub(red*255.0f, green*255.0f, blue*255.0f, alpha*255.0f);
    }

    void color4f(const vec4f& c)
    {
        this->color4f(c.red, c.green, c.blue, c.alpha);
    }


public:
    virtual int draw_arrays(int shape, const vec2f* vs, size_t size)
    {
        this->begin(shape);
        for(size_t i=0; i<size; ++i){
            this->vertex(vs[i].x, vs[i].y);
        }
        this->end();
        return 0;
    }

    virtual int draw_elements(int shape, const vec2f* vs,  const int* ids, size_t size)
    {
        this->begin(shape);
        int id;
        for(size_t i=0; i<size; ++i){
            id = ids[i];
            this->vertex(vs[id].x, vs[id].y);
        }
        this->end();
        return 0;
    }



    void draw_point(float x, float y, float z = 0.0f)
    {
        this->begin(CGL_POINTS);
        this->vertex(x, y, z, 0.0f, 0.0f, color);
        this->end();
    }

    template<typename T>
    void draw_point(const vec2<T>& vec)
    {
        return this->draw_point(vec.x, vec.y);
    }

    template<typename T>
    void draw_point(const vec3<T>& vec)
    {
        return this->draw_point(vec.x, vec.y, vec.z);
    }



    int draw_line(float x1, float y1, float x2, float y2)
    {
        this->begin(CGL_LINES);
        vertex_type v;
        v.color = color;
        v.x = x1;
        v.y = y1;
        v.z = 0.0f;
        this->vertex(v);
        v.x = x2;
        v.y = y2;
        this->vertex(v);
        this->end();

        return 0;
    }

    template<typename T>
    int draw_line(const vec2<T>& p1, const vec2<T>& p2)
    {
        return this->draw_line(p1.x, p1.y, p2.x, p2.y);
    }

    template<typename T>
    int draw_line(const vec3<T>& p1, const vec3<T>& p2)
    {
        return this->draw_line(p1.x, p1.y, p2.x, p2.y);
    }

    //绘制渐变颜色的线 ！！这里Alpha有问题
    int draw_line(float x1, float y1, float x2, float y2, vec4ub color1, vec4ub color2)
    {
        this->bind_texture(0);
        this->begin(CGL_LINES);
        vertex_type v;
        //color1.alpha = 255-color1.alpha;
        //color2.alpha = 255-color2.alpha;
        v.x = x1;
        v.y = y1;
        v.z = 0.0f;
        v.color = color1;
        this->vertex(v);
        v.x = x2;
        v.y = y2;
        v.color = color2;
        this->vertex(v);
        this->end();
        return 0;
    }

    void dot_line(float x1, float y1, float x2, float y2, int solid, int space)
    {
        vec2i st;
        internal_line_stipple(x1, y1, x2, y2, solid, space, st);
    }

    template<typename T>
    void dot_lines(const vec2<T>* ls, size_t size, int solid, int space)
    {
        vec2i st;
        for(size_t i=1; i<size; ++i){
            internal_line_stipple(ls[i-1].x, ls[i-1].y, ls[i].x, ls[i].y, solid, space, st);
        }
    }

    virtual void draw_polygon(vec2f* vs, size_t size)
    {
        this->begin(CGL_LINE_STRIP);
        for(size_t i=0; i<size; ++i){
            this->vertex(vs[i].x, vs[i].y, 0.0f, 0.0f, 0.0f, color);
        }
        this->end();
    }

    void polyline(vec2f* vs, size_t size)
    {
        this->bind_texture(0);
        this->begin(CGL_LINE_STRIP);
        for(size_t i=0; i<size; ++i){
            this->vertex(vs[i].x, vs[i].y, 0.0f, 0.0f, 0.0f, color);
        }
        this->end();
    }

    void polyline(std::vector<vec2f>& vs)
    {
        this->polyline(&vs[0], vs.size());
    }

    int draw_rect(float x, float y, float width, float height)
    {
        return this->internal_draw_rect(CGL_LINE, x, y, width, height);
    }

    void dot_rect(float x, float y, float width, float height, int solid, int space)
    {
        vec2i st;
        internal_line_stipple(x, y, x + width, y, solid, space, st);
        internal_line_stipple(x + width, y, x + width, y + height, solid, space, st);
        internal_line_stipple(x + width, y + height, x, y + height, solid, space, st);
        internal_line_stipple(x, y + height, x, y, solid, space, st);
    }

    template<typename T>
    void draw_rect(const vec4<T>& rect)
    {
        this->draw_rect(rect.x, rect.y, rect.width, rect.height);
    }

    int fill_rect(float x, float y, float width, float height)
    {
        return this->internal_draw_rect(CGL_FILL, x, y, width, height);
    }

    template<typename T>
    void fill_rect(const vec4<T>& rect)
    {
        this->fill_rect(rect.x, rect.y, rect.width, rect.height);
    }

    void draw_arc(float x, float y, float width, float height, float angle_begin, float angle_end, float step)
    {
        this->internal_draw_arc(CGL_LINE_STRIP, x, y, width, height, angle_begin, angle_end, step);
    }

    void dot_arc(float x, float y, float width, float height, float angle_begin, float angle_end, float step)
    {
        this->internal_draw_arc(CGL_LINES, x, y, width, height, angle_begin, angle_end, step);
    }


    void draw_sector(float x, float y, float width, float height, float angle_begin, float angle_end, float step = 10.0f)
    {
        this->internal_draw_sector(CGL_LINE, x, y, width, height, angle_begin, angle_end, step);
    }

    void fill_sector(float x, float y, float width, float height, float angle_begin, float angle_end, float step = 10.0f)
    {
        this->internal_draw_sector(CGL_FILL, x, y, width, height, angle_begin, angle_end, step);
    }


    void draw_ellipse(float x, float y, float width, float height, float step = 10.0f)
    {
        this->draw_arc(x, y, width, height, 0.0f, 360.0f, step);
    }

    void fill_ellipse(float x, float y, float width, float height, float step = 10.0f)
    {
        this->fill_sector(x, y, width, height, 0.0f, 360.0f, step);
    }

    void dot_ellipse(float x, float y, float width, float height, float step = 10.0f)
    {
        this->internal_draw_arc(CGL_LINES, x, y, width, height, 0.0f, 360.0f, step);
    }


    //r / 10.0f
    void draw_circle(float x, float y, float r, float step = 10.0f)
    {
        this->draw_ellipse(x, y, r, r, step);
    }

    void fill_circle(float x, float y, float r, float step = 10.0f)
    {
        this->fill_ellipse(x, y, r, r, step);
    }

    void dot_circle(float x, float y, float r, float step = 10.0f)
    {
        this->dot_ellipse(x, y, r, r, step);
    }


    //绘制圆角矩形, rx ry 圆角范围
    void draw_roundrect(float x, float y, float width, float height, float rx, float ry, float step = 10.0f)
    {
        this->internal_draw_roundrect(CGL_LINE, x, y, width, height, rx, ry, step);
    }

    void fill_roundrect(float x, float y, float width, float height, float rx, float ry, float step = 10.0f)
    {
        this->internal_draw_roundrect(CGL_FILL, x, y, width, height, rx, ry, step);
    }



    int draw_image(intptr_t image,
        float x1, float y1, float x2, float y2,
        float x3, float y3, float x4, float y4,
        float u1, float v1, float u2, float v2)
    {
        vertex_type v;
        v.color = color;

        this->bind_texture(image);
        this->begin(CGL_QUADS);
        #if CGL_COORDINATE == CGL_LOWER_LEFT
        v.x = x1; v.y = y1; v.u = u1; v.v = v2;
        this->vertex(v);
        v.x = x2; v.y = y2; v.u = u2; v.v = v2;
        this->vertex(v);
        v.x = x3; v.y = y3; v.u = u2; v.v = v1;
        this->vertex(v);
        v.x = x4; v.y = y4; v.u = u1; v.v = v1;
        this->vertex(v);
        #else //CGL_COORDINATE == CGL_LOWER_LEFT
        v.x = x1; v.y = y1; v.u = u1; v.v = v1;
        this->vertex(v);
        v.x = x2; v.y = y2; v.u = u2; v.v = v1;
        this->vertex(v);
        v.x = x3; v.y = y3; v.u = u2; v.v = v2;
        this->vertex(v);
        v.x = x4; v.y = y4; v.u = u1; v.v = v2;
        this->vertex(v);
        #endif//CGL_COORDINATE == CGL_LOWER_LEFT
        this->end();

        return 0;
    }

    #if 0
    void draw_image(intptr_t image, vertex_type v1, vertex_type v2, vertex_type v3, vertex_type v4)
    {
        #if CGL_COORDINATE == CGL_LOWER_LEFT
        std::swap(v1.v, v3.v);
        std::swap(v2.v, v4.v);
        #endif

        this->bind_texture(image);
        this->begin(CGL_TRIANGLE_FAN);
        this->vertex(v1);
        this->vertex(v2);
        this->vertex(v3);
        this->vertex(v4);
        this->end();
    }
    #endif

    inline int draw_image(intptr_t image, float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        return this->draw_image(image,
            x, y,
            x + width, y,
            x + width, y + height,
            x, y + height,
            u1, v1, u2, v2);
    }

    template<typename T>
    int draw_image(intptr_t image, const vec4<T>& rect,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        return this->draw_image(image, rect.x, rect.y, rect.width, rect.height, u1, v1, u2, v2);
    }



    int draw_image(const teximage* image, float x, float y, float width, float height,
        float u1, float v1, float u2, float v2)
    {
        if(image){
            return this->draw_image(image->id, x, y, width, height, u1, v1, u2, v2);
        }
        return CGL_ERROR;
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& rect, float u1, float v1, float u2, float v2)
    {
        return this->draw_image(image, rect.x, rect.y, rect.width, rect.height, u1, v1, u2, v2);
    }

    int draw_image(const teximage* image, float x, float y, float width, float height)
    {
        return this->draw_image(image, x, y, width, height, image->u1, image->v1, image->u2, image->v2);
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& rect)
    {
        return this->draw_image(image, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
    }

    //draw image aligned
    int draw_image(const teximage* image, float x, float y, float width, float height, int align)
    {
        if(image){
            vec4f rect(x, y, width, height);
            rect = rect.align(image->width, image->height, align);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& rect, int align)
    {
        return this->draw_image(image, rect.x, rect.y, rect.width, rect.height, align);
    }

    //draw image aligned, scale size
    int draw_image(const teximage* image, float x, float y, float width, float height, float scale, int align)
    {
        if(image){
            vec4f rect(x, y, width, height);
            rect = rect.align(image->width * scale, image->height * scale, align);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& bounds, float scale, int align)
    {
        if(image){
            vec4<T> rect = bounds.align(image->width * scale, image->height * scale, align);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    //draw image aligned and fitbounds
    int draw_image(const teximage* image, float x, float y, float width, float height, int align, bool fit)
    {
        if(image){
            vec4f rect(x, y, width, height);
            rect = rect.align(image->width, image->height, align, fit);
            this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& bounds, int align, bool fit)
    {
        if(image){
            vec4<T> rect = bounds.align(image->width, image->height, align, fit);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    //draw image aligned, scale size, fitbounds
    int draw_image(const teximage* image, float x, float y, float width, float height, float scale, int align, bool fit)
    {
        if(image){
            vec4f rect(x, y, width, height);
            rect = rect.align(image->width * scale, image->height * scale, align, fit);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }

    template<typename T>
    int draw_image(const teximage* image, const vec4<T>& bounds, float scale, int align, bool fit)
    {
        if(image){
            vec4<T> rect = bounds.align(image->width * scale, image->height * scale, align, fit);
            return this->draw_image(image->id, rect.x, rect.y, rect.width, rect.height, image->u1, image->v1, image->u2, image->v2);
        }
        return CGL_ERROR;
    }


    //绘制精灵，以xy为中心坐标
    void point_image(
        intptr_t image,
        float x, float y,            //中心坐标
        float size_x, float size_y,    //扩展大小
        float u1, float v1, float u2, float v2)
    {
        this->draw_image(image,
            x - size_x, y - size_y,
            x + size_x, y - size_y,
            x + size_x, y + size_y,
            x - size_x, y + size_y,
            u1, v1, u2, v2);
    }

    //绘制精灵，以xy为中心坐标
    void point_image(
        intptr_t image,
        float x, float y,            //中心坐标
        float size_x, float size_y,    //扩展大小
        float offset_x, float offset_y,
        float u1, float v1, float u2, float v2)
    {
        this->draw_image(image,
            x - size_x, y - size_y,
            x + size_x, y - size_y,
            x + size_x, y + size_y,
            x - size_x, y + size_y,
            u1, v1, u2, v2);
    }

    //绘制精灵，以xy为中心坐标，旋转angle度
    void point_image(
        intptr_t image,
        float x, float y,            //中心坐标
        float size_x, float size_y,    //扩展大小
        float angle,                //旋转角度
        float u1, float v1, float u2, float v2)
    {
        //angle *= M_RD;
        float sinval = math::fast_sin(angle);
        float cosval = math::fast_cos(angle);
        float cosx = size_x * cosval;
        float sinx = size_x * sinval;
        float cosy = size_y * cosval;
        float siny = size_y * sinval;

        this->draw_image(image,
            x - cosx - siny, y - cosy + sinx,
            x + cosx - siny, y - cosy - sinx,
            x + cosx + siny, y + cosy - sinx,
            x - cosx + siny, y + cosy + sinx,
            u1, v1, u2, v2);
    }

    //绘制精灵，以xy为中心坐标，偏移、缩放、旋转、翻转
    void point_image(
        intptr_t image,
        float x,        //中心坐标
        float y,
        float size_x,    //扩展大小
        float size_y,
        float offset_x,    //偏移位置
        float offset_y,
        float scale,    //缩放
        float angle,    //旋转角度
        int flip,        //是否旋转 CGL_X CGL_Y
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        //angle *= M_RD;
        float sine = math::fast_sin(angle);
        float cosine = math::fast_cos(angle);

        //scale
        size_x *= scale;
        size_y *= scale;

        //rotate
        float cosx = size_x * cosine;
        float sinx = size_x * sine;
        float cosy = size_y * cosine;
        float siny = size_y * sine;

        //offset
        x += offset_x * cosine + offset_y * sine;
        y += offset_y * cosine - offset_x * sine;

        //flip
        if(flip & CGL_X){
            std::swap(u1, u2);
        }
        if(flip & CGL_Y){
            std::swap(v1, v2);
        }

        this->draw_image(image,
            x - cosx - siny, y - cosy + sinx,
            x + cosx - siny, y - cosy - sinx,
            x + cosx + siny, y + cosy - sinx,
            x - cosx + siny, y + cosy + sinx,
            u1, v1, u2, v2);
    }

    void point_image(const teximage* image, float x, float y)
    {
        float size_x = image->width * 0.5f;
        float size_y = image->height * 0.5f;
        this->point_image(image->id, x, y, size_x, size_y, image->u1, image->v1, image->u2, image->v2);
    }

    void point_image(const teximage* image, float x, float y, float size_x, float size_y)
    {
        this->point_image(image->id, x, y, size_x, size_y, image->u1, image->v1, image->u2, image->v2);
    }

    void point_image(const teximage* image, float x, float y, float angle)
    {
        float size_x = image->width * 0.5f;
        float size_y = image->height * 0.5f;
        this->point_image(image->id, x, y, size_x, size_y, angle, image->u1, image->v1, image->u2, image->v2);
    }

    void point_image(const teximage* image, float x, float y, float size_x, float size_y, float angle)
    {
        this->point_image(image->id, x, y, size_x, size_y, angle, image->u1, image->v1, image->u2, image->v2);
    }

    void point_image(
            const teximage* image,
            float x,        //中心坐标
            float y,
            float offset_x,    //偏移位置
            float offset_y,
            float scale,    //缩放
            float angle,    //旋转角度
            int flip        //是否旋转 CGL_X CGL_Y
            )
    {
        return this->point_image(
            image->id,
            x, y,
            image->width * 0.5f, image->height * 0.5f,
            offset_x, offset_y,
            scale, angle, flip,
            image->u1, image->v1, image->u2, image->v2);
    }

    void point_image(
            const teximage* image,
            float x,        //中心坐标
            float y,
            float size_x,    //扩展大小
            float size_y,
            float offset_x,    //偏移位置
            float offset_y,
            float scale,    //缩放
            float angle,    //旋转角度
            int flip        //是否旋转 CGL_X CGL_Y
            )
    {
        return this->point_image(
            image->id,
            x, y,
            size_x, size_y,
            offset_x, offset_y,
            scale, angle, flip,
            image->u1, image->v1, image->u2, image->v2);
    }

//暂时不使用
//    void draw(draw_context& dc)
//    {
//        if(this!=&dc)
//        {
//            drawcall d;
//            d.command = CGL_DRAW_CONTEXT;
//            d.object = &dc;
//            commandBuffer.push_back(d);
//        }
//    }



public://文字输出
    int char_width(unichar_t ch)
    {
        unifont* f = m_gl->load_font(font);
        return f->char_width(ch);// font_char_width(font, ch);
    }

    int char_height()
    {
        unifont* f = m_gl->load_font(font);
        return f->char_height();//font_char_height(font);
    }

    int text_width(const unichar_t* text, size_t length)
    {
        unifont* f = m_gl->load_font(font);
        return f->text_width(text, length);//font_text_width(font, text, length);
    }

protected:
    int vprint(float x, float y, const unichar_t* text, size_t length)
    {
        unifont* f = m_gl->load_font(font);
        return f->print(this, x, y, text, length);
    }



private://internal draw
    int internal_draw_rect(int fillmode, float x, float y, float width, float height);
    void internal_draw_arc(int mode, float x, float y, float width, float height, float angle_begin, float angle_end, float step);
    void internal_draw_sector(int fillmode, float x, float y, float width, float height, float angle_begin, float angle_end, float step = 10.0f);

    void internal_draw_roundrect(int fillmode, float x, float y, float width, float height, float rx, float ry, float step = 10.0f);

    //绘制点画线
    int internal_line_stipple(int x1, int y1, int x2, int y2, int solid, int space, vec2i& offset);

};

//更新数据到vbo
void draw_context::update(int mode)
{
    #ifdef CGL_SUPPERT_VBO
    if(vertexBuffer.empty())return ;
    if(m_vbo.handle())
    {
        m_vbo.bind();
        m_vbo.write(
            sizeof(vertex_t)*vertexBuffer.size(),
            OFFSET_ARRAY(&vertexBuffer[0]),
            //GL_DYNAMIC_d);
            GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);//必须执行，可能会出现别的问题
    }
    #endif
}

//---------------------------------------------------------------------------
// 内部函数


int draw_context::internal_line_stipple(int x1, int y1, int x2, int y2, int solid, int space, vec2i& offset)
{
    using namespace std;

    const static int bits = 16;
    const static int one = 1 << bits;

    int dx = x2 - x1;
    int dy = y2 - y1;
    int x_step, y_step;
    int k = abs(dx);

    if(k < abs(dy)){
        k = abs(dy);
        x_step = (dx << bits) / k;
        y_step = y1 < y2 ? one : -one;
    }
    else{
        if(k == 0){
            return 0;
        }

        x_step = x1 < x2 ? one : -one;
        y_step = (dy << bits) / k;
    }

    x1 <<= bits;
    y1 <<= bits;
    x1 += one >> 1;//四舍五入
    y1 += one >> 1;

    int i = offset.x;
    int j = offset.y;
    vertex_type v;
    v.color = color;

    this->begin(CGL_LINES);

    //恢复绘制模式
    if(offset.y && (offset.y & 1)){//solid
        v.x = x1 >> bits;
        v.y = y1 >> bits;
        this->vertex(v);
    }
    //恢复绘制进度
    x1 += x_step * offset.x;
    y1 += y_step * offset.x;

    for( ; i < k; ++j){
        v.x = x1 >> bits;
        v.y = y1 >> bits;
        this->vertex(v);

        if(j & 1){//space mode
            x1 += x_step * space;
            y1 += y_step * space;
            i += space;
        }
        else{//solid mode
            x1 += x_step * solid;
            y1 += y_step * solid;
            i += solid;
        }
    }
    //space不用画，solid要结尾，保证顶点数量为2的倍数
    if(j & 1){
        v.x = x2;
        v.y = y2;
        this->vertex(v);
    }

    this->end();

    //保存绘制模式、进度
    offset.x = i - k;
    offset.y = j;

    return k;
}

int draw_context::internal_draw_rect(int fillmode, float x, float y, float width, float height)
{
    this->bind_texture(0);
    //线宽过大，线的连接处会有问题。LINE模式比FILL模式正好大一圈
    this->begin(fillmode == CGL_FILL ? CGL_QUADS : CGL_LINE_LOOP);
    vertex_type v;
    v.color = color;
    v.x = x;         v.y = y;          this->vertex(v);
    v.x = x + width; v.y = y;          this->vertex(v);
    v.x = x + width; v.y = y + height; this->vertex(v);
    v.x = x;         v.y = y + height; this->vertex(v);
    this->end();

    return 0;
}

void draw_context::internal_draw_arc(int mode, float x, float y, float width, float height, float angle_begin, float angle_end, float step)
{
    vertex_type v;
    v.color = color;
    this->bind_texture(0);
    this->begin(mode);
    int count = 0;
    for(float angle=angle_begin; angle<angle_end; angle+=step){
        v.x = x + width  * math::fast_cos(angle);// std::cos(angle * M_RD);
        v.y = y - height * math::fast_sin(angle);//std::sin(angle * M_RD);
        this->vertex(v);
        ++count;
    }
    v.x = x + width  * math::fast_cos(angle_end);//std::cos(angle_end  * M_RD);
    v.y = y - height * math::fast_sin(angle_end);//std::sin(angle_end * M_RD);
    this->vertex(v);
    ++count;

    //这里没有修正GL_LINES模式下，顶点保证双数的问题
    if(count & 1){
        this->vertex(v);
    }

    this->end();
}

void draw_context::internal_draw_sector(int fillmode, float x, float y, float width, float height, float angle_begin, float angle_end, float step)
{
    vertex_type v;
    v.color = color;
    this->bind_texture(0);
    this->begin(fillmode == CGL_FILL ? CGL_TRIANGLE_FAN : CGL_LINE_LOOP);
    v.x = x;
    v.y = y;
    this->vertex(v);//圆心
    for(float angle=angle_begin; angle<angle_end; angle+=step)
    {
        v.x = x + width * math::fast_cos(angle);//std::cos(angle*M_RD);
        v.y = y - height * math::fast_sin(angle);//std::sin(angle*M_RD);
        this->vertex(v);
    }
    v.x = x + width * math::fast_cos(angle_end);//std::cos(angle_end*M_RD);
    v.y = y - height * math::fast_sin(angle_end);//std::sin(angle_end*M_RD);
    this->vertex(v);
    this->end();
}

void draw_context::internal_draw_roundrect(int fillmode, float x, float y, float width, float height, float rx, float ry, float step)
{
    vertex_type v;
    float px, py;
    v.color = color;
    this->bind_texture(0);
    if(fillmode != CGL_FILL){
        this->begin(CGL_LINE_LOOP);
    }
    else{
        this->begin(CGL_TRIANGLE_FAN);
        //中点
        v.x = x+width/2;
        v.y = y+height/2;
        this->vertex(v);
    }

    //左上角弧
    px = x+rx;
    py = y+ry;
    for(float angle=90.0f; angle<180.0f; angle+=step)
    {
        v.x = px+rx * cos(angle*M_RD);
        v.y = py-ry * sin(angle*M_RD);
        this->vertex(v);
    }
    v.x = x;
    v.y = y+ry;
    this->vertex(v);

    //左下角弧
    px = x+rx;
    py = y+height-ry;
    for(float angle=180.0f; angle<270.0f; angle+=step)
    {
        v.x = px+rx * cos(angle*M_RD);
        v.y = py-ry * sin(angle*M_RD);
        this->vertex(v);
    }
    v.x = x+rx;
    v.y = y+height;
    this->vertex(v);

    //右下角弧
    px = x+width-rx;
    py = y+height-ry;
    for(float angle=270.0f; angle<360.0f; angle+=step)
    {
        v.x = px+rx * cos(angle*M_RD);
        v.y = py-ry * sin(angle*M_RD);
        this->vertex(v);
    }
    v.x = x+width;
    v.y = y+height-ry;
    this->vertex(v);

    //右上角弧
    px = x+width-rx;
    py = y+ry;
    for(float angle=0.0f; angle<90.0f; angle+=step)
    {
        v.x = px+rx * cos(angle*M_RD);
        v.y = py-ry * sin(angle*M_RD);
        this->vertex(v);
    }
    v.x = x+width-rx;
    v.y = y;
    this->vertex(v);

    if(fillmode == CGL_FILL)
    {
        //左边边框封口
        v.x = x+rx;
        v.y = y;
        this->vertex(v);
    }
    this->end();
}


//---------------------------------------------------------------------------

#if 0
void draw_context::paint()
{
    if(commandBuffer.empty())return ;

    //保存现场
    #ifndef CGL_PLATFORM_GLES
    glPushAttrib(GL_CURRENT_BIT|GL_ENABLE_BIT|GL_HINT_BIT|
        GL_POINT_BIT|GL_LINE_BIT|GL_POLYGON_BIT|
        GL_PIXEL_MODE_BIT|GL_SCISSOR_BIT);

    //初始化状态
    glColor4ub(color.red, color.green, color.blue, color.alpha);
    glPolygonMode(GL_FRONT_AND_BACK, state.fill_mode);
    glBlendFunc(state.blendfunc.src, state.blendfunc.dst);
    glPointSize(state.point_size);
    glLineWidth(state.line_width);
    //直角坐标系
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    //glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
    glScissor(0, 0, INT16_MAX, INT16_MAX);

    for(command_buffer::iterator itr = commandBuffer.begin();
        itr != commandBuffer.end(); ++itr)
    {
        switch(itr->command)
        {
        case CGL_POINTS:
        case CGL_LINES:
        case CGL_LINE_LOOP:
        case CGL_LINE_STRIP:
        case CGL_TRIANGLE_STRIP:
        case CGL_TRIANGLES:
        case CGL_TRIANGLE_FAN:
        {
            glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            #ifdef CGL_SUPPERT_VBO
            if(m_vbo.handle())
            {
                m_vbo.bind();
                glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(0));
                glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, u)));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, color)));
            }
            else
            #endif
            {
                glVertexPointer(3, GL_FLOAT, sizeof(vertex_type), OFFSET_ARRAY(&vertexBuffer[0]));
                glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_type), OFFSET_ARRAY(&vertexBuffer[0].u));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_type), OFFSET_ARRAY(&vertexBuffer[0].color));
            }

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            glDrawArrays(itr->command, itr->start, itr->size);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);

            glPopClientAttrib();
            break;
        }
        case CGL_CLEAR:
            glClearColor(itr->red, itr->green, itr->blue, itr->alpha);
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            break;
        case CGL_COLOR:
            glColor4f(itr->red, itr->green, itr->blue, itr->alpha);
            break;
        case CGL_FLUSH:
            glFlush();
            break;

        case CGL_DRAWARRAYS:
        {
            vtx3f* va = (vtx3f*)d.object;

            glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            #ifdef CGL_SUPPERT_VBO
            if(m_vbo.handle())
            {
                m_vbo.bind();
                glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(0));
                glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, u)));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_t), OFFSET_ARRAY(offsetof(vertex_t, color)));
            }
            else
            #endif
            {
                glVertexPointer(3, GL_FLOAT, sizeof(vertex_type), OFFSET_ARRAY(va));
                glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_type), OFFSET_ARRAY(&va->u));
                glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vertex_type), OFFSET_ARRAY(&va->color));
            }

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);

            glDrawArrays(CGL_TRIANGLES, itr->start, itr->size);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);

            glPopClientAttrib();
        }

        case CGL_DRAW_CONTEXT:
            reinterpret_cast<draw_context*>(itr->object)->paint();
            break;

        /*
        case CGL_PRINTA:
            basefont_print(*this, itr->px, itr->py, itr->textA, itr->length);
            break;
        case CGL_PRINTW:
            basefont_print(*this, itr->px, itr->py, itr->textW, itr->length);
            break;
        */

        case CGL_TEXTURE_1D:
            glBindTexture(GL_TEXTURE_1D, itr->value);
            break;
        case CGL_TEXTURE_2D:
            if(itr->value){
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, itr->value);
            }
            else{
                glDisable(GL_TEXTURE_2D);
            }
            break;
        case CGL_TEXTURE_3D:
            glBindTexture(GL_TEXTURE_3D, itr->value);
            break;

        case CGL_INIT_MATRIX:
            glLoadIdentity();
            break;
        case CGL_PUSH_MATRIX:
            glPushMatrix();
            break;
        case CGL_POP_MATRIX:
            glPopMatrix();
            break;
        case CGL_ROTATE:
            glRotatef(itr->angle, itr->x, itr->y, itr->z);
            break;
        case CGL_TRANSLATE:
            glTranslatef(itr->x, itr->y, itr->z);
            break;
        case CGL_SCALE:
            glScalef(itr->x, itr->y, itr->z);
            break;

        #if 0 //!defined(CGL_PLATFORM_GLES2)
        case CGL_FILL_MODE:
            switch(itr->value)
            {
            case CGL_POINT:
                glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
                break;
            case CGL_LINE:
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                break;
            case CGL_FILL:
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                break;
            default:
                break;
            }
            break;
        #endif

        case CGL_BLEND_FUNC:
            glBlendFunc(itr->blend_src, itr->blend_dst);
            break;
        case CGL_CLIPRECT:
            #if CGL_COORDINATE == CGL_LOWER_LEFT
            glScissor(itr->x, itr->y, itr->width, itr->height);
            #else
            //反转模式
            glGetIntegerv(GL_VIEWPORT, viewport);
            glScissor(itr->x, viewport[3] - itr->y - itr->height, itr->width, itr->height);
            #endif
            break;
        case CGL_POINT_SIZE:
            glPointSize(itr->value);
            break;
        case CGL_LINE_WIDTH:
            glLineWidth(itr->value);
            break;
        default:
            break;
        }
    }

    #ifdef CGL_SUPPERT_VBO
    if(m_vbo.handle())
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    #endif

    glPopAttrib();

    #endif
}

#endif


//---------------------------------------------------------------------------
// 扩展绘制

void cross_coordsystem(graphics::draw_context& dc,
    float x, float y, float width, float height,
    const vec2i* mouse)
{
    dc.color = graphics::color::gray;
    dc.color.alpha = 128;
    dc.pen_width(1.0f);
    float w = width * 0.5f;
    float h = height * 0.5f;
    dc.dot_line(x + w, y, x + w, y + height, 4, 4);
    dc.dot_line(x, y + h, x + width, y + h, 4, 4);

    if(mouse){
        dc.color = vec4ub(255, 255, 255, 128);
        dc.dot_line(mouse->x, -height, mouse->x, height, 2, 8);
        dc.dot_line(-width, mouse->y, width, mouse->y, 2, 8);

        char buf[32] = {0};
        dc.color = vec4ub(255, 255, 255, 255);
        sprintf(buf, "%i", mouse->x);
        dc.print(mouse->x, y, buf, strlen(buf));
        sprintf(buf, "%i", mouse->y);
        dc.print(x, mouse->y, buf, strlen(buf));
    }
}

//绘制一个圆+直线，模拟一个角色
void draw_unit(graphics::draw_context& dc, float x, float y,
    float size, float angle, vec4ub color, float vision, vec4ub vision_color)
{
    dc.bind_texture(0);
    dc.pen_width(2.0f);
    dc.color = color;
    dc.draw_ellipse(x, y, size, size);

    vec2f p(size, 0);
    p.rotate(angle);
    dc.draw_line(x, y, x + p.x, y + p.y);

    dc.color = vision_color;
    dc.color.alpha = 128;
    dc.dot_circle(x, y, vision, 2);
    dc.pen_width(1.0f);
}

//---------------------------------------------------------------------------

//绘制九宫格图像
//fillmode 平铺、拉伸
void draw_nine_block(draw_context& dc, teximage* image,
    float x, float y, float width, float height,
    float x1, float x2,
    float y1, float y2,
    int fillmode)
{

}

//---------------------------------------------------------------------------
//ui elements

//draw window frame
void draw_rect(draw_context& dc, float x, float y, float width, float height, float pen_width,
    vec4ub pen, vec4ub brush = vec4ub())
{
    float right = x + width;// - 1.0f;
    float bottom = y + height;// - 1.0f;
    dc.bind_texture(0);
    if(0 < pen_width){
        dc.begin(CGL_TRIANGLE_STRIP);
        //top
        dc.vertex(x, y, 0, 0, 0, pen);
        dc.vertex(x + pen_width, y + pen_width, 0, 0, 0, pen);
        dc.vertex(right, y, 0, 0, 0, pen);
        dc.vertex(right - pen_width, y + pen_width, 0, 0, 0, pen);

        //right
        dc.vertex(right, y + height, 0, 0, 0, pen);
        dc.vertex(right - pen_width, bottom - pen_width, 0, 0, 0, pen);

        //bottom
        dc.vertex(x, bottom, 0, 0, 0, pen);
        dc.vertex(x + pen_width, bottom - pen_width, 0, 0, 0, pen);

        //left
        dc.vertex(x, y, 0, 0, 0, pen);
        dc.vertex(x + pen_width, y + pen_width, 0, 0, 0, pen);

        dc.end();
    }

    if(brush.alpha != 0){
        dc.begin(CGL_TRIANGLE_STRIP);
        dc.vertex(x + pen_width, y + pen_width, 0, 0, 0, brush);
        dc.vertex(right - pen_width, y + pen_width, 0, 0, 0, brush);
        dc.vertex(x + pen_width, bottom - pen_width, 0, 0, 0, brush);
        dc.vertex(right - pen_width, bottom - pen_width, 0, 0, 0, brush);
        dc.end();
    }
}

//draw arc
//绘制任意线宽的椭圆
//no brush fill
void draw_arc(graphics::draw_context& dc, float x, float y, float cx, float cy, float pen_width, int begin, int end, int step = 10)
{
    float sine, cosine;
    if(end < begin)end += 360;
    pen_width *= 0.5f;
    //inner < std::min(cx, cy)
    dc.begin(CGL_TRIANGLE_STRIP);
    sine = sin(begin * M_RD);
    cosine = cos(begin * M_RD);
    dc.vertex(x + cosine * (cx - pen_width), y - sine * (cy - pen_width));
    dc.vertex(x + cosine * (cx + pen_width), y - sine * (cy + pen_width));
    for(int i=begin; i<end; i+=step){
        sine = sin(i * M_RD);
        cosine = cos(i * M_RD);
        dc.vertex(x + cosine * (cx - pen_width), y - sine * (cy - pen_width));
        dc.vertex(x + cosine * (cx + pen_width), y - sine * (cy + pen_width));
    }
    sine = sin(end * M_RD);
    cosine = cos(end * M_RD);
    dc.vertex(x + cosine * (cx - pen_width), y - sine * (cy - pen_width));
    dc.vertex(x + cosine * (cx + pen_width), y - sine * (cy + pen_width));
    dc.end();
}

void draw_ellipse(graphics::draw_context& dc, float x, float y, float cx, float cy, float pen_width, int step = 10)
{
    draw_arc(dc, x, y, cx, cy, pen_width, 0, 360, step);
}

void draw_tab_button(draw_context& dc, float x, float y, float width, float height,
    float pen_width, int style, vec4ub pen, vec4ub brush = vec4ub())
{
    float right = x + width;// - 1.0f;
    float bottom = y + height;// - 1.0f;
    dc.bind_texture(0);
    if(0 < pen_width){
        dc.begin(CGL_TRIANGLE_STRIP);
        switch(style){
        case CGL_LEFT:
            //bottom
            dc.vertex(right, y+height, 0, 0, 0, pen);
            dc.vertex(right-pen_width, bottom-pen_width, 0, 0, 0, pen);
            dc.vertex(x, bottom, 0, 0, 0, pen);
            dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, pen);

            //left
            dc.vertex(x, y, 0, 0, 0, pen);
            dc.vertex(x+pen_width, y+pen_width, 0, 0, 0, pen);

            //top
            dc.vertex(right, y, 0, 0, 0, pen);
            dc.vertex(right-pen_width, y+pen_width, 0, 0, 0, pen);
            break;
        case CGL_UP:
            //left
            dc.vertex(x, bottom, 0, 0, 0, pen);
            dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, pen);
            dc.vertex(x, y, 0, 0, 0, pen);
            dc.vertex(x+pen_width, y+pen_width, 0, 0, 0, pen);

            //top
            dc.vertex(right, y, 0, 0, 0, pen);
            dc.vertex(right-pen_width, y+pen_width, 0, 0, 0, pen);

            //right
            dc.vertex(right, y+height, 0, 0, 0, pen);
            dc.vertex(right-pen_width, bottom-pen_width, 0, 0, 0, pen);
            break;
        case CGL_RIGHT:
            //top
            dc.vertex(x, y, 0, 0, 0, pen);
            dc.vertex(x+pen_width, y+pen_width, 0, 0, 0, pen);
            dc.vertex(right, y, 0, 0, 0, pen);
            dc.vertex(right-pen_width, y+pen_width, 0, 0, 0, pen);

            //right
            dc.vertex(right, y+height, 0, 0, 0, pen);
            dc.vertex(right-pen_width, bottom-pen_width, 0, 0, 0, pen);

            //bottom
            dc.vertex(x, bottom, 0, 0, 0, pen);
            dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, pen);
            break;
        case CGL_DOWN:
            //right
            dc.vertex(right, y, 0, 0, 0, pen);
            dc.vertex(right-pen_width, y+pen_width, 0, 0, 0, pen);
            dc.vertex(right, y+height, 0, 0, 0, pen);
            dc.vertex(right-pen_width, bottom-pen_width, 0, 0, 0, pen);

            //bottom
            dc.vertex(x, bottom, 0, 0, 0, pen);
            dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, pen);

            //left
            dc.vertex(x, y, 0, 0, 0, pen);
            dc.vertex(x+pen_width, y+pen_width, 0, 0, 0, pen);
            break;
        default:
            break;
        }

        dc.end();
    }

    if(brush.alpha != 0){
        dc.begin(CGL_TRIANGLE_STRIP);
        dc.vertex(x+pen_width, y+pen_width, 0, 0, 0, brush);
        dc.vertex(right-pen_width, y+pen_width, 0, 0, 0, brush);
        dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, brush);
        dc.vertex(right-pen_width, bottom-pen_width, 0, 0, 0, brush);
        dc.end();
    }
}

//draw window
void draw_frame(draw_context& dc,
    float x, float y, float width, float height,
    float title,
    float pen_width,
    vec4ub pen, vec4ub brush = vec4ub())
{
    float right = x + width;// - 1.0f;
    float bottom = y + height;// - 1.0f;
    dc.bind_texture(0);
    if(0.0f < pen_width){
        dc.begin(CGL_TRIANGLE_STRIP);
        //top
        dc.vertex(x, y, 0, 0, 0, pen);
        dc.vertex(x + pen_width, y + pen_width + title, 0, 0, 0, pen);
        dc.vertex(right, y, 0, 0, 0, pen);
        dc.vertex(right - pen_width, y + pen_width + title, 0, 0, 0, pen);

        //right
        dc.vertex(right, bottom, 0, 0, 0, pen);
        dc.vertex(right - pen_width, bottom - pen_width, 0, 0, 0, pen);

        //bottom
        dc.vertex(x, bottom, 0, 0, 0, pen);
        dc.vertex(x+pen_width, bottom-pen_width, 0, 0, 0, pen);

        //left
        dc.vertex(x, y, 0, 0, 0, pen);
        dc.vertex(x + pen_width, y + pen_width + title, 0, 0, 0, pen);

        dc.end();
    }
    else if(0.0f < title){
        dc.begin(CGL_TRIANGLE_STRIP);
        //top
        dc.vertex(x, y, 0, 0, 0, pen);
        dc.vertex(x, y + title, 0, 0, 0, pen);
        dc.vertex(right, y, 0, 0, 0, pen);
        dc.vertex(right, y + title, 0, 0, 0, pen);
    }

    if(brush.alpha != 0){
        dc.begin(CGL_TRIANGLE_STRIP);
        dc.vertex(x + pen_width, y + pen_width + title, 0, 0, 0, brush);
        dc.vertex(right - pen_width, y + pen_width + title, 0, 0, 0, brush);
        dc.vertex(x + pen_width, bottom - pen_width, 0, 0, 0, brush);
        dc.vertex(right - pen_width, bottom - pen_width, 0, 0, 0, brush);
        dc.end();
    }
}

//绘制一个窗口的阴影
void draw_win_shadow(draw_context& dc,
    float x, float y, float width, float height,
    float size,     //阴影大小
    vec4ub color)   //阴影颜色
{
    const int inner_alpha = color.alpha;
    const int outer_alpha = 0;

    float right = x + width - 1.0f;
    float bottom = y + height - 1.0f;

    dc.bind_texture(0);

    //left
    dc.begin(CGL_TRIANGLE_STRIP);
    color.alpha = outer_alpha;
    dc.vertex(x-size/2, y-size/2, 0, 0, 0, color);
    color.alpha = inner_alpha;
    dc.vertex(x, y, 0, 0, 0, color);
    color.alpha = outer_alpha;
    dc.vertex(x-size/2, bottom+size, 0, 0, 0, color);
    color.alpha = inner_alpha;
    dc.vertex(x, bottom, 0, 0, 0, color);
    dc.end();

    //top
    dc.begin(CGL_TRIANGLE_STRIP);
    color.alpha = outer_alpha;
    dc.vertex(x-size/2, y-size/2, 0, 0, 0, color);
    dc.vertex(right+size, y-size/2, 0, 0, 0, color);
    color.alpha = inner_alpha;
    dc.vertex(x, y, 0, 0, 0, color);
    dc.vertex(right, y, 0, 0, 0, color);
    dc.end();

    //right
    dc.begin(CGL_TRIANGLE_STRIP);
    color.alpha = inner_alpha;
    dc.vertex(right, y, 0, 0, 0, color);
    color.alpha = outer_alpha;
    dc.vertex(right+size, y-size/2, 0, 0, 0, color);
    color.alpha = inner_alpha;
    dc.vertex(right, bottom, 0, 0, 0, color);
    color.alpha = outer_alpha;
    dc.vertex(right+size, bottom+size, 0, 0, 0, color);
    dc.end();

    //bottom
    dc.begin(CGL_TRIANGLE_STRIP);
    color.alpha = inner_alpha;
    dc.vertex(x, bottom, 0, 0, 0, color);
    dc.vertex(right, bottom, 0, 0, 0, color);
    color.alpha = outer_alpha;
    dc.vertex(x-size/2, bottom+size, 0, 0, 0, color);
    dc.vertex(right+size, bottom+size, 0, 0, 0, color);
    dc.end();
}

//>
void draw_more_sign(unicontext* dc, float x, float y, float w, float h)
{
//    dc.begin(CGL_LINE_STRIP);
//    dc.vertex(x, y);
//    dc.vertex(x+w, y+h*0.5f);
//    dc.vertex(x, y+h);
//    dc.end();
    vec2f ps[] = {vec2f(x, y), vec2f(x + w, y + h * 0.5f), vec2f(x, y + h)};
    dc->draw_arrays(CGL_LINE_STRIP, ps, 3);
}

//<
void draw_less_sign(unicontext* dc, float x, float y, float w, float h)
{
//    dc.begin(CGL_LINE_STRIP);
//    dc.vertex(x+w, y);
//    dc.vertex(x, y+h*0.5f);
//    dc.vertex(x+w, y+h);
//    dc.end();
    vec2f ps[] = {vec2f(x + w, y), vec2f(x, y + h * 0.5f), vec2f(x + w, y + h)};
    dc->draw_arrays(CGL_LINE_STRIP, ps, 3);
}

//^
void draw_up_sign(unicontext* dc, float x, float y, float w, float h)
{
//    dc.begin(CGL_LINE_STRIP);
//    dc.vertex(x, y+h);
//    dc.vertex(x+w*0.5f, y);
//    dc.vertex(x+w, y+h);
//    dc.end();
    vec2f ps[] = {vec2f(x, y + h), vec2f(x + w * 0.5f, y), vec2f(x + w, y + h)};
    dc->draw_arrays(CGL_LINE_STRIP, ps, 3);
}

//v
void draw_down_sign(unicontext* dc, float x, float y, float w, float h)
{
//    dc.begin(CGL_LINE_STRIP);
//    dc.vertex(x, y);
//    dc.vertex(x+w*0.5f, y+h);
//    dc.vertex(x+w, y);
//    dc.end();
    vec2f ps[] = {vec2f(x, y), vec2f(x + w * 0.5f, y + h), vec2f(x + w, y)};
    dc->draw_arrays(CGL_LINE_STRIP, ps, 3);
}

//<-
void draw_left_arrow(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w*0.5f, y+h);
    dc.end();
    dc.draw_line(x, y+h*0.5f, x+w, y+h*0.5f);
}

//->
void draw_right_arrow(draw_context& dc, float x, float y, float w, float h)
{
    draw_left_arrow(dc, x+w, y, -w, h);
}

//
void draw_up_arrow(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x+w, y+h*0.5f);
    dc.end();
    dc.draw_line(x+w*0.5f, y, x+w*0.5f, y+h);
}

//
void draw_down_arrow(draw_context& dc, float x, float y, float w, float h)
{
    draw_up_arrow(dc, x, y+h, w, -h);
}

//<<
void draw_pair_left(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x+w, y);
    dc.vertex(x+w*0.5f, y+h*0.5f);
    dc.vertex(x+w, y+h);
    dc.end();
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w*0.5f, y+h);
    dc.end();
}

//>>
void draw_pair_right(draw_context& dc, float x, float y, float w, float h)
{
    draw_pair_left(dc, x+w, y, -w, h);
}

//pair ^
void draw_pair_up(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x+w, y+h*0.5f);
    dc.end();
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x, y+h);
    dc.vertex(x+w*0.5f, y+h*0.5f);
    dc.vertex(x+w, y+h);
    dc.end();
}

//pair v
void draw_pair_down(draw_context& dc, float x, float y, float w, float h)
{
    draw_pair_up(dc, x, y+h, w, -h);
}

//还原
void draw_restore(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x + w*0.25f, y + h * 0.25f);
    dc.vertex(x + w*0.25f, y);
    dc.vertex(x + w, y);
    dc.vertex(x + w, y + h * 0.75f);
    dc.vertex(x + w*0.75f, y + h*0.75f);
    dc.end();

    dc.draw_rect(x, y+h*0.25f, w*0.75f, h*0.75f);
}

//rect
void draw_maxsize(draw_context& dc, float x, float y, float w, float h)
{
    dc.draw_rect(x, y, w, h);
}

//最小化符号 -
void draw_minsize(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINES);
    dc.vertex(x,     y + h * 0.5f);
    dc.vertex(x + w, y + h * 0.5f);
    dc.end();
}

//对号
void draw_checkmark(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINE_STRIP);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w*0.375f, y+h*0.875f);
    dc.vertex(x+w, y+h*0.25f);
    dc.end();
}

//+
void draw_plus(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINES);
    /*
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x+w*0.5f, y+h);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w, y+h*0.5f);
    */
    dc.vertex(x+w*0.5f, y);
    dc.vertex(x+w*0.5f, y+h);
    dc.vertex(x, y+h*0.5f);
    dc.vertex(x+w, y+h*0.5f);
    dc.end();
}

//x
void draw_cross(draw_context& dc, float x, float y, float w, float h)
{
    dc.begin(CGL_LINES);
    dc.vertex(x, y);
    dc.vertex(x + w, y + h);
    dc.vertex(x + w, y);
    dc.vertex(x, y + h);
    dc.end();
}

//check box, sign==true ? checkmark : cross
void draw_checkbox(draw_context& dc, float x, float y, float w, float h, bool checked, bool sign = true)
{
    dc.draw_rect(x, y, w, h);
    if(checked){
        if(sign){
            draw_checkmark(dc, x+w*0.125f, y+h*0.125f, w*0.75f, h*0.75f);
        }
        else{
            draw_cross(dc, x+w*0.125f, y+h*0.125f, w*0.75f, h*0.75f);
        }
    }
}

//radio button
void draw_radio(draw_context& dc, float x, float y, float w, float h, bool checked)
{
    dc.draw_ellipse(x+w*0.5f, y+w*0.5f, w*0.5f, h*0.5f, 15.0f);
    if(checked){
        dc.fill_ellipse(x+w*0.5f, y+w*0.5f, w*0.375f, h*0.375f, 15.0f);
    }
}

}//end namespace graphics
}//end namespace cgl

#endif //DRAW_CONTEXT_HPP_20151111123500