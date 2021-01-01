/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 softgl.hpp

 2018-03-25 00:35:27

*/
#ifndef SOFTGL_HPP_20180325003527
#define SOFTGL_HPP_20180325003527

#include <cgl/mat4.hpp>
#include <cgl/math.hpp>
#include <cgl/stack.hpp>
#include <cgl/base/float.h>
#include <cgl/vertex.hpp>
#include <cgl/vec.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/pixel.hpp>
#include <cgl/graphics/draw_context.hpp>
#include <cgl/graphics/color.hpp>
#include <cgl/std/dynamic_array.hpp>

#ifdef CGL_PLATFORM_WINDOWS
#include <cgl/graphics/gdi/bitmap.hpp>
#endif

#ifdef INC_VCL
#include <vcl.h>
extern void print(String, String);
#endif

extern cgl::graphics::pixelimage* __cgl_getTexture(int index);

namespace cgl{
namespace graphics{

//默认定点位
//default 12 4096 最大纹理4096
//10    1024
//#define SOFT3D_SHIFT        16
//#define SOFT3D_FIXED_POINT    (1<<SOFT3D_SHIFT)
//#define SOFT3D_MARK            ((SOFT3D_FIXED_POINT)-1)

#define SOFT3D_TEXTURE_SHIFT 16    //8位之下，马赛克化
#define SOFT3D_TEXTURE_FIXED (1<<SOFT3D_TEXTURE_SHIFT)

#define SOFT3D_AREA_SHIFT 16
#define SOFT3D_AREA_FIXED (1<<SOFT3D_AREA_SHIFT)
#define SOFT3D_AREA_MAX 32768    //大于32768就会溢出

#if SOFT3D_SHIFT > 16
#error SOFT3D_SHIFT can not greater than 16
#endif

inline vtx3i transform3i(const vtx3f& v, const mat4f& mat)
{
    vec4f p = mat.transform(v.x, v.y, v.z);
    return vtx3i(round(p.x), round(p.y), round(p.z), round(v.u * SOFT3D_TEXTURE_FIXED), round(v.v * SOFT3D_TEXTURE_FIXED), v.color);
}

inline vtx3f transform3f(const vtx3f& v, const mat4f& mat)
{
    vec4f p = mat.transform(v.x, v.y, v.z);
    return vtx3f(p.x, p.y, p.z, v.u, v.v,
        vec4f(v.color.red, v.color.green, v.color.blue, v.color.alpha));
}

template<typename VT, typename TT, typename CT>
float slope2D(const vtx3t2c4<VT, TT, CT>& a, const vtx3t2c4<VT, TT, CT>& b)
{
    return float(b.x - a.x) / float(b.y - a.y);
}

template<typename T>
struct pixel_state
{
    T a1, a2, a3;    //三角形面积占比
    uint32_t color;
};

//根据点的分量比例，计算颜色值
inline uint32_t calc_color(const vec4ub& c1, const vec4ub& c2, const vec4ub& c3,
    int a1, int a2, int a3)
{
    int a = (a1 * c1.alpha + a2 * c2.alpha + a3 * c3.alpha) >> SOFT3D_AREA_SHIFT;
    int r = (a1 * c1.red   + a2 * c2.red   + a3 * c3.red)   >> SOFT3D_AREA_SHIFT;
    int g = (a1 * c1.green + a2 * c2.green + a3 * c3.green) >> SOFT3D_AREA_SHIFT;
    int b = (a1 * c1.blue  + a2 * c2.blue  + a3 * c3.blue)  >> SOFT3D_AREA_SHIFT;
    return RGBA(byte_clamp(r), byte_clamp(g), byte_clamp(b), byte_clamp(a));
}

inline uint32_t calc_color(const vec4ub& c1, const vec4ub& c2, const vec4ub& c3,
    float a1, float a2, float a3)
{
    float a = a1 * c1.alpha + a2 * c2.alpha + a3 * c3.alpha;
    float r = a1 * c1.red   + a2 * c2.red   + a3 * c3.red;
    float g = a1 * c1.green + a2 * c2.green + a3 * c3.green;
    float b = a1 * c1.blue  + a2 * c2.blue  + a3 * c3.blue;
    return RGBA(byte_clamp(round(r)), byte_clamp(round(g)), byte_clamp(round(b)), byte_clamp(round(a)));
}

//计算三角形内部任意一点距离三个顶点的分量比（面积比）
void calc_pixel(pixel_state<int>& ps, int x, int y,
    const vtx3i& v1, const vtx3i& v2, const vtx3i& v3,
    int area, int shift)
{
    ps.a1 = math::triangle_area(x, y, v3.x, v3.y, v2.x, v2.y);
    ps.a2 = math::triangle_area(x, y, v1.x, v1.y, v3.x, v3.y);
    ps.a1 >>= shift;
    ps.a2 >>= shift;
    ps.a1 = ps.a1 * SOFT3D_AREA_FIXED / area;
    ps.a2 = ps.a2 * SOFT3D_AREA_FIXED / area;
    ps.a3 = SOFT3D_AREA_FIXED - ps.a1 - ps.a2;
//    ps.a1 = ps.a1 * 256 / area;
//    ps.a2 = ps.a2 * 256 / area;
//    ps.a3 = 255 - ps.a3;

    ps.color = calc_color(v1.color, v2.color, v3.color, ps.a1, ps.a2, ps.a3);
}

void calc_pixelf(pixel_state<float>& ps, float x, float y,
    const vtx3f& v1, const vtx3f& v2, const vtx3f& v3,
    float area)
{
//    ps.a1 = math::triangle_area(x, y, v3.x, v3.y, v2.x, v2.y);
//    ps.a2 = math::triangle_area(x, y, v1.x, v1.y, v3.x, v3.y);
//    ps.a1 = ps.a1 * area;
//    ps.a2 = ps.a2 * area;
//    ps.a3 = 1.0f - ps.a1 - ps.a2;
    math::get_uvf(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y, area, x, y, ps.a2, ps.a3);
    ps.a1 = 1.0f - ps.a2 - ps.a3;
    ps.color = calc_color(v1.color, v2.color, v3.color, ps.a1, ps.a2, ps.a3);
}

class soft_zbuffer
{
private:
    memblock<float> buffer;
    size_t width;
    size_t height;
public:
    soft_zbuffer():buffer(), width(), height()
    {
    }

