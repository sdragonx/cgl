/*

 win32app.cpp

 sdragonx 2018-08-02 03:06:02

*/
#ifndef WIN32APP_CPP_20180802030602
#define WIN32APP_CPP_20180802030602

#include "core.hpp"
#include "win32app.hpp"

namespace cgl{
namespace frame{

native_app::native_app() : m_is_run(true)
{
}

tstring_t native_app::exename()
{
	TCHAR buf[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf, MAX_PATH);
	return tstring_t(buf);
}

tstring_t native_app::path()
{
	TCHAR buf[MAX_PATH] = {0};
	GetModuleFileName(NULL, buf, MAX_PATH);
	for(int i=cslen(buf); i>0; --i)
	{
		if(buf[i]=='\\' || buf[i]=='/')
		{
			buf[i] = '\0';
			break;
		}
	}
	return tstring_t(buf);
}

int native_app::run()
{
	/*
	MSG msg = {0};
	while(true){
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		CGL_MAINFRAME.process();
		CGL_MAINFRAME.draw_frame();
	}
	return msg.wParam;
	*/
	return 0;
}

void native_app::process_message()
{
	while(PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
	}
}

void native_app::exit()
{
	m_is_run = false;
	PostQuitMessage(0);
}

int native_app::msgbox(const TCHAR* msg, const TCHAR* title, int type)
{
	HWND hwnd = 0;//m_main ? m_main : handle();
	return MessageBox(hwnd, msg, title, type);
}

}//end namespace frame
}//end namespace cgl

#endif //WIN32APP_CPP_20180802030602
