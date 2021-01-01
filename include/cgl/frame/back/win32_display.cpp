/*

 display.cpp

 sdragonx 2018-07-17 22:23:57

*/
#ifndef DISPLAY_CPP_20180717222357
#define DISPLAY_CPP_20180717222357

#include <cgl/frame/core.hpp>
#include <cgl/frame/display.hpp>

namespace cgl{
namespace frame{

display::display()
{
	state.hwnd = null;
	state.winstyle = -1;//GetWindowLong(CGL_MAIN_FRAME.window.handle(), GWL_STYLE);
	state.fullscreen = false;
}

display_mode display::get_mode()
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

int display::set_mode(int width, int height, int colorbits, int refresh_rate, bool fullscreen)
{
	RECT rc = {0};
	state.fullscreen = fullscreen;
	HWND hwnd = uniframe<CGL_PLATFORM>::instance().window.handle();

	if(!hwnd || hwnd == INVALID_HANDLE_VALUE){
		return CGL_ERROR;
	}

	if(fullscreen)
	{
		DEVMODE devmode;	// 设备模式
		memset(&devmode, 0, sizeof(DEVMODE));
		devmode.dmSize = sizeof(DEVMODE); 	// Devmode 结构的大小
		devmode.dmPelsWidth  = width;		// 所选屏幕宽度
		devmode.dmPelsHeight = height;  	// 所选屏幕高度
		devmode.dmBitsPerPel = colorbits ? colorbits : 32;	// 每象素所选的色彩深度
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

int display::enum_display_mode(std::vector<display_mode>& ls)
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

}//end namespace frame
}//end namespace cgl

#endif //DISPLAY_CPP_20180717222357

