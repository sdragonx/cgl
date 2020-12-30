/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gles2.hpp

 2018-05-29 01:31:48

*/
#ifndef CONTEXT_GLES2_HPP_20180529013148
#define CONTEXT_GLES2_HPP_20180529013148

#include <cgl/stack.hpp>
#include "gl.hpp"
#include "program.hpp"

#ifdef CGL_PLATFORM_ANGLE

#ifndef EGL_ANGLE_platform_angle
#define EGL_ANGLE_platform_angle 1
#define EGL_PLATFORM_ANGLE_ANGLE          0x3202
#define EGL_PLATFORM_ANGLE_TYPE_ANGLE     0x3203
#define EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE 0x3204
#define EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE 0x3205
#define EGL_PLATFORM_ANGLE_TYPE_DEFAULT_ANGLE 0x3206
#define EGL_PLATFORM_ANGLE_DEBUG_LAYERS_ENABLED_ANGLE 0x3451
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE 0x3209
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE 0x320A
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_NULL_ANGLE 0x345E
#endif /* EGL_ANGLE_platform_angle */

#ifndef EGL_ANGLE_platform_angle_d3d
#define EGL_ANGLE_platform_angle_d3d 1
#define EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE 0x3207
#define EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE 0x3208
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_D3D_WARP_ANGLE 0x320B
#define EGL_PLATFORM_ANGLE_DEVICE_TYPE_D3D_REFERENCE_ANGLE 0x320C
#define EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE 0x320F
#endif /* EGL_ANGLE_platform_angle_d3d */

#ifndef EGL_ANGLE_platform_angle_null
#define EGL_ANGLE_platform_angle_null 1
#define EGL_PLATFORM_ANGLE_TYPE_NULL_ANGLE 0x33AE
#endif /* EGL_ANGLE_platform_angle_null */

#ifndef EGL_ANGLE_platform_angle_opengl
#define EGL_ANGLE_platform_angle_opengl 1
#define EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE 0x320D
#define EGL_PLATFORM_ANGLE_TYPE_OPENGLES_ANGLE 0x320E
#define EGL_PLATFORM_ANGLE_EGL_HANDLE_ANGLE 0x3480
#endif /* EGL_ANGLE_platform_angle_opengl */

#ifndef EGL_ANGLE_platform_angle_vulkan
#define EGL_ANGLE_platform_angle_vulkan 1
#define EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE 0x3450
#endif /* EGL_ANGLE_platform_angle_vulkan */

#ifndef EGL_ANGLE_create_context_client_arrays
#define EGL_ANGLE_create_context_client_arrays 1
#define EGL_CONTEXT_CLIENT_ARRAYS_ENABLED_ANGLE 0x3452
#endif /* EGL_ANGLE_create_context_client_arrays */

#ifndef EGL_ANGLE_create_context_extensions_enabled
#define EGL_ANGLE_create_context_extensions_enabled 1
#define EGL_EXTENSIONS_ENABLED_ANGLE 0x345F
#endif /* EGL_ANGLE_create_context_extensions_enabled */


#ifndef EGL_ANGLE_create_context_webgl_compatibility
#define EGL_ANGLE_create_context_webgl_compatibility 1
#define EGL_CONTEXT_WEBGL_COMPATIBILITY_ANGLE 0x3AAC
#endif /* EGL_ANGLE_create_context_webgl_compatibility */

#ifndef EGL_CHROMIUM_create_context_bind_generates_resource
#define EGL_CHROMIUM_create_context_bind_generates_resource 1
#define EGL_CONTEXT_BIND_GENERATES_RESOURCE_CHROMIUM 0x3AAD
#endif /* EGL_CHROMIUM_create_context_bind_generates_resource */

//es_angle

//typedef std::vector<GLint> es_config;

EGLint es_d3d11[] = {
    EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
    EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE,
    EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
    EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 9,
    EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 3,
    EGL_NONE
};

EGLint es_d3d9[] = {
    EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
    EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE,
//    EGL_PLATFORM_ANGLE_ENABLE_AUTOMATIC_TRIM_ANGLE, EGL_TRUE,
//    EGL_PLATFORM_ANGLE_USE_WARP_ANGLE, EGL_TRUE,
//    EGL_PLATFORM_ANGLE_EXPERIMENTAL_DIRECT_RENDERING, EGL_FALSE,
    EGL_NONE
};

EGLint es_opengl[] = {
    EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE,
    EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE, 3,
    EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE, 2,
    EGL_NONE
};

EGLint es_opengles[] = {
    EGL_NONE
};

EGLint es_vulkan[] = {
    EGL_PLATFORM_ANGLE_TYPE_ANGLE, EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE,
    EGL_NONE
};

