/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 image.h

 2015-04-02 04:15:45

 设备无关的图像操作

 //png tga tiff dds都是LOWER_LEFT，bmp是UPPER_LEFT

 stadard use stb_image

 define CGL_USING_ILIMAGE to use devil

 devil 和 stb_image 这两个库默认打开的图片，行都不是4字节对齐的，直接绘制到window DC会有问题

*/
#ifndef CGL_CIMAGE_H_20150402041545
#define CGL_CIMAGE_H_20150402041545

#pragma once

#ifdef __GNUC__
    #pragma GCC system_header
    #pragma GCC diagnostic ignored "-Wunused-variable"
#endif

#include <cgl/public.h>
#include <cgl/math.hpp>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/palette.hpp>
#include <cgl/graphics/image/bmp.hpp>
#include <cgl/io/filebuf.hpp>
#include <cgl/vec.hpp>

#ifdef CGL_USING_ILIMAGE
    #include <cgl/graphics/ilimage.hpp>

#else//stb_image

    #include <cgl/zlib.hpp>
    unsigned char * costom_stbi_zlib_compress(unsigned char *data, int data_len, int *out_len, int quality);

    #define STB_IMAGE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION    //stb_image_write
    #define STB_IMAGE_WRITE_STATIC            //stb_image_write
    #define STBIW_ZLIB_COMPRESS costom_stbi_zlib_compress    //stb_image_write
    #define STBI_WRITE_NO_STDIO
    #define STBI_NO_SIMD
    //#define STBI_NO_STDIO
    //#define STBI_NO_PNG
    //#define STBI_NO_ZLIB
    #define STBI_ASSERT CGL_ASSERT
    #ifndef STBI_MALLOC
        #define STBI_MALLOC(sz)       malloc(sz)
        #define STBI_REALLOC(p,newsz) realloc(p,newsz)
        #define STBI_FREE(p)          free(p)
    #endif

    #ifdef CGL_PLATFORM_CBUILDER
        #pragma option push
        #pragma option -w-8004        //is assigned a value that is never used
        #pragma option -w-8008        //Condition is always true
        #pragma option -w-8012        //Comparing signed and unsigned values
        #pragma option -w-8066        //Unreachable code
        #pragma option -w-8062        //Previous options and warnings not restored
    #endif

    using std::ldexp;
    using std::frexp;
    using std::cos;
    using std::sin;
    using std::sqrt;
    using std::fabs;
    using std::acos;

    #include <cgl/external/stb/stb_image.h>
    #include <cgl/external/stb/stb_image_write.h>

    #ifdef CGL_PLATFORM_CBUILDER
        #pragma option pop
    #endif

    unsigned char * costom_stbi_zlib_compress(unsigned char *data, int data_len, int *out_len, int quality)
    {
        size_t size = cgl::zlib_compressBound(data_len);
        byte_t* buf = (unsigned char*)STBI_MALLOC(size);
        cgl::zlib_compress(buf, &size, data, data_len, quality);
        *out_len = size;
        return buf;
    }

    void costom_stbi_write_func(void *context, void *data, int size)
    {
        cgl::io::unifile* file = static_cast<cgl::io::unifile*>(context);
        file->write(data, size);
    }

#endif

