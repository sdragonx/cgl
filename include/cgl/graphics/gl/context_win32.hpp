/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 context_win32.hpp

 2018-05-09 08:23:03

*/
#ifndef CONTEXT_WIN32_HPP_20180509082303
#define CONTEXT_WIN32_HPP_20180509082303

#include "gl.hpp"

#if defined(__gl2_h_) || defined(__glew_h__) || defined(__glext_h_) || defined(__glad_h_)

#include <cgl/graphics/gl/program.hpp>
#include <cgl/graphics/gl/glmultisample.hpp>

#endif

namespace cgl{
namespace graphics{


void vtx_begin(const vtx3f* vtx)
{
    glVertexPointer(3, GL_FLOAT, sizeof(vtx3f), vtx);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx3f), &vtx->u);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx3f), vtx->color.data);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
}

void vtx_end(const vtx3f* vtx)
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void vtx_begin(const vtx3fx* vtx)
{
    glVertexPointer(3, GL_FLOAT, sizeof(vtx3fx), vtx);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vtx3fx), &vtx->u);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(vtx3fx), vtx->color.data);
    glNormalPointer(GL_FLOAT, sizeof(vtx3fx), &vtx[0].normal.data);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
}

void vtx_end(const vtx3fx* vtx)
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void draw_arrays(int mode, const vec2f* vs, size_t size)
{
    glVertexPointer(2, GL_FLOAT, sizeof(vec2f), vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawArrays(mode, 0, size);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void draw_elements(int shape, const vec2f* vs, const int* indices, int size)
{
    glVertexPointer(2, GL_FLOAT, sizeof(vec2f), vs);
    glEnableClientState(GL_VERTEX_ARRAY);
    glDrawElements(shape, size, GL_UNSIGNED_INT, indices);
    glDisableClientState(GL_VERTEX_ARRAY);
}



class native_glcontext : public device_context
{
private:
    platform::display m_display;
    platform::window  m_window;
    HGLRC             m_context;
    bool display_is_get;    //is dc get from GetDC()
    int  m_texture;

public:
    vec4i viewport;
    glmultisample msaa;

public:
    native_glcontext() : device_context(), m_display(null), m_window(null), m_context(null)
    {
        display_is_get = false;
        m_texture = 0;
    }

    ~native_glcontext()
    {
    }

    HGLRC handle()const
    {
        return m_context;
    }

    int create(platform::display display, platform::window window)
    {
        if(display){
            m_display = display;
        }
        else{
            m_display = GetDC(window);
            display_is_get = true;
        }
        //m_winstyle = GetWindowLong(hwnd, GWL_STYLE);

        int result = init_context_ext(m_display);

        if(result < 0){
            result = init_context(m_display);
            if(result < 0){
                return result;
            }
        }

        //初始化扩展库
        #ifdef __glad_h_
            gladLoadGL();
        #elif defined(__glew_h__)
            glewInit();
        #endif

        return 0;
    }

    void dispose()
    {
        //释放opengl设备
        if(m_context){
            wglMakeCurrent(NULL, NULL);
            //wglGetCurrentContext()
            wglDeleteContext(m_context);
            m_context = null;
        }

        if(m_display && display_is_get){
            if(m_window){
                ReleaseDC(m_window, m_display);
            }
        }

        m_display = null;
        m_window = null;
    }

    bool ready()const
    {
        return m_context;
    }

    int make_current()
    {
        return wglMakeCurrent(m_display, m_context);
    }

    int make_null()
    {
        return wglMakeCurrent(null, null);
    }

    bool swap_buffers()
    {
        if(msaa.is_enabled()){
            vec4i rect;
            glGetIntegerv(GL_VIEWPORT, rect.data);
            msaa.bitblt(rect.x, rect.y, rect.width, rect.height);
            msaa.active();
        }
        return SwapBuffers(m_display);
    }

    int multisample(int level);//0 is closed

    vec4i display_bounds()
    {
        RECT rc;
        GetClientRect(m_window, &rc);
        return vec4i(rc.left, rc.top, rc.right-rc.left, rc.bottom - rc.top);
    }

    void bind_texture(int texture)
    {
        //if(m_texture != texture){
            glBindTexture(GL_TEXTURE_2D, texture);
            m_texture = texture;
        //}
    }

    void push_attrib()
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
    }

    void pop_attrib()
    {
        glPopAttrib();
    }

    //
    // 矩阵操作
    //

    void matrix_mode(GLenum mode)
    {
        glMatrixMode(mode);
    }

    void get_matrix(mat4f* modelMatrix, mat4f* projMatrix)const
    {
        if(modelMatrix)glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix->data);
        if(projMatrix)glGetFloatv(GL_PROJECTION_MATRIX, projMatrix->data);
    }

    void identity()
    {
        glLoadIdentity();
    }

    void translate(float x, float y, float z)
    {
        glTranslatef(x, y, z);
    }

    void rotate(float angle, float x, float y, float z)
    {
        glRotatef(angle, x, y, z);
    }

    void scale(float x, float y, float z)
    {
        glScalef(x, y, z);
    }

    void push_matrix()
    {
        glPushMatrix();
    }

    void pop_matrix()
    {
        glPopMatrix();
    }

    void ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        glOrtho(left, right, bottom, top, zNear, zFar);
    }

    void frustum(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        glFrustum(left, right, bottom, top, zNear, zFar);
    }

    vec4f project(const vec4f& v)
    {
        return vec4f();
    }

    vec4f unproject(float x, float y)
    {
        mat4f modelMatrix;
        mat4f projMatrix;

        glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix.data);
        glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.data);

        return graphics::unproject(x, y, modelMatrix, projMatrix);
    }

    vec4f unproject(float x, float y, float z)
    {
        mat4f modelView;
        mat4f projMatrix;
        vec4i viewport;
        vec4f p;

        glGetFloatv(GL_MODELVIEW_MATRIX, modelView.data);
        glGetFloatv(GL_PROJECTION_MATRIX, projMatrix.data);
        glGetIntegerv(GL_VIEWPORT, viewport.data);

        math::unproject(x, viewport.height - y - 1, z, modelView, projMatrix, viewport, p);

        return p;

        mat4f m = projMatrix * modelView;

        if(!m.inverse(m)){
            return vec4f();
        }

        // Transformation of normalized coordinates between -1 and 1
        p.x = (x-(float)viewport.x) / (float)viewport.width * 2.0f - 1.0f;
        p.y = (y-(float)viewport.y) / (float)viewport.height * 2.0f - 1.0f;
        p.z = 2.0f * z - 1.0f;
        p.w = 1.0f;

        // Objects coordinates
        p = m.transform(p);

        if(is_zero(p.w)){
            return vec4f();
        }

        //p = p / p.w;
        p.perspective_division();

        return p;
    }

    //
    // 顶点绘制
    //

    int draw_arrays(int shape, const vtx3f* vtx, size_t pos, size_t size)
    {
        vtx_begin(vtx);
        glDrawArrays(shape, pos, size);
        vtx_end(vtx);
        return 0;
    }

    int draw_arrays(int shape, const vtx3fx* vtx, size_t pos, size_t size)
    {
        vtx_begin(vtx);
        glDrawArrays(shape, pos, size);
        vtx_end(vtx);
        return 0;
    }

    int draw_elements(int shape, const vtx3f* vtx, const size_t* indices, size_t size)
    {
        vtx_begin(vtx);
        glDrawElements(shape, size, GL_UNSIGNED_INT, indices);
        vtx_end(vtx);
        return 0;
    }

    int draw_elements(int shape, const vtx3fx* vtx, const size_t* indices, size_t size)
    {
        vtx_begin(vtx);
        glDrawElements(shape, size, GL_UNSIGNED_INT, indices);
        vtx_end(vtx);
        return 0;
    }

    //
    // vertical sync
    //

    void set_vsync(int stat);
    int vsync();

