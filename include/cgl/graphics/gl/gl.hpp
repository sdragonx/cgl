/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 gl.hpp

 2016-03-15 16:41:29

*/
#ifndef GL_HPP_20160315164129
#define GL_HPP_20160315164129

//#define _PTRDIFF_T_DEFINED

//stl
#include <algorithm>
#include <map>
#include <stack>
#include <vector>

//cgl
#include <cgl/public.h>
#include <cgl/graphics/color.hpp>
#include <cgl/graphics/context.hpp>
#include <cgl/math.hpp>
#include <cgl/matrix.hpp>
#include <cgl/vec.hpp>
#include <cgl/vertex.hpp>

//#define CGL_OPENGL_ENABLE_DEPTH_BUFFER

#if defined(CGL_PLATFORM_GLES) || defined(CGL_PLATFORM_ANDROID) || defined(CGL_PLATFORM_APPLE) || defined(CGL_PLATFORM_ANGLE)
//---------------------------------------------------------------------------
//
// OpenGL ES
//

#ifndef CGL_PLATFORM_GLES
    #define CGL_PLATFORM_GLES
#endif
#ifdef CGL_PLATFORM_WINDOWS
    //#define CGL_PLATFORM_ANGLE
#endif

#ifdef CGL_PLATFORM_ANDROID
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES3/gl3.h>

#else
    #include "libEGL.hpp"
    #include "libGLESv2.hpp"
#endif

#ifndef GL_MODELVIEW
    #define GL_MODELVIEW  0x1700
    #define GL_PROJECTION 0x1701
#endif

namespace cgl{ namespace graphics{

//eglGetError()
const char* egl_error_string(int error)
{
    switch(error){
    case EGL_SUCCESS:
        return "The last function succeeded without error.";
    case EGL_NOT_INITIALIZED:
        return "EGL is not initialized, or could not be initialized, for the specified EGL display connection.";
    case EGL_BAD_ACCESS:
        return "EGL cannot access a requested resource (for example a context is bound in another thread).";
    case EGL_BAD_ALLOC:
        return "EGL failed to allocate resources for the requested operation.";
    case EGL_BAD_ATTRIBUTE:
        return "An unrecognized attribute or attribute value was passed in the attribute list.";
    case EGL_BAD_CONTEXT:
        return "An EGLContext argument does not name a valid EGL rendering context.";
    case EGL_BAD_CONFIG:
        return "An EGLConfig argument does not name a valid EGL frame buffer configuration.";
    case EGL_BAD_CURRENT_SURFACE:
        return "The current surface of the calling thread is a window, pixel buffer or pixmap that is no longer valid.";
    case EGL_BAD_DISPLAY:
        return "An EGLDisplay argument does not name a valid EGL display connection.";
    case EGL_BAD_SURFACE:
        return "An EGLSurface argument does not name a valid surface (window, pixel buffer or pixmap) configured for GL rendering.";
    case EGL_BAD_MATCH:
        return "Arguments are inconsistent (for example, a valid context requires buffers not supplied by a valid surface).";
    case EGL_BAD_PARAMETER:
        return "One or more argument values are invalid.";
    case EGL_BAD_NATIVE_PIXMAP:
        return "A NativePixmapType argument does not refer to a valid native pixmap.";
    case EGL_BAD_NATIVE_WINDOW:
        return "A NativeWindowType argument does not refer to a valid native window.";
    case EGL_CONTEXT_LOST:
        return "A power management event has occurred. The application must destroy all contexts and reinitialise OpenGL ES state and objects to continue rendering.";
    default:
        return "unknown error code.";
    }
}

void egl_check(int code)
{
    CGL_LOG(egl_error_string(code));
}

}}//end namespace cgl::graphics

#else
//---------------------------------------------------------------------------
//
// OpenGL
//

//#define GL_GLEXT_PROTOTYPES
//#include <gl/glew.h>
#include <cgl/external/glad/glad.h>
//#include <gl/gl.h>
//#include <gl/glext.h>
//#include <gl/glu.h>    //mipmap

#define GL_CLAMP_TO_EDGE 0x812F

#if defined(__BORLANDC__) || defined(_MSC_VER)
    #pragma comment(lib,"opengl32.lib")
    #pragma comment(lib,"glu32.lib")
    //#pragma comment(lib, "glew32.lib")
#endif

#endif

//opengl
#ifndef GL_BGR
    #define GL_BGR  0x80E0
    #define GL_BGRA 0x80E1
#endif

#ifndef GL_ADD
    #define GL_ADD           0x0104
    #define GL_MODULATE      0x2100
    #define GL_DECAL         0x2101
    #define GL_COMBINE       0x8570
    #define GL_PRIMARY_COLOR 0x8577
#endif

namespace cgl{
namespace graphics{

vec4f unproject(float x, float y, const mat4f& modelMatrix, const mat4f& projMatrix)
{
    vec4i viewport;
    float winX, winY, winZ;

    glGetIntegerv(GL_VIEWPORT, viewport.data);

    winX = x;
    winY = viewport.height - y - 1;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    vec4f p;
    math::unproject(winX, winY, winZ, modelMatrix, projMatrix, viewport, p);
    return p;
}

//glGetError()
const char* gl_error_string(int err)
{
    switch(err)
    {
    case GL_NO_ERROR:
        return "ok.";
    case GL_INVALID_ENUM:
        return "invalid enum.";
    case GL_INVALID_VALUE:
        return "invalid value.";
    case GL_INVALID_OPERATION:
        return "invalid operation.";
    #ifndef CGL_PLATFORM_GLES
    case GL_STACK_OVERFLOW:
        return "stack overflow.";
    case GL_STACK_UNDERFLOW:
        return "stack underflow.";
    #endif
    case GL_OUT_OF_MEMORY:
        return "out of memory.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "framebuffer is not complete.";
    #ifndef CGL_PLATFORM_GLES
    case GL_CONTEXT_LOST:
        return "OpenGL context has been lost.";
//    case GL_TABLE_TOO_LARGE:
//        return "table too large.";
    #endif
    default:
        return "unknown error code.";
    }
}

void gl_check(int code)
{
    CGL_LOG(gl_error_string(code));
}

#ifdef GL_VERSION_4_3

void GLAPIENTRY gluMessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    CGL_LOG("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
        type,
        severity,
        message );
}

// During init, enable debug output
void gluEnableDebug()
{
    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( gluMessageCallback, 0 );
}

void gluDisableDebug()
{
    glDisable( GL_DEBUG_OUTPUT );
}

#endif



}}//end namespace cgl::graphcis

#endif //GL_HPP_20160315164129