namespace cgl{
namespace graphics{

//---------------------------------------------------------------------------

#pragma pack(push, 1)
/*
#ifndef _WINGDI_
typedef struct tagBITMAP
{
    int32_t  bmType;
    int32_t  bmWidth;
    int32_t  bmHeight;
    int32_t  bmWidthBytes;
    uint16_t bmPlanes;
    uint16_t bmBitsPixel;
    void*    bmBits;
}BITMAP;

//BMP文件 BITMAPFILEHEADER + BITMAPINFOHEADER + PALETTE + DATA
typedef struct tagBITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t  biXPelsPerMeter;
    int32_t  biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
}RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
}BITMAPINFO;

#endif//_WINGDI_
*/

typedef struct IMAGE
{
    int32_t  type;
    int32_t  width;
    int32_t  height;
    int32_t  stride;
    uint16_t format;
    uint16_t pixelbits; //1 2 4 8 16 24 32 48 64 //改成channel type
    byte_t*  data;
}IMAGE, image_state;

//graphics::bitmap
//graphics::bitmap_state

#pragma pack(pop)

//---------------------------------------------------------------------------

class pixelimage;

//计算调色板大小
inline int image_palsize(int pixelbits)
{
    return (pixelbits < 15) ? 4 * (1 << pixelbits) : 0;
}

//返回图像扫描线宽度
int image_linebytes(int width, int pixelbits, int align);

//检测图像数据行是否对齐到4
inline bool linebytes_is_aligned(int size)
{
    return (size & 3) == 0;
}

//反向扫描 BITMAP存放的数据是颠倒的
void* image_scanline(void* data, int height, int stride, int y);

//清除图像
void image_clear(void* data, size_t height, size_t linebytes);

//填充图像
void image_fill(void* data, int width, int height, int linebytes, int pixelbits, const vec4ub& color);

void image_fill(void* data, int height, int linebytes, byte_t color);

int bitblt(pixelimage& image, int x, int y, int width, int height,
    const pixelimage& source, size_t src_x, size_t src_y,
    int flag = CGL_COPY);

//返回像素格式的位宽度
int pixelformat_bits(int format)
{
    switch(format){
    case CGL_INDEX:
        return 8;
    case CGL_MONO:
        return 1;
    case CGL_RED:
    case CGL_GREEN:
    case CGL_BLUE:
    case CGL_ALPHA:
        return 8;
    case CGL_RGB:
        return 24;
    case CGL_RGBA:
        return 32;
    case CGL_LUMINANCE:
        return 8;
    case CGL_LUMINANCE_ALPHA:
        return 16;
    case CGL_ABGR:
        return 32;
    case CGL_BGR:
        return 24;
    case CGL_BGRA:
        return 32;
    case CGL_ALPHA4:
        return 4;
    case CGL_ALPHA8:
        return 8;
    case CGL_RGB565:
        return 16;
    default:
        return format;
    }
}

int pixelbits_format(size_t bits)
{
    switch(bits){
    case 1:
        return CGL_MONO;
    case 8:
        return CGL_ALPHA;
    case 16:
        return CGL_LUMINANCE_ALPHA;
    case 24:
        return CGL_RGB;
    case 32:
        return CGL_RGBA;
    default:
        return 0;
    }
}

int bm_pixelbits_format(size_t bits)
{
    switch(bits){
    case 1:
        return CGL_MONO;
    case 8:
        return CGL_INDEX;
    case 16:
        return CGL_RGB565;
    case 24:
        return CGL_BGR;
    case 32:
        return CGL_BGRA;
    default:
        return 0;
    }
}

class pixelimage
{
public:
    enum{
        pf1bit  = 1,
        pf4bit  = 4,
        pf8bit  = 8,
        pf16bit = 16,
        pf565   = 16,
        pf24bit = 24,
        pf32bit = 32
    };

private:
    enum{
        IMAGE_NULL = 0,
        IMAGE_CREATE,   //need free memory
        IMAGE_MAP,      //can not free memory
        BIND_ILIMAGE,
        BIND_STBIMAGE
    };

    intptr_t m_handle;
    IMAGE m_image;
    //pal[MAX_SIZE]    vector<vec4ub> pal
    void* m_palette;
    /*
    size_t time;//gif animation
    cimage *layers;
    cimage *mipmaps
    cimage *next;
    */

public:
    pixelimage():m_handle(), m_palette()
    {
        memset(&m_image, 0, sizeof(m_image));
    }

    ~pixelimage()
    {
        this->dispose();
    }

    //创建图像
    int create(
        size_t width,                   //图像大小
        size_t height,
        int    format,                  //像素格式
        bool   aligned = false,         //行宽度是否对齐到4
        const void* data = null,        //图像数据
        const void* palette = null);    //调色板

    int assign(const pixelimage& image);

    int open(const char* filename);
    int open(const wchar_t* filename);
    int open_memory(const byte_t* data, size_t size);

    //映射图像数据，不需要释放
    int map(int width, int height, int linebytes, int format, void* data);

    //save to bmp
    int save(const char* filename);
    int save(const wchar_t* filename);
    int save_png(const char* filename);
    int save_png(const wchar_t* filename);

    //释放图像
    void dispose();

