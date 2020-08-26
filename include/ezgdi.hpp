/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ezgdi.hpp

 2020-01-01 16:37:22

 EZGDI��һ���Ƚϼ򵥡���ѧ�����õ�C++�⣬���Ŀ��ּ�ڰ�����ѧ��ѧϰʹ��C++��
 ��ӭ����EZGDI��Ҳ��ӭ�����ҵ�GITHUB������������
 https://github.com/sdragonx/ezgdi

*/
#ifndef EZGDI_HPP
#define EZGDI_HPP

#ifndef UNICODE
    #error UNICODE can not defined.
#endif

#ifndef _UNICODE
    #define _UNICODE
#endif

#ifndef NO_WIN32_LEAN_AND_MEAN
    #define NO_WIN32_LEAN_AND_MEAN
#endif

#ifndef STRICT
    #define STRICT
#endif

#if defined(__GNUC__)
	#define EZ_PUBLIC_DECLARE __attribute__((weak))
#else
	#define EZ_PUBLIC_DECLARE __declspec(selectany)
#endif

#include <cmath>
#include <float.h>
#include <stdint.h>
#include <tchar.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <gdiplus.h>

/****************************************************************************
 *                                                                          *
 *                                   ͨ��                                   *
 *                                                                          *
 ****************************************************************************/

//enum����
enum{
    EZ_NULL,

    EZ_FIXED,               //�̶���С
    EZ_SIZEABLE,            //������
    EZ_FULLSCREEN,          //ȫ��

    EZ_LEFT   = 1,          //��
    EZ_RIGHT  = 2,          //��
    EZ_UP     = 4,          //��
    EZ_DOWN   = 8,          //��
    EZ_TOP    = EZ_UP,      //����
    EZ_BOTTOM = EZ_DOWN,    //�ײ�

    EZ_CENTER_H = EZ_LEFT | EZ_RIGHT,           //ˮƽ����
    EZ_CENTER_V = EZ_UP | EZ_DOWN,              //��ֱ����
    EZ_CENTER   = EZ_CENTER_H | EZ_CENTER_V,    //����

    EZ_MIDDLE   = 16,       //��

    EZ_OK = 0,
    EZ_ERROR = -1,
};

//byte_t����
typedef uint8_t byte_t;

//�����¼�
typedef void (*EZ_KEY_EVENT)(int key);

//����¼�
typedef void (*EZ_MOUSE_EVENT)(int x, int y, int button);

//��ʱ���¼�
typedef void (*EZ_TIMER_EVENT)();

//���ڻ����¼�
typedef void (*EZ_PAINT_EVENT)();

/****************************************************************************
 *                                                                          *
 *                             Unicode�ַ�����                              *
 *                                                                          *
 ****************************************************************************/

//�ַ���ת��������
template<typename T, typename _char_t>
inline T string_cast(const std::basic_string<_char_t>& str)
{
    std::basic_stringstream<_char_t> stm(str);
    T n;
    stm>>n;
    return n;
}

//��������ת�ַ���
template<typename _char_t, typename T>
std::basic_string<_char_t> to_string(const T& value)
{
    std::basic_stringstream<_char_t> stm;
    stm<<value;
    return stm.str();
}

//ansi to unicode
std::wstring to_unicode(const char* str, int length)
{
    std::vector<wchar_t> buf;
    int n = MultiByteToWideChar(CP_ACP, 0, str, length, NULL, 0);
    buf.resize(n);
    MultiByteToWideChar(CP_ACP, 0, str, length, &buf[0], n);
    return std::wstring(&buf[0], n);
}

class ezstring : public std::wstring
{
public:
    ezstring() : std::wstring() { }
    ezstring(const char* str) : std::wstring() { assign(str, strlen(str)); }
    ezstring(const char* str, size_t size) : std::wstring() { assign(str, size); }
    ezstring(const wchar_t* str) : std::wstring(str) { }
    ezstring(const wchar_t* str, size_t size) : std::wstring(str, size) { }

    //����ת�ַ���
    ezstring(int n) : std::wstring(to_string<wchar_t>(n)) { }

