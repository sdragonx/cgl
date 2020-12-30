/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 libEGL.hpp

 2020-01-28 01:49:36

*/
#ifndef LIBEGL_HPP_20200128014936
#define LIBEGL_HPP_20200128014936

#if defined(__glad_egl_h_)
    #define EGLAPI
    #define EGLAPIENTRY
#endif

#include <egl/egl.h>
#include <cgl/public.h>
#include <cgl/system/shared_obj.hpp>

#ifdef __cplusplus
extern "C"{
#endif

//EGL_VERSION_1_0
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLCHOOSECONFIGPROC) (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLCOPYBUFFERSPROC) (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target);
typedef EGLAPI EGLContext EGLAPIENTRY (*PFNEGLCREATECONTEXTPROC) (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEPBUFFERSURFACEPROC) (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEPIXMAPSURFACEPROC) (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEWINDOWSURFACEPROC) (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLDESTROYCONTEXTPROC) (EGLDisplay dpy, EGLContext ctx);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLDESTROYSURFACEPROC) (EGLDisplay dpy, EGLSurface surface);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLGETCONFIGATTRIBPROC) (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLGETCONFIGSPROC) (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLAPI EGLDisplay EGLAPIENTRY (*PFNEGLGETCURRENTDISPLAYPROC) (void);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLGETCURRENTSURFACEPROC) (EGLint readdraw);
typedef EGLAPI EGLDisplay EGLAPIENTRY (*PFNEGLGETDISPLAYPROC) (EGLNativeDisplayType display_id);
typedef EGLAPI EGLint EGLAPIENTRY (*PFNEGLGETERRORPROC) (void);
typedef EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY (*PFNEGLGETPROCADDRESSPROC) (const char *procname);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLINITIALIZEPROC) (EGLDisplay dpy, EGLint *major, EGLint *minor);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLMAKECURRENTPROC) (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLQUERYCONTEXTPROC) (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value);
typedef EGLAPI const char *EGLAPIENTRY (*PFNEGLQUERYSTRINGPROC) (EGLDisplay dpy, EGLint name);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLQUERYSURFACEPROC) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLSWAPBUFFERSPROC) (EGLDisplay dpy, EGLSurface surface);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLTERMINATEPROC) (EGLDisplay dpy);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLWAITGLPROC) (void);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLWAITNATIVEPROC) (EGLint engine);

//EGL_VERSION_1_1
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLBINDTEXIMAGEPROC) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLRELEASETEXIMAGEPROC) (EGLDisplay dpy, EGLSurface surface, EGLint buffer);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLSURFACEATTRIBPROC) (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLSWAPINTERVALPROC) (EGLDisplay dpy, EGLint interval);

//EGL_VERSION_1_2
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLBINDAPIPROC) (EGLenum api);
typedef EGLAPI EGLenum EGLAPIENTRY (*PFNEGLQUERYAPIPROC) (void);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC) (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLRELEASETHREADPROC) (void);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLWAITCLIENTPROC) (void);

//EGL_VERSION_1_4
typedef EGLAPI EGLContext EGLAPIENTRY (*PFNEGLGETCURRENTCONTEXTPROC) (void);

//EGL_VERSION_1_5
typedef EGLAPI EGLSync EGLAPIENTRY (*PFNEGLCREATESYNCPROC) (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLDESTROYSYNCPROC) (EGLDisplay dpy, EGLSync sync);
typedef EGLAPI EGLint EGLAPIENTRY (*PFNEGLCLIENTWAITSYNCPROC) (EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLGETSYNCATTRIBPROC) (EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value);
typedef EGLAPI EGLImage EGLAPIENTRY (*PFNEGLCREATEIMAGEPROC) (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib *attrib_list);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLDESTROYIMAGEPROC) (EGLDisplay dpy, EGLImage image);
typedef EGLAPI EGLDisplay EGLAPIENTRY (*PFNEGLGETPLATFORMDISPLAYPROC) (EGLenum platform, void *native_display, const EGLAttrib *attrib_list);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEPLATFORMWINDOWSURFACEPROC) (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list);
typedef EGLAPI EGLSurface EGLAPIENTRY (*PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC) (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list);
typedef EGLAPI EGLBoolean EGLAPIENTRY (*PFNEGLWAITSYNCPROC) (EGLDisplay dpy, EGLSync sync, EGLint flags);

