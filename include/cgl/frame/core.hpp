/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 platform_base.hpp

 2018-05-24 15:53:25

*/
#ifndef PLATFORM_BASE_HPP_20180524155325
#define PLATFORM_BASE_HPP_20180524155325

#include <cgl/console.hpp>
#include <cgl/graphics.hpp>
#include <cgl/ui.hpp>

#include <cgl/audio/al.hpp>
#include <cgl/frame/window.hpp>
#include <cgl/frame/display.hpp>
#include <cgl/frame/system.hpp>
#include <cgl/graphics/image_store.hpp>
#include <cgl/io/zip_store.hpp>
#include <cgl/system/cursor.hpp>

#ifndef CGL_FRAME_NAME
  #define CGL_FRAME_NAME "luna"
#endif

namespace cgl{

#pragma pack(push, 1)
struct uniframe_state
{
    int use_audio : 1;
};
#pragma pack(pop)

template<int PLATFORM>
class basic_uniframe
{
public:
    typedef basic_uniframe this_type;

public:
    frame::native_system    system;
    frame::native_app       app;
    frame::native_window    window;
    system::cursor          cursor;
    frame::native_display   display;
    consoleline<tchar_t>    console;
    graphics::glcontext     gl;
    ui::winmain             ui;
    al::alcontext           audio;
    io::zip_store           store;
    graphics::image_store   images;
    struct {
        int images;
    }resource;

protected:
    graphics::draw_context m_dc;
    uniframe_state state;

public:
    basic_uniframe();
    ~basic_uniframe();

    int init(platform::instance instance, platform::display display, platform::window window);

    void shutdown();

//    void run();

    void update(float time = 0.0f);
    int draw_frame();

public:
    void print(const string_t& text);
    void print(const string_t& title, const string_t& text);
    void print(const string_t& note, const string_t& title, const string_t& text);
    void show_message(const string_t& text);
    void show_console();
private:
    void draw_console();
};

typedef basic_uniframe<CGL_PLATFORM> uniframe;

//void __cgl_init_instance(uniframe& frame);
//uniframe& __cgl_instance();
//graphics::unicontext* __cgl_graphics();

}//end namespace cgl

#endif //PLATFORM_BASE_HPP_20180524155325