    //��ʵ��ת�ַ���
    ezstring(float n) : std::wstring(to_string<wchar_t>(n)) { }

    //˫ʵ��ת�ַ���
    ezstring(double n) : std::wstring(to_string<wchar_t>(n)) { }

    //���ƹ���
    ezstring(const ezstring& str) : std::wstring(str.c_str(), str.length()) { }

    using std::wstring::assign;
    ezstring& assign(const char* str, size_t size = -1)
    {
        std::vector<wchar_t> buf;
        int n = MultiByteToWideChar(CP_ACP, 0, str, int(size), NULL, 0);
        buf.resize(n);
        MultiByteToWideChar(CP_ACP, 0, str, int(size), &buf[0], n);
        std::wstring::assign(&buf[0], n);
        return *this;
    }

    int    to_int()const    { return string_cast<int>(*this);    }//�ַ���ת����
    float  to_float()const  { return string_cast<float>(*this);  }//�ַ���ת��ʵ��
    double to_double()const { return string_cast<double>(*this); }//�ַ���ת˫ʵ��
};

/****************************************************************************
 *                                                                          *
 *                                ��ѧ������                                *
 *                                                                          *
 ****************************************************************************/

#ifndef M_PI
    #define M_PI      3.141592653589793238462   //acos(-1.0)
    #define M_PI_2    1.570796326794896619231   //M_PI/2
#endif

#ifndef M_RD
    #define M_RD       0.017453292519943295769  //����(radian)
    #define M_INV_RD  57.295779513082320876798  //���ȵĵ���(reciprocal) 1.0/M_RD
#endif

//�ж���ֵ�Ƿ���0
template<typename T>inline bool is_zero(T n) { return n == 0; };
template<>inline bool is_zero<float>(float n) { return n < 0.0 ? (n > -FLT_EPSILON) : (n < FLT_EPSILON); }
template<>inline bool is_zero<double>(double n) { return n < 0.0 ? (n > -DBL_EPSILON) : (n < DBL_EPSILON); }

//�ж���ֵ�Ƿ����
template<typename T>
inline bool is_equal(T a, T b) { return is_zero(a - b); }

//����0 ~ n֮��������
#ifdef _MSC_VER
inline int random(int n) { return rand() % n; }
#endif

//����0 ~ 1֮������������
double randReal() { return double(rand()) / RAND_MAX; }

//����minVal ~ maxVal֮������������
double randReal(double minVal, double maxVal)
{
    return minVal + (maxVal - minVal) * randReal();
}

//��������Ļ���
double radian(double x, double y)
{
    double n = 0.0;
    if(!is_zero(y)){
        n = M_PI_2 - std::atan(x / y);//����б���󻡶ȣ�����
        if(y < 0.0)n += M_PI;
    }
    return n;
}

//ͨ��xy��ýǶ�
double get_angle(double x, double y)
{
    return radian(x, y) * M_INV_RD;
}

//��source�ݽ���dest������Ϊspeed
template<typename T>
T step(T source, T dest, T speed)
{
    if(source < dest){
        source += speed;
        if(source > dest){
            source = dest;
        }
    }
    else if(source > dest){
        source -= speed;
        if(source < dest){
            source = dest;
        }
    }
    return source;
}

/****************************************************************************
 *                                                                          *
 *                                  ������                                  *
 *                                                                          *
 ****************************************************************************/

template<typename T> class vec2;
template<typename T> class vec4;

template<typename T>
class vec2
{
public:
    T x, y;

    vec2() : x(), y() { }
    vec2(T _x, T _y) : x(_x), y(_y) { }

    vec2& operator=(const vec2& other)
    {
        x = other.x;
        y = other.y;
        return *this;
    }

	vec2& set(T _x, T _y) { x = _x; y = _y; return *this; }

    vec2 operator+(const vec2& other) { return vec2(x + other.x, y + other.y); }
    vec2 operator-(const vec2& other) { return vec2(x - other.x, y - other.y); }
    vec2& operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
    vec2& operator-=(const vec2& other) { x -= other.x; y -= other.y; return *this; }

