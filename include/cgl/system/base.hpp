/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 system_base.hpp

 2018-07-18 13:08:04

*/
#ifndef SYSTEM_BASE_HPP_20180718130804
#define SYSTEM_BASE_HPP_20180718130804

#include <cgl/public.h>
#include "windows/keymap.hpp"

namespace cgl{

/*
0x00xx    窗口消息
0x10xx    键盘消息
0x20xx    鼠标消息
0x40xx
0x00xx0000    //用户消息

*/

const int msg_done     = CGL_DONE;//消息处理完毕msg_completed
const int msg_unproc = CGL_ONE;
const int msg_error  = CGL_ERROR;

const int APP_MESSAGE    = 0x0000;
const int WINDOW_MESSAGE = 0x0100;
const int INPUT_MESSAGE  = 0x0200;

/*
enum {
    APP_START,
    APP_STOP,
    APP_RESUME,
    APP_PAUSE,
    APP_DESTROY,
    APP_SAVE,
    APP_CONFIG,
    APP_LOW_MEMORY,
    APP_POWER        //电源事件，休眠、关机
};

enum {
    WINDOW_CREATE = 0x0100,
    WINDOW_CLOSE,
    WINDOW_SHOW,
    WINDOW_HIDE,
    WINDOW_ACTIVATE,
    WINDOW_DEACTIVATE,
    WINDOW_ENABLE,
    WINDOW_DISABLE,
    WINDOW_MOVE,
    WINDOW_SIZE,
    WINDOW_PAINT,
    WINDOW_CURSOR,
    WINDOW_RESHAPE
};

enum {
    INPUT_MOUSE_MOVE = 0x200,
    INPUT_MOUSE_DOWN,
    INPUT_MOUSE_UP,
    INPUT_CLICK,
    INPUT_DOUBLE_CLICK,
    INPUT_MOUSE_WHEEL,
    INPUT_SCROLL,
    INPUT_HOVER_ENTER,
    INPUT_HOVER_LEAVE,
    INPUT_HOLD
};

enum {
    INPUT_KEY_DOWN = 0x0300,
    INPUT_KEY_UP,
    INPUT_KEY_PRESS
};

*/

const int msg_window    = 0x0000;
const int msg_keyboard  = 0x0100;
const int msg_mouse     = 0x0200;//鼠标消息

//
// window
//

const int msg_create        = 0x0001;
//const int msg_destroy     = 0x0002;
const int msg_move          = 0x0003;
//const int msg_hide        = 0x0004;
const int msg_size          = 0x0005;//msg_frame
const int msg_activate      = 0x0006;//param 0 失去焦点；1获得焦点
//const int msg_focus       = 0x0007;
//const int msg_killfocus   = 0x0008
const int msg_enable        = 0x000A;
//const int msg_redraw      = 0x000B;
const int msg_paint         = 0x000F;
const int msg_close         = 0x0010;
const int msg_show          = 0x0018;
const int msg_cursor        = 0x0020;

//const int msg_move      = 0x00A1;    //鼠标拖动 data = bool
const int msg_dragwindow = 0x0401;    //WM_USER+1 拖动窗口
//const int msg_frame = 0x0402;    //父窗口大小发送变化
const int msg_reshape    = 0x0402;

//const int msg_desktop    = 0x0410;    //desktop window changed
//const int msg_parent    = 0x0411;    //parent window changed

const int msg_power     = 0x0048;    //logoff restart poweroff

const int msg_command   = 0x0111;
const int msg_timer     = 0x0113;

//
// input
//

const int msg_keydown   = 0x0100;
const int msg_keyup     = 0x0101;
const int msg_keypress  = 0x0102;

const int msg_mousemove   = 0x0200;
const int msg_mousedown   = 0x0201;
const int msg_mouseup     = 0x0202;
const int msg_doubleclick = 0x0203;
const int msg_mousewheel  = 0x020A;
const int msg_mouseenter  = 0x02A1;
const int msg_mouseleave  = 0x02A3;

const int msg_touch       = 0x0210;
const int msg_touch_begin = 0x0211;
const int msg_touch_end   = 0x0212;
const int msg_longtouch   = 0x0213;//测试 长按鼠标
//const int msg_hold = msg_longtouch;

const int msg_mousehit    = 0x02FF;//测试鼠标消息的窗口

const int msg_cut   = 0x0300;
const int msg_copy  = 0x0301;
const int msg_paste = 0x0302;
const int msg_clear = 0x0303;
const int msg_undo  = 0x0304;

//鼠标按键
const int mouse_left    = 0x0001;
const int mouse_right    = 0x0002;
const int mouse_middle    = 0x0010;
const int mouse_x        = 0x0020;
const int mouse_buttons    = mouse_left|mouse_right|mouse_middle|mouse_x;

//shift state
const int fk_shift = 0x01;
const int fk_alt   = 0x02;
const int fk_ctrl  = 0x04;

//cursor
enum cursor_mode{
    cs_null = 0,
    cs_appstarting,
    cs_arrow,
    cs_cross,
    cs_help,
    cs_hand,
    cs_pen,
    cs_no,
    cs_size_all,
    cs_size_nesw,
    cs_size_ns,
    cs_size_nwse,
    cs_size_we,
    cs_wait
};


}//end namespace cgl

#endif //SYSTEM_BASE_HPP_20180718130804