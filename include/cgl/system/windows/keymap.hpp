/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 keymap.hpp

 2020-08-05 15:44:08

*/
#ifndef KEYMAP_HPP_20200805154408
#define KEYMAP_HPP_20200805154408

#include <cgl/public.h>

namespace cgl{
namespace system{
namespace {

}//end namespace
}//end namespace system

//¼üÅÌ°´¼ü
const int key_back      = 8;
const int key_tab       = 9;
const int key_clear     = 12;
const int key_enter     = 13;
const int key_return    = key_enter;
const int key_shift     = 16;
const int key_ctrl      = 17;
const int key_alt       = 18;
const int key_pause     = 19;
const int key_capslock  = 20;//capital
const int key_kana      = 21;
const int key_hangul    = 21;
const int key_junja     = 23;
const int key_final     = 24;
const int key_hanja     = 25;
const int key_kanji     = 25;
const int key_esc       = 27;
const int key_convert   = 28;
const int key_nonconvert= 29;
const int key_accept    = 30;
const int key_modechange= 31;
const int key_space     = 32;
const int key_pageup    = 33;
const int key_prior     = 33;
const int key_pagedown  = 34;
const int key_next      = 34;
const int key_end       = 35;
const int key_home      = 36;
const int key_left      = 37;
const int key_up        = 38;
const int key_right     = 39;
const int key_down      = 40;
const int key_select    = 41;
const int key_print     = 42;
const int key_execute   = 43;
const int key_snapshot  = 44;//print screen
const int key_insert    = 45;
const int key_delete    = 46;
const int key_help      = 47;

const int key_0 = 48;
const int key_1 = 49;
const int key_2 = 50;
const int key_3 = 51;
const int key_4 = 52;
const int key_5 = 53;
const int key_6 = 54;
const int key_7 = 55;
const int key_8 = 56;
const int key_9 = 57;

const int key_a = 65;
const int key_b = 66;
const int key_c = 67;
const int key_d = 68;
const int key_e = 69;
const int key_f = 70;
const int key_g = 71;
const int key_h = 72;
const int key_i = 73;
const int key_j = 74;
const int key_k = 75;
const int key_l = 76;
const int key_m = 77;
const int key_n = 78;
const int key_o = 79;
const int key_p = 80;
const int key_q = 81;
const int key_r = 82;
const int key_s = 83;
const int key_t = 84;
const int key_u = 85;
const int key_v = 86;
const int key_w = 87;
const int key_x = 88;
const int key_y = 89;
const int key_z = 90;

const int key_left_win  = 91;
const int key_right_win = 92;
const int key_apps      = 93;    //menu
const int key_sleep     = 95;

const int key_numpad0   = 96;//Ğ¡¼üÅÌ 0
const int key_numpad1   = 97;//Ğ¡¼üÅÌ 1
const int key_numpad2   = 98;//Ğ¡¼üÅÌ 2
const int key_numpad3   = 99;//Ğ¡¼üÅÌ 3
const int key_numpad4   = 100;//Ğ¡¼üÅÌ 4
const int key_numpad5   = 101;//Ğ¡¼üÅÌ 5
const int key_numpad6   = 102;//Ğ¡¼üÅÌ 6
const int key_numpad7   = 103;//Ğ¡¼üÅÌ 7
const int key_numpad8   = 104;//Ğ¡¼üÅÌ 8
const int key_numpad9   = 105;//Ğ¡¼üÅÌ 9
const int key_multiply  = 106;//Ğ¡¼üÅÌ *
const int key_add       = 107;//Ğ¡¼üÅÌ +
const int key_separator = 108;//Ğ¡¼üÅÌ enter
const int key_subtract  = 109;//Ğ¡¼üÅÌ -
const int key_decimal   = 110;//Ğ¡¼üÅÌ .
const int key_divide    = 111;//Ğ¡¼üÅÌ /

const int key_f1  = 112;
const int key_f2  = 113;
const int key_f3  = 114;
const int key_f4  = 115;
const int key_f5  = 116;
const int key_f6  = 117;
const int key_f7  = 118;
const int key_f8  = 119;
const int key_f9  = 120;
const int key_f10 = 121;
const int key_f11 = 122;
const int key_f12 = 123;
const int key_f13 = 124;
const int key_f14 = 125;
const int key_f15 = 126;
const int key_f16 = 127;
const int key_f17 = 128;
const int key_f18 = 129;
const int key_f19 = 130;
const int key_f20 = 131;
const int key_f21 = 132;
const int key_f22 = 133;
const int key_f23 = 134;
const int key_f24 = 135;

const int key_numlock       = 144;
const int key_scroll        = 145;
const int key_left_shift    = 160;
const int key_right_shift   = 161;
const int key_left_ctrl     = 162;
const int key_right_ctrl    = 163;
const int key_left_alt      = 164;
const int key_right_alt     = 165;

const int key_browser_back      = 166;
const int key_browser_forward   = 167;
const int key_browser_refresh   = 168;
const int key_browser_stop      = 169;
const int key_browser_search    = 170;
const int key_browser_favorites = 171;
const int key_browser_home      = 172;
const int key_volume_mute       = 173;//VolumeMute
const int key_volume_down       = 174;//VolumeDown
const int key_volume_up         = 175;//VolumeUp

const int key_MEDIA_NEXT_TRACK = 176;
const int key_MEDIA_PREV_TRACK = 177;
const int key_MEDIA_STOP = 178;
const int key_MEDIA_PLAY_PAUSE = 179;

const int key_LAUNCH_MAIL = 180;
const int key_LAUNCH_MEDIA_SELECT = 181;
const int key_LAUNCH_APP1 = 182;
const int key_LAUNCH_APP2 = 183;

const int key_OEM_1 = 186;//= ; :
const int key_OEM_PLUS = 187;//= +
const int key_OEM_COMMA = 188;//,<
const int key_OEM_MINUS = 189;//- _
const int key_OEM_PERIOD = 190;//.>
const int key_OEM_2 = 191;// / ?
const int key_OEM_3 = 192;//` ~
const int key_OEM_4 = 219;//[ {
const int key_OEM_5 = 220;//\ |
const int key_OEM_6 = 221;//] }
const int key_OEM_7 = 222;//' "
const int key_OEM_8 = 223;//
const int key_OEM_102 = 226;
const int key_PACKET = 231;
const int key_PROCESSKEY = 229;
const int key_ATTN = 246;
const int key_CRSEL = 247;
const int key_EXSEL = 248;
const int key_EREOF = 249;
const int key_PLAY = 250;
const int key_ZOOM = 251;
const int key_NONAME = 252;
const int key_PA1 = 253;
const int key_OEM_CLEAR = 254;

}//end namespace cgl

#endif //KEYMAP_HPP_20200805154408