protected:
    int init_context(HDC hDC);

    int init_context_ext(HDC hdc);
};



int native_glcontext::init_context(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {0};

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 8;
    //pfd.cRedShift = 16;
    pfd.cGreenBits = 8;
    //pfd.cGreenShift = 8;
    pfd.cBlueBits = 8;
    //pfd.cBlueShift = 0;
    pfd.cAlphaBits = 8;
    //pfd.cAlphaShift = 0;
    //pfd.cAccumBits = 64;
    //pfd.cAccumRedBits = 16;
    //pfd.cAccumGreenBits = 16;
    //pfd.cAccumBlueBits = 16;
    //pfd.cAccumAlphaBits = 0;
    #ifdef CGL_OPENGL_ENABLE_DEPTH_BUFFER
    pfd.cDepthBits = 24;
    #endif
    pfd.cStencilBits = 8;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.bReserved = 0;
    pfd.dwLayerMask = 0;
    pfd.dwVisibleMask = 0;
    pfd.dwDamageMask = 0;

    int pixel_id = ChoosePixelFormat(hDC, &pfd);

    if(!pixel_id){
        //Let's choose a default index.
        pixel_id = 1;
        if(FALSE == DescribePixelFormat(hDC, pixel_id, sizeof(PIXELFORMATDESCRIPTOR), &pfd)){
            return -1;
        }
    }

    #if 1
    if(FALSE == SetPixelFormat(hDC, pixel_id, &pfd)){
        return -2;
    }

    m_context = wglCreateContext(m_display);
    wglMakeCurrent(m_display, m_context);

    #else//opengl 3.x and upper

    HGLRC rc = enable_multisample(m_display, pixel_id, &pfd);

    //wglDeleteContext(m_context);

//    if(FALSE == SetPixelFormat(hDC, pf, &pfd)){
//        return -3;
//    }

//    m_context = wglCreateContext(m_display);
    m_context = rc;
    wglMakeCurrent(m_display, m_context);
    #endif

    return 0;
}

