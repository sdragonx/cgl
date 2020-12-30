/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 font.hpp

 2015-04-05 00:01:39

 使用freetype把字符生成灰度图像

*/
#ifndef FONT_HPP_20150405000139
#define FONT_HPP_20150405000139

#include <cgl/graphics/image.hpp>
#include <cgl/graphics/palette.hpp>
#include <cgl/graphics/font.hpp>
#include <cgl/graphics/tesselation.hpp>

//freetype2
//#include <freetype2/ft2build.h>
//*
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

#if defined CGL_PLATFORM_CBUILDER
    #if defined CGL_PLATFORM_CB6
        #pragma link "freetype_cb6.lib"
    #else
        #pragma link "freetype_2010.lib"
    #endif
#endif
//*/
//#include <cgl/external/freetype/FreeTypeAmalgam.h>
//#include <cgl/external/freetype/FreeTypeAmalgam.c>

namespace cgl{
namespace graphics{

//上下反转图像数据
inline int ft_bmpflip(FT_Bitmap& bmp)
{
    int linebytes = std::abs(bmp.pitch);
    return image_flipY(bmp.buffer, bmp.rows, linebytes);
}

//gray256 复制ft图像
inline int ft_bitblt(byte_t* dst, int linebytes, int x, int y, FT_Bitmap& src)
{
    byte_t* pdst = dst + linebytes*y;
    byte_t* psrc = src.buffer;
    for(size_t i=0; i<src.rows; ++i)
    {
        memcpy(pdst+x, psrc, src.width);
        pdst+=linebytes;
        psrc+=src.width;
    }
    return 0;
}

//
// freetype_decompose
// 字体轮廓解析
// 2020-03-01 02:46:40
//

int freetype_decompose(path_command& pathc, FT_Outline* outline)
{
    if ( !outline ){
        return -1;
    }
    FT_Vector lastpos;
    FT_Vector v_control;
    FT_Vector v_middle;

    FT_Vector* first;
    FT_Vector* last;
    char*      tags;

    int i_first = 0;
    int i_last;
    int tag;

    FT_Vector v_start;
    FT_Vector v_last;

    for (int n = 0; n < outline->n_contours; n++ ){
        CGL_LOG( "Outline %d\n", n );
        i_last = outline->contours[n];
        if ( i_last < 0 ){
            CGL_LOG_ERROR("freetype_tesselate : last < 0");
            return -1;
        }

        first = outline->points + i_first;
        last = outline->points + i_last;
        tags  = outline->tags + i_first;
        tag   = FT_CURVE_TAG( tags[0] );

        v_start = *first;
        v_last  = *last;

        // A contour cannot start with a cubic control point!
        if ( tag == FT_CURVE_TAG_CUBIC ){
            CGL_LOG_ERROR("tag == FT_CURVE_TAG_CUBIC");
            return -1;
        }

        // check first point to determine origin
        if ( tag == FT_CURVE_TAG_CONIC ){
            // first point is conic control.  Yes, this happens.
            if ( FT_CURVE_TAG( outline->tags[i_last] ) == FT_CURVE_TAG_ON ){
                // start at last point if it is on the curve
                v_start = v_last;
                last--;
            }
            else{
                // if both first and last points are conic,
                // start at their middle and record its position
                // for closure
                v_start.x = ( v_start.x + v_last.x ) / 2;
                v_start.y = ( v_start.y + v_last.y ) / 2;

                // v_last = v_start;
            }
            first--;
            tags--;
        }

        //---------------------------------------------------------------------------
        //move to start point
        pathc.move_to(v_start);
        lastpos = v_start;

        while ( first < last ){
            ++first;
            tag = FT_CURVE_TAG(*++tags);
            switch ( tag ){
            case FT_CURVE_TAG_ON://直线
                pathc.line_to(*first);
                lastpos = *first;
                break;
            case FT_CURVE_TAG_CONIC://二次贝塞尔曲线
                v_control = *first;

                while( first < last ){
                    ++first;
                    tag = FT_CURVE_TAG(*++tags);

                    if ( tag == FT_CURVE_TAG_ON ){//遇到直线点，终止画曲线
                        break;
                    }

                    if ( tag != FT_CURVE_TAG_CONIC ){
                        CGL_LOG("tag != FT_CURVE_TAG_CONIC");
                        return -1;
                    }

                    v_middle.x = ( v_control.x + first->x ) / 2;
                    v_middle.y = ( v_control.y + first->y ) / 2;

                    pathc.conic_curve(lastpos, v_control, v_middle);
                    lastpos = v_middle;
                    v_control = *first;
                }

                if ( tag == FT_CURVE_TAG_ON ){//连接到直线点
                    pathc.conic_curve(lastpos, v_control, *first);
                    lastpos = *first;
                }
                else{//最后一个点，首尾连接
                    pathc.conic_curve(lastpos, v_control, v_start);
                }
                break;
            case FT_CURVE_TAG_CUBIC://三次贝塞尔曲线
                if ( first + 1 > last || FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC ){
                    CGL_LOG_DEBUG("first + 1 > last || FT_CURVE_TAG( tags[1] ) != FT_CURVE_TAG_CUBIC");
                    return -1;
                }
                first += 2;
                tags  += 2;

                if ( first <= last ){
                    pathc.cubic_curve(lastpos, first[-2], first[-1], *first);
                    lastpos = *first;
                }
                else{
                    pathc.cubic_curve(lastpos, first[-2], first[-1], v_start);
                    //lastpos = v_start;
                }
                break;
            default:
                break;
            }
        }
        //close the contour with a line segment
        pathc.line_to(v_start);
        //---------------------------------------------------------------------------

        i_first = i_last + 1;
    }

    return 0;
}




struct ft_rect
{
public:
    float xmin, xmax, ymin, ymax;
    public:
    ft_rect() { }
    ft_rect(float left, float top, float right, float bottom)
    :xmin(left), xmax(right), ymin(top), ymax(bottom)
    {
    }