    intptr_t handle()const{ return m_handle; }
    bool is_null()const { return m_image.type == IMAGE_NULL; }

    size_t width()const     { return m_image.width;     }
    size_t height()const    { return m_image.height;    }
    size_t pixelbits()const { return m_image.pixelbits; }
    size_t pixelbytes()const{ return m_image.pixelbits / 8; }
    int    format()const    { return m_image.format;    }
    int    linebytes()const { return m_image.stride; }
    byte_t* data() { return m_image.data; }
    const byte_t* data()const { return m_image.data; }
    void* palette() { return m_palette; }
    const void* palette()const { return m_palette; }

    //清空图像
    void clear();

    void fill(vec4ub color);
    void fill(byte_t value);
    void flip(int x, int y);

    //flip 是否反向扫描
    byte_t* scanline(int y, bool flip = false)
    {
        return flip ? data() + linebytes() * (height() - y - 1) : data() + linebytes() * y;
    }

    const byte_t* scanline(int y, bool flip = false)const
    {
        return flip ? data() + linebytes() * (height() - y - 1) : data() + linebytes() * y;
    }

    #ifdef CGL_PLATFORM_WINDOWS
    int map(BITMAP& hbmp);
    int map(HBITMAP hbmp);
    void paint(HDC dc, int x, int y, int w, int h, void* palette = null);
    void copy(HBITMAP hbmp, int x, int y, int w, int h, void* palette = null);
    HBITMAP gen_bitmap();
    #endif

    pixelimage& operator=(const pixelimage& image)
    {
        this->assign(image);
        return *this;
    }

protected:
    int save_bitmap(io::binfile& file);
    int save_png(io::binfile& file);
};

int pixelimage::create(size_t width, size_t height, int format, bool aligned, const void* data, const void* palette)
{
    this->dispose();

    //计算像素位宽度
    int pixelbits = pixelformat_bits(format);

    //计算调色板大小
    int palsize = image_palsize(pixelbits);

    //复制调色板
    if(palette){
        if(palsize){
            m_palette = malloc(palsize);
            memcpy(m_palette, palette, palsize);
        }
    }

    //创建图像
    m_image.type = IMAGE_CREATE;
    m_image.width = width;
    m_image.height = height;
    m_image.stride = image_linebytes(width, pixelbits, aligned ? 4 : 1);
    m_image.format = format;
    m_image.pixelbits = pixelbits;
    m_image.data = (byte_t*)malloc(m_image.stride * m_image.height);

    if(data){
        memcpy(m_image.data, data, m_image.height * m_image.stride);
    }

    return 0;
}

int pixelimage::assign(const pixelimage& image)
{
    bool is_aligned = image_linebytes(image.width(), image.pixelbits(), 1) < (int)image.linebytes();
    if(this->create(image.width(), image.height(), image.format(), is_aligned, image.data(), image.palette()) == CGL_ERROR){
        return CGL_ERROR;
    }
    //bitblt(...)
    return 0;
}

int pixelimage::open(const char* filename)
{
    memblock<byte_t> buf;

    if(!io::load_file(buf, filename)){
        return pixelimage::open_memory(buf.data(), buf.size());
    }
    else{
        return CGL_ERROR;
    }
}

#ifdef CGL_USING_ILIMAGE

int pixelimage::open(const wchar_t* filename)
{
    using namespace std;
    this->dispose();

    ilimage image;
    //设置图像源点，都设置成左上角起始
    //openil::ilEnable(IL_ORIGIN_SET);
    //openil::ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    if(!image.open(filename)){
        return CGL_ERROR;
    }
    this->map(image.width(), image.height(),
        image.size() / image.height(), image.format(), image.data());

    m_handle = image.release();
    m_image.type = BIND_ILIMAGE;
    m_image.format = image.format();

    return 0;
}


int pixelimage::open_memory(const byte_t* data, size_t size)
{
    this->dispose();

    ilimage image;
    //设置图像源点，都设置成左上角起始
    //ilEnable(IL_ORIGIN_SET);
    //ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    if(!image.open_memory(data, size)){
        return CGL_ERROR;
    }

    this->map(image.width(),
        image.height(),
        image.size() / image.height(),
        image.format(),
        image.data());