struct eglAPI
{
    //EGL_VERSION_1_0
    PFNEGLCHOOSECONFIGPROC eglChooseConfig;
    PFNEGLCOPYBUFFERSPROC eglCopyBuffers;
    PFNEGLCREATECONTEXTPROC eglCreateContext;
    PFNEGLCREATEPBUFFERSURFACEPROC eglCreatePbufferSurface;
    PFNEGLCREATEPIXMAPSURFACEPROC eglCreatePixmapSurface;
    PFNEGLCREATEWINDOWSURFACEPROC eglCreateWindowSurface;
    PFNEGLDESTROYCONTEXTPROC eglDestroyContext;
    PFNEGLDESTROYSURFACEPROC eglDestroySurface;
    PFNEGLGETCONFIGATTRIBPROC eglGetConfigAttrib;
    PFNEGLGETCONFIGSPROC eglGetConfigs;
    PFNEGLGETCURRENTDISPLAYPROC eglGetCurrentDisplay;
    PFNEGLGETCURRENTSURFACEPROC eglGetCurrentSurface;
    PFNEGLGETDISPLAYPROC eglGetDisplay;
    PFNEGLGETERRORPROC eglGetError;
    PFNEGLGETPROCADDRESSPROC eglGetProcAddress;
    PFNEGLINITIALIZEPROC eglInitialize;
    PFNEGLMAKECURRENTPROC eglMakeCurrent;
    PFNEGLQUERYCONTEXTPROC eglQueryContext;
    PFNEGLQUERYSTRINGPROC eglQueryString;
    PFNEGLQUERYSURFACEPROC eglQuerySurface;
    PFNEGLSWAPBUFFERSPROC eglSwapBuffers;
    PFNEGLTERMINATEPROC eglTerminate;
    PFNEGLWAITGLPROC eglWaitGL;
    PFNEGLWAITNATIVEPROC eglWaitNative;

    //EGL_VERSION_1_1
    PFNEGLBINDTEXIMAGEPROC eglBindTexImage;
    PFNEGLRELEASETEXIMAGEPROC eglReleaseTexImage;
    PFNEGLSURFACEATTRIBPROC eglSurfaceAttrib;
    PFNEGLSWAPINTERVALPROC eglSwapInterval;

    //EGL_VERSION_1_2
    PFNEGLBINDAPIPROC eglBindAPI;
    PFNEGLQUERYAPIPROC eglQueryAPI;
    PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC eglCreatePbufferFromClientBuffer;
    PFNEGLRELEASETHREADPROC eglReleaseThread;
    PFNEGLWAITCLIENTPROC eglWaitClient;

    //EGL_VERSION_1_4
    PFNEGLGETCURRENTCONTEXTPROC eglGetCurrentContext;

    //EGL_VERSION_1_5
    PFNEGLCREATESYNCPROC eglCreateSync;
    PFNEGLDESTROYSYNCPROC eglDestroySync;
    PFNEGLCLIENTWAITSYNCPROC eglClientWaitSync;
    PFNEGLGETSYNCATTRIBPROC eglGetSyncAttrib;
    PFNEGLCREATEIMAGEPROC eglCreateImage;
    PFNEGLDESTROYIMAGEPROC eglDestroyImage;
    PFNEGLGETPLATFORMDISPLAYPROC eglGetPlatformDisplay;
    PFNEGLCREATEPLATFORMWINDOWSURFACEPROC eglCreatePlatformWindowSurface;
    PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC eglCreatePlatformPixmapSurface;
    PFNEGLWAITSYNCPROC eglWaitSync;
};

typedef __eglMustCastToProperFunctionPointerType (*EGL_LOAD)(const char* );

void EGL_init_1_0(eglAPI& egl, EGL_LOAD load)
{
    //EGL_VERSION_1_0
    egl.eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)load("eglChooseConfig");
    egl.eglCopyBuffers = (PFNEGLCOPYBUFFERSPROC)load("eglCopyBuffers");
    egl.eglCreateContext = (PFNEGLCREATECONTEXTPROC)load("eglCreateContext");
    egl.eglCreatePbufferSurface = (PFNEGLCREATEPBUFFERSURFACEPROC)load("eglCreatePbufferSurface");
    egl.eglCreatePixmapSurface = (PFNEGLCREATEPIXMAPSURFACEPROC)load("eglCreatePixmapSurface");
    egl.eglCreateWindowSurface = (PFNEGLCREATEWINDOWSURFACEPROC)load("eglCreateWindowSurface");
    egl.eglDestroyContext = (PFNEGLDESTROYCONTEXTPROC)load("eglDestroyContext");
    egl.eglDestroySurface = (PFNEGLDESTROYSURFACEPROC)load("eglDestroySurface");
    egl.eglGetConfigAttrib = (PFNEGLGETCONFIGATTRIBPROC)load("eglGetConfigAttrib");
    egl.eglGetConfigs = (PFNEGLGETCONFIGSPROC)load("eglGetConfigs");
    egl.eglGetCurrentDisplay = (PFNEGLGETCURRENTDISPLAYPROC)load("eglGetCurrentDisplay");
    egl.eglGetCurrentSurface = (PFNEGLGETCURRENTSURFACEPROC)load("eglGetCurrentSurface");
    egl.eglGetDisplay = (PFNEGLGETDISPLAYPROC)load("eglGetDisplay");
    egl.eglGetError = (PFNEGLGETERRORPROC)load("eglGetError");
    egl.eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)load("eglGetProcAddress");
    egl.eglInitialize = (PFNEGLINITIALIZEPROC)load("eglInitialize");
    egl.eglMakeCurrent = (PFNEGLMAKECURRENTPROC)load("eglMakeCurrent");
    egl.eglQueryContext = (PFNEGLQUERYCONTEXTPROC)load("eglQueryContext");
    egl.eglQueryString = (PFNEGLQUERYSTRINGPROC)load("eglQueryString");
    egl.eglQuerySurface = (PFNEGLQUERYSURFACEPROC)load("eglQuerySurface");
    egl.eglSwapBuffers = (PFNEGLSWAPBUFFERSPROC)load("eglSwapBuffers");
    egl.eglTerminate = (PFNEGLTERMINATEPROC)load("eglTerminate");
    egl.eglWaitGL = (PFNEGLWAITGLPROC)load("eglWaitGL");
    egl.eglWaitNative = (PFNEGLWAITNATIVEPROC)load("eglWaitNative");
}