    void Include(const vec2f &r)
    {
        xmin = std::min(xmin, r.x);
        ymin = std::min(ymin, r.y);
        xmax = std::max(xmax, r.x);
        ymax = std::max(ymax, r.y);
    }

    float width() const
    {
        return xmax - xmin + 1;
    }
    float height()const
    {
        return ymax - ymin + 1;
    }
};

// A horizontal pixel span generated by the FreeType renderer.
struct ft_span
{
    ft_span() { }
    ft_span(int _x, int _y, int _width, int _coverage)
        :x(_x), y(_y), width(_width), coverage(_coverage)
    {
    }

    int x, y, width, coverage;
};

//对freetype的封装
class ftfont
{
public:
    typedef std::vector<ft_span> Spans;

private:
    FT_Library m_library;
    FT_Face m_face;
    int m_size;

public:
    ftfont():m_library(null), m_face(null), m_size()
    {
    }
    ~ftfont()
    {
        this->dispose();
    }

    FT_Face handle()const
    {
        return m_face;
    }

    int open(const char* filename)
    {
        if(this->is_open())this->dispose();
        FT_Init_FreeType(&m_library);
        if(check_error(FT_New_Face(m_library, filename, 0, &m_face)))
        {
            return -1;
        }
        //设置字体解码模式
        FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

        this->set_size(16, 72);
        return 0;
    }

    int open_memory(const void* data, size_t size)
    {
        if(this->is_open())this->dispose();
        FT_Init_FreeType(&m_library);
        FT_New_Memory_Face(m_library, (byte_t*)data, size, 0, &m_face);
        //设置字体解码模式
        FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);

        this->set_size(16, 72);
        return 0;
    }

    void dispose()
    {
        if(m_library){
            if(m_face){
                FT_Done_Face(m_face);
                m_face = null;
            }
            FT_Done_FreeType(m_library);
            m_library = null;
        }
    }

    bool is_open()const
    {
        return m_library;
    }

    //设置字体大小
    //96dpi
    //72dpi高度和图片一直
    int set_size(int size, int dpi = 72)
    {
        // 设定字体字符宽高和分辨率
        m_size = size;
        return FT_Set_Char_Size(m_face, 0, size << 6, dpi, dpi);
    }

    int char_width(wchar_t ch)
    {
        FT_UInt gindex = FT_Get_Char_Index(m_face, ch);
        if (FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP) != 0){
            CGL_LOG("ftfont: char_width() FT_Load_Glyph failed.");
            return 0;
        }

        FT_GlyphSlot slot = m_face->glyph;

        return slot->advance.x >> 6;
    }

    int char_height(wchar_t ch)
    {
        return m_size;
    }