#ifndef WGL_ARB_create_context
#define WGL_ARB_create_context 1

#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096

#endif

#ifndef WGL_ARB_create_context_profile
#define WGL_ARB_create_context_profile 1

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#endif

#ifndef WGL_ARB_multisample
#define WGL_ARB_multisample 1

#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042

#endif

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B

#endif

void set_attribute(int* attrib, int name, int value)
{
    while(*attrib){
        if(*attrib == name){
            *++attrib = value;
            break;
        }
        attrib += 2;
    }
}

int native_glcontext::init_context_ext(HDC hDC)
{
    if(init_context(hDC) != 0){
        return -1;
    }

    typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormat = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

    if(wglChoosePixelFormat == 0 || wglCreateContextAttribs == 0){
        CGL_LOG("wglChoosePixelFormat error.");
        return 0;
    }

    //删除当前OpenGL环境
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_context);
    m_context = null;

    int nPixelFormat = -1;
    int nPixCount = 0;
    float fPixAttribs[]= {
        0,0
    };

    int pixel_attrib[] = {
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,    // Must support OGL rendering
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,    // pf that can run a window
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,    // must be HW accelerated
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,     // Double buffered context

        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,             // 8 bits of each R, G and B
        WGL_DEPTH_BITS_ARB, 24,             // 24 bits of depth precision for window
        WGL_STENCIL_BITS_ARB, 8,            // 开启模板缓冲区,模板缓冲区位数=8

        WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,    // MSAA on,开启多重采样
        WGL_SAMPLES_ARB, 4,                 // 4x MSAA ,多重采样样本数量为4

        0, 0
    };

    //设置像素格式
    wglChoosePixelFormat(hDC, pixel_attrib, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
    if (nPixelFormat == -1) {
        //关闭多重采样
        set_attribute(pixel_attrib, WGL_SAMPLE_BUFFERS_ARB, GL_FALSE);
        wglChoosePixelFormat(hDC, pixel_attrib, fPixAttribs, 1, &nPixelFormat, (UINT*)&nPixCount);
    }

    GLint attribs[] = {
//        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,   //只包含核心环境(默认)
        WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB, //创建一个兼容环境

        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,//主版本3
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,//次版本号3

//        WGL_CONTEXT_MAJOR_VERSION_ARB, major,   //主版本
//        WGL_CONTEXT_MINOR_VERSION_ARB, minor,   //次版本号

        0, 0
    };

    HGLRC hRC = wglCreateContextAttribs(hDC, 0, attribs);
    if (hRC == NULL) {
        MessageBox(WindowFromDC(hDC), TEXT("wglCreateContextAttribs() error."), TEXT("OpenGL"), 0);
        return CGL_ERROR;
    }

    if (FALSE == wglMakeCurrent(hDC, hRC)) {
        wglDeleteContext(hRC);
        MessageBox(WindowFromDC(hDC), TEXT("wglMakeCurrent() error."), TEXT("OpenGL"), MB_OK);
        return CGL_ERROR;
    }

    m_context = hRC;

    return 0;
}

//WGL_EXT_swap_control 扩展
//打开垂直分布，限制帧率，等待屏幕第几次更新时刷新屏幕
void native_glcontext::set_vsync(int stat)
{
    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT;
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
    if(wglSwapIntervalEXT){
        wglSwapIntervalEXT(stat);
    }
}

int native_glcontext::vsync()
{
    typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
    PFNWGLEXTGETSWAPINTERVALPROC wglGetSwapIntervalEXT;
    wglGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
    if(wglGetSwapIntervalEXT){
        return wglGetSwapIntervalEXT();
    }
    else{
        return -1;
    }
}

int native_glcontext::multisample(int level)
{
    if(level == 0){
        msaa.disable();
        msaa.dispose();
    }
    else{
        int n = msaa.max_samples();
        if(level > n)level = n;
        vec4i rect;
        glGetIntegerv(GL_VIEWPORT, rect.data);
        msaa.create(rect.width, rect.height, level);
        msaa.enable();
        msaa.active();
    }
    return 0;
}


}//end namespace graphics
}//end namespace cgl

#endif //CONTEXT_WIN32_HPP_20180509082303