enum es_device_type{
    CGL_ANGLE_D3D9,
    CGL_ANGLE_D3D11,
    CGL_ANGLE_OPENGL,
    CGL_ANGLE_OPENGLES,
    CGL_ANGLE_VULKAN
};

GLint* es_device(int n)
{
    GLint* device[] = {es_d3d11, es_d3d9, es_opengl, es_opengles, es_vulkan};
    return device[n];
}

const char* es_device_name(int id)
{
    const char* names[] = {"Direct3D 9", "Direct3D 11", "OpenGL", "OpenGL ES", "Vulkan"};
    return names[id];
}

#endif //CGL_PLATFORM_ANGLE

namespace cgl{
namespace graphics{

/*
struct native_glcontext_state
{
    vec4ub color;
    GLuint texture;
    vec4i  viewport;
    vec4i  cliprect;
};
*/

class native_glcontext : public device_context
{
public:
    struct version_type{
        EGLint  major;//主版本号
        EGLint  minor;//次版本号
    };

protected:
    EGLDisplay m_display;
    EGLContext m_context;
    EGLSurface m_surface;
    EGLConfig  m_config;
    EGLNativeDisplayType m_native_display;
    EGLNativeWindowType  m_native_window;

    enum{
        MODE_PROJECTION = 0,
        MODE_MODELVIEW  = 1
    };
    value_stack<mat4f> m_mat[2];
    int m_mat_mode;

protected:
    glprogram program;
    GLuint m_texture;
    version_type m_egl_version;    //egl version

public:
    vec4i viewport;

public:
    native_glcontext() : device_context(),
        m_display(EGL_NO_DISPLAY),
        m_context(EGL_NO_CONTEXT),
        m_surface(EGL_NO_SURFACE),
        m_config(null),
        m_native_display(null),
        m_native_window(null)
    {
        m_mat_mode = MODE_MODELVIEW;
        m_texture = 0;
    }

    int create(EGLNativeDisplayType display, EGLNativeWindowType window)
    {
        int result;

        #ifdef LIBEGL_HPP_20200128014936
        libEGL_init();
        #endif

        #ifdef CGL_PLATFORM_WINDOWS
        bool display_is_get = false;
        if(!display){
            display = GetDC(window);
            display_is_get = true;
        }
        #endif

        result = init_display(display);
        if(result !=  CGL_OK){
            return result;
        }

        result = init_config();
        if(result != CGL_OK){
            return result;
        }

        if(m_surface == EGL_NO_SURFACE){
            result = init_surface(window);
            if(result != CGL_OK){
                return result;
            }
        }

        if(m_context == EGL_NO_CONTEXT){
            result = init_context();
            if(result != CGL_OK){
                return result;
            }
        }

        // Make the context current
        while(!this->make_current()){
            EGLint err = eglGetError();
            if( err == EGL_CONTEXT_LOST ){
                init_context();
            }
            else{
                return EGL_BAD_CURRENT_SURFACE;
            }
        }

        if(result == CGL_OK){
            CGL_LOG("native_glcontext::create successed.");
        }

        //加载GLESv2函数
        #ifdef LIBGLESV2_HPP_20200128030259
        libGLESv2_init();
        #endif

        result = init_shader();

        #ifdef CGL_PLATFORM_WINDOWS
        if(display_is_get){
            //ReleaseDC(window, display);
        }
        #endif

        return result;
    }

    int init_display(EGLNativeDisplayType display = EGL_DEFAULT_DISPLAY)
    {
        // init display
        if(m_display != EGL_NO_DISPLAY){
            return CGL_OK;
        }

        m_native_display = display;

        #ifdef CGL_PLATFORM_ANGLE
        GLint* device = es_device(CGL_ANGLE_OPENGL);
        m_display = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, display, device);
        #else
        m_display = eglGetDisplay(display);
        #endif

        if(m_display == EGL_NO_DISPLAY){
            m_display = eglGetDisplay(display);
        }

        if(m_display == EGL_NO_DISPLAY){
            CGL_LOG_ERROR("native_glcontext::eglGetDisplay : %s", "EGL_BAD_CONFIG ");
            return EGL_BAD_DISPLAY;
        }

        eglInitialize(m_display, &m_egl_version.major, &m_egl_version.minor);

        return CGL_OK;
    }