    //字体字符数量
    int count()const
    {
         return m_face->num_glyphs;
    }

    //查找字符
    int find(int ch)const
    {
        return FT_Get_Char_Index(m_face, ch);
    }

    int render_font(pixelimage& image, char_info& info, int ch, bool bitmode = false);
    int render_outline(
        pixelimage& image,
        char_info& info,
        int ch,
        float outline_width,
        const vec4ub& color = color::white,            //字体颜色
        const vec4ub& outline_color = color::black);    //描边颜色

private:
    int check_error(int err)
    {
        if(err){
            if (err == FT_Err_Unknown_File_Format)
            { //... 可以打开和读这个文件，但不支持它的字体格式
                CGL_LOG("freetype: unknown font format.");
            }
            else
            { //... 其它的错误码意味着这个字体文件不能打开和读， ... 或者简单的说它损坏了...
                CGL_LOG("freetype: font open faild.");
            }
        }
        return err;
    }

    static void RasterCallback(int y, int count, const FT_Span* spans, void* user);
    void RenderSpans(FT_Outline * const outline, Spans *spans);
};

int ftfont::render_font(pixelimage& image, char_info& info, int ch, bool bitmode)
{
    FT_GlyphSlot slot = NULL;
    int width  = 0;
    int height = 0;

    //装载渲染字体
    if(FT_Load_Char(m_face, ch, bitmode ? FT_LOAD_MONOCHROME|FT_LOAD_RENDER : FT_LOAD_RENDER) != 0)
    {
        return -1;
    }

    slot = m_face->glyph;

    //输出字体坐标信息
    info.x = slot->bitmap_left;
    info.y = slot->bitmap_top;
    info.next_x = slot->advance.x>>6;
    info.next_y = slot->advance.y>>6;

    if(ch == 0x20){//空格
        return -2;
    }

    if(slot->format != FT_GLYPH_FORMAT_BITMAP)
    {
        CGL_LOG("freetype: rander character faild.");
        return -3;
    }

    width = slot->bitmap.width;
    height = slot->bitmap.rows;

    //输出图像
    image.create(width, height, bitmode ? 1 : 8, (void*)PALETTE_WIN256);
    int linebytes = std::abs(slot->bitmap.pitch);
    byte_t *pdst = image.data();
    byte_t *psrc = slot->bitmap.buffer;
    for(int y=0; y<height; ++y)
    {
        pdst = image.scanline(y);
        memcpy(pdst, psrc, width);
        pdst += image.linebytes();
        psrc += linebytes;
    }

    return 0;
}

// Each time the renderer calls us back we just push another span entry on our list.
void ftfont::RasterCallback(int y, int count, const FT_Span* spans, void* user)
{
    ftfont::Spans *sptr = (ftfont::Spans*)user;
    for(int i = 0; i < count; ++i){
        sptr->push_back(ft_span(spans[i].x, y, spans[i].len, spans[i].coverage));
    }
}

// Set up the raster parameters and render the outline.

void ftfont::RenderSpans(FT_Outline * const outline, Spans *spans)
{
    FT_Raster_Params params;
    memset(&params, 0, sizeof(params));
    params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
    params.gray_spans = RasterCallback;
    params.user = spans;

    FT_Outline_Render(m_library, outline, &params);
}

