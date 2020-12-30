/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 source.hpp

 2018-07-16 15:07:34

*/

#ifdef CGL_PLATFORM_CBUILDER
    #pragma checkoption -vu
#endif

#ifdef CGL_PLATFORM_WINDOWS

// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf
// The following line is to favor the high performance NVIDIA GPU if there are multiple GPUs
// Has to be .exe module to be correctly detected.
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// And the AMD equivalent
// Also has to be .exe module to be correctly detected.
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;

#endif

//---------------------------------------------------------------------------
//
// system
//
#ifdef SYSTEM_HPP_20100829154028
//#include "system/system.cpp"
#endif

//---------------------------------------------------------------------------
//
// graphics
//

#ifdef GRAPHICS_FONT_HPP_20180718234739
//#include <cgl/core/font.cpp>
#endif

#ifdef GLCONTEXT_HPP_20180510212834

//#include <cgl/graphics/gl/glcontext.cpp>
//#include <cgl/graphics/gl/glfont.cpp>

#endif

//---------------------------------------------------------------------------
//
// ui
//

#ifdef CGL_UI_H_20150203230237

#include "ui/console.cpp"
#include "ui/form_window.cpp"
//#include "ui/widget.cpp"

#endif

//---------------------------------------------------------------------------
//
// uniframe
//
#ifdef PLATFORM_BASE_HPP_20180524155325

#include <cgl/frame/core.cpp>

#ifdef CGL_PLATFORM_WINDOWS
    //#include <cgl/frame/win32_window.cpp>
    //#include <cgl/frame/win32app.cpp>
    //#include <cgl/frame/win32_display.cpp>
#endif

#endif


/*******************************************************************************
 *
 * third party
 *
 */


#ifndef CGL_LIB_HPP_20180716150734
#define CGL_LIB_HPP_20180716150734

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option push
    #pragma option -w-8004        //is assigned a value that is never used
    #pragma option -w-8008        //Condition is always true
    #pragma option -w-8012        //Comparing signed and unsigned values
    #pragma option -w-8066        //Unreachable code
    #pragma option -w-8062        //Previous options and warnings not restored
#endif

#ifdef __glad_h_
    #ifdef CGL_PLATFORM_GLES
        #include <cgl/external/glad/libGLESv2.c>
    #else
        #include <cgl/external/glad/glad.c>
    #endif
#endif

#ifdef CGL_PLATFORM_CBUILDER
    #pragma option pop
#endif

//---------------------------------------------------------------------------

#else //CGL_LIB_HPP_20180716150734

#error cgl source.hpp only include once.

#endif //CGL_LIB_HPP_20180716150734