/*

extern HGLRC enable_multisample(HDC hDC, int pid, const PIXELFORMATDESCRIPTOR *ppfd);

void append_attrib(int* attributes, int& where, int attrib, int value)
{
    attributes[where++] = attrib;
    attributes[where++] = value;
}

static void fghFillPixelFormatAttributes( int *attributes, const PIXELFORMATDESCRIPTOR *ppfd )
{
  int where = 0;

//  ATTRIB_VAL( WGL_CONTEXT_MAJOR_VERSION_ARB, 3 );
//  ATTRIB_VAL( WGL_CONTEXT_MINOR_VERSION_ARB, 3);

  append_attrib(attributes, where, WGL_DRAW_TO_WINDOW_ARB, GL_TRUE);
  append_attrib(attributes, where, WGL_SUPPORT_OPENGL_ARB, GL_TRUE );
  append_attrib(attributes, where, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB );

  append_attrib(attributes, where, WGL_COLOR_BITS_ARB, ppfd->cColorBits );
  append_attrib(attributes, where, WGL_ALPHA_BITS_ARB, ppfd->cAlphaBits );
  append_attrib(attributes, where, WGL_DEPTH_BITS_ARB, ppfd->cDepthBits );
  append_attrib(attributes, where, WGL_STENCIL_BITS_ARB, ppfd->cStencilBits );

  append_attrib(attributes, where, WGL_DOUBLE_BUFFER_ARB, GL_TRUE);// ( fgState.DisplayMode & GLUT_DOUBLE ) != 0 );

//  if ( fgState.DisplayMode & GLUT_SRGB ) {
//    ATTRIB_VAL( WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, TRUE );
//  }

  append_attrib(attributes, where, WGL_SAMPLE_BUFFERS_ARB, GL_TRUE );
  append_attrib(attributes, where, WGL_SAMPLES_ARB, 4);// fgState.SampleNumber );
  attributes[where] = 0;
}

HGLRC WINAPI CreateContextAttribs(HDC hDC, HGLRC hShareContext, const int *attribList)
{
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if(!wglCreateContextAttribsARB)
    {
        return 0;
    }
    return wglCreateContextAttribsARB(hDC, hShareContext, attribList);
}

HGLRC enable_multisample(HDC hDC, int pid, const PIXELFORMATDESCRIPTOR *ppfd)
{
    HWND hwnd = CreateWindowEx(0, L"luna.window", L"asd", 0, 0, 0, 100, 100, 0, 0, GetModuleHandle(NULL), 0);
    HDC dc = GetDC(hwnd);// hDC;//CreateCompatibleDC(0);
    SetPixelFormat(dc, pid, ppfd);
        HGLRC rc = wglCreateContext(dc);
        wglMakeCurrent(dc, rc);

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARBProc =
      (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");
    if ( wglChoosePixelFormatARBProc ){
        int attributes[100] = {0};
        int iPixelFormat;
        BOOL bValid;
        float fAttributes[] = { 0, 0 };
        UINT numFormats;
        fghFillPixelFormatAttributes( attributes, ppfd );

        bValid = wglChoosePixelFormatARBProc(hDC, attributes, fAttributes, 1, &iPixelFormat, &numFormats);

        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

        if(!SetPixelFormat(hDC, iPixelFormat, ppfd)){
            return 0;
        }

        if ( bValid && numFormats > 0 )
        {
            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            if(!wglCreateContextAttribsARB)
            {
                return 0;
            }

            int attribList[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 1,
//                    WGL_DRAW_TO_WINDOW_ARB, 1,
//                    WGL_SUPPORT_OPENGL_ARB, 1,

                WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,

                WGL_COLOR_BITS_ARB, 24,
                WGL_ALPHA_BITS_ARB, 8,
                WGL_DEPTH_BITS_ARB, 24,
                WGL_STENCIL_BITS_ARB, 8,

//                    WGL_SAMPLE_BUFFERS_ARB, 1,
//                    WGL_SAMPLES_ARB, 4,
                //WGL_CONTEXT_CORE_PROFILE_BIT_ARB 只包含核心环境
                //WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 创建一个能够兼容所有OpenGL老版本的环境。
                //WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
                  WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                0, 0
            };

//      int attributes2[] = {
//      WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
//      WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
//      WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
//      WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
//      WGL_COLOR_BITS_ARB, 32,
//        WGL_DEPTH_BITS_ARB, 24,
//      WGL_STENCIL_BITS_ARB, 8,
//      0
//    };

            HGLRC rc2 = wglCreateContextAttribsARB(hDC, NULL, attributes);// attributes);

            if(!rc2)
            {
                int a  = glGetError();
                const char* s;// glGetString(glGetError());
            }

            return rc2;

        }
    }

    return 0;
}
*/
