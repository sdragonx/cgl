/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 glcontext.hpp

 2018-05-10 21:28:34

*/
#ifndef GLCONTEXT_HPP_20180510212834
#define GLCONTEXT_HPP_20180510212834

#include <cgl/public.h>

#if defined(CGL_PLATFORM_GLES)
    #include <cgl/graphics/gl/context_gles2.hpp>

    #define glPolygonMode(a,b)
#elif defined(CGL_PLATFORM_WINDOWS)
    #include <cgl/graphics/gl/context_win32.hpp>
#else
    #error cgl not suppert platform.
#endif

#include <cgl/graphics/draw_context.hpp>

#include <cgl/graphics/gl/glcamera.hpp>
#include <cgl/graphics/gl/glfont_manager.hpp>
#include <cgl/graphics/gl/gltexture.hpp>
#include <cgl/graphics/gl/glmultitexture.hpp>
#include <cgl/graphics/gl/gllight.hpp>

#include <cgl/graphics/fps.hpp>
#include <cgl/graphics/gl/glu.hpp>

namespace cgl{
namespace graphics{
namespace gl{

#pragma pack(push, 1)
struct glcontext_state
{
    int max_image_size;

    int non_pow_of_two:1;
    int vbo:1;
    int fbo:1;
    int multisample:1;

    glcontext_state()
    {
    }
};

#pragma pack(pop)

}//end namespace gl

class glcontext;

//vec4f unProject(int x, int y);

class glcontext : public native_glcontext
{
private:
    //vec4i m_viewport;
public:
    gl::glcontext_state const state;
    gltextures            textures;
    gl::glmultitexture  texunits;
    gl::glfont_manager  fonts;
    gllights        lights;

//    vec4ub color;
public:
    glcontext();
    ~glcontext();

    int create(platform::display display, platform::window window);
    int init_state();
    void dispose();

    //复制图像内存
    void copy_screen(int x, int y, int width, int height, int format, void *data);
    int save_screen(const char* filename);
    int save_screen(const wchar_t* filename);
//---------------------------------------------------------------------------
public://opengl api
    bool IsExtensionSupported(const char *extension);

    void viewport(int left, int top, int width, int height);
    vec4i viewport()const;

    void fill_mode(bool front, bool back);
    void depth_test(bool);
    void dither(bool);
    void cull_face(bool);
    void blendfunc(int source, int dest);
    void cliprect(int x, int y, int width, int height);

    //int use_program(int program);
public:
    void clear(float red, float green, float blue, float alpha = 1.0f);
    void push_view2D(int left, int top, int width, int height);
    void push_view3D(double width, double height, double fovy = 60.0, double zNear = 0.001, double zFar = 6400.0);
    void pop_view();

    void pen_width(float width);

    int draw_line(float x1, float y1, float x2, float y2);
    int draw_line(float x1, float y1, float z1, float x2, float y2, float z2);

    int draw_line(const vec2f& p1, const vec2f& p2);
    int draw_line(const vec3f& p1, const vec3f& p2);
    int draw_line(const vec4f& p1, const vec4f& p2);

    int draw_rect(float x1, float y1, float x2, float y2);
    int fill_rect(float x1, float y1, float x2, float y2);