void EGL_init_1_1(eglAPI& egl, EGL_LOAD load)
{
    //EGL_VERSION_1_1
    egl.eglBindTexImage = (PFNEGLBINDTEXIMAGEPROC)load("eglBindTexImage");
    egl.eglReleaseTexImage = (PFNEGLRELEASETEXIMAGEPROC)load("eglReleaseTexImage");
    egl.eglSurfaceAttrib = (PFNEGLSURFACEATTRIBPROC)load("eglSurfaceAttrib");
    egl.eglSwapInterval = (PFNEGLSWAPINTERVALPROC)load("eglSwapInterval");
}

void EGL_init_1_2(eglAPI& egl, EGL_LOAD load)
{
    //EGL_VERSION_1_2
    egl.eglBindAPI = (PFNEGLBINDAPIPROC)load("eglBindAPI");
    egl.eglQueryAPI = (PFNEGLQUERYAPIPROC)load("eglQueryAPI");
    egl.eglCreatePbufferFromClientBuffer = (PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC)load("eglCreatePbufferFromClientBuffer");
    egl.eglReleaseThread = (PFNEGLRELEASETHREADPROC)load("eglReleaseThread");
    egl.eglWaitClient = (PFNEGLWAITCLIENTPROC)load("eglWaitClient");
}

void EGL_init_1_4(eglAPI& egl, EGL_LOAD load)
{
    //EGL_VERSION_1_4
    egl.eglGetCurrentContext = (PFNEGLGETCURRENTCONTEXTPROC)load("eglGetCurrentContext");
}

void EGL_init_1_5(eglAPI& egl, EGL_LOAD load)
{
    //EGL_VERSION_1_5
    egl.eglCreateSync = (PFNEGLCREATESYNCPROC)load("eglCreateSync");
    egl.eglDestroySync = (PFNEGLDESTROYSYNCPROC)load("eglDestroySync");
    egl.eglClientWaitSync = (PFNEGLCLIENTWAITSYNCPROC)load("eglClientWaitSync");
    egl.eglGetSyncAttrib = (PFNEGLGETSYNCATTRIBPROC)load("eglGetSyncAttrib");
    egl.eglCreateImage = (PFNEGLCREATEIMAGEPROC)load("eglCreateImage");
    egl.eglDestroyImage = (PFNEGLDESTROYIMAGEPROC)load("eglDestroyImage");
    egl.eglGetPlatformDisplay = (PFNEGLGETPLATFORMDISPLAYPROC)load("eglGetPlatformDisplay");
    egl.eglCreatePlatformWindowSurface = (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)load("eglCreatePlatformWindowSurface");
    egl.eglCreatePlatformPixmapSurface = (PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC)load("eglCreatePlatformPixmapSurface");
    egl.eglWaitSync = (PFNEGLWAITSYNCPROC)load("eglWaitSync");
}

#ifdef __cplusplus
}//extern "C"
#endif



CGL_PUBLIC_DECLARE eglAPI &libEGL = cgl::single<eglAPI>::value;