    int init_config()
    {
        // choose config
        EGLint numConfigs = 0;

        EGLint config_attribs[] = {
            EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,// EGL_OPENGL_ES3_BIT,//EGL_OPENGL_ES2_BIT|EGL_OPENGL_ES3_BIT,
            EGL_BLUE_SIZE,    8,
            EGL_GREEN_SIZE,    8,
            EGL_RED_SIZE,    8,
            EGL_ALPHA_SIZE,    8,
            #ifdef CGL_OPENGL_ENABLE_DEPTH_BUFFER
            EGL_DEPTH_SIZE,    24,
            #endif
            EGL_STENCIL_SIZE, 8,
//            EGL_SAMPLE_BUFFERS, 1,
//            EGL_SAMPLES, 4,
            EGL_NONE
        };

        #ifdef CGL_OPENGL_ENABLE_DEPTH_BUFFER
        // try 24bit depth buffer
        eglChooseConfig(m_display, config_attribs, &m_config, 1, &numConfigs);

        // fall back to 16bit depth buffer
        if(!numConfigs){
            int i = 0;
            for( ; config_attribs[i]; ++i){
                if(config_attribs[i] == EGL_DEPTH_SIZE)break;
            }
            config_attribs[++i] = 16;
            eglChooseConfig(m_display, config_attribs, &m_config, 1, &numConfigs);
        }
        #else
        eglChooseConfig(m_display, config_attribs, &m_config, 1, &numConfigs);
        #endif

        if(!numConfigs){
            CGL_LOG_ERROR("native_glcontext::eglChooseConfig : %s", "EGL_BAD_CONFIG");
            return EGL_BAD_CONFIG;
        }

        //CGL_LOG("API : %i", eglQueryAPI()); //EGL_OPENGL_ES_API
        //if (!eglBindAPI(EGL_OPENGL_ES_API)){ // error }

        return CGL_OK;
    }

    int init_context()
    {
        // create context
        EGLint context_attrib[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            //EGL_CONTEXT_OPENGL_DEBUG, EGL_TRUE,
            //EGL_CONTEXT_WEBGL_COMPATIBILITY_ANGLE, EGL_TRUE,
            //EGL_EXTENSIONS_ENABLED_ANGLE, EGL_TRUE,
            //EGL_CONTEXT_OPENGL_ROBUST_ACCESS_EXT, EGL_TRUE,
            //EGL_CONTEXT_BIND_GENERATES_RESOURCE_CHROMIUM, EGL_TRUE,
            //EGL_CONTEXT_CLIENT_ARRAYS_ENABLED_ANGLE, EGL_TRUE,
            //EGL_CONTEXT_PROGRAM_BINARY_CACHE_ENABLED_ANGLE, bool
            EGL_NONE };
        m_context = eglCreateContext(m_display, m_config, EGL_NO_CONTEXT, context_attrib);
        if(m_context == EGL_NO_CONTEXT){
            CGL_LOG_ERROR("native_glcontext::eglCreateContext : %s", "EGL_BAD_CONTEXT");
            return EGL_BAD_CONTEXT;
        }

        return CGL_OK;
    }

    //安卓窗口重建，surface会丢失
    int init_surface(EGLNativeWindowType window)
    {
        m_native_window = window;

        #ifdef CGL_PLATFORM_ANDROID
        EGLint format = 0;
        eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);
        ANativeWindow_setBuffersGeometry(m_native_window, 0, 0, format);
        #endif

        // Create a surface
        //default EGL_RENDER_BUFFER = EGL_BACK_BUFFER
        //EGLint surface_attrib[] = { EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE };
        m_surface = eglCreateWindowSurface(m_display, m_config, m_native_window, null);

        if(m_surface == EGL_NO_SURFACE){
            CGL_LOG_ERROR("native_glcontext::eglCreateWindowSurface : %s", "EGL_BAD_SURFACE");
            return EGL_BAD_SURFACE;
        }

        return CGL_OK;
    }

    void free_surface()
    {
        if(m_display != EGL_NO_DISPLAY){
            //this->make_null();
            if( m_surface != EGL_NO_SURFACE ){
                eglDestroySurface(m_display, m_surface);
                m_surface = EGL_NO_SURFACE;
            }
        }
    }

    int init_shader()
    {
        // init shader, before make_current.
        if(program.create(basic_vs, basic_fs) != CGL_OK){
            return CGL_ERROR;
        }
        return CGL_OK;
    }

    void dispose()
    {
        program.dispose();

        if(m_display != EGL_NO_DISPLAY){
            //this->make_null();
            if(m_context != EGL_NO_CONTEXT){
                eglDestroyContext(m_display, m_context);
            }

            if(m_surface != EGL_NO_SURFACE){
                eglDestroySurface(m_display, m_surface);
            }
            eglTerminate(m_display);
        }

        m_display = EGL_NO_DISPLAY;
        m_context = EGL_NO_CONTEXT;
        m_surface = EGL_NO_SURFACE;
        m_native_window  = null;
    }

    bool ready()const
    {
        return m_display && m_context && m_surface;
    }