    int draw_image3d(intptr_t image,
        float x1, float y1, float z1,
        float x2, float y2, float z2,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

    int draw_image(intptr_t image, float x, float y, float width, float height,
        float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);

    void draw_image(const teximage& image, float x, float y, float width, float height);

    int draw(const draw_context& dc);

    int draw_text(int left, int top, int width, int height, const unichar_t* text, size_t length, int style);

protected:
    int vprint(float x, float y, const unichar_t* text, size_t length = TEXT_MAX);

    void perspective(float fovy, float aspect, float zNear, float zFar);

public://texture
    int create_texture(int width, int height, int format,
        int source_format, int source_type, const byte_t* pixels = null,
        int mipmaps = 0);
    int load_texture(const wchar_t* filename, int format, int mipmaps = 0);
    int default_texture();
    void delete_texture(int tex);
    //void bind_texture(int tex);//native_glcontext

public://font
    unifont* load_font(const font_t& font);

    int char_width(unichar_t ch)
    {
        unifont* f = fonts.load(font);
        return f->char_width(ch);
    }
    int char_height()
    {
        unifont* f = fonts.load(font);
        return f->char_height();
    }
    int text_width(const unichar_t* text, size_t length)
    {
        unifont* f = fonts.load(font);
        return f->text_width(text, length);
    }

private:
    void get_state();
};

glcontext::glcontext() : native_glcontext(),
    state(),
    textures(),
    texunits(),
    fonts(this)
    #ifndef CGL_PLATFORM_GLES
    ,lights()
    #endif
{

}

glcontext::~glcontext()
{
    this->dispose();
}

int glcontext::create(platform::display display, platform::window window)
{
    if(!this->ready()){
        if(native_glcontext::create(display, window) == CGL_ERROR){
            return CGL_ERROR;
        }
    }

    return init_state();
}

int glcontext::init_state()
{
    get_state();

    //设置默认状态

    //启用纹理映射
    glEnable(GL_TEXTURE_2D);

    //打开混合
    glEnable(GL_BLEND);
    //alpha混合函数
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //启用剪裁测试
    glEnable(GL_SCISSOR_TEST);

    //优化透视
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    #ifndef CGL_PLATFORM_GLES
    //平滑过滤 GL_FLAT GL_SMOOTH
    //glShadeModel(GL_SMOOTH);

    //反锯齿 GL_FASTEST GL_NICEST
    //glEnable(GL_POINT_SMOOTH);
    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    //绘制的图形会有虚线
    //glEnable(GL_POLYGON_SMOOTH);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    #endif

    //启用alpha测试
    //glEnable(GL_ALPHA_TEST);

    //深度测试
    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);//设置深度测试函数
    glDepthFunc(GL_LEQUAL);//default
    glClearDepth(1.0);//设置清除深度缓冲区所用的值

    #ifndef CGL_PLATFORM_GLES
    //设置多边形显示模式
    //GL_POINT GL_LINE GL_FILL
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_LINE);
    #endif

    /*
    //用OpenGL实现支持的最大各异向程度设置最大各异向程度参数
    glTexParameterf(GL_TEXTURE_2D, 0x84FE, 16);
    // 注意：这里的 xxx 代表 各向异性过滤 的异向程度参数
    // 你可以设置为：4 , 8 , 16 等。这里也可以在 InitGL() 驱动 OpenGL 函数里写上
    //int LrgSupAni;
    GL_TEXTURE_MAX_ANISOTROPY
    GL_MAX_TEXTURE_MAX_ANISOTROPY
    //glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &LrgSupAni);
    //那么 xxx 就得小于或等于 LrgSupAni。
    */

    //glGetIntegerv(GL_VIEWPORT, m_viewport.data);

    return 0;
}

void glcontext::dispose()
{
    //free fonts
    fonts.dispose();

    //free textures
    textures.clear();

    //free context
    native_glcontext::dispose();
}

bool glcontext::IsExtensionSupported(const char *extension)
{
    const char *extensions;
    const char *start;
    const char *where, *terminator;

    /* Extension names should not have spaces. */
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return false;

    /* It takes a bit of care to be fool-proof about parsing the
    OpenGL extensions string. Don't be fooled by sub-strings,
    etc. */
    extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));

    if(!extensions){
        return false;
    }

    start = extensions;
    for (;;)
    {
        where = strstr(start, extension );
        if ( !where ) break;

        terminator = where + strlen(extension);
        if ( (where == start || *(where - 1) == ' ') && (*terminator == ' ' || *terminator == '\0') )
            return true;
        start = terminator;
    }

    return false;
}

