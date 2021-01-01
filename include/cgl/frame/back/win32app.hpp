/*

 win32app.hpp

 sdragonx 2018-08-02 02:52:04

*/
#ifndef WIN32APP_HPP_20180802025204
#define WIN32APP_HPP_20180802025204

#include <windows.h>
#include <cgl/string.hpp>

namespace cgl{
namespace core{

template<typename T>
class app<CGL_WINDOWS, T>
{
public:
	typedef wchar_t char_type;
	typedef cstring<char_type> string_type;
private:
	bool m_is_run;
	MSG m_msg;
public:
	app() : m_is_run(true)
	{

	}

	string_type exename()
	{
		char_type buf[MAX_PATH] = {0};
		tvalue<char_type>(GetModuleFileNameA, GetModuleFileNameW)(NULL, buf, MAX_PATH);
		return string_type(buf);
	}

	string_type path()
	{
		char_type buf[] = {'/', '\\', '\0'};
		string_type dir = this->exename();
		dir.erase(dir.find_last_of(buf), -1);
		return dir;
	}

	int run()
	{
		MSG msg = {0};
		while(true){
			while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			//CGL_MAINFRAME.process();
			//CGL_MAINFRAME.draw_frame();
		}
		return msg.wParam;
		return 0;
	}

	void process_message()
	{
		while(PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}

	void exit()
	{
		m_is_run = false;
		PostQuitMessage(0);
	}

	int msgbox(const char_type* msg, const char_type* title, int type)
	{
		HWND hwnd = 0;//m_main ? m_main : handle();
		return tvalue<char_type>(MessageBoxA, MessageBoxW)(hwnd, msg, title, type);
	}

	bool is_run()const { return m_is_run; }
private:
	//template<typename F>
	//friend LRESULT CALLBACK win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	friend class cgl::core::window<CGL_WINDOWS, T>;
};

}//end namespace frame
}//end namespace cgl

#endif //WIN32APP_HPP_20180802025204