    m_handle = image.release();
    m_image.type = BIND_ILIMAGE;
    m_image.format = image.format();

    return 0;
}

int pixelimage::save_png(const char* filename)
{


    return -1;
}

int pixelimage::save_png(const wchar_t* filename)
{
    //if(
    ilimage image;
    image.create(width(), height(), CGL_RGBA, data());
    image.save(filename);
    image.close();
    return -1;
}

#elif defined(STBI_INCLUDE_STB_IMAGE_H)

int pixelimage::open(const wchar_t* filename)
{
    using namespace std;
    this->dispose();

    memblock<byte_t> buf;

    if(!io::load_file(buf, filename)){
        pixelimage::open_memory(buf.data(), buf.size());
    }
    else{
        return CGL_ERROR;
    }

    return 0;
}

int pixelimage::open_memory(const byte_t* data, size_t size)
{
    this->dispose();

    int w, h, channels;
    //加载图片转换成RGBA格式
    byte_t *img = stbi_load_from_memory(data, size, &w, &h, &channels, 4);
    channels = 4;
    if(!img){
        return CGL_ERROR;
    }
    this->map(w, h, w * channels, pixelbits_format(channels * 8), img);
    switch(channels)
    {
    case STBI_grey:
        m_image.format = CGL_ALPHA;
        break;
    case STBI_grey_alpha:
        m_image.format = CGL_LUMINANCE_ALPHA;
        break;
    case STBI_rgb:
        m_image.format = CGL_RGB;
        break;
    case STBI_rgb_alpha:
        m_image.format = CGL_RGBA;
        break;
    }
    m_image.type = BIND_STBIMAGE;

    return 0;
}

int pixelimage::save_png(const char* filename)
{
    stbi_flip_vertically_on_write(CGL_TRUE);
    //stbi_write_png(filename, width(), height(), pixelbits() / 8, data(), linebytes());
    io::binfile f;
    f.open(filename, io_write|io_create, sh_share);
    if(f.is_open()){
        stbi_write_png_to_func(costom_stbi_write_func, &f, width(), height(), pixelbits() / 8, data(), linebytes());
        return 0;
    }
    return CGL_ERROR;
}

int pixelimage::save_png(const wchar_t* filename)
{
    stbi_flip_vertically_on_write(CGL_TRUE);
    //stbi_write_png(filename, width(), height(), pixelbits() / 8, data(), linebytes());
    io::binfile f;
    f.open(filename, io_write|io_create, sh_share);
    if(f.is_open()){
        stbi_write_png_to_func(costom_stbi_write_func, &f, width(), height(), pixelbits() / 8, data(), linebytes());
        return 0;
    }
    return CGL_ERROR;
}

#endif//STBI_INCLUDE_STB_IMAGE_H

int pixelimage::map(int width, int height, int linebytes, int format, void* data)
{
    this->dispose();
    if(!data){
        return CGL_ERROR;
    }
    m_image.type = IMAGE_MAP;
    m_image.width = width;
    m_image.height = height;
    m_image.stride = linebytes;
    m_image.format = format;
    m_image.pixelbits = pixelformat_bits(format);
    m_image.data = (byte_t*)data;
    return 0;
}

int pixelimage::save(const char* filename)
{
    io::binfile f;
    f.open(filename, io_write|io_create, sh_share);
    if(f.is_open()){
        return this->save_bitmap(f);
    }
    return -1;
}

int pixelimage::save(const wchar_t* filename)
{
    io::binfile f;
    f.open(filename, io_write|io_create, sh_share);
    if(f.is_open()){
        return this->save_bitmap(f);
    }
    return -1;
}

int pixelimage::save_bitmap(io::binfile& f)
{
    return bmp_save(&f, this->width(), this->height(), this->pixelbits(), this->linebytes(), this->data(), this->palette());
}

void pixelimage::dispose()
{
    if(this->is_null()){
        return ;
    }

    if(m_image.type == IMAGE_CREATE){
        free(m_image.data);
    }
    #ifdef CGL_USING_ILIMAGE
    else if(m_image.type == BIND_ILIMAGE){
        ilimage image;
        image.bind(m_handle);
        image.close();
    }
    #elif defined(STBI_INCLUDE_STB_IMAGE_H)
    else if(m_image.type == BIND_STBIMAGE){
        stbi_image_free(m_image.data);
    }
    #endif

    if(m_palette){
        free(m_palette), m_palette = null;
    }
    m_handle = 0;
    memset(&m_image, 0, sizeof(m_image));
}