// 查询、输出当前设备信息
void glcontext::get_state()
{
    //GL_ARB_draw_buffers
    //GL_ARB_pixel_buffer_object
    int m = 0;
    int n = 0;

    CGL_LOG("GL_RENDERER: %s", glGetString(GL_RENDERER));
    CGL_LOG("GL_VERSION: %s", glGetString(GL_VERSION));


    #ifdef CGL_PLATFORM_GLES
    CGL_LOG("EGL version : %i.%i", m_egl_version.major, m_egl_version.minor);

    if(m_egl_version.minor >= 5){
        eglQueryContext(m_display, m_context, EGL_CONTEXT_MAJOR_VERSION, &m);
        eglQueryContext(m_display, m_context, EGL_CONTEXT_MINOR_VERSION, &n);
        CGL_LOG("OpenGL ES version : %i.%i", m, n);
    }
    else{
        eglQueryContext(m_display, m_context, EGL_CONTEXT_CLIENT_VERSION, &m);
        CGL_LOG("OpenGL ES version : %i.%i", m, 0);
    }

    #else
    glGetIntegerv(GL_MAJOR_VERSION, &m);
    glGetIntegerv(GL_MINOR_VERSION, &n);
    CGL_LOG("OpenGL version : %i.%i", m, n);
    #endif

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &n);
    CGL_LOG("max texture size : %i", n);
    const_write(state).max_image_size = n;
    const_write(state).non_pow_of_two = IsExtensionSupported("GL_ARB_texture_non_power_of_two");
    const_write(state).vbo = IsExtensionSupported("GL_ARB_vertex_buffer_object");
    const_write(state).fbo = IsExtensionSupported("GL_ARB_framebuffer_object") || IsExtensionSupported("GL_EXT_framebuffer_object");
    const_write(state).multisample = IsExtensionSupported("GL_ARB_multisample");

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &n);
    CGL_LOG("max texture units : %i", n);


//    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &n);
//    CGL_LOG("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS : %i", n);

//    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &n);
//    CGL_LOG("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS : %i", n);

    glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &n);
    CGL_LOG("GL_MAX_ARRAY_TEXTURE_LAYERS : %i", n);

    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &n);
    CGL_LOG("GL_MAX_3D_TEXTURE_SIZE : %i", n);

    #ifndef CGL_PLATFORM_GLES
    glGetIntegerv(GL_MAX_SAMPLES, &n);
    CGL_LOG("max samples : %i", n);
    #endif
}

void glcontext::viewport(int left, int top, int width, int height)
{
    glViewport(left, top, width, height);
    glScissor (left, top, width, height);
    /*
    m_viewport.x = left;
    m_viewport.y = top;
    m_viewport.width = width;
    m_viewport.height = height;
    */
}

vec4i glcontext::viewport()const
{
    vec4i rect;
    glGetIntegerv(GL_VIEWPORT, rect.data);
    return rect;
}

void glcontext::fill_mode(bool front, bool back)
{
    #ifndef CGL_PLATFORM_GLES
    if(front){
        glPolygonMode(GL_FRONT, GL_FILL);
    }
    else{
        glPolygonMode(GL_FRONT, GL_LINE);
    }

    if(back){
        glPolygonMode(GL_BACK, GL_FILL);
    }
    else{
        glPolygonMode(GL_BACK, GL_LINE);
    }

    #endif
}

void glcontext::depth_test(bool value)
{
    if(value){
        glEnable(GL_DEPTH_TEST);
    }
    else{
        glDisable(GL_DEPTH_TEST);
    }
}

//激活抖动
void glcontext::dither(bool enable)
{
    if(enable){
        glEnable(GL_DITHER);
    }
    else{
        glDisable(GL_DITHER);
    }
}

//设置舍弃面
void glcontext::cull_face(bool enable)
{
    if(enable){
        glEnable(GL_CULL_FACE);
    }
    else{
        glDisable(GL_CULL_FACE);
    }
    //glCullFace(GL_BACK);
}

void glcontext::blendfunc(int source, int dest)
{
    glBlendFunc(source, dest);
}

void glcontext::cliprect(int x, int y, int width, int height)
{
    glScissor(x, y, width, height);
}

//---------------------------------------------------------------------------
//
// drawing
//
//---------------------------------------------------------------------------

void glcontext::clear(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}

