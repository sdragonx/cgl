/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 vcl.hpp

 2018-09-07 14:02:54

 -D_RTLDLL          使用动态链接库
 -D;USEPACKAGES     使用bpk链接库

*/
#ifndef VCL_HPP_20161001161007
#define VCL_HPP_20161001161007

//#ifdef CGL_PLATFORM_VCL

#include <cgl/vcl/base.hpp>
#include <cgl/vcl/controls.hpp>
//#include <cgl/vcl/classes.hpp>
#include <cgl/vcl/events.hpp>
#include <cgl/vcl/graphics.hpp>
#include <cgl/vcl/string.hpp>

#include <cgl/vcl/canvas.hpp>
#include <cgl/vcl/memlink.hpp>

//custom control
#include <cgl/vcl/sizebox.hpp>
#include <cgl/vcl/rotatebox.hpp>

#ifdef GLCONTEXT_HPP_20180510212834
  #include <cgl/vcl/gl_utils.hpp>
#endif

//speed test
#define speed_test_begin(n, t) \
    t = clock();\
    for(int i=0; i< n; ++i){

#define speed_test_end } print(clock() - t);

int64_t cpu_tick()
{
    int64_t t;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&t));
    return t;
}

namespace cgl{
namespace vcl{

CGL_PUBLIC_DECLARE String appPath  = ExtractFilePath(Application->ExeName);
CGL_PUBLIC_DECLARE String tempPath = String();

CGL_PUBLIC_DECLARE String TEXT_NULL = L"NULL";

String InitTempPath(String project)
{
    TCHAR path[MAX_PATH] = {0};
    GetTempPath(MAX_PATH, path);
    tempPath = path;
    tempPath += project;
    tempPath += IntToHex(random(RAND_MAX), 4);
    tempPath += L"\\";
    MkDir(tempPath);
    return tempPath;
}

const TColor TColor_Lists[] = {
    clBlack, clMaroon, clGreen, clOlive, clNavy, clPurple, clTeal, clGray, clSilver, clRed,
    clLime, clYellow, clBlue, clFuchsia, clAqua, clWhite, clMoneyGreen, clSkyBlue, clCream, clMedGray,
    clActiveBorder, clActiveCaption, clAppWorkSpace, clBackground, clBtnFace, clBtnHighlight,
    clBtnShadow, clBtnText, clCaptionText, clGradientActiveCaption, clGradientInactiveCaption,
    clGrayText, clHighlight, clHighlightText, clHotLight, clInactiveBorder, clInactiveCaption,
    clInactiveCaptionText, clInfoBk, clInfoText, clMenu, clMenuBar, clMenuHighlight, clMenuText,
    clScrollBar, cl3DDkShadow, cl3DLight, clWindow, clWindowFrame, clWindowText};

const TColor TColor_Base[] = {
    clBlack, clRed, clYellow, clGreen, clOlive, clBlue, clGray
};

/*
    2010 TBitmap 透明

    方式1：
    Image2->Picture->Bitmap->Handle = bm_open(L"a.png");
    Image2->Transparent = true;
    Image2->Picture->Bitmap->AlphaFormat = afIgnored;
    Image2->Picture->Bitmap->TransparentMode = tmAuto;

    TBmp* bmp = new TBmp;
    方式2：
    bmp->Handle = bm_open(L"a.png");
    bmp->AlphaFormat = afDefined;//afPremultiplied;//这个必须预乘，结果才正确
    //bmp->TransparentMode = tmAuto;
    Canvas->Draw(100, 100, bmp);
    delete bmp;

*/

String FormatFileTime(const FILETIME& ftime)
{
    FILETIME   local_ftime;
    SYSTEMTIME sys_time = {0};
    TCHAR buf[256] = {0};
    String str;
    FileTimeToLocalFileTime(&ftime, &local_ftime);
    FileTimeToSystemTime(&local_ftime, &sys_time);
    GetDateFormat(0, DATE_LONGDATE, &sys_time, NULL, buf, 256);
    str = buf;
    GetTimeFormat(0, TIME_FORCE24HOURFORMAT, &sys_time, NULL, buf, 256);
    str += buf;
    return str;
}

String TimeString()
{
    SYSTEMTIME t;
    char buf[64];
    GetLocalTime(&t);
    snprintf(buf, 64, "%04hu-%02hu-%02hu %02hu:%02hu:%02hu.%03hu",
        t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
    return buf;
}

String toStr(int a)
{
    return IntToStr(a);
}

String toStr(float a)
{
    return FormatFloat("0.00", a);
}

template<typename T>
String toStr(T data[], size_t size)
{
    String s = "{";
    for(size_t i=0; i<size; ++i){
        s += toStr(data[i]);
        if(i != size - 1){
            s += ", ";
        }
    }
    s += "}";
    return s;
}

TColor toTColor(const vec3ub& color)
{
    return TColor(RGB(color.r, color.g, color.b));
}

TColor toTColor(const vec4ub& color)
{
    return TColor(RGB(color.r, color.g, color.b));
}

TColor toTColorBGR(const vec4ub& color)
{
    return TColor(RGB(color.b, color.g, color.r));
}

template<typename T>
inline TPoint toTPoint(const vec2<T>& rect)
{
    return TPoint(rect.x, rect.y);
}

template<typename T>
inline TRect toTRect(const vec4<T>& rect)
{
    return TRect(rect.x, rect.y, rect.get_right(), rect.get_bottom());
}

template<typename T>
inline TRect toTRect(T x, T y, T w, T h)
{
    return TRect(x, y, x + w, y + h);
}

String toStr(const TPoint& v)
{
    return String(v.x) + ", " + String(v.y);
}

template<typename T>
String toStr(const vec2<T>& v)
{
    return toStr(v.data, 2);
}

template<typename T>
String toStr(const vec3<T>& v)
{
    return toStr(v.data, 3);
}

template<typename T>
String toStr(const vec4<T>& v)
{
    return toStr(v.data, 4);
}

String toStr(TRect rect)
{
    return String("{") + rect.left + ", " + rect.top + ", " + rect.right + ", " + rect.bottom + "}";
}

String format_size(double n)
{
    char* sizeName[] = {"", "K", "M", "G"};
    int i = 0;
    for( ; n > 1024.0; ++i){
        n *= 0.0009765625;// 1/1024
    }
    char buf[64] = {0};
    snprintf(buf, 64, "%0.3f %s", n, sizeName[i]);
    return buf;
}

//TTime一秒时间 1/86400
const double TTimeOneSecond = 1.0 / 86400.0;

const String TEXT_FULL_TIME = "YYYY-MM-DD hh:mm:ss";
const String TEXT_TIME_MASK = "!9000\\-00\\-00 90:00:00;1;0";

String FormatTime(TTime t)
{
    return t.FormatString(TEXT_FULL_TIME);
}

//print

void SetOutMemo(TMemo* Memo)
{
    single<TMemo*>::value = Memo;
}

void cls()
{
    single<TMemo*>::value->Lines->Clear();
}

void print(String format, ...)
{
    String buf;
    va_list body;
    va_start(body, format);
    buf.sprintf(format.c_str(), body);
    va_end(body);
    single<TMemo*>::value->Lines->Add(buf);
}


}//end namespace vcl
}//end namespace cgl

using namespace cgl;
using namespace cgl::graphics;
using namespace cgl::vcl;
using namespace cgl::windows;

//#endif //CGL_PLATFORM_VCL

#endif //VCL_HPP_20161001161007

