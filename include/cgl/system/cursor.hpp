/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 cursor.hpp

 2018-05-10 23:19:54

*/
#ifndef CURSOR_HPP_20180510231954
#define CURSOR_HPP_20180510231954

#include <cgl/system/base.hpp>

namespace cgl{
namespace system{

/*
class cursor
{
public:
    int set_cursor(int id);
    int set_pos(int x, int y);
    int show(bool visible);
};
*/

/*
IDC_APPSTARTING ��׼�ļ�ͷ��Сɳ©
IDC_ARROW ��׼�ļ�ͷ
IDC_CROSS ʮ�ֹ��
IDC_HELP ��׼�ļ�ͷ���ʺ�
IDC_IBEAM ���ֹ��
IDC_NO ��ֹȦ
IDC_SIZEALL �����ͷָ�򶫡������ϡ���
IDC_SIZENESW ˫��ͷָ�򶫱�������
IDC_SIZENS ˫��ͷָ���ϱ�
IDC_SIZENWSE ˫��ͷָ�������Ͷ���
IDC_SIZEWE ˫��ͷָ����
IDC_UPARROW ��ֱ��ͷ
IDC_WAIT ɳ©
*/

//---------------------------------------------------------------------------
#ifdef CGL_PLATFORM_WINDOWS

class cursor
{
public:
    //MAKEINTRESOURCEA(i) (LPSTR)((ULONG_PTR)((WORD)(i))) 
    static int set_cursor(int id)
    {
        //SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(native_index(id))));
        #ifdef INC_VCL
        Screen->Cursor = native_index(id);
        #else
        SetCursor(LoadCursor(NULL, native_index(id)));
        #endif
        return 0;
    }

    static int set_pos(int x, int y)
    {
        return SetCursorPos(x, y);
    }

    static int show(bool visible)
    {
        return ShowCursor(visible);
    }
private:
    #ifdef INC_VCL
    static TCursor native_index(int id)
    {
        switch(id)
        {
        case cs_null:
            return crNone;
        case cs_appstarting:
            return crAppStart;
        case cs_arrow:
            return crArrow;
        case cs_cross:
            return crCross;
        case cs_help:
            return crHelp;
        case cs_hand:
            return crHandPoint;
        case cs_pen:
            return crIBeam;
        case cs_no:
            return crNo;
        case cs_size_all:
            return crSizeAll;
        case cs_size_nesw:
            return crSizeNESW;
        case cs_size_ns:
            return crSizeNS;
        case cs_size_nwse:
            return crSizeNWSE;
        case cs_size_we:
            return crSizeWE;
        case cs_wait:
            return crHourGlass;
        default:
            return crNone;
        }
    }
    #else
    static tchar_t* native_index(int id)
    {
        switch(id)
        {
        case cs_null:
            return 0;
        case cs_appstarting:
            return IDC_APPSTARTING;
        case cs_arrow:
            return IDC_ARROW;
        case cs_cross:
            return IDC_CROSS;
        case cs_help:
            return IDC_HELP;
        case cs_hand:
            return IDC_HAND;
        case cs_pen:
            return IDC_IBEAM;
        case cs_no:
            return IDC_NO;
        case cs_size_all:
            return IDC_SIZEALL;
        case cs_size_nesw:
            return IDC_SIZENESW;
        case cs_size_ns:
            return IDC_SIZENS;
        case cs_size_nwse:
            return IDC_SIZENWSE;
        case cs_size_we:
            return IDC_SIZEWE;
        case cs_wait:
            return IDC_WAIT;
        default:
            return 0;
        }
    }
    #endif
};

#else
//---------------------------------------------------------------------------

class cursor
{
public:
    static int set_cursor(int id)
    {
        return 0;
    }

    static int set_pos(int x, int y)
    {
        return 0;
    }

    static int show(bool visible)
    {
        return 0;
    }
};

#endif

}//end namespace system
}//end namespace cgl

#endif //CURSOR_HPP_20180510231954