//EGL_VERSION_1_0
inline EGLAPI EGLBoolean EGLAPIENTRY eglChooseConfig (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    return libEGL.eglChooseConfig(dpy, attrib_list, configs, config_size, num_config);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglCopyBuffers (EGLDisplay dpy, EGLSurface surface, EGLNativePixmapType target)
{
    return libEGL.eglCopyBuffers(dpy, surface, target);
}

inline EGLAPI EGLContext EGLAPIENTRY eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_context, const EGLint *attrib_list)
{
    return libEGL.eglCreateContext(dpy, config, share_context, attrib_list);
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list)
{
    return libEGL.eglCreatePbufferSurface(dpy, config, attrib_list);
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreatePixmapSurface (EGLDisplay dpy, EGLConfig config, EGLNativePixmapType pixmap, const EGLint *attrib_list)
{
    return libEGL.eglCreatePixmapSurface(dpy, config, pixmap, attrib_list);
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreateWindowSurface (EGLDisplay dpy, EGLConfig config, EGLNativeWindowType win, const EGLint *attrib_list)
{
    return libEGL.eglCreateWindowSurface(dpy, config, win, attrib_list);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglDestroyContext (EGLDisplay dpy, EGLContext ctx)
{
    return libEGL.eglDestroyContext(dpy, ctx);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglDestroySurface (EGLDisplay dpy, EGLSurface surface)
{
    return libEGL.eglDestroySurface(dpy, surface);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigAttrib (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value)
{
    return libEGL.eglGetConfigAttrib(dpy, config, attribute, value);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglGetConfigs (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config)
{
    return libEGL.eglGetConfigs(dpy, configs, config_size, num_config);
}

inline EGLAPI EGLDisplay EGLAPIENTRY eglGetCurrentDisplay (void)
{
    return libEGL.eglGetCurrentDisplay();
}

inline EGLAPI EGLSurface EGLAPIENTRY eglGetCurrentSurface (EGLint readdraw)
{
    return libEGL.eglGetCurrentSurface(readdraw);
}

inline EGLAPI EGLDisplay EGLAPIENTRY eglGetDisplay (EGLNativeDisplayType display_id)
{
    return libEGL.eglGetDisplay(display_id);
}

inline EGLAPI EGLint EGLAPIENTRY eglGetError (void)
{
    return libEGL.eglGetError();
}

inline EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY eglGetProcAddress (const char *procname)
{
    return libEGL.eglGetProcAddress(procname);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor)
{
    return libEGL.eglInitialize(dpy, major, minor);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx)
{
    return libEGL.eglMakeCurrent(dpy, draw, read, ctx);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglQueryContext (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value)
{
    return libEGL.eglQueryContext(dpy, ctx, attribute, value);
}

inline EGLAPI const char *EGLAPIENTRY eglQueryString (EGLDisplay dpy, EGLint name)
{
    return libEGL.eglQueryString(dpy, name);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglQuerySurface (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value)
{
    return libEGL.eglQuerySurface(dpy, surface, attribute, value);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglSwapBuffers (EGLDisplay dpy, EGLSurface surface)
{
    return libEGL.eglSwapBuffers(dpy, surface);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglTerminate (EGLDisplay dpy)
{
    return libEGL.eglTerminate(dpy);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglWaitGL (void)
{
    return libEGL.eglWaitGL();
}

EGLAPI EGLBoolean EGLAPIENTRY eglWaitNative (EGLint engine)
{
    return libEGL.eglWaitNative(engine);
}

//EGL_VERSION_1_1
inline EGLAPI EGLBoolean EGLAPIENTRY eglBindTexImage (EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    return libEGL.eglBindTexImage(dpy, surface, buffer);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglReleaseTexImage (EGLDisplay dpy, EGLSurface surface, EGLint buffer)
{
    return libEGL.eglReleaseTexImage(dpy, surface, buffer);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglSurfaceAttrib (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value)
{
    return libEGL.eglSurfaceAttrib(dpy, surface, attribute, value);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglSwapInterval (EGLDisplay dpy, EGLint interval)
{
    return libEGL.eglSwapInterval(dpy, interval);
}

//EGL_VERSION_1_2
inline EGLAPI EGLBoolean EGLAPIENTRY eglBindAPI (EGLenum api)
{
    return libEGL.eglBindAPI(api);
}

inline EGLAPI EGLenum EGLAPIENTRY eglQueryAPI (void)
{
    return libEGL.eglQueryAPI();
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreatePbufferFromClientBuffer (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list)
{
    return libEGL.eglCreatePbufferFromClientBuffer(dpy, buftype, buffer, config, attrib_list);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglReleaseThread (void)
{
    return libEGL.eglReleaseThread();
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglWaitClient (void)
{
    return libEGL.eglWaitClient();
}

//EGL_VERSION_1_4
inline EGLAPI EGLContext EGLAPIENTRY eglGetCurrentContext (void)
{
    return libEGL.eglGetCurrentContext();
}

//EGL_VERSION_1_5
inline EGLAPI EGLSync EGLAPIENTRY eglCreateSync (EGLDisplay dpy, EGLenum type, const EGLAttrib *attrib_list)
{
    return libEGL.eglCreateSync(dpy, type, attrib_list);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglDestroySync (EGLDisplay dpy, EGLSync sync)
{
    return libEGL.eglDestroySync(dpy, sync);
}

inline EGLAPI EGLint EGLAPIENTRY eglClientWaitSync (EGLDisplay dpy, EGLSync sync, EGLint flags, EGLTime timeout)
{
    return libEGL.eglClientWaitSync(dpy, sync, flags, timeout);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglGetSyncAttrib (EGLDisplay dpy, EGLSync sync, EGLint attribute, EGLAttrib *value)
{
    return libEGL.eglGetSyncAttrib(dpy, sync, attribute, value);
}

inline EGLAPI EGLImage EGLAPIENTRY eglCreateImage (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLAttrib *attrib_list)
{
    return libEGL.eglCreateImage(dpy, ctx, target, buffer, attrib_list);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglDestroyImage (EGLDisplay dpy, EGLImage image)
{
    return libEGL.eglDestroyImage(dpy, image);
}

inline EGLAPI EGLDisplay EGLAPIENTRY eglGetPlatformDisplay (EGLenum platform, void *native_display, const EGLAttrib *attrib_list)
{
    if(libEGL.eglGetPlatformDisplay){
        return libEGL.eglGetPlatformDisplay(platform, native_display, attrib_list);
    }
    return NULL;
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreatePlatformWindowSurface (EGLDisplay dpy, EGLConfig config, void *native_window, const EGLAttrib *attrib_list)
{
    return libEGL.eglCreatePlatformWindowSurface(dpy, config, native_window, attrib_list);
}

inline EGLAPI EGLSurface EGLAPIENTRY eglCreatePlatformPixmapSurface (EGLDisplay dpy, EGLConfig config, void *native_pixmap, const EGLAttrib *attrib_list)
{
    return libEGL.eglCreatePlatformPixmapSurface(dpy, config, native_pixmap, attrib_list);
}

inline EGLAPI EGLBoolean EGLAPIENTRY eglWaitSync (EGLDisplay dpy, EGLSync sync, EGLint flags)
{
    return libEGL.eglWaitSync(dpy, sync, flags);
}

//
// libEGL_init()
//

namespace cgl{

#if 1

__eglMustCastToProperFunctionPointerType libEGL_load(const char* name)
{
    static cgl::shared_obj soEGL;
    static cgl::shared_obj soGLESv2;
    if(!soEGL.handle){
        soGLESv2.open("libGLESv2.dll");
        soEGL.open("libEGL.dll");
    }
    return (__eglMustCastToProperFunctionPointerType)soEGL.load(name);
}

int libEGL_init()
{
    EGL_init_1_0(libEGL, libEGL_load);
    EGL_init_1_1(libEGL, libEGL_load);
    EGL_init_1_2(libEGL, libEGL_load);
    EGL_init_1_4(libEGL, libEGL_load);
    EGL_init_1_5(libEGL, libEGL_load);
    return 0;
}

#else

int libEGL_init()
{
    return libEGL<>::init();
}

#endif

}//end namespace cgl

#endif //LIBEGL_HPP_20200128014936


/*
c++ º¯Êý×ª»»Ê§°Ü
template<typename T = void>
class libEGL
{
public:
    static cgl::shared_obj so;

    //EGL_VERSION_1_0
    static PFNEGLCHOOSECONFIGPROC eglChooseConfig;
    static PFNEGLCOPYBUFFERSPROC eglCopyBuffers;
    static PFNEGLCREATECONTEXTPROC eglCreateContext;
    static PFNEGLCREATEPBUFFERSURFACEPROC eglCreatePbufferSurface;
    static PFNEGLCREATEPIXMAPSURFACEPROC eglCreatePixmapSurface;
    static PFNEGLCREATEWINDOWSURFACEPROC eglCreateWindowSurface;
    static PFNEGLDESTROYCONTEXTPROC eglDestroyContext;
    static PFNEGLDESTROYSURFACEPROC eglDestroySurface;
    static PFNEGLGETCONFIGATTRIBPROC eglGetConfigAttrib;
    static PFNEGLGETCONFIGSPROC eglGetConfigs;
    static PFNEGLGETCURRENTDISPLAYPROC eglGetCurrentDisplay;
    static PFNEGLGETCURRENTSURFACEPROC eglGetCurrentSurface;
    static PFNEGLGETDISPLAYPROC eglGetDisplay;
    static PFNEGLGETERRORPROC eglGetError;
    static PFNEGLGETPROCADDRESSPROC eglGetProcAddress;
    static PFNEGLINITIALIZEPROC eglInitialize;
    static PFNEGLMAKECURRENTPROC eglMakeCurrent;
    static PFNEGLQUERYCONTEXTPROC eglQueryContext;
    static PFNEGLQUERYSTRINGPROC eglQueryString;
    static PFNEGLQUERYSURFACEPROC eglQuerySurface;
    static PFNEGLSWAPBUFFERSPROC eglSwapBuffers;
    static PFNEGLTERMINATEPROC eglTerminate;
    static PFNEGLWAITGLPROC eglWaitGL;
    static PFNEGLWAITNATIVEPROC eglWaitNative;

    //EGL_VERSION_1_1
    static PFNEGLBINDTEXIMAGEPROC eglBindTexImage;
    static PFNEGLRELEASETEXIMAGEPROC eglReleaseTexImage;
    static PFNEGLSURFACEATTRIBPROC eglSurfaceAttrib;
    static PFNEGLSWAPINTERVALPROC eglSwapInterval;

    //EGL_VERSION_1_2
    static PFNEGLBINDAPIPROC eglBindAPI;
    static PFNEGLQUERYAPIPROC eglQueryAPI;
    static PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC eglCreatePbufferFromClientBuffer;
    static PFNEGLRELEASETHREADPROC eglReleaseThread;
    static PFNEGLWAITCLIENTPROC eglWaitClient;

    //EGL_VERSION_1_4
    static PFNEGLGETCURRENTCONTEXTPROC eglGetCurrentContext;

    //EGL_VERSION_1_5
    static PFNEGLCREATESYNCPROC eglCreateSync;
    static PFNEGLDESTROYSYNCPROC eglDestroySync;
    static PFNEGLCLIENTWAITSYNCPROC eglClientWaitSync;
    static PFNEGLGETSYNCATTRIBPROC eglGetSyncAttrib;
    static PFNEGLCREATEIMAGEPROC eglCreateImage;
    static PFNEGLDESTROYIMAGEPROC eglDestroyImage;
    static PFNEGLGETPLATFORMDISPLAYPROC eglGetPlatformDisplay;
    static PFNEGLCREATEPLATFORMWINDOWSURFACEPROC eglCreatePlatformWindowSurface;
    static PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC eglCreatePlatformPixmapSurface;
    static PFNEGLWAITSYNCPROC eglWaitSync;

public:
    libEGL()
    {

    }

    static int init()
    {
        #ifdef CGL_PLATFORM_WINDOWS
        so.open("libEGL.dll");
        #else
        so.open("libEGL.so");
        #endif
        init_EGL_1_0();
        init_EGL_1_1();
        init_EGL_1_2();
        init_EGL_1_4();
        init_EGL_1_5();
        return 0;
    }

    static void init_EGL_1_0()
    {
        //EGL_VERSION_1_0
        HMODULE dll = LoadLibrary("libegl.dll");
        //shared_obj::function_type
        FARPROC func_ptr = GetProcAddress(dll, "eglChooseConfig");
        //eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)GetProcAddress(dll, "eglChooseConfig");// so.load("eglChooseConfig");//(PFNEGLCHOOSECONFIGPROC)func_ptr;//  (PFNEGLCHOOSECONFIGPROC)so.load("eglChooseConfig");
        //  (PFNEGLCHOOSECONFIGPROC)so.load("eglChooseConfig");
        eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)func_ptr;
        eglCopyBuffers = (PFNEGLCOPYBUFFERSPROC)so.load("eglCopyBuffers");
        eglCreateContext = (PFNEGLCREATECONTEXTPROC)so.load("eglCreateContext");
        eglCreatePbufferSurface = (PFNEGLCREATEPBUFFERSURFACEPROC)so.load("eglCreatePbufferSurface");
        eglCreatePixmapSurface = (PFNEGLCREATEPIXMAPSURFACEPROC)so.load("eglCreatePixmapSurface");
        eglCreateWindowSurface = (PFNEGLCREATEWINDOWSURFACEPROC)so.load("eglCreateWindowSurface");
        eglDestroyContext = (PFNEGLDESTROYCONTEXTPROC)so.load("eglDestroyContext");
        eglDestroySurface = (PFNEGLDESTROYSURFACEPROC)so.load("eglDestroySurface");
        eglGetConfigAttrib = (PFNEGLGETCONFIGATTRIBPROC)so.load("eglGetConfigAttrib");
        eglGetConfigs = (PFNEGLGETCONFIGSPROC)so.load("eglGetConfigs");
        eglGetCurrentDisplay = (PFNEGLGETCURRENTDISPLAYPROC)so.load("eglGetCurrentDisplay");
        eglGetCurrentSurface = (PFNEGLGETCURRENTSURFACEPROC)so.load("eglGetCurrentSurface");
        eglGetDisplay = (PFNEGLGETDISPLAYPROC)so.load("eglGetDisplay");
        eglGetError = (PFNEGLGETERRORPROC)so.load("eglGetError");
        eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)so.load("eglGetProcAddress");
        eglInitialize = (PFNEGLINITIALIZEPROC)so.load("eglInitialize");
        eglMakeCurrent = (PFNEGLMAKECURRENTPROC)so.load("eglMakeCurrent");
        eglQueryContext = (PFNEGLQUERYCONTEXTPROC)so.load("eglQueryContext");
        eglQueryString = (PFNEGLQUERYSTRINGPROC)so.load("eglQueryString");
        eglQuerySurface = (PFNEGLQUERYSURFACEPROC)so.load("eglQuerySurface");
        eglSwapBuffers = (PFNEGLSWAPBUFFERSPROC)so.load("eglSwapBuffers");
        eglTerminate = (PFNEGLTERMINATEPROC)so.load("eglTerminate");
        eglWaitGL = (PFNEGLWAITGLPROC)so.load("eglWaitGL");
        eglWaitNative = (PFNEGLWAITNATIVEPROC)so.load("eglWaitNative");
    }

    static void init_EGL_1_1()
    {
        //EGL_VERSION_1_1
        eglBindTexImage = (PFNEGLBINDTEXIMAGEPROC)so.load("eglBindTexImage");
        eglReleaseTexImage = (PFNEGLRELEASETEXIMAGEPROC)so.load("eglReleaseTexImage");
        eglSurfaceAttrib = (PFNEGLSURFACEATTRIBPROC)so.load("eglSurfaceAttrib");
        eglSwapInterval = (PFNEGLSWAPINTERVALPROC)so.load("eglSwapInterval");
    }

    static void init_EGL_1_2()
    {
        //EGL_VERSION_1_2
        eglBindAPI = (PFNEGLBINDAPIPROC)so.load("eglBindAPI");
        eglQueryAPI = (PFNEGLQUERYAPIPROC)so.load("eglQueryAPI");
        eglCreatePbufferFromClientBuffer = (PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC)so.load("eglCreatePbufferFromClientBuffer");
        eglReleaseThread = (PFNEGLRELEASETHREADPROC)so.load("eglReleaseThread");
        eglWaitClient = (PFNEGLWAITCLIENTPROC)so.load("eglWaitClient");
    }

    static void init_EGL_1_4()
    {
        //EGL_VERSION_1_4
        eglGetCurrentContext = (PFNEGLGETCURRENTCONTEXTPROC)so.load("eglGetCurrentContext");
    }

    static void init_EGL_1_5()
    {
        //EGL_VERSION_1_5
        eglCreateSync = (PFNEGLCREATESYNCPROC)so.load("eglCreateSync");
        eglDestroySync = (PFNEGLDESTROYSYNCPROC)so.load("eglDestroySync");
        eglClientWaitSync = (PFNEGLCLIENTWAITSYNCPROC)so.load("eglClientWaitSync");
        eglGetSyncAttrib = (PFNEGLGETSYNCATTRIBPROC)so.load("eglGetSyncAttrib");
        eglCreateImage = (PFNEGLCREATEIMAGEPROC)so.load("eglCreateImage");
        eglDestroyImage = (PFNEGLDESTROYIMAGEPROC)so.load("eglDestroyImage");
        eglGetPlatformDisplay = (PFNEGLGETPLATFORMDISPLAYPROC)so.load("eglGetPlatformDisplay");
        eglCreatePlatformWindowSurface = (PFNEGLCREATEPLATFORMWINDOWSURFACEPROC)so.load("eglCreatePlatformWindowSurface");
        eglCreatePlatformPixmapSurface = (PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC)so.load("eglCreatePlatformPixmapSurface");
        eglWaitSync = (PFNEGLWAITSYNCPROC)so.load("eglWaitSync");
    }
};

template<typename T>
cgl::shared_obj libEGL<T>::so = cgl::shared_obj();

//EGL_VERSION_1_0
template<typename T> PFNEGLCHOOSECONFIGPROC libEGL<T>::eglChooseConfig = NULL;
template<typename T> PFNEGLCOPYBUFFERSPROC libEGL<T>::eglCopyBuffers = NULL;
template<typename T> PFNEGLCREATECONTEXTPROC libEGL<T>::eglCreateContext = NULL;
template<typename T> PFNEGLCREATEPBUFFERSURFACEPROC libEGL<T>::eglCreatePbufferSurface = NULL;
template<typename T> PFNEGLCREATEPIXMAPSURFACEPROC libEGL<T>::eglCreatePixmapSurface = NULL;
template<typename T> PFNEGLCREATEWINDOWSURFACEPROC libEGL<T>::eglCreateWindowSurface = NULL;
template<typename T> PFNEGLDESTROYCONTEXTPROC libEGL<T>::eglDestroyContext = NULL;
template<typename T> PFNEGLDESTROYSURFACEPROC libEGL<T>::eglDestroySurface = NULL;
template<typename T> PFNEGLGETCONFIGATTRIBPROC libEGL<T>::eglGetConfigAttrib = NULL;
template<typename T> PFNEGLGETCONFIGSPROC libEGL<T>::eglGetConfigs = NULL;
template<typename T> PFNEGLGETCURRENTDISPLAYPROC libEGL<T>::eglGetCurrentDisplay = NULL;
template<typename T> PFNEGLGETCURRENTSURFACEPROC libEGL<T>::eglGetCurrentSurface = NULL;
template<typename T> PFNEGLGETDISPLAYPROC libEGL<T>::eglGetDisplay = NULL;
template<typename T> PFNEGLGETERRORPROC libEGL<T>::eglGetError = NULL;
template<typename T> PFNEGLGETPROCADDRESSPROC libEGL<T>::eglGetProcAddress = NULL;
template<typename T> PFNEGLINITIALIZEPROC libEGL<T>::eglInitialize = NULL;
template<typename T> PFNEGLMAKECURRENTPROC libEGL<T>::eglMakeCurrent = NULL;
template<typename T> PFNEGLQUERYCONTEXTPROC libEGL<T>::eglQueryContext = NULL;
template<typename T> PFNEGLQUERYSTRINGPROC libEGL<T>::eglQueryString = NULL;
template<typename T> PFNEGLQUERYSURFACEPROC libEGL<T>::eglQuerySurface = NULL;
template<typename T> PFNEGLSWAPBUFFERSPROC libEGL<T>::eglSwapBuffers = NULL;
template<typename T> PFNEGLTERMINATEPROC libEGL<T>::eglTerminate = NULL;
template<typename T> PFNEGLWAITGLPROC libEGL<T>::eglWaitGL = NULL;
template<typename T> PFNEGLWAITNATIVEPROC libEGL<T>::eglWaitNative = NULL;

//EGL_VERSION_1_1
template<typename T> PFNEGLBINDTEXIMAGEPROC libEGL<T>::eglBindTexImage = NULL;
template<typename T> PFNEGLRELEASETEXIMAGEPROC libEGL<T>::eglReleaseTexImage = NULL;
template<typename T> PFNEGLSURFACEATTRIBPROC libEGL<T>::eglSurfaceAttrib = NULL;
template<typename T> PFNEGLSWAPINTERVALPROC libEGL<T>::eglSwapInterval = NULL;

//EGL_VERSION_1_2
template<typename T> PFNEGLBINDAPIPROC libEGL<T>::eglBindAPI = NULL;
template<typename T> PFNEGLQUERYAPIPROC libEGL<T>::eglQueryAPI = NULL;
template<typename T> PFNEGLCREATEPBUFFERFROMCLIENTBUFFERPROC libEGL<T>::eglCreatePbufferFromClientBuffer = NULL;
template<typename T> PFNEGLRELEASETHREADPROC libEGL<T>::eglReleaseThread = NULL;
template<typename T> PFNEGLWAITCLIENTPROC libEGL<T>::eglWaitClient = NULL;

//EGL_VERSION_1_4
template<typename T> PFNEGLGETCURRENTCONTEXTPROC libEGL<T>::eglGetCurrentContext = NULL;

//EGL_VERSION_1_5
template<typename T> PFNEGLCREATESYNCPROC libEGL<T>::eglCreateSync = NULL;
template<typename T> PFNEGLDESTROYSYNCPROC libEGL<T>::eglDestroySync = NULL;
template<typename T> PFNEGLCLIENTWAITSYNCPROC libEGL<T>::eglClientWaitSync = NULL;
template<typename T> PFNEGLGETSYNCATTRIBPROC libEGL<T>::eglGetSyncAttrib = NULL;
template<typename T> PFNEGLCREATEIMAGEPROC libEGL<T>::eglCreateImage = NULL;
template<typename T> PFNEGLDESTROYIMAGEPROC libEGL<T>::eglDestroyImage = NULL;
template<typename T> PFNEGLGETPLATFORMDISPLAYPROC libEGL<T>::eglGetPlatformDisplay = NULL;
template<typename T> PFNEGLCREATEPLATFORMWINDOWSURFACEPROC libEGL<T>::eglCreatePlatformWindowSurface = NULL;
template<typename T> PFNEGLCREATEPLATFORMPIXMAPSURFACEPROC libEGL<T>::eglCreatePlatformPixmapSurface = NULL;
template<typename T> PFNEGLWAITSYNCPROC libEGL<T>::eglWaitSync = NULL;

*/
