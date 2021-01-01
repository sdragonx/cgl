/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 platform_display.hpp

 2018-05-24 16:21:46

*/
#ifndef PLATFORM_DISPLAY_HPP_20180524162146
#define PLATFORM_DISPLAY_HPP_20180524162146

#include <vector>
#include <cgl/frame/window.hpp>

namespace cgl{

struct display_mode
{
    int width;
    int height;
    int bpp;        //color bits per pixel
    int frequency;    //refresh frequency
};

namespace frame{

//native display state

#ifdef CGL_PLATFORM_WINDOWS
struct display_state
{
    native_window* window;
    DWORD winstyle;
    bool  fullscreen;
};

#else

struct display_state
{
};

#endif



class native_display
{
private:
    display_state state;

public:
    native_display(native_window& window);
    display_mode mode();
    int set_mode(int width, int height, int colorbits, int refresh_rate, bool fullscreen);
    int enum_display_mode(std::vector<display_mode>& ls);
};



#ifdef CGL_PLATFORM_WINDOWS

native_display::native_display(native_window& window)
{
    state.window = &window;
    state.winstyle = -1;//GetWindowLong(CGL_MAIN_FRAME.window.handle(), GWL_STYLE);
    state.fullscreen = false;
}

display_mode native_display::mode()
{
    DEVMODE devmode;
    memset(&devmode, 0, sizeof (devmode));
    devmode.dmSize = sizeof(DEVMODE);
    devmode.dmDriverExtra = 0;

    display_mode dm = {0};

    EnumDisplaySettings(null, ENUM_CURRENT_SETTINGS, &devmode);

    dm.width = devmode.dmPelsWidth;
    dm.height = devmode.dmPelsHeight;
    dm.bpp = devmode.dmBitsPerPel;
    dm.frequency = devmode.dmDisplayFrequency;

    return dm;
}

int native_display::set_mode(int width, int height, int colorbits, int refresh_rate, bool fullscreen)
{
    RECT rc = {0};
    state.fullscreen = fullscreen;
    HWND hwnd = state.window->handle();// frame_type::instance().window.handle();

    if(!hwnd || hwnd == INVALID_HANDLE_VALUE){
        return CGL_ERROR;
    }

    if(fullscreen)
    {
        DEVMODE devmode;    // 设备模式
        memset(&devmode, 0, sizeof(DEVMODE));
        devmode.dmSize = sizeof(DEVMODE);     // Devmode 结构的大小
        devmode.dmPelsWidth  = width;        // 所选屏幕宽度
        devmode.dmPelsHeight = height;      // 所选屏幕高度
        devmode.dmBitsPerPel = colorbits ? colorbits : 32;    // 每象素所选的色彩深度
        devmode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
        state.winstyle = GetWindowLong(hwnd, GWL_STYLE);
        SetWindowLong(hwnd, GWL_STYLE, WS_POPUP);
        ChangeDisplaySettings(&devmode, CDS_FULLSCREEN);
        //SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
    }
    else
    {
        ChangeDisplaySettings(NULL, 0);//恢复显示模式
        if(state.winstyle != UINT32_MAX)SetWindowLong(hwnd, GWL_STYLE, state.winstyle);
        SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
    }
    ShowWindow(hwnd, SW_SHOW);
    GetClientRect(hwnd, &rc);
    return 0;
}

int native_display::enum_display_mode(std::vector<display_mode>& ls)
{
    DEVMODE devmode;
    memset(&devmode, 0, sizeof (devmode));
    devmode.dmSize = sizeof(DEVMODE);
    devmode.dmDriverExtra = 0;

    display_mode dm;
    for(int i=0; EnumDisplaySettings(0, i, &devmode); ++i)
    {
        dm.width = devmode.dmPelsWidth;
        dm.height = devmode.dmPelsHeight;
        dm.bpp = devmode.dmBitsPerPel;
        dm.frequency = devmode.dmDisplayFrequency;
        ls.push_back(dm);
    }
    return 0;
}

//CGL_PLATFORM_WINDOWS end
#else

//android
native_display::native_display(native_window& window)
{
}

#endif

}//end namespace frame
}//end namespace cgl

#endif //PLATFORM_DISPLAY_HPP_20180524162146