    ~soft_zbuffer()
    {
        buffer.dispose();
        width = height = 0;
    }

    void resize(int w, int h, int z)
    {
        buffer.resize(w * h);
        this->clear(z);
    }

    void clear(float value)
    {
        std::fill(buffer.begin(), buffer.end(), value);
    }

    void set_depth(int x, int y, float value)
    {
        buffer[y * width + x] = value;
    }

    float depth(int x, int y)
    {
        return buffer[y * width + x];
    }

    bool test(int x, int y, float value)
    {
        float &depth = buffer[y * width + x];
        if(depth < value){//less
            depth = value;
            return true;
        }
        return false;
    }
};

class softgl
{
private:
    //int buf_width, buf_height;
    int pixelformat;
    pixelimage imagebuf;
    //pixellines colorbuf;
    pixelbuffer<uint32_t> pixelbuf;
    pixelbuffer<uint32_t> tex2d;
    int texture;
    //soft_zbuffer zbuffer;
    value_stack<mat4f, dynamic_array<mat4f> > matrix;
    vec4i m_viewport;

public:
    union{
        bool bilinear:1;        //双线过滤
        bool fusion:1;          //插值
        bool int_mode:1;        //整形模式
        bool anti_aliasing:1;   //反锯齿
    };

public:
    softgl();

    void viewport(int x, int y, int width, int height);

    void depthtest(bool enable)
    {
        //
    }

    void clear(uint32_t color)
    {
        if(pixelformat == CGL_BGRA|| pixelformat == CGL_BGR){
            color = RedBlueSwap(color);
        }
        pixelbuf.fill(color);
    }

    void draw_triangle(const vtx3f& v1, const vtx3f& v2, const vtx3f& v3);
    int  draw(graphics::draw_context& dc);

    void bind_buffer(pixelimage& image, int pf);
    void bind_texture(pixelimage& image);
    void bind_texture(int tex);

    #ifdef CGL_PLATFORM_WINDOWS
    void bind_buffer(HBITMAP hbmp);
    void bind_texture(HBITMAP hbmp);
    void paint(HDC dest, int x, int y, int w, int h);
    #endif

