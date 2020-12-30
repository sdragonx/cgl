/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 stb_font.hpp

 2018-07-31 18:28:38

 stbtt_GetCodepointBox(f.handle(), text[i], &x0, &y0, &x1, &y1);

*/
#ifndef STB_FONT_HPP_20180731182838
#define STB_FONT_HPP_20180731182838

#include <cgl/io/filebuf.hpp>
#include <cgl/graphics/font.hpp>
#include <cgl/graphics/image.hpp>
#include <cgl/graphics/tesselation.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_assert CGL_ASSERT
#define STBTT_RASTERIZER_VERSION 1

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option push
    #pragma option -w-8004        //is assigned a value that is never used
    #pragma option -w-8008        //Condition is always true
    #pragma option -w-8012        //Comparing signed and unsigned values
    #pragma option -w-8066        //Unreachable code
    #pragma option -w-8062        //Previous options and warnings not restored
#endif

#include <cgl/external/stb/stb_truetype.h>

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option pop
#endif

namespace cgl{
namespace graphics{

//字体轮廓操作
void transform(stbtt_vertex& v,
    int x, int y,       //平移
    float scale,        //缩放
    bool invert = true) //翻转
{
    if(invert){
        //v.x = -v.x;
        v.y = -v.y;
        //v.cx = -v.cx;
        v.cy = -v.cy;
    }
    v.x *= scale;
    v.y *= scale;
    v.x += x;
    v.y += y;
    v.cx *= scale;
    v.cy *= scale;
    v.cx += x;
    v.cy += y;
}

void tesselate_char(std::vector<vec2f>& vs, std::vector<int>& ids, stbtt_vertex* points, int size)
{
    vec2f current;
    tesselator tess;
    tess.init();

    std::vector<vec2f> temp;

    for(int i=0; i<size; ++i){
        switch(points[i].type){
        case STBTT_vmove:
            current = vec2f(points[i].x, points[i].y);
            tess.add_contour(temp);
            temp.clear();
            break;
        case STBTT_vline:
            temp.push_back(vec2f(points[i].x, points[i].y));
            current = vec2f(points[i].x, points[i].y);
            break;
        case STBTT_vcurve://二次贝塞尔曲线
            math::conic_bezier(temp, current.x , current.y, points[i].cx, points[i].cy, points[i].x, points[i].y, 4);
            current = vec2f(points[i].x, points[i].y);
            break;
        case STBTT_vcubic://三次贝塞尔曲线
            CGL_LOG_ERROR("tesselate_char");
//            stbtt__tesselate_cubic(points, &num_points, x,y,
//                 vertices[i].cx, vertices[i].cy,
//                 vertices[i].cx1, vertices[i].cy1,
//                 vertices[i].x,  vertices[i].y,
//                 objspace_flatness_squared, 0);
//               x = vertices[i].x, y = vertices[i].y;
            break;
        default:
            break;
        }
    }

    if(!temp.empty()){
        tess.add_contour(temp);
    }

    tess.tesselate();
    tess.copy(vs, ids);
}

class stb_font
{
public:
    std::vector<byte_t> m_buffer;
    stbtt_fontinfo m_font;
    int m_size;
    float m_scale;
public:
    stb_font() : m_buffer(), m_size(), m_scale()
    {
        memset(&m_font, 0, sizeof(m_font));
    }

    stbtt_fontinfo* handle()
    {
        return &m_font;
    }

    int open(const char* filename)
    {
        if(io::load_file(m_buffer, filename) != CGL_OK){
            CGL_LOG_ERROR("stb_ttf_font::open : file open error -> %s", filename);
            return -1;
        }

        stbtt_InitFont(&m_font, &m_buffer[0], stbtt_GetFontOffsetForIndex(&m_buffer[0], 0));
        set_size(20);
        return 0;
    }

    int open(const wchar_t* filename)
    {
        if(io::load_file(m_buffer, filename) != CGL_OK){
            CGL_LOG_ERROR("stb_ttf_font::open : file open error -> %s", filename);
            return -1;
        }

        stbtt_InitFont(&m_font, &m_buffer[0], stbtt_GetFontOffsetForIndex(&m_buffer[0], 0));
        set_size(20);
        return 0;
    }

    int open_memory(const void* data, size_t size)
    {
        m_buffer.resize(size);
        memcpy(&m_buffer[0], data, size);

        stbtt_InitFont(&m_font, &m_buffer[0], stbtt_GetFontOffsetForIndex(&m_buffer[0], 0));
        set_size(20);
        return 0;
    }

    void dispose()
    {
        memset(&m_font, 0, sizeof(m_font));
        std::vector<byte_t> tmp;
        m_buffer.swap(tmp);
        m_size = 0;
        m_scale = 0;
    }

    bool is_open()const
    {
        return !m_buffer.empty();
    }

    void set_size(int size)
    {
        //stb_font default dpi 96
        if(m_size != size){
            float n = size * 96 / 72;// * 1.27f;//scale size eqaul to freetype
            m_scale = stbtt_ScaleForPixelHeight(handle(), n);
            m_size = size;
        }
    }

    int size()const
    {
        return m_size;
    }

    int char_width(wchar_t ch)
    {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(handle(), ch, &advance, &lsb);
        return advance * m_scale;
    }

