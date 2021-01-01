/*

 win32_window.cpp

 sdragonx 2018-07-20 01:07:36

*/
#ifndef WIN32_WINDOW_CPP_20180720010736
#define WIN32_WINDOW_CPP_20180720010736

#include <cgl/frame/core.hpp>
#include <cgl/ui/win32_message.hpp>

namespace cgl{
namespace frame{

native_window::native_window():m_hwnd(null), m_winstyle(), m_fullscreen(false), m_prevWndProc(null)
{

}

native_window::~native_window()
{

}

//ע�ᴰ����
int native_window::regclass(LPCTSTR cname, HICON icon, int style)
{
	WNDCLASSEX wc = {0};
	int atom = 0;

	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;//CS_DBLCLKS ֧�����˫���¼�
	wc.lpfnWndProc	 = win32_wndproc;
	wc.hInstance	 = GetModuleHandle(NULL);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = cname;
	wc.hIcon 		 = icon;
	wc.hIconSm		 = NULL;

	//ע�ᴰ����
	atom = RegisterClassEx(&wc);

	if(!atom)
	{
		MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}
	return atom;
}

int native_window::create(LPCTSTR title, int left, int top, int width, int height, int style)
{
	const TCHAR* classname = TEXT("luna_window");
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

	//Ԥ������Ϣ
	//this->on_precreate(cs);

	//������������ָ��this
	cs.lpCreateParams = this;

	//��������
	m_hwnd = CreateWindowEx(
		cs.dwExStyle,	//���ڵ���չ���
		cs.lpszClass,	//����
		cs.lpszName,	//����
		cs.style,		//���
		cs.x,			//���λ��
		cs.y,			//����λ��
		cs.cx,			//���
		cs.cy,			//�߶�
		cs.hwndParent,	//�����ڵľ��
		cs.hMenu,		//�˵��ľ�������Ӵ��ڵı�ʶ��
		cs.hInstance,	//Ӧ�ó���ʵ���ľ��
		cs.lpCreateParams);//ָ�򴰿ڵĴ�������

	if(m_hwnd == NULL)
	{
		MessageBox(NULL, TEXT("Window Creation Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
		return -1;
	}
	//this->set_icon(icon);
	//set_owner(winapp.handle());
	return 0;
}

int native_window::set_window(HWND hwnd)
{
	m_hwnd = hwnd;
	m_prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)win32_wndproc);
	return 0;
}

void native_window::show(bool bshow)
{
	ShowWindow(handle(), bshow ? SW_SHOW : SW_HIDE);
}

int native_window::set_title(const cstring<tchar_t>& title)
{
	return SetWindowText(handle(), title.c_str());
}

cstring<tchar_t> native_window::title()
{
	TCHAR text[MAX_PATH] = {0};
	GetWindowText(handle(), text, MAX_PATH);
	return cstring<tchar_t>(text);
}

int native_window::width()
{
	return bounds_rect().width;
}

int native_window::height()
{
	return bounds_rect().height;
}

vec4i native_window::bounds_rect()
{
	RECT rc;
	GetWindowRect(handle(), &rc);
	return vec4i(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

vec4i native_window::client_rect()
{
	RECT rc;
	GetClientRect(handle(), &rc);
	return vec4i(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

int native_window::move(int x, int y)
{
	vec4i rc = bounds_rect();
	MoveWindow(handle(), x, y, rc.width, rc.height, true);
	return 0;
}

int native_window::resize(int w, int h)
{
	vec4i rc = bounds_rect();
	MoveWindow(handle(), rc.x, rc.y, w, h, true);
	return 0;
}

void native_window::set_focus()
{
	SetFocus(handle());
}

LRESULT CALLBACK win32_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_QUIT:
		break;
	case WM_DESTROY:
		uniframe<CGL_PLATFORM>::instance().app.m_is_run = false;
		break;

	case WM_ERASEBKGND:
		return CGL_TRUE;//return not 0
	case WM_SIZE:
		uniframe<CGL_PLATFORM>::instance().gl.viewport(0, 0, LOWORD(lparam), HIWORD(lparam));
		break;
	case WM_SETCURSOR://���ع����Ϣ
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

#endif //WIN32_WINDOW_CPP_20180720010736