    void identity() { matrix.value.identity(); }
    void translate(float x, float y, float z = 0.0f) { matrix.value.translate(x, y, z); }
    void scale(float x, float y, float z = 1.0f) { matrix.value.scale(x, y, z); }
    void rotate(float angle, float x, float y, float z) { matrix.value.rotate(angle, x, y, z); }
    void push_matrix() { matrix.push(); }
    void pop_matrix() { matrix.pop(); }

private:
    void set_pixel(int x, int y, uint32_t color)
    {
        uint32_t* p = (uint32_t*)pixelbuf[y];
        if(pixelformat == CGL_RGBA){
            color = RedBlueSwap(color);
            //bswap32(color);
        }
        p[x] = blend32(p[x], color);
    }

    void draw_trianglef(const vtx3f& v1, const vtx3f& v2, const vtx3f& v3);
    void draw_trianglei(const vtx3f& v1, const vtx3f& v2, const vtx3f& v3);

    void render_pixel(int x, int y, float u, float v, uint32_t color);
};

softgl::softgl()
{
    texture = 0;
    bilinear = false;
    fusion = false;
    int_mode = true;
}

void softgl::viewport(int x, int y, int width, int height)
{
    m_viewport = vec4i(x, y, width, height);
    //buf_width = width;
    //buf_height = height;
    if(width != imagebuf.width() || height != imagebuf.height()){
        imagebuf.create(width, height, 32);
        this->bind_buffer(imagebuf, CGL_RGBA);
    }
    //zbuffer.resize(w, h, zfar);
    matrix.value.identity();
}

void softgl::bind_buffer(pixelimage& image, int pf)
{
    if(image.is_null()){
        pixelformat = 0;
        //colorbuf.bind(imagebuf, true);
        pixelbuf.assign(imagebuf);
    }
    else{
        pixelformat = pf;
        //colorbuf.bind(image, true);
        pixelbuf.assign(imagebuf);
    }
}

void softgl::bind_texture(pixelimage& image)
{
    if(!image.is_null()){
        tex2d.assign(image);
        texture = (int)&image;
    }
    else{
        tex2d.dispose();
    }
}

void softgl::bind_texture(int tex)
{
    if(tex){
        pixelimage* image = __cgl_getTexture(tex);
        if(image){
            return this->bind_texture(*image);
        }
    }
    texture = tex;
}

#ifdef CGL_PLATFORM_WINDOWS

void softgl::bind_buffer(HBITMAP hbmp)
{
    imagebuf.map(hbmp);
    pixelbuf.assign(imagebuf);
//    if(colorbuf.pixelbits() == 32){
//        pixelformat = CGL_BGRA;
//    }
//    else if(colorbuf.pixelbits() == 24){
//        pixelformat = CGL_BGR;
//    }
//    else{
//        pixelformat = 0;
//    }

}

void softgl::bind_texture(HBITMAP hbmp)
{
    if(hbmp){
        tex2d.assign(hbmp);
        texture = (int)hbmp;
        return ;
    }
    texture = 0;
}

void softgl::paint(HDC dest, int x, int y, int w, int h)
{
    imagebuf.paint(dest, m_viewport.x, m_viewport.y, imagebuf.width(), imagebuf.height());
    //BitBlt(dest, x, y, w, h, dc, 0, 0, SRCCOPY);
    //alphablend(dest, x, y, w, h, dc, 0, 0, width, height, 255);
}

#endif

void softgl::draw_triangle(const vtx3f& vtx1, const vtx3f& vtx2, const vtx3f& vtx3)
{
    if(int_mode){
        draw_trianglei(vtx1, vtx2, vtx3);
    }
    else{
        draw_trianglef(vtx1, vtx2, vtx3);
    }
}

void softgl::draw_trianglei(const vtx3f& vtx1, const vtx3f& vtx2, const vtx3f& vtx3)
{
    //sort v1 < v2 < v3
    //     v1
    //    / |
    //   /  |
    // v2   |
    //   \  |
    //    \ |
    //     v3
    //
    vtx3i v1, v2, v3;
    v1 = transform3i(vtx1, matrix.value);
    v2 = transform3i(vtx2, matrix.value);
    v3 = transform3i(vtx3, matrix.value);

    //顶点排序
    if(v2.y < v1.y)std::swap(v1, v2);
    if(v3.y < v2.y)std::swap(v2, v3);
    if(v2.y < v1.y)std::swap(v1, v2);

    //不绘制0面积三角形
    if(is_zero(v3.y - v1.y)){
        return ;
    }

    //计算三角形面积
    int area = math::triangle_area(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
    if(is_zero(area)){
        return ;
    }

    //动态顶点位置
    int area_shift = 0;
    while(area > SOFT3D_AREA_MAX){//面积精度
        ++area_shift;
        area >>= 1;
    }

    //计算长边步长
    float s  = slope2D(v1, v3);//长边步长
    float s1 = slope2D(v1, v2);
    float s2 = slope2D(v2, v3);
    float x_step  = 0;
    float x1_step = 0;
    float x2_step = 0;
    float x1, x2;

    int min_x, max_x;//剪裁矩形
    int min_y, max_y;

    pixel_state<int> ps1, ps2;
    int u, v;
    int uu1, uu2, uu3;
    int vv1, vv2, vv3;
    int u1_step, u2_step, u3_step;
    int v1_step, v2_step, v3_step;
    uint32_t v_color;//vertex color
    uint32_t t_color;//texture color
    int n;
    int alpha;
    int a_step;

    //剪裁y
    min_y = v1.y < 0 ? 0 : v1.y;
    v = min_y - v1.y;
    if(v1.y < 0){
        u = -v1.y;
        v = v2.y - v1.y;
        x_step = s * u;
        if(u < v){
            x1_step = s1 * u;
        }
        else{
            x2_step = s2 * (u - v);
        }
    }
    max_y = v3.y < pixelbuf.height() ? v3.y : pixelbuf.height();
    if(min_y >= max_y)return ;
    for(int y=min_y; y < max_y; ++y)
    {
        //if(y < 0)continue;
        //if(y >= height)break;

        x1 = v1.x + x_step;//(y - v1.y) * s;
        x_step += s;

        if(y < v2.y){
            x2 = v1.x + x1_step;//(y - v1.y) * s1;
            x1_step += s1;
        }
        else{
            x2 = v2.x + x2_step;//(y - v2.y) * s2;
            x2_step += s2;
        }

        if(x1 < x2){
            min_x = x1;
            max_x = x2;
        }
        else{
            min_x = x2;
            max_x = x1;
        }
        //剪裁x
        if(min_x < 0)min_x = 0;
        if(max_x >= pixelbuf.width())max_x = pixelbuf.width();
        if(min_x >= max_x)continue ;

        //计算行首和行尾像素信息
        calc_pixel(ps1, min_x, y, v1, v2, v3, area, area_shift);
        calc_pixel(ps2, max_x, y, v1, v2, v3, area, area_shift);

        n = max_x - min_x;//行宽度

        if(texture){
            u1_step = int(ps2.a1 - ps1.a1) / n * v1.u;
            u2_step = int(ps2.a2 - ps1.a2) / n * v2.u;
            u3_step = int(ps2.a3 - ps1.a3) / n * v3.u;
            uu1 = ps1.a1 * v1.u;
            uu2 = ps1.a2 * v2.u;
            uu3 = ps1.a3 * v3.u;

            v1_step = int(ps2.a1 - ps1.a1) / n * v1.v;
            v2_step = int(ps2.a2 - ps1.a2) / n * v2.v;
            v3_step = int(ps2.a3 - ps1.a3) / n * v3.v;
            vv1 = ps1.a1 * v1.v;
            vv2 = ps1.a2 * v2.v;
            vv3 = ps1.a3 * v3.v;
        }

        alpha = 0;
        a_step = 255 * 65536 / n;
        for(int x = min_x; x < max_x; ++x){
            //int m = x - min_x;
            if(!texture){
                v_color = blend32(ps1.color, ps2.color, alpha >> 16);// m * 256 / n);
                set_pixel(x, y, v_color);
            }
            else{
                if(fusion)v_color = blend32(ps1.color, ps2.color, alpha >> 16);// m * 256 / n);
                /*/纹理坐标 30帧左右影响
                a = ps1.a1 + (ps2.a1 - ps1.a1) * m / n;
                b = ps1.a2 + (ps2.a2 - ps1.a2) * m / n;
                c = ps1.a3 + (ps2.a3 - ps1.a3) * m / n;
                u = a * v1.u + b * v2.u + c * v3.u;
                v = a * v1.v + b * v2.v + c * v3.v;
                /*/
                u = uu1 + uu2 + uu3;
                v = vv1 + vv2 + vv3;
                uu1 += u1_step;
                uu2 += u2_step;
                uu3 += u3_step;
                vv1 += v1_step;
                vv2 += v2_step;
                vv3 += v3_step;
                //*/

                //还原area精度
                //u >>=  SOFT3D_TEXTURE_SHIFT;
                //v >>=  SOFT3D_TEXTURE_SHIFT;
                //u &= 0xFFFFF;
                //v &= 0xFFFFF;
                u >>=  SOFT3D_AREA_SHIFT;//SOFT3D_SHIFT;
                v >>=  SOFT3D_AREA_SHIFT;//SOFT3D_SHIFT;
                u &= (SOFT3D_AREA_FIXED - 1);
                v &= (SOFT3D_AREA_FIXED - 1);
                //u &= 0x7FFF;// SOFT3D_MARK;
                //v &= 0x7FFF;// SOFT3D_MARK;
                u *= (tex2d.width() - 1);
                v *= (tex2d.height() - 1);

                if(bilinear){
                    //u <<= (16 - SOFT3D_SHIFT);
                    //v <<= (16 - SOFT3D_SHIFT);
                    t_color = bilinear_color(tex2d, u, v);
                    if(fusion){
                        t_color = fusion32(t_color, v_color);
                    }
                }
                else{
                    //还原纹理精度
                    u >>= SOFT3D_TEXTURE_SHIFT;//SOFT3D_SHIFT;
                    v >>= SOFT3D_TEXTURE_SHIFT;//SOFT3D_SHIFT;
                    //u >>= SOFT3D_SHIFT;
                    //v >>= SOFT3D_SHIFT;
                    /*
                    if(tex2d.width < px || tex2d.height < py){//if not floor
                        print(px, py);
                        return ;
                    }
                    /*/
                    t_color = tex2d.get_pixel(u, v);
                    if(fusion){
                        t_color = fusion32(t_color, v_color);
                    }
                }

                //alpha test
                //if(t_color & 0xFF)//png alpha
                if(t_color & 0xFF000000)//windows alpha
                {//效率影响很小
                    set_pixel(x, y, t_color);
                }
            }//texture
            alpha += a_step;
        }//end for x
    }//end for y
}

void softgl::render_pixel(int x, int y, float u, float v, uint32_t color)
{
    int px, py;
    if(bilinear){//掉2帧
        #if 1
        px = round(u * 65535.0f);
        py = round(v * 65535.0f);
        px &= 0xFFFF;
        py &= 0xFFFF;
        px *= (tex2d.width() - 1);
        py *= (tex2d.height() - 1);
        //px >>= 12;
        //py >>= 12;
        #else
        u -= floor32(u);
        v -= floor32(v);
        px = u * (tex2d.width - 1) * 65535.0f;
        py = v * (tex2d.height - 1) * 65535.0f;
        #endif
        if(fusion){
            color = fusion32(bilinear_color(tex2d, px, py), color);
        }
        else{
            color = bilinear_color(tex2d, px, py);
        }
    }
    else{
        #if 1
        px = round(u * 4096.0f);
        py = round(v * 4096.0f);
        px &= 4095;
        py &= 4095;
        px *= tex2d.width();
        py *= tex2d.height();
        px >>= 12;
        py >>= 12;
        #else
        u -= floor32(u);
        v -= floor32(v);
        px = u * (tex2d.width - 0);
        py = v * (tex2d.height - 0);
        #endif
        //*
        if(tex2d.width() <= px || tex2d.height() <= py){//if not floor
            //print(px, py);
            return ;
        }
        //*/
        if(fusion){
            color = fusion32(tex2d.get_pixel(px, py), color);
        }
        else{
            color = tex2d.get_pixel(px, py);
        }
    }
    //if(color & 0xff)
    {//效率影响很小
        set_pixel(x, y, color);
        //DrawPixel(x, y, cl);
    }
}

void softgl::draw_trianglef(const vtx3f& vtx1, const vtx3f& vtx2, const vtx3f& vtx3)
{
    //sort v1 < v2 < v3
    //     v1
    //    / |
    //   /  |
    // v2   |
    //   \  |
    //    \ |
    //     v3
    //
    typedef float real_t;

    //DWORD t = GetTickCount();

    vtx3f v1, v2, v3;
    v1 = transform3f(vtx1, matrix.value);
    v2 = transform3f(vtx2, matrix.value);
    v3 = transform3f(vtx3, matrix.value);

    if(v2.y < v1.y)std::swap(v1, v2);
    if(v3.y < v2.y)std::swap(v2, v3);
    if(v2.y < v1.y)std::swap(v1, v2);

    //CGL_LOG("time = %u", GetTickCount() - t); t = GetTickCount();

    if(is_zero(v3.y - v1.y)){
        return ;
    }

    //计算三角形面积
//    real_t area = math::triangle_area(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
//    area = 1.0f / area;
    real_t area = math::calc_area(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
    if(is_zero(area)){
        return ;
    }

    //计算长边步长
    real_t s  = slope2D(v1, v3);//长边步长
    real_t s1 = slope2D(v1, v2);
    real_t s2 = slope2D(v2, v3);
    real_t x1, x2;

    pixel_state<real_t> ps1, ps2;
    real_t a, b, c;
    real_t u, v;
    uint32_t px, py;
    uint32_t color;
    real_t m, n;
    real_t inv_n;
    //real_t mo;

    real_t t1, t2, t3;

    int min_y, max_y;
    int min_x, max_x;

    min_y = round(v1.y);
    max_y = round(v3.y);
    if(min_y < 0){
        min_y = 0;
    }
    if(max_y >= pixelbuf.height()){
        max_y = pixelbuf.height();
    }

    //for(int y=v1.y; y < v3.y; ++y)
    for(int y=min_y; y < max_y; ++y)
    {
//        if(y < 0)continue;
//        if(y >= pixelbuf.height())break;

        x1 = v1.x + (y - v1.y) * s;

        if(y < v2.y){
            x2 = v1.x + (y - v1.y) * s1;
        }
        else{
            x2 = v2.x + (y - v2.y) * s2;
        }

        //剪裁
        if(x1 < x2){
            min_x = round(x1);
            max_x = round(x2);
        }
        else{
            min_x = round(x2);
            max_x = round(x1);
        }
        if(min_x < 0.0f){
            min_x = 0;
        }
        if(max_x >= pixelbuf.width()){
            max_x = pixelbuf.width();
        }

        //计算最大颜色值和最小颜色值
        if(min_x < max_x){
            //mo = 1.0f / (max_x - min_x);//这个很慢
            calc_pixelf(ps1, min_x, y, v1, v2, v3, area);
            calc_pixelf(ps2, max_x, y, v1, v2, v3, area);
            n = max_x - min_x;
            inv_n = 1.0f / n;
        }
        else{
            continue;
        }
        /*
        if(tex2d.id){//影响不大
            t1 = real_t(ps2.a1 - ps1.a1) / n;
            t2 = real_t(ps2.a2 - ps1.a2) / n;
            t3 = real_t(ps2.a3 - ps1.a3) / n;
            a = ps1.a1;
            b = ps1.a2;
            c = ps1.a3;
        }
        //*/

        for(int x = min_x; x < max_x; ++x){
            m = real_t(x - min_x) * inv_n;
            //m *= 255.0f;
            color = blend32(ps1.color, ps2.color, round(m));// round(255 * m));
            if(!texture){//颜色填充模式
                set_pixel(x, y, color);
            }
            else{
                //*
                a = ps1.a1 + (ps2.a1 - ps1.a1) * m;
                b = ps1.a2 + (ps2.a2 - ps1.a2) * m;
                c = ps1.a3 + (ps2.a3 - ps1.a3) * m;
                u = a * v1.u + b * v2.u + c * v3.u;
                v = a * v1.v + b * v2.v + c * v3.v;
                /*/
                //精度有所损失，int模式，精度损失很大
                u = a * v1.u + b * v2.u + c * v3.u;
                v = a * v1.v + b * v2.v + c * v3.v;
                a += t1;
                b += t2;
                c += t3;
                //*/

                //裂缝
//                if(u < 0)u = 0;
//                if(v < 0)v = 0;
//                if(u > 1.0)u = 1.0;
//                if(v > 1.0)v = 1.0;
//                u = fmod(u, 1.0f);
//                v = fmod(v, 1.0f);

                render_pixel(x, y, u, v, color);
            }//if(tex2d.tex)
            //m += mo;
        }//end for x
    }//end for y
    //CGL_LOG("time = %u", GetTickCount() - t); t = GetTickCount();
}

int softgl::draw(draw_context& dc)
{
    if(!dc.size())return -1;
    size_t begin, end;
    const draw_context::vertex_type *pv = dc.vertex_pointer();
    for(size_t i=0; i<dc.size(); ++i)
    {
        const drawcall& c = dc.commands(i);
        switch(c.command)
        {
        case CGL_POINTS:
            for(int i=0; i<c.size; ++i){
                //image.set_pixel(pv[c.start + i].x, pv[c.start + i].y, pv[c.start + i].color);
            }
            break;
        case CGL_LINES:
        case CGL_LINE_LOOP:
        case CGL_LINE_STRIP:
        case CGL_TRIANGLE_STRIP:
            //print("draw", c.command);
            break;
        case CGL_TRIANGLES:
            end = c.offset + c.size;
            begin = c.offset;
            for( ; begin < end; ){
                draw_triangle(pv[begin++], pv[begin++], pv[begin++]);
            }
            break;
        case CGL_TRIANGLE_FAN:
        {
            end = c.offset + c.size - 1;
            begin = c.offset + 1;
            for(; begin < end; ++begin){
                draw_triangle(pv[c.offset], pv[begin] , pv[begin + 1]);
            }
            /*
            image.bitmap.Canvas.Pen.Color = clRed;
            for(begin = c.start + 1; begin < end; ++begin){
                //image.bitmap.Canvas.MoveTo(pv[c.start + 0].x, pv[c.start + 0].y);
                //image.bitmap.Canvas.LineTo(pv[begin].x, pv[begin].y);
                //image.bitmap.Canvas.LineTo(pv[begin + 1].x, pv[begin + 1].y);
            }
            //image.bitmap.Canvas.LineTo(pv[c.start].x, pv[c.start].y);
            */
            break;
        }
        case CGL_CLEAR:
            //this.clear(c.red/255.0f, c.green/255.0f, c.blue/255.0f, c.alpha/255.0f);
            break;
        case CGL_COLOR:
            //glColor4f(itr.red, itr.green, itr.blue, itr.alpha);
            //color =
            break;
        case CGL_FLUSH:
            //glFlush();
            break;
        case CGL_DRAW_CONTEXT:
            //reinterpret_cast<draw_context*>(itr.object).paint();
            break;

        case CGL_TEXTURE_1D:
            //glBindTexture(GL_TEXTURE_1D, c.value);
            break;
        case CGL_TEXTURE_2D:
            //print("bindimage", c.value);
            this->bind_texture(c.value);
            //this.bind_texture(c.value);
            break;
        case CGL_TEXTURE_3D:
            //glBindTexture(GL_TEXTURE_3D, itr.value);
            break;


        case CGL_IDENTITY:
            matrix.value.identity();
            break;
        case CGL_PUSH_MATRIX:
            matrix.push();
            break;
        case CGL_POP_MATRIX:
            matrix.pop();
            break;
        case CGL_ROTATE:
            matrix.value.rotate(c.angle, c.x, c.y, c.z);
            break;
        case CGL_TRANSLATE:
            matrix.value.translate(c.x, c.y, c.z);
            break;
        case CGL_SCALE:
            matrix.value.scale(c.x, c.y, c.z);
            break;

        /*
        case CGL_BLEND_FUNC:
            glBlendFunc(c.blend_src, c.blend_dst);
            break;
        case CGL_CLIPRECT:
            #if CGL_COORDINATE == CGL_LOWER_LEFT
                glScissor(c.x, c.y, c.width, c.height);
            #else
                //反转模式
                glGetIntegerv(GL_VIEWPORT, viewport);
                glScissor(c.x, viewport[3] - c.y - c.height, c.width, c.height);
            #endif
            break;
        */
        /*
        case CGL_POINT_SIZE:
            glPointSize(c.value);
            break;
        case CGL_LINE_WIDTH:
        */
        case CGL_PEN_WIDTH:
            //this.pen_width(c.width);
            break;
        default:
            break;
        }
    }
    return 0;
}

}//end namespace graphcis
}//end namespace cgl

#endif //SOFTGL_HPP_20180325003527