//void glColor3f(float red, float green, float blue, float alpha = 1.0f);
void glcontext::push_view2D(int left, int top, int width, int height)
{
    this->matrix_mode(GL_PROJECTION);
    this->push_matrix();
    this->identity();

#if CGL_COORDINATE == CGL_LOWER_LEFT
    //直角坐标系
    this->ortho(left, width, top, height, 0, INT16_MAX);
    //重新设置正面，默认GL_CCW
    glFrontFace(GL_CCW);
#else
    //windows坐标系
    //减一是错误的，绘图会有失误。
    //this->ortho(left, left+width-1, top+height-1, top, INT16_MIN, INT16_MAX);
    this->ortho(left, left+width, top+height, top, INT16_MIN, INT16_MAX);
    glFrontFace(GL_CW);
#endif

    //十字坐标系
    //glOrtho(-width/2, width/2, -height/2, height/2, 0, INT_MAX);//

    //反转屏幕
    //glScalef(1.0f, -1.0f, 1.0f);
    //glTranslatef(0.0f, -height, 0.0f);

    this->matrix_mode(GL_MODELVIEW);
    this->identity();
    this->translate(0.375f, 0.375f, 0.0f);//GL_POINTS and GL_LINES don't touch the right pixels
    glDisable(GL_DEPTH_TEST);//关闭深度测试
    glDisable(GL_CULL_FACE); //关闭面剔除
}

//
// 4:3  fovy = 60
// 16:9 fovy = 80
// 视角设置过大，图像会严重失真
//
void glcontext::perspective(float fovy, float aspectRatio, float zNear, float zFar)
{
    using namespace std;
    float fovyDelta = fovy * M_RD * 0.5;
    const float frustumHeight = tan(fovyDelta) * zNear;
    const float frustumWidth = frustumHeight * aspectRatio;

    this->frustum(-frustumWidth, frustumWidth, -frustumHeight, frustumHeight, zNear, zFar);
}

void glcontext::push_view3D(double width, double height, double fovy, double zNear, double zFar)
{
    //glPushAttrib(GL_ALL_ATTRIB_BITS);
    this->matrix_mode(GL_PROJECTION);
    this->push_matrix();
    this->identity();

    if(fovy < 0.0f){
        fovy = 60;// math::view_angle(width, height);
    }
    this->perspective(fovy, width / height, zNear, zFar);

    this->matrix_mode(GL_MODELVIEW);
    this->identity();

    glEnable(GL_DEPTH_TEST);//启用深度测试
    glEnable(GL_CULL_FACE); //启用面剔除

    glFrontFace(GL_CCW);
}

void glcontext::pop_view()
{
    this->matrix_mode(GL_PROJECTION);
    this->pop_matrix();
    this->matrix_mode(GL_MODELVIEW);
    //glPopAttrib();
}

void glcontext::pen_width(float width)
{
    #ifndef CGL_PLATFORM_GLES
    glPointSize(width);
    #endif
    glLineWidth(width);
}

int glcontext::draw_line(float x1, float y1, float x2, float y2)
{
    vtx3f vtx[] = {
        vtx3f(x1, y1, 0.0f, 0.0f, 0.0f, color),
        vtx3f(x2, y2, 0.0f, 1.0f, 1.0f, color)
    };

    return this->draw_arrays(GL_LINES, vtx, 0, 4);
}

int glcontext::draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
{
    vtx3f vtx[] = {
        vtx3f(x1, y1, z1, 0.0f, 0.0f, color),
        vtx3f(x2, y2, z2, 1.0f, 1.0f, color)
    };

    return this->draw_arrays(GL_LINES, vtx, 0, 4);
}

int glcontext::draw_line(const vec2f& p1, const vec2f& p2)
{
    return this->draw_line(p1.x, p1.y, p2.x, p2.y);
}

