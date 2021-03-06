/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 core.cpp

 2018-07-17 22:48:50

*/
#ifndef CORE_CPP_20180717224850
#define CORE_CPP_20180717224850

#include <cgl/frame/core.hpp>
#include <cgl/graphics/font.hpp>
#include <cgl/ui/console.hpp>

extern cgl::uniframe luna;

namespace cgl{

#ifndef CGL_CUSTOM_PRINT

void __cgl_print(const char* text)
{
    luna.print(text);
}

void __cgl_print(const wchar_t* text)
{
    luna.print(text);
}

#ifdef CGL_DEBUG
void _debug_print(const char* text, const char* file, int line)
{
    char buf[1024];
    snprintf(buf, 1024, "%08X %s \t- %s(%i)", std::clock(), text, file, line);
    print_message(buf);
}
#endif//CGL_DEBUG

#endif//CGL_CUSTOM_PRINT


#ifndef CGL_PLATFORM_ANDROID
#if 0
template<int PLATFORM>
uniframe& uniframe<PLATFORM>::instance()
{
    static uniframe _frame;
    return _frame;
}
#endif
#endif//CGL_PLATFORM_ANDROID


template<int PLATFORM>
basic_uniframe<PLATFORM>::basic_uniframe() :
    system(),
    app(),
    window(app),
    cursor(),
    display(window),
    console(),
    gl(),
    ui(gl),
    audio(),
    store(),
    images(&gl),
    m_dc(gl)
{
    images.store = &store;
}

template<int PLATFORM>
basic_uniframe<PLATFORM>::~basic_uniframe()
{
    this->shutdown();
}

template<int PLATFORM>
int basic_uniframe<PLATFORM>::init(platform::instance instance,
    platform::display ntv_display,
    platform::window ntv_window)
{
    CGL_LOG("uniframe init.");

    if(ntv_window){
        this->window.set_window(ntv_window);
    }
    else{
        #ifdef CGL_PLATFORM_ANDROID
        this->window.create(L"luna", 0, 0, 1024, 576);
        #else
        this->window.create(L"luna", 0, 0, 1024, 576);
        #endif
        this->window.show(true);
    }

    #ifdef CGL_PLATFORM_WINDOWS
    this->window.m_gl = &gl;
    this->window.m_ui = &ui;
    #endif

    #ifndef CGL_NO_OPENGL
    this->gl.create(ntv_display, window.handle());
    #endif

    //setlocale(LC_ALL, "");    //系统默认代码页
    //setlocale(LC_ALL, "C");    //ASCII代码页，默认
    //setlocale(LC_ALL, "chs");    //GBK2312代码页

    //ui
    ui.set_context(&gl);

    //image store
    images.init();
    images.store = &store;

    //audio
	#ifndef CGL_NO_AUDIO
    audio.init();
	#endif

    //set float point
    //_controlfp(MCW_EM, MCW_EM);

    return 0;
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::shutdown()
{
    m_dc.init();
    ui.components.clear();
    #ifndef CGL_NO_OPENGL
    gl.dispose();
    #endif
    audio.dispose();
}

//template<int PLATFORM>
//void basic_uniframe<PLATFORM>::run()
//{
//    app.run();
//}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::update(float time)
{
    ui.update();
    audio.update();
}

template<int PLATFORM>
int basic_uniframe<PLATFORM>::draw_frame()
{
    m_dc.init();
    draw_console();
    ui::message msg(msg_paint, 0, &m_dc);
    ui.send_message(msg);
    gl.draw(m_dc);
    return 0;
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::draw_console()
{
    console.paint(&m_dc);
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::print(const string_t& text)
{
    console.print(text);
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::print(const string_t& title, const string_t& text)
{
    console.print(title + "   " + text);
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::print(const string_t& note, const string_t& title, const string_t& text)
{
    console.print(note + "   " + title + "    " + text);
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::show_message(const string_t& text)
{
    #ifdef CGL_PLATFORM_WINDOWS
    MessageBoxA(window.handle(), text.c_str(), CGL_FRAME_NAME, MB_OK);
    #endif
}

template<int PLATFORM>
void basic_uniframe<PLATFORM>::show_console()
{
    ui.create_window<ui::console>("luna.console", "console", 0, 0, 400, 300)->set_frame(this);
}

}//end namespace cgl

#endif //CORE_CPP_20180717224850
