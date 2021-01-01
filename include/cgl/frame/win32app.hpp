/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 win32app.hpp

 2018-08-02 02:52:04

*/
#ifndef WIN32APP_HPP_20180802025204
#define WIN32APP_HPP_20180802025204

#include <cgl/string.hpp>
#include <cgl/vec.hpp>
#include <cgl/graphics/context.hpp>
#include <cgl/ui/win32_message.hpp>

namespace cgl{
namespace frame{

class native_app;
class native_window;

LRESULT CALLBACK winapp_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

class native_app
{
public:
    typedef wchar_t char_type;
    typedef cstring<char_type> string_type;

    friend class native_window;

private:
    bool m_is_running;
    MSG m_msg;

public:
    native_app() : m_is_running(true)
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

    void run()
    {
        while(m_is_running){
            this->process_message();
            //CGL_MAINFRAME.process();
            //CGL_MAINFRAME.draw_frame();
        }
    }

    bool is_running()const { return m_is_running; }

    void process_message()
    {
        while(PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE)){
            TranslateMessage(&m_msg);
            DispatchMessage(&m_msg);
        }
    }

    void exit()
    {
        m_is_running = false;
        PostQuitMessage(0);
    }

    int msgbox(const char_type* msg, const char_type* title, int type)
    {
        HWND hwnd = 0;//m_main ? m_main : handle();
        return tvalue<char_type>(MessageBoxA, MessageBoxW)(hwnd, msg, title, type);
    }
};



class native_window
{
public:
    typedef HWND handle_type;
    typedef wchar_t char_type;
    typedef cstring<char_type> string_type;

private:
    native_app* m_app;
    HWND m_hwnd;
//    DWORD m_winstyle;
//    bool m_fullscreen;
    WNDPROC m_prevWndProc;
    void* data;

public:
    cgl::graphics::device_context* m_gl;
    ui::winmain* m_ui;
public:
    native_window(native_app& app) :
        m_app(&app),
        m_hwnd(null),
//        m_winstyle(),
//        m_fullscreen(false),
        m_prevWndProc(null),
        m_gl(null),
        m_ui(null)
    {

    }
    ~native_window()
    {

    }

    HWND handle()const { return m_hwnd; }

    int create(const char_type* title, int left, int top, int width, int height, int style = WS_OVERLAPPEDWINDOW)
    {
        const char_type* classname = L"luna.window";
        regclass(classname, 0, 0);

        CREATESTRUCTW cs = {0};
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
        m_hwnd = CreateWindowExW(
            cs.dwExStyle,    //窗口的扩展风格
            cs.lpszClass,    //类名
            cs.lpszName,    //标题
            cs.style,        //风格
            cs.x,            //左边位置
            cs.y,            //顶部位置
            cs.cx,            //宽度
            cs.cy,            //高度
            cs.hwndParent,    //父窗口的句柄
            cs.hMenu,        //菜单的句柄或是子窗口的标识符
            cs.hInstance,    //应用程序实例的句柄
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
        CloseWindow(m_hwnd);
    }

    int set_window(HWND hwnd)
    {
        //close()
        m_hwnd = hwnd;
        SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LPARAM>(this));
        m_prevWndProc = (WNDPROC)SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG)winapp_wndproc);
        return 0;
    }

    void show(bool bshow)
    {
        ShowWindow(handle(), bshow ? SW_SHOW : SW_HIDE);
    }

    int set_title(const string_type& title)
    {
        return SetWindowTextW(handle(), title.c_str());
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
        WNDCLASSEXW wc = {0};
        int atom = 0;

        memset(&wc,0,sizeof(wc));
        wc.cbSize         = sizeof(WNDCLASSEX);
        wc.style         = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;//CS_DBLCLKS 支持鼠标双击事件
        wc.lpfnWndProc     = winapp_wndproc;
        wc.hInstance     = GetModuleHandle(NULL);
        wc.hCursor         = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.lpszClassName = cname;
        wc.hIcon          = icon;
        wc.hIconSm         = NULL;

        //注册窗口类
        atom = RegisterClassExW(&wc);

        if(!atom)
        {
            MessageBox(NULL, TEXT("Window Registration Failed!"), TEXT("Error!"), MB_ICONEXCLAMATION|MB_OK);
            return -1;
        }
        return atom;
    }

    LRESULT CALLBACK msg_proc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch(msg){
        case WM_DESTROY:
			m_app->m_is_running = false;
            break;
        case WM_CLOSE:
            break;
        case WM_QUIT:
            break;

        case WM_ERASEBKGND:
            return CGL_TRUE;//return not 0
        case WM_SIZE:
            if(m_gl)m_gl->viewport(0, 0, LOWORD(lparam), HIWORD(lparam));
            break;
        case WM_SETCURSOR://拦截光标消息
            if(m_ui)send_winmsg(m_ui, msg_cursor, 0, 0);
            return 0;
        default:
            break;
        }

        if(m_ui){//发送ui消息
            send_winmsg(m_ui, msg, wparam, lparam);
        }

        if(m_prevWndProc){
            return CallWindowProc(m_prevWndProc, m_hwnd, msg, wparam, lparam);
        }
        else{
            return DefWindowProc(m_hwnd, msg, wparam, lparam);
        }
    }

    friend LRESULT CALLBACK winapp_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};

//程序消息主函数
LRESULT CALLBACK winapp_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    //窗口的USERDATA保存了窗口的window类指针
    native_window *win = reinterpret_cast<native_window*>(GetWindowLongPtr(hwnd, GWL_USERDATA));

    if(win){//window窗口消息
        return win->msg_proc(msg, wparam, lparam);
    }
    else if(msg == WM_CREATE){
        LPCREATESTRUCTW pcs = LPCREATESTRUCTW(lparam);
        win = reinterpret_cast<native_window*>(pcs->lpCreateParams);
        if(win){//创建class window窗口
            //过程是通过CreateWindowEx把创建窗口的window指针通过CreateParams发送过来
            //然后通过再向window转发WM_CREATE消息，把这里的窗口句柄发送给window
            //window接收WM_CREATE消息之后，把窗口句柄hwnd赋值给window::m_hwnd
            win->m_hwnd = hwnd;
            //设置hwnd的USERDATA为window类指针
            ::SetLastError(ERROR_SUCCESS);
            SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LPARAM>(win));
            if(GetLastError()!=ERROR_SUCCESS)
                return -1;//取消创建，销毁窗口
            else
                return win->msg_proc(msg, wparam, lparam);//发送hwnd句柄，完成创建
        }
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

}//end namespace frame
}//end namespace cgl

#endif //WIN32APP_HPP_20180802025204
