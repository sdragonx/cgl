/*

 win32_window.hpp

 sdragonx 2018-05-10 23:39:41

*/
#ifndef WIN32_WINDOW_HPP_20180510233941
#define WIN32_WINDOW_HPP_20180510233941

#include <cgl/public.h>
#include <windows.h>
#include <cgl/frame/core.hpp>
#include <cgl/frame/win32app.hpp>
#include <cgl/ui/win32_message.hpp>

namespace cgl{

template<int> class uniframe;

namespace core{

//template<typename T>
LRESULT CALLBACK win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

template<typename T>
class window<CGL_WINDOWS, T>
{
public:
	//typedef HWND
	typedef T char_type;
	typedef cstring<char_type> string_type;
private:
	HWND m_hwnd;
	DWORD m_winstyle;
	bool m_fullscreen;
	WNDPROC m_prevWndProc;
public:
	window() : m_hwnd(null), m_winstyle(), m_fullscreen(false), m_prevWndProc(null)
	{

	}
	~window()
	{

	}

	HWND handle()const { return m_hwnd; }

	int create(const char_type* title, int left, int top, int width, int height, int style = WS_OVERLAPPEDWINDOW)
	{
		const char_type* classname = TEXT("luna_window");
		regclass(classname, 0, 0);

		CREATESTRUCT cs = {0};
		cs.hInstance = GetModuleHandle(NULL);
		cs.hMenu = NULL;
		cs.hwndParent = NULL;
		cs.cy = height;
		cs.cx = width;
		cs.y  = top;
		cs.x  = left;
		cs.style = style;
		cs.lpszName = title;
		cs.lpszClass = classname;
		//cs.dwExStyle = stylex;

		//预创建消息
		//this->on_precreate(cs);

		//创建参数必须指向this
		cs.lpCreateParams = this;

		//创建窗口
		m_hwnd = CreateWindowEx(
			cs.dwExStyle,	//窗口的扩展风格
			cs.lpszClass,	//类名
			cs.lpszName,	//标题
			cs.style,		//风格
			cs.x,			//左边位置
			cs.y,			//顶部位置
			cs.cx,			//宽度
			cs.cy,			//高度
			cs.hwndParent,	//父窗口的句柄
			cs.hMenu,		//菜单的句柄或是子窗口的标识符
			cs.hInstance,	//应用程序实例的句柄
			cs.lpCreateParams);//指向窗口的创建数据

		if(m_hwnd == NULL)
		{
			MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
			return -1;
		}
		//this->set_icon(icon);
		//set_owner(winapp.handle());
		return 0;
	}

	void close()
	{
		//
	}

	int set_window(HWND hwnd)
	{
		//close()
		m_hwnd = hwnd;
		m_prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)win32_wndproc);
		return 0;
	}

	void show(bool bshow)
	{
		ShowWindow(handle(), bshow ? SW_SHOW : SW_HIDE);
	}

	int set_title(const string_type& title)
	{
		return SetWindowText(handle(), title.c_str());
	}

	string_type title()
	{
		char_type text[MAX_PATH] = {0};
		tvalue<char_type>(GetWindowTextA, GetWindowTextW)(handle(), text, MAX_PATH);
		return string_type(text);
	}

	int width()
	{
		return bounds().width;
	}

	int height()
	{
		return bounds().height;
	}

	vec4i bounds()
	{
		RECT rc;
		GetWindowRect(handle(), &rc);
		return vec4i(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	vec4i client()
	{
		RECT rc;
		GetClientRect(handle(), &rc);
		return vec4i(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
	}

	int move(int x, int y)
	{
		vec4i rc = bounds();
		MoveWindow(handle(), x, y, rc.width, rc.height, true);
		return 0;
	}

	int resize(int w, int h)
	{
		vec4i rc = bounds();
		MoveWindow(handle(), rc.x, rc.y, w, h, true);
		return 0;
	}

	void set_focus()
	{
		SetFocus(handle());
	}
private:
	int regclass(const char_type* cname, HICON icon, int style)
	{
		WNDCLASSEX wc = {0};
		int atom = 0;

		memset(&wc,0,sizeof(wc));
		wc.cbSize		 = sizeof(WNDCLASSEX);
		wc.style		 = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;//CS_DBLCLKS 支持鼠标双击事件
		wc.lpfnWndProc	 = win32_wndproc;//< uniframe<CGL_WINDOWS> >;
		wc.hInstance	 = GetModuleHandle(NULL);
		wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszClassName = cname;
		wc.hIcon 		 = icon;
		wc.hIconSm		 = NULL;

		//注册窗口类
		atom = RegisterClassEx(&wc);

		if(!atom)
		{
			MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
			return -1;
		}
		return atom;
	}

	static LRESULT CALLBACK win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

template<typename T>
LRESULT CALLBACK window<CGL_WINDOWS, T>::win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_QUIT:
		break;
	case WM_DESTROY:
		uniframe<CGL_WINDOWS>::instance().app.m_is_run = false;
		break;

	case WM_ERASEBKGND:
		return CGL_TRUE;//return not 0
	case WM_SIZE:
		uniframe<CGL_PLATFORM>::instance().gl.viewport(0, 0, LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_SETCURSOR://拦截光标消息
		//send_winmsg(&luna.ui, msg_cursor, 0, 0);
		//return 0;
		break;
	default:
		break;
	}

	send_winmsg(&uniframe<CGL_PLATFORM>::instance().ui, msg, wparam, lparam);

	if(uniframe<CGL_PLATFORM>::instance().window.m_prevWndProc){
		return CallWindowProc(uniframe<CGL_PLATFORM>::instance().window.m_prevWndProc, hwnd, msg, wparam, lparam);
	}
	else{
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}

}//end namespace frame
}//end namespace cgl


#endif //WIN32_WINDOW_HPP_20180510233941