// Render the specified character as a colored glyph with a colored outline
int ftfont::render_outline(pixelimage& image, char_info& info,
    int ch,
    float outline_width,
    const vec4ub& color,
    const vec4ub& outline_color)
{
    // Load the glyph we are looking for.
    FT_UInt gindex = FT_Get_Char_Index(m_face, ch);
    if (FT_Load_Glyph(m_face, gindex, FT_LOAD_NO_BITMAP) != 0)
    {
        return -1;
    }

    // This is unused in this test but you would need this to draw
    // more than one glyph.
    FT_GlyphSlot slot = m_face->glyph;
    float bearingX = slot->metrics.horiBearingX >> 6;
    float bearingY = slot->metrics.horiBearingY >> 6;

    // 描边字体偏移量修正
    info.x = bearingX - outline_width;
    info.y = bearingY + outline_width;
    info.next_x = slot->advance.x >> 6;
    info.next_y = slot->advance.y >> 6;

    if(ch == 0x20){//空格
        return -2;
    }

    // Need an outline for this to work.
    if (m_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    {
        return -3;
    }

    // Render the basic glyph to a span list.
    // 渲染字体
    Spans spans;
    RenderSpans(&m_face->glyph->outline, &spans);

    // Next we need the spans for the outline.
    Spans outlineSpans;

    // Set up a stroker.
    // 渲染描边
    FT_Stroker stroker;
    FT_Stroker_New(m_library, &stroker);
    FT_Stroker_Set(stroker, (int)(outline_width * 64),
        FT_STROKER_LINECAP_ROUND, FT_STROKER_LINEJOIN_ROUND, 0);

    FT_Glyph glyph;
    if (FT_Get_Glyph(m_face->glyph, &glyph) == 0)
    {
        FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
        // Again, this needs to be an outline to work.
        if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
        {
            // Render the outline spans to the span list
            FT_Outline *outline = &reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
            RenderSpans(outline, &outlineSpans);
        }

        // Clean up afterwards.
        FT_Stroker_Done(stroker);
        FT_Done_Glyph(glyph);
    }

    // Now we need to put it all together.
    if (spans.empty())
    {
        return -4;
    }

    // Figure out what the bounding rect is for both the span lists.
    // 计算边框
    ft_rect rect(spans.front().x, spans.front().y, spans.front().x, spans.front().y);
    for (Spans::iterator s = spans.begin(); s != spans.end(); ++s)
    {
        rect.Include(vec2f(s->x, s->y));
        rect.Include(vec2f(s->x + s->width - 1, s->y));
    }

    for (Spans::iterator s = outlineSpans.begin(); s != outlineSpans.end(); ++s)
    {
        rect.Include(vec2f(s->x, s->y));
        rect.Include(vec2f(s->x + s->width - 1, s->y));
    }

    // Get some metrics of our image.
    int imgWidth = rect.width();
    int imgHeight = rect.height();
    int x, y;

    #if 0//32色 ================================
    // Allocate data for our image and clear it out to transparent.
    vec4ub *dst_line;
    std::vector<vec4ub> pxl;
    pxl.resize(imgWidth * imgHeight);//, vec4ub(0, 255, 128, 255));

    // Loop over the outline spans and just draw them into the image.
    //输出描边
    for(Spans::iterator s = outlineSpans.begin(); s != outlineSpans.end(); ++s)
    {
        y = (imgHeight - 1 - (s->y - rect.ymin)) * imgWidth + s->x - rect.xmin;
        for(int w = 0; w < s->width; ++w)
        {
            int x = y + w;
            pxl[x] = vec4ub(outline_color.r, outline_color.g, outline_color.b, s->coverage * outline_color.a / 255);
        }
    }

    // Then loop over the regular glyph spans and blend them into the image.
    //输出文字，颜色是RGBA，和bmp存储的不太一样
    for (Spans::iterator s = spans.begin(); s != spans.end(); ++s)
    {
        y = (imgHeight - 1 - (s->y - rect.ymin)) * imgWidth + s->x - rect.xmin;
        for (int w = 0; w < s->width; ++w)
        {
            /*
            int x = y + w;
            vec4ub &dst = pxl[x];
            vec4ub src = vec4ub(color.r, color.g, color.b, s->coverage);
            dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
            dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
            dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
            dst.a = std::min(255, dst.a + src.a);
            //*/
        }
    }

    // 创建、返回图像
    image.create(imgWidth, imgHeight, 32);
    dst_line = &pxl[0];
    for(int i=0; i<imgHeight; ++i)
    {
        void* p = image.scanline(i);
        memcpy(p, dst_line, sizeof(vec4ub) * imgWidth);// * height);
        dst_line +=  imgWidth;
    }
    #else//8位色 ================================
    // Allocate data for our image and clear it out to transparent.
    image.create(imgWidth, imgHeight, 8);
    image.clear();

    // Loop over the outline spans and just draw them into the image.
    // 输出描边
    byte_t* dest;
    for(Spans::iterator s = outlineSpans.begin(); s != outlineSpans.end(); ++s)
    {
        y = (imgHeight - 1 - (s->y - rect.ymin));
        dest = image.scanline(y);
        dest += int(s->x - rect.xmin);
        for(x = 0; x < s->width; ++x){
            dest[x] = s->coverage;//* outline_color.a / 255;
        }
    }
    #endif//================================

    return 0;
}

}//end namespace graphics
}//end namespace cgl

#endif //FONT_HPP_20150405000139