    bool make_current()
    {
        if(eglMakeCurrent(m_display, m_surface, m_surface, m_context) != EGL_TRUE){
            CGL_LOG_ERROR("native_glcontext::eglMakeCurrent error.");
            return false;
        }
        return true;
    }

    bool make_null()
    {
        return eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    bool swap_buffers()
    {
        if(!eglSwapBuffers(m_display, m_surface)){
            CGL_LOG_ERROR("eglSwapBuffers : %i", eglGetError());
            return false;
        }
        return true;
    }

    void set_vsync(int stat)
    {
        eglSwapInterval(m_display, stat);
    }

    vec4i display_bounds()
    {
        EGLint w, h;
        eglQuerySurface(m_display, m_surface, EGL_WIDTH,  &w);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT,  &h);
        return vec4i(0, 0, w, h);
    }

    void bind_texture(int texture)
    {
        m_texture = texture;
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void push_attrib()
    {

    }

    void pop_attrib()
    {
    }

    //
    // 矩阵操作
    //

    void matrix_mode(GLuint mode)
    {
        if(mode == GL_PROJECTION){
            m_mat_mode = MODE_PROJECTION;
        }
        else{
            m_mat_mode = MODE_MODELVIEW;
        }
    }

    void get_matrix(mat4f* modelMatrix, mat4f* projMatrix)const
    {
        if(modelMatrix)*modelMatrix = m_mat[MODE_MODELVIEW].value;
        if(projMatrix)*projMatrix = m_mat[MODE_PROJECTION].value;
    }

    void identity()
    {
        m_mat[m_mat_mode].value.identity();
    }

    void translate(float x, float y, float z)
    {
        m_mat[m_mat_mode].value.translate(x, y, z);
    }

    void rotate(float angle, float x, float y, float z)
    {
        m_mat[m_mat_mode].value.rotate(angle, x, y, z);
    }

    void scale(float x, float y, float z)
    {
        m_mat[m_mat_mode].value.scale(x, y, z);
    }

    void push_matrix()
    {
        m_mat[m_mat_mode].push();
    }

    void pop_matrix()
    {
        m_mat[m_mat_mode].pop();
    }

    void ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        m_mat[m_mat_mode].value.ortho(left, right, bottom, top, zNear, zFar);
    }

    void frustum(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        m_mat[m_mat_mode].value.frustum(left, right, bottom, top, zNear, zFar);
    }

    int draw_arrays(int shape, const vtx3f* vtx, size_t pos, size_t size)
    {
        //mat4f mvp = m_mat[MODE_PROJECTION].value * m_mat[MODE_MODELVIEW].value;
        //glBindTexture(GL_TEXTURE_2D, m_texture);
        program.begin(m_mat[MODE_MODELVIEW].value, m_mat[MODE_PROJECTION].value, m_texture);
        program.draw_arrays(shape, vtx, pos, size);
        program.end();
        return 0;
    }

    int draw_arrays(int shape, const vtx3fx* vtx, size_t pos, size_t size)
    {
        //mat4f mvp = m_mat[MODE_PROJECTION].value * m_mat[MODE_MODELVIEW].value;
        //glBindTexture(GL_TEXTURE_2D, m_texture);
        program.begin(m_mat[MODE_MODELVIEW].value, m_mat[MODE_PROJECTION].value, m_texture);
        program.draw_arrays(shape, vtx, pos, size);
        program.end();
        return 0;
    }

    int draw_elements(int shape, const vtx3f* vtx, const GLuint* indices, size_t size)
    {
        //mat4f mvp = m_mat[MODE_PROJECTION].value * m_mat[MODE_MODELVIEW].value;
        //glBindTexture(GL_TEXTURE_2D, m_texture);
        program.begin(m_mat[MODE_MODELVIEW].value, m_mat[MODE_PROJECTION].value, m_texture);
        program.draw_elements(shape, vtx, indices, size);
        program.end();
        return 0;
    }

    int draw_elements(int shape, const vtx3fx* vtx, const GLuint* indices, size_t size)
    {
        //mat4f mvp = m_mat[MODE_PROJECTION].value * m_mat[MODE_MODELVIEW].value;
        //glBindTexture(GL_TEXTURE_2D, m_texture);
        program.begin(m_mat[MODE_MODELVIEW].value, m_mat[MODE_PROJECTION].value, m_texture);
        program.draw_elements(shape, vtx, indices, size);
        program.end();
        return 0;
    }

    vec4f unProject(int x, int y)
    {
        return gluUnProject(x, y, m_mat[MODE_MODELVIEW].value, m_mat[MODE_PROJECTION].value);
    }
};

}//end namespace graphics
}//end namespace cgl

#endif //CONTEXT_GLES2_HPP_20180529013148