void pixelimage::clear()
{
    if(!this->is_null()){
        memset(data(), 0, height() * linebytes());
    }
}

void pixelimage::fill(vec4ub color)
{
    //#warning pixelimage fill
    image_fill(data(), width(), height(), linebytes(), pixelbits(), color);
}

void pixelimage::fill(byte_t value)
{
    //#warning pixelimage fill
    image_fill(data(), height(), linebytes(), value);
}

void pixelimage::flip(int x, int y)
{
    if(x){
        byte_t* p = data();
        for(size_t y=0; y<height(); ++y){
            memswap(p, width(), pixelbytes());
            p += linebytes();
        }
    }
    if(y){
        memswap(data(), height(), linebytes());
    }
}

//---------------------------------------------------------------------------
#ifdef CGL_PLATFORM_WINDOWS

int pixelimage::map(BITMAP& bmp)
{
    return this->map(bmp.bmWidth, bmp.bmHeight,
        bmp.bmWidthBytes, bm_pixelbits_format(bmp.bmBitsPixel), static_cast<byte_t*>(bmp.bmBits));
}

int pixelimage::map(HBITMAP hbmp)
{
    if(!hbmp){
        return CGL_ERROR;
    }
    BITMAP bmp;
    GetObject(hbmp, sizeof(BITMAP), &bmp);
    if(this->map(bmp) == 0){
        m_handle = intptr_t(hbmp);
        return 0;
    }
    return CGL_ERROR;
}

void pixelimage::paint(HDC dc, int x, int y, int w, int h, void* palette)
{
    #pragma pack(push, 1)
    struct{//BMP图像头+256调色板
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD pal[256];
    }bminfo;
    #pragma pack(pop)

    memset(&bminfo, 0, sizeof(bminfo));

    int palsize = 0;
    if(pixelbits() <= 8){
        palsize = 1 << pixelbits();
        if(palette){
            memcpy(bminfo.pal, palette, palsize * sizeof(RGBQUAD));
        }
    }

    bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bminfo.bmiHeader.biWidth = width();
    bminfo.bmiHeader.biHeight = height();
    bminfo.bmiHeader.biPlanes = 1;
    bminfo.bmiHeader.biBitCount = WORD(pixelbits());
    bminfo.bmiHeader.biCompression = BI_RGB;
    bminfo.bmiHeader.biSizeImage = linebytes() * height();
    bminfo.bmiHeader.biClrUsed = palsize;
    bminfo.bmiHeader.biClrImportant = 0;

    if(width() < size_t(w))w = width();
    if(height() < size_t(h))h = height();

    if(linebytes_is_aligned(linebytes())){//行4字节
        SetDIBitsToDevice(dc, x, y, w, h,
            0, 0, 0,
            height(),
            data(), (BITMAPINFO*)&bminfo, DIB_RGB_COLORS);
    }
    else{//行未对齐
        pixelimage image;
        image.create(width(), height(), format(), true);

        for(size_t i=0; i<height(); ++i){
            memcpy(image.scanline(i), scanline(i), linebytes());
        }

        bminfo.bmiHeader.biSizeImage = image.linebytes() * height();

        SetDIBitsToDevice(dc, x, y, w, h,
            0, 0, 0,
            image.height(),
            image.data(), (BITMAPINFO*)&bminfo, DIB_RGB_COLORS);
    }
}