int glcontext::draw_line(const vec3f& p1, const vec3f& p2)
{
    return this->draw_line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

int glcontext::draw_line(const vec4f& p1, const vec4f& p2)
{
    return this->draw_line(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

int glcontext::draw_rect(float x1, float y1, float x2, float y2)
{
    vtx3f vtx[] = {
        vtx3f(x1, y1, 0.0f, 0.0f, 0.0f, color),
        vtx3f(x2, y1, 0.0f, 1.0f, 0.0f, color),
        vtx3f(x2, y2, 0.0f, 1.0f, 1.0f, color),
        vtx3f(x1, y2, 0.0f, 0.0f, 1.0f, color)
    };

    this->bind_texture(0);
    return this->draw_arrays(GL_LINE_LOOP, vtx, 0, 4);
}

int glcontext::fill_rect(float x1, float y1, float x2, float y2)
{
    vtx3f vtx[] = {
        vtx3f(x1, y1, 0.0f, 0.0f, 0.0f, color),
        vtx3f(x2, y1, 0.0f, 1.0f, 0.0f, color),
        vtx3f(x2, y2, 0.0f, 1.0f, 1.0f, color),
        vtx3f(x1, y2, 0.0f, 0.0f, 1.0f, color)
    };

    this->bind_texture(0);
    return this->draw_arrays(CGL_QUADS, vtx, 0, 4);
}

int glcontext::draw_image3d(intptr_t image,
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float u1, float v1, float u2, float v2)
{
    vtx3f vtx[] = {
        #if CGL_COORDINATE == CGL_LOWER_LEFT
        vtx3f(x1, y1, z1, u1, v2, color),
        vtx3f(x2, y1, z1, u2, v2, color),
        vtx3f(x2, y2, z2, u2, v1, color),
        vtx3f(x1, y2, z2, u1, v1, color)
        #else
        vtx3f(x1, y1, z1, u1, v1, color),
        vtx3f(x2, y1, z1, u2, v1, color),
        vtx3f(x2, y2, z2, u2, v2, color),
        vtx3f(x1, y2, z2, u1, v2, color)
        #endif
    };
    this->bind_texture(image);
    return this->draw_arrays(CGL_TRIANGLE_FAN, vtx, 0, 4);
}

int glcontext::draw_image(intptr_t image, float x, float y, float width, float height,
    float u1, float v1, float u2, float v2)
{
    /*
    vtx3f vtx[] = {
        #if CGL_COORDINATE == CGL_LOWER_LEFT
        vtx3f(x,         y,          0.0f, u1, v2, color),
        vtx3f(x + width, y,          0.0f, u2, v2, color),
        vtx3f(x + width, y + height, 0.0f, u2, v1, color),
        vtx3f(x        , y + height, 0.0f, u1, v1, color)
        #else
        vtx3f(x,         y,          0.0f, u1, v1, color),
        vtx3f(x + width, y,          0.0f, u2, v1, color),
        vtx3f(x + width, y + height, 0.0f, u2, v2, color),
        vtx3f(x,         y + height, 0.0f, u1, v2, color)
        #endif
    };
    this->bind_texture(image);
    return this->draw_arrays(CGL_QUADS, vtx, 0, 4);
    */
    return this->draw_image3d(image, x, y, 0.0f, x + width, y + height, 0.0f, u1, v1, u2, v2);
}

void glcontext::draw_image(const teximage& image, float x, float y, float width, float height)
{
    this->draw_image(image.id, x, y, width, height, image.u1, image.v1, image.u2, image.v2);
}

int glcontext::vprint(float x, float y, const unichar_t* text, size_t length)
{
    glfont* font = fonts.load(cstring<char>(), 16, 256);
    return font->tab_print(this, x, y, text, length);
}

int glcontext::draw_text(int left, int top, int width, int height, const unichar_t* text, size_t length, int style)
{
    glfont* font = fonts.load(cstring<char>(), 16, 256);
    return font->draw_text(this, left, top, width, height, text, length, style);
}

//---------------------------------------------------------------------------
//
// texture
//
//---------------------------------------------------------------------------

int glcontext::create_texture(int width, int height, int format,
        int source_format, int source_type, const byte_t* pixels,
        int mipmaps)
{
    return this->textures.create(width, height, format, source_format, source_type, pixels, mipmaps);
}

int glcontext::load_texture(const wchar_t* filename, int format, int mipmaps)
{
    return this->textures.open(filename, format, mipmaps);
}

int glcontext::default_texture()
{
    return this->textures.defimage();
}

void glcontext::delete_texture(int tex)
{
    this->textures.close(tex);
}

//
// save screen to png image
//

void glcontext::copy_screen(int x, int y, int width, int height, int format, void *data)
{
    glReadPixels(x, y, width, height, format, GL_UNSIGNED_BYTE, data);
}

int glcontext::save_screen(const char* filename)
{
    pixelimage image;
    vec4i rect = this->viewport();
    image.create(rect.width, rect.height, 24, false);
    this->copy_screen(0, 0, rect.width, rect.height, CGL_RGB, image.data());
    image.save_png(filename);
    image.dispose();
    return 0;
}

int glcontext::save_screen(const wchar_t* filename)
{
    pixelimage image;
    vec4i rect = this->viewport();
    image.create(rect.width, rect.height, 24, false);
    this->copy_screen(0, 0, rect.width, rect.height, CGL_RGB, image.data());
    image.save_png(filename);
    image.dispose();
    return 0;
}

//---------------------------------------------------------------------------
//
// font
//
//---------------------------------------------------------------------------

unifont* glcontext::load_font(const font_t& font)
{
    return this->fonts.load(font);
}

//---------------------------------------------------------------------------
//
// draw_context
//
//---------------------------------------------------------------------------

int glcontext::draw(const draw_context& dc)
{
    if(dc.empty()){
        return -1;
    }

    //保存现场
    //this->push_attrib();

    //初始化状态
    #ifndef CGL_PLATFORM_GLES
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPointSize(1.0f);
    #endif
    glLineWidth(1.0f);
    glBlendFunc(CGL_SRC_ALPHA, CGL_ONE_MINUS_SRC_ALPHA);
    //直角坐标系
    int viewport[4];
    //glGetIntegerv(GL_VIEWPORT, viewport);
    //glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
    glScissor(0, 0, INT16_MAX, INT16_MAX);

    for(size_t i=0; i<dc.size(); ++i)
    {
        const drawcall& c = dc.commands(i);
        switch(c.command)
        {
        case CGL_POINTS:
        case CGL_LINES:
        case CGL_LINE_LOOP:
        case CGL_LINE_STRIP:
        case CGL_TRIANGLE_STRIP:
        case CGL_TRIANGLES:
        case CGL_TRIANGLE_FAN:
        {
            //glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
            //if(c.object)
            //else
            this->draw_arrays(c.command, dc.vertex_pointer(), c.offset, c.size);
            //glPopClientAttrib();
            break;
        }

        case CGL_QUADS:
            this->draw_elements(GL_TRIANGLES, dc.vertex_pointer(), dc.index_pointer() + c.offset, c.size);
            break;

        case CGL_CLEAR:
            this->clear(c.red/255.0f, c.green/255.0f, c.blue/255.0f, c.alpha/255.0f);
            break;
        case CGL_COLOR:
            //glColor4f(itr->red, itr->green, itr->blue, itr->alpha);
            //color =
            break;
        case CGL_FLUSH:
            //glFlush();
            break;
        case CGL_DRAW_CONTEXT:
            //reinterpret_cast<draw_context*>(itr->object)->paint();
            break;

        case CGL_DRAWARRAYS:
        {
            vtx3f* va = (vtx3f*)c.object;
            if(va)
            this->draw_arrays(CGL_TRIANGLES, va, 0, 48);
            break;
        }

        case CGL_TEXTURE_1D:
            //glBindTexture(GL_TEXTURE_1D, c.value);
            break;
        case CGL_TEXTURE_2D:
            this->bind_texture(c.value);
            break;
        case CGL_TEXTURE_3D:
            //glBindTexture(GL_TEXTURE_3D, itr->value);
            break;

        case CGL_IDENTITY:
            this->identity();
            break;
        case CGL_PUSH_MATRIX:
            this->push_matrix();
            break;
        case CGL_POP_MATRIX:
            this->pop_matrix();
            break;
        case CGL_ROTATE:
            this->rotate(c.angle, c.x, c.y, c.z);
            break;
        case CGL_TRANSLATE:
            this->translate(c.x, c.y, c.z);
            break;
        case CGL_SCALE:
            this->scale(c.x, c.y, c.z);
            break;

        case CGL_BLEND:
            this->blendfunc(c.blend_src, c.blend_dst);
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
        /*
        case CGL_POINT_SIZE:
            glPointSize(c.value);
            break;
        case CGL_LINE_WIDTH:
        */
        case CGL_PEN_WIDTH:
            this->pen_width(c.width);
            break;
        default:
            break;
        }
    }

    //this->pop_attrib();
    return 0;
}

}//end namespace graphics
}//end namespace cgl

#endif //GLCONTEXT_HPP_20180510212834