    bool operator==(const vec2& other)const
    {
        return is_equal(x, other.x) && is_equal(y, other.y);
    }

    bool operator!=(const vec2& other)const
    {
        return !is_equal(x, other.x) || !is_equal(y, other.y);
    }

    T length()const
    {
        return sqrt(x * x + y * y);
    }

    vec2& rotate(double angle)
    {
        angle *= M_RD;
        double sine = sin(angle);
        double cosine = cos(angle);
        return set(
            x * cosine - y * sine,
            y * cosine + x * sine);
    }
};

template<typename T>
class vec4
{
public:
    union{
        struct{ T x, y, z, w; }; //�ռ�����
        struct{ T r, g, b, a; }; //��ɫ����
        struct{ T _x, _y, width, height; }; //����
    };

    vec4() : x(), y(), z(), w() { }
    vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }

    bool contains(int vx, int vy)
    {
        return vx >= left && vx <= (left + width) && vy >= top && vy <= (top + height);
    }

    vec4 operator&(const vec4& v)
    {
        int max_x = max(a.x, b.x);
        int max_y = max(a.y, b.y);
        return vec4(max_x, max_y, min(a.x + a.width, b.x + b.width)-max_x, min(a.y + a.height, b.y + b.height)-max_y);
    }
};

typedef vec2<int>       vec2i;
typedef vec2<float>     vec2f;
typedef vec2<double>    vec2d;

typedef vec4<byte_t>    vec4ub;
typedef vec4<int>       vec4i;
typedef vec4<float>     vec4f;
typedef vec4<double>    vec4d;

/****************************************************************************
 *                                                                          *
 *                                  ͼƬ��                                  *
 *                                                                          *
 ****************************************************************************/

class ezImage
{
public:
    Gdiplus::Bitmap* m_handle;

public:
    ezImage();
    ~ezImage();

    Gdiplus::Bitmap* handle()const;

    //����һ��ͼƬ��Ĭ��Ϊ32λɫ
    int create(int width, int height, int format = PixelFormat32bppARGB);

    //��һ��ͼƬ��֧��bmp��jpg��png����̬gif�ȳ�����ʽ
    int open(const ezstring& filename);

    //����Դ�е�ͼƬ
    int open(int id, PCTSTR resource_type = TEXT("BITMAP"));

    //ӳ��һ��HBITMAP����
    int map(HBITMAP hbmp);

    //����ͼƬ
    int save(const ezstring& filename);

    //�Զ��ͷ�ͼƬ
    void close();

    //����ͼƬ�Ŀ��
    int width()const;

    //����ͼƬ�ĸ߶�
    int height()const;
};

/****************************************************************************
 *                                                                          *
 *                                  ������                                  *
 *                                                                          *
 ****************************************************************************/

/*

ʾ������1��
int main(int argc, char* argv[])
{
    //��ʼ���⣬��������
    ezInit("���ڱ���", 800, 600);

    while(ezLoop()){
      //TODO : �����ͼ����

    }

    //�˳�ʱ������Դ
    ezClose();
}

ʾ������2��
void MyPaint();

int main(int argc, char* argv[])


    //��ʼ���⣬��������
    ezInit("���ڱ���", 800, 600);

    //���û�ͼ�¼�����
    ezOnPaint(MyPaint);

    //������ִ�к���
    ezRun();

    //�˳�ʱ������Դ
    ezClose();
}

void MyPaint()
{
    //TODO : �����ͼ����
}

***/

//ͼ�ο��ʼ��
int ezInit(
    const ezstring& title,  //���ڱ���
    int width,              //���ڿ��
    int height,             //���ڸ߶�
    int style               //������ʽ EZ_FIXED �̶���С��EZ_SIZEABLE �����ţ�EZ_FULLSCREEN ȫ��
    );

//ͼ�ο�ر�
void ezClose();

//��������ھ��
HWND ezHWnd();

//�������ô��ڴ�С
void ezResize(
    int width,              //���ڿ��
    int height              //���ڸ߶�
    );