void pixelimage::copy(HBITMAP hbmp, int x, int y, int w, int h, void* palette)
{
    BITMAP bm;
    GetObject(hbmp, sizeof(bm), &bm);
    if(size_t(bm.bmBitsPixel) != this->pixelbits()){
        return ;
    }

    byte_t* p = (byte_t*)bm.bmBits;
    byte_t* src;
    size_t offset;
    if(this->pixelbits() == pf32bit){
        for(size_t i=0; i<height(); ++i){
            src = (byte_t*)this->scanline(i, true);
            offset = 0;
            for(size_t j=0; j<width(); ++j){
                p[offset+0] = src[offset+2];
                p[offset+1] = src[offset+1];
                p[offset+2] = src[offset+0];
                p[offset+3] = src[offset+3];
                offset += 4;
            }
            p += bm.bmWidthBytes;
        }
    }
    else if(this->pixelbits() == pf24bit){
        for(size_t i=0; i<height(); ++i){
            src = (byte_t*)this->scanline(i, true);
            offset = 0;
            for(size_t j=0; j<width(); ++j){
                //memcpy(p, scanline(i), linebytes());
                p[offset+0] = src[offset+2];
                p[offset+1] = src[offset+1];
                p[offset+2] = src[offset+0];
                offset += 3;
            }
            p += bm.bmWidthBytes;
        }
    }
    else{
        for(size_t i=0; i<height(); ++i){
            src = (byte_t*)this->scanline(i, true);
            memcpy(p, src, linebytes());
            p += bm.bmWidthBytes;
        }
    }
}

HBITMAP pixelimage::gen_bitmap()
{
    return 0;
}

#endif//CGL_PLATFORM_WINDOWS

//---------------------------------------------------------------------------
//一些通用的图像处理函数

int image_linebytes(int width, int pixelbits, int align)
{
    int linebytes;
    //计算行宽
    if(pixelbits < 8){
        int pixels = 8 / pixelbits;
        linebytes = (width + pixels - 1) / pixels;
    }
    else{
        linebytes = width * (pixelbits / 8);
    }
    //字节对齐
    switch(align)
    {
    case 2:
        if(linebytes & 1){
            ++linebytes;
        }
        break;
    case 4:
        linebytes += 3;
        linebytes &= ~3;
        break;
    default:
        break;
    }
    return linebytes;
}

void* image_scanline(void* data, int height, int stride, int y)
{
    if(stride < 0){//反向扫描
        return ((byte_t*)data) - stride * (height - y - 1);
    }
    else{
        return ((byte_t*)data) + stride * y;
    }
}

void image_clear(void* data, size_t height, size_t linebytes)
{
    memset(data, 0, height * linebytes);
}

//void image_fill_RGB555

//pf8bits
void image_fill(void* data, int height, int linebytes, byte_t color)
{
    memset(data, color, height * linebytes);
}

void image_fill(void* data, int width, int height, int linebytes, int pixelbits, const vec4ub& color)
{
    if(!data){
        return ;
    }

    byte_t* source = static_cast<byte_t*>(data);

    switch(pixelbits){
    case 1:
    case 4:
    case 8:
        break;
    case 15:
    case 16:{
        uint16_t c = (pixelbits == 15 ? RGBA_to_RGB555(color.r, color.g, color.b) : RGBA_to_RGB565(color.r, color.g, color.b));
        uint16_t* dest;
        for(int y=0; y<height; ++y){
            dest = (uint16_t*)(source);
            std::fill(dest, dest + width, c);
            source += linebytes;
        }
        break;
    }
    case 24:{
        vec3ub c(color.red, color.green, color.blue);
        vec3ub* dest;
        for(int y=0; y<height; ++y){
            dest = (vec3ub*)(source);
            std::fill(dest, dest + width, c);
            source += linebytes;
        }
        break;
    }
    case 32:{
        uint32_t c = RGBA(color.red, color.green, color.blue, color.alpha);
        uint32_t* dest;
        for(int y=0; y<height; ++y){
            dest = (uint32_t*)(source);
            std::fill(dest, dest + width, c);
            source += linebytes;
        }
        break;
    }
    default:
        break;
    }
}

//图像上下颠倒
inline int image_flipY(byte_t* data, int height, int linebytes)
{
    memswap(data, data + height * linebytes, linebytes);
    return 0;
}