    //line height
    int char_height()
    {
        /*
        int h;
        stbtt_GetFontVMetrics(handle(), 0, 0, &h);
        return h * m_scale;
        */
        return m_size * 72 / 96;// * m_scale;
    }

    vec4i bounding_box()
    {
        int x1, y1, x2, y2;
        stbtt_GetFontBoundingBox(this->handle(), &x1, &y1, &x2, &y2);
        return vec4i(x1, y1, x2 - x1, y2 - y1);
    }

    //字体数量
    int count()const
    {
        return m_font.numGlyphs;
    }

    //查找字符
    int find(int ch)const
    {
        return stbtt_FindGlyphIndex(&m_font, ch);
    }

    int render_font_by_id(pixelimage& image, char_info& info, int ch)
    {
        int x, y, w, h;
        int advance, lsb;

        byte_t* bmp = stbtt_GetGlyphBitmapSubpixel(handle(), 0, m_scale, 0, 0, ch, &w, &h, &x, &y);

        if(!bmp){
            return CGL_ERROR;
        }

        stbtt_GetCodepointHMetrics(handle(), ch, &advance, &lsb);

        info.x = x;//lsb * m_scale;
        info.y = -y;
        info.next_x = advance * m_scale;
        info.next_y = 0;

        byte_t *dest;
        byte_t *source = bmp;
        image.create(w, h, 8, false);
        for (int i=0; i < h; ++i){
            dest = image.scanline(i);
            memcpy(dest, source, w);
            source += w;
        }
        stbtt_FreeBitmap(bmp, 0);
        return 0;
    }

    int render_font(pixelimage& image, char_info& info, int ch)
    {
        int x, y, w, h;
        int advance, lsb;

        byte_t* bmp = stbtt_GetCodepointBitmap(handle(), 0, m_scale, ch, &w, &h, &x, &y);

        if(!bmp){
            return CGL_ERROR;
        }

        stbtt_GetCodepointHMetrics(handle(), ch, &advance, &lsb);

        //int line;
        //stbtt_GetFontVMetrics(handle(), 0, 0, &line);//is zero

        info.x = x;//lsb * m_scale;
        info.y = -y;
        info.next_x = advance * m_scale;
        info.next_y = 0;

        byte_t *dest;
        byte_t *source = bmp;
        image.create(w, h, 8, false);
        for (int i=0; i < h; ++i){
            dest = image.scanline(i);
            memcpy(dest, source, w);
            source += w;
        }
        stbtt_FreeBitmap(bmp, 0);
        return 0;
    }

    //这个渲染特别慢
    int render_outline(pixelimage& image, char_info& info, int ch, int width)
    {
        int x, y, w, h;
        int advance, lsb;
        const float pixel_dist_scale = 8.0f;
        int outline_size = width * pixel_dist_scale;

        byte_t* bmp = stbtt_GetCodepointSDF(handle(),
            m_scale,
            ch,
            width,                //生成的图片扩大多少像素
            outline_size,        //描边大小，和pixel_dist_scale有关
            pixel_dist_scale,    //数值越小，描边越虚化
            &w, &h, &x, &y);

        stbtt_GetCodepointHMetrics(handle(), ch, &advance, &lsb);

        info.x = x;// + width / 2;//lsb * m_scale;
        info.y = -y - 0;
        info.next_x = advance * m_scale;
        info.next_y = 0;

        byte_t *dest;
        byte_t *source = bmp;
        int n;
        image.create(w, h, 8, false);

        for (int i=0; i < h; ++i){
            dest = image.scanline(i);
            for(int x=0; x < w; ++x){
                n = source[x];
                if(n){
                    if(n < outline_size){//outer color
                        n *= 32;
                        if(n >=255)n = 255;
                        dest[x] = n;
                    }
                    else{//inner color
                        dest[x] = 255;
                    }
                }
                else dest[x] = 0;
            }
            source += w;
        }
        stbtt_FreeSDF(bmp, 0);
        return 0;
    }

    int tesselate(std::vector<vec2f>& vs, std::vector<int>& ids, int ch)
    {
        //(f.handle()->fontstart);
        //(f.handle()->numGlyphs);

        stbtt_vertex *verts;

        vs.clear();
        ids.clear();

        int id = stbtt_FindGlyphIndex(this->handle(), ch);
        if(id){
            int num_verts = stbtt_GetGlyphShape(this->handle(), id, &verts);
            int x1, y1, x2, y2;
            stbtt_GetFontBoundingBox(this->handle(), &x1, &y1, &x2, &y2);
            for(int i=0; i<num_verts; ++i){
                transform(verts[i], 0, y2, 1);
                //transform(verts[i], 0, 512, 1);
                //draw_point(dc, verts[i].x, verts[i].y);
                //print(verts[i].x, verts[i].y);
            }
            tesselate_char(vs, ids, verts, num_verts);
            STBTT_free(verts, 0);
            return 0;
        }
        return CGL_ERROR;
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //STB_FONT_HPP_20180731182838


#if 0
stb 字体轮廓导出

    int length = 20;
    const char* name = stbtt_GetFontNameString(f.handle(),
        &length,
        STBTT_PLATFORM_ID_MICROSOFT,
        STBTT_MS_EID_UNICODE_BMP,
        STBTT_MS_LANG_ENGLISH,
        0);
#endif