//���ô��ڿͻ�ȥ��С
void ezClientResize(
    int width,              //�ͻ������
    int height              //�ͻ����߶�
    );

//����ȫ��
void ezFullScreen(
    bool value              //true ȫ����false ȡ��ȫ��
    );

//���GDI��ͼ�豸
HDC ezHDC();

//��Ϣѭ������
bool ezLoop();

//������ִ�к���
void ezRun();

/****************************************************************************
 *                                                                          *
 *                          �����¼������밴������                          *
 *                                                                          *
 ****************************************************************************/

/* �жϰ����Ƿ���
 * ���ð�����
 * VK_ESCAPE        ESC
 * VK_RETURN        �س�
 * VK_LEFT          �����
 * VK_RIGHT         �ҷ����
 * VK_UP            �Ϸ����
 * VK_DOWN          �·����
 * VK_SPACE         �ո�
 */
bool ezKeyState(int key);

//�����¼�ӳ��
void ezOnKeyUp(EZ_KEY_EVENT function);
void ezOnKeyDown(EZ_KEY_EVENT function);
void ezOnChar(EZ_KEY_EVENT function);

//����¼�ӳ��
void ezOnMouseMove(EZ_MOUSE_EVENT function);
void ezOnMouseDown(EZ_MOUSE_EVENT function);
void ezOnMouseUp(EZ_MOUSE_EVENT function);

//���ü�ʱ��
void ezTimer(
    UINT interval       //��ʱ��ʱ��������λ���룬����0ֹͣ��ʱ��
    );

//��ʱ���¼�
void ezOnTimer(EZ_TIMER_EVENT function);

//���ڻ����¼�
void ezOnPaint(EZ_PAINT_EVENT function);

/****************************************************************************
 *                                                                          *
 *                                 ��ͼ����                                 *
 *                                                                          *
 ****************************************************************************/

//���GDI+��ͼ�豸
Gdiplus::Graphics* ezGraphics();

//������ʾ����
enum {
    EZ_SPEED,   //�ٶ�����
    EZ_MEDIUM,  //�е�����
    EZ_QUALITY, //��������
};

int ezEffectLevel(int level);

//����
void ezClear(BYTE r, BYTE g, BYTE b, BYTE a = 255);

//���Ļ�����ɫ
void ezPenColor(BYTE r, BYTE g, BYTE b, BYTE a = 255);
void ezPenColor(DWORD argb);

//��ȡ������ɫ
DWORD ezPenColor();

//���ʿ��
void ezPenWidth(float width);

//����ģʽ
enum{
    EZ_SOLID,       //ʵ�Ļ��ʣ�Ĭ�ϣ�
    EZ_DASH,        // -------
    EZ_DOT,         // .......
    EZ_DASH_DOT,    // -.-.-.-
    EZ_CUSTOM = 5   //�Զ���
};

//���û���ģʽ
void ezPenStyle(int mode);

//���õ㻭ģʽ���
void ezDashStyle(
    const float* dash,      //�㻭ģʽ���������������
    int size                //�����������С
    );

//���������ɫ
void ezFillColor(BYTE r, BYTE g, BYTE b, BYTE a = 255);
void ezFillColor(DWORD argb);

//��ȡ�����ɫ
DWORD ezFillColor();

//����һ����
void ezPoint(float x, float y);

//�����߶�
void ezDrawLine(float x1, float y1, float x2, float y2);

//����һ�����ľ���
void ezDrawRect(float x, float y, float width, float height);

//���һ������
void ezFillRect(float x, float y, float width, float height);

//����Բ�Ǿ���
void ezDrawRoundRect(float x, float y, float width, float height, float cx, float cy);

//���Բ�Ǿ���
void ezFillRoundRect(float x, float y, float width, float height, float cx, float cy);

//���ƿ�����Բ��xyΪԲ��
void ezDrawEllipse(float x, float y, float cx, float cy);

//�����Բ
void ezFillEllipse(float x, float y, float cx, float cy);

//���ƿ���Բ��xyΪԲ��
void ezDrawCircle(float x, float y, float r);