int bitblt(pixelimage& image, int x, int y, int width, int height,
    const pixelimage& source, size_t src_x, size_t src_y,
    int flag)
{
    if(image.is_null()){
        CGL_LOG("image is null.");
        return -1;
    }

    if(image.pixelbits() != source.pixelbits()){
        CGL_LOG("pixelformat diffrent.");
        return -2;
    }

    if(source.pixelbits() < 8){
        CGL_LOG("pixelformat less 8bits.");
        return -3;
    }

    if(x < 0){
        src_x += -x;
        width += x;
        if(width <= 0){
            return -4;
        }
        x = 0;
    }

    if(src_x >= source.width()){
        return -4;
    }
    if(x + width >= (int)image.width()){
        width = image.width() - x;
    }

    if(y < 0){
        src_y += -y;
        height += y;
        if(height <= 0){
            return -5;
        }
        y = 0;
    }

    if(src_y >= source.height()){
        return -5;
    }
    if(y + height >= (int)image.height()){
        height = image.height() - y;
    }

    width = std::min(width, int(source.width() - src_x));
    height = std::min(height, int(source.height() - src_y));
    int pixelbytes = source.pixelbits() / 8;

    byte_t* pdst = image.scanline(y);
    const byte_t* psrc = source.scanline(src_y);
    pdst += pixelbytes * x;
    psrc += pixelbytes * src_x;
    width *= pixelbytes;//通道数量
    switch(flag)
    {
    case CGL_ADD:
        for(int i=0; i<height; ++i){
            for(int j=0; j<width; ++j){
                pdst[j] += psrc[j];
            }
            pdst += image.linebytes();
            psrc += source.linebytes();
        }
        break;
    case CGL_SUB:
        for(int i=0; i<height; ++i){
            for(int j=0; j<width; ++j){
                pdst[j] -= psrc[j];
            }
            pdst += image.linebytes();
            psrc += source.linebytes();
        }
        break;
    case CGL_AND:
        for(int i=0; i<height; ++i){
            for(int j=0; j<width; ++j){
                pdst[j] &= psrc[j];
            }
            pdst += image.linebytes();
            psrc += source.linebytes();
        }
        break;
    case CGL_COPY:
        for(int i=0; i<height; ++i){
            memcpy(pdst, psrc, width);
            pdst += image.linebytes();
            psrc += source.linebytes();
        }
        break;
    case CGL_OR:
        for(int i=0; i<height; ++i){
            for(int j=0; j<width; ++j){
                pdst[j] |= psrc[j];
            }
            pdst += image.linebytes();
            psrc += source.linebytes();
        }
        break;
    case CGL_INVERT:
        for(int i=0; i<height; ++i){
            for(int j=0; j<width; ++j){
                pdst[j] = ~pdst[j];
            }
            pdst += image.linebytes();
        }
        break;
    }
    return 0;
}


int image_expand(pixelimage& image, const pixelimage& source, int size)
{
    image.create(source.width() + size * 2, source.height() + size * 2, source.format(), false, source.palette());

    //center
    bitblt(image, size, size, source.width(), source.height(), source, 0, 0, CGL_COPY);

    //top
    bitblt(image, size, 0, source.width(), size, source, 0, 0, CGL_COPY);
    //bottom
    bitblt(image, size, source.height() + size, source.width(), size, source, 0, source.height() - size, CGL_COPY);
    //left
    bitblt(image, 0, size, size, source.height(), source, 0, 0, CGL_COPY);
    //bottom
    bitblt(image, source.width() + size, size, size, source.height(), source, source.width() - size, 0, CGL_COPY);

    bitblt(image, 0, 0, size, size, source, 0, 0, CGL_COPY);
    bitblt(image, source.width() + size, 0, size, size, source, source.width() - size, 0, CGL_COPY);
    bitblt(image, 0, source.height() + size, size, size, source, 0, source.height() - size, CGL_COPY);
    bitblt(image, source.width() + size, source.height() + size, size, size, source, source.width() - size, source.height() - size, CGL_COPY);

    return 0;
}

void im8_to_im32(pixelimage& dest, const pixelimage& source)
{
    dest.create(source.width(), source.height(), CGL_RGBA);
    uint32_t* pdst;
    const byte_t* psrc;
    for(size_t y=0; y<source.height(); ++y){
        pdst = (uint32_t*)dest.scanline(y);
        psrc = source.scanline(y);
        for(size_t x=0; x<source.width(); ++x){
            pdst[x] = RGBA(255, 255, 255, psrc[x]);
        }
    }
}

}//end namespace graphics
}//end namespace cgl

#endif //CGL_CIMAGE_H_20150402041545