//���Բ
void ezFillCircle(float x, float y, float r);

//�����������߶�
void ezDrawPolyline(const vec2f* points, size_t size);

//���ƶ����
void ezDrawPolygon(const vec2f* points, size_t size);

//�������
void ezFillPolygon(const vec2f* points, size_t size);

/****************************************************************************
 *                                                                          *
 *                                 ���庯��                                 *
 *                                                                          *
 ****************************************************************************/

//������ʽ�������������
enum EZGDI_FONTSTYLE{
    EZ_NORMAL       = 0,    //��ͨ����
    EZ_BOLD         = 1,    //����
    EZ_ITALIC       = 2,    //б��
    EZ_UNDERLINE    = 4,    //�»���
    EZ_STRIKEOUT    = 8     //ɾ����
};

//�������塣�������֡���С�����
void ezFont(const ezstring& name, float size, EZGDI_FONTSTYLE style = EZ_NORMAL);

//������������
void ezFontName(const ezstring& name);
void ezFontSize(float size);
void ezFontStyle(int style);

//��ȡ��������
ezstring ezFontName();
int ezFontSize();
int ezFontStyle();

//������ɫ
void ezFontColor(BYTE r, BYTE g, BYTE b, BYTE a = 255);
void ezFontColor(UINT color);

//�������
void ezText(float x, float y, const wchar_t* text, size_t length);
void ezText(float x, float y, const ezstring& text);

void ezText(float x, float y, float width, float height, const ezstring& text, int align = 0);

//�����ʽ���������printfʹ������
void ezPrint(float x, float y, const wchar_t* format, ...);

//����ַ��������ؿ��
float ezTextWidth(const ezstring& text);

//����ַ��������ظ߶�
float ezTextHeight(const ezstring& text);

/****************************************************************************
 *                                                                          *
 *                                 ͼƬ����                                 *
 *                                                                          *
 ****************************************************************************/

//����ͼƬ�����ù����ͷ�
ezImage* ezLoadImage(const ezstring& filename);

//������Դ�е�ͼƬ
ezImage* ezLoadImage(int id, PCTSTR resource_type = TEXT("PNG"));

//����ͼƬ��Ĭ�ϱ���ΪPNG��ʽ
int ezSaveImage(ezImage* image, const ezstring& filename);

//��xyλ�û���ͼƬ��ԭʼ��С
void ezDraw(ezImage* image, float x, float y);

//��xyλ�û���ͼƬ������
void ezDraw(ezImage* image, float x, float y, float width, float height);

//��xyλ�û���ͼƬ������תһ���Ƕ�
void ezRotateImage(ezImage* image, float x, float y, float rotate);

//��xyλ�û���ͼƬ�����ţ�����תһ���Ƕ�
void ezRotateImage(ezImage* image, float x, float y, float width, float height, float rotate);

//---------------------------------------------------------------------------
//
// ��ý��
//
//---------------------------------------------------------------------------

//���ű�������
void ezPlayMusic(PCTSTR filename);

//������Դ�е�����
void ezPlayResourceMusic(PCTSTR filename, PCTSTR resource_type = TEXT("MP3"));
void ezPlayResourceMusic(int id, PCTSTR resource_type = TEXT("MP3"));

//����wav�ļ���loop�Ƿ�ѭ��
int ezPlaySound(PCTSTR filename, bool loop = false);

//������Դ�е�wav�ļ�
int ezPlayResourceSound(PCTSTR filename, bool loop = false);
int ezPlayResourceSound(int id, bool loop = false);

//---------------------------------------------------------------------------
//
// ����
//
//---------------------------------------------------------------------------

//��Ϣ�Ի���
int ezMsgBox(const ezstring& msg, const ezstring& title, int type = MB_OK);

//��ʾ��Ϣ
void ezMessage(const ezstring& msg);

//��ʾ�����
ezstring ezInputBox(const ezstring& title, const ezstring& message, const ezstring value = ezstring());

#include "ezgdi.inl"

#endif //EZGDI_HPP
