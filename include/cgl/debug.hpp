/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 debug.hpp

 2006-07-30 19:13:12

 定义 CGL_DEBUG_MEMORY 开启内存遗漏检测

*/
#ifndef CGL_DEBUG_H_20060730191312
#define CGL_DEBUG_H_20060730191312

#ifdef _DEBUG
    #define CGL_DEBUG
#endif

#ifdef CGL_DEBUG

#include <cgl/public.h>
#include <ctime>
#include <string>
#include <fstream>
#include <map>
#include <tchar.h>

#ifdef CGL_PLATFORM_WINDOWS
#include <windows.h>    //wincon.h
#endif

namespace cgl{

#ifdef CGL_PLATFORM_WINDOWS

//输出字符串到编辑器消息栏
void printdebug(const char *format, ...)
{
    char buf[4096];
    va_list body;
    va_start(body, format);
    std::vsnprintf(buf, 4096, format, body);
    va_end(body);
    OutputDebugStringA(buf);
}

void printdebug(const wchar_t *format, ...)
{
    wchar_t buf[4096];
    va_list body;
    va_start(body, format);
    std::vsnwprintf(buf, 4096, format, body);
    va_end(body);
    OutputDebugStringW(buf);
}

void show_console(bool show)
{
    static bool visible = false;
    if(show){
        if(!visible){
            AllocConsole();
            freopen("conin$", "r", stdin);
            freopen("conout$", "w", stdout);
            freopen("conerr$", "w", stderr);
            visible = true;
        }
    }
    else{
        if(visible){
            FreeConsole();
            visible = false;
        }
    }
}

#endif

//---------------------------------------------------------------------------
#ifdef CGL_DEBUG_MEMORY
// 检测内存遗漏，这个很久以前写的，没测试安全性

typedef struct _debugMEMINFO
{
    size_t Index;
    size_t Line;
    void*  Address;
    size_t Size;
    std::string File;
}debugMEMINFO;

class debugMemoryManage
{
public:
    ~debugMemoryManage()
    {
        OutToFile("LostMemory.txt");
        Recycle();
    }

    void New(const char* file, long line, void* address, long size)
    {
        debugMEMINFO meminfo;
        ++index;
        meminfo.Index   = index;
        meminfo.File    = file;
        meminfo.Line    = line;
        meminfo.Address = address;
        meminfo.Size    = size;
        MemData.insert(map<long, debugMEMINFO>::value_type((long)address, meminfo));
        //LogOut(tostr(index)+"\t"+file+"\t"+tostr(line)+"\t"+tostr(address)+"\t"+tostr(size), "new.txt");
    }

    void Delete(void* address)
    {
        map<long, debugMEMINFO>::iterator itr;
        itr = MemData.find( (long)address );
        if(itr!=MemData.end()){
            MemData.erase(itr);
        }
        //logOut(tostr(address), "delete.txt");
    }

    void OutToFile(const char* fn = "Memory.txt")
    {
        map<long, debugMEMINFO>::iterator itr;
        debugMEMINFO *p;
        time_t now = time(0);
        tm     time = *localtime(&now);
        _TCHAR buf[20];
        //#include <clocale>
        //setlocale(LC_TIME, "");
        _tcsftime(buf, 20, _T("%Y-%m-%d %X"), &time);

        FILE *f = _tfopen(fn, _T("a"));
        _ftprintf(f, _T("%s%s\n"), _T("----- Output Debug Inforno At Time : "), buf);
        _ftprintf(f, _T("%s\n"), _T("Index    Address   Size     Line     File"));

        itr = MemData.begin();
        for(;itr!=MemData.end();++itr)
        {
            p =&(*itr).second;
            _ftprintf(f, _T("%-8u %Np  %-8u %-8u %s\n"),
                p->Index, p->Address, p->Size, p->Line, p->File.c_str());
        }
        _ftprintf(f, _T("\n"));
        fclose(f);
    }

    //回收遗漏的内存
    void Recycle()
    {
        map<long, debugMEMINFO>::iterator itr;
        debugMEMINFO *p;

        itr = MemData.begin();
        for(;itr!=MemData.end();++itr){
            p = &(*itr).second;
            //delete p->Address;    //在devcpp中编译不通过
            free (long*)p->Address;
        }
    }
private:
    map<long, debugMEMINFO> MemData;
    size_t index;
};

//如果在这里直接定义或定义成static类型，
//内存里面未释放的指针数据就由操作系统来释放，而非这个类来释放了
CGL_PUBLIC_DECLARE debugMemoryManage MemManage = debugMemoryManage();

inline void * operator new(size_t size, const char* file, int line)
{
    void *ptr = malloc(size);
    MemManage.New(file, line, ptr, size);
    return(ptr);
};

inline void * operator new[](size_t size, const char* file, int line)
{
    void *ptr = malloc(size);
    MemManage.New(file, line, ptr, size);
    return(ptr);
};

inline void operator delete(void *p)
{
    MemManage.Delete(p);
    free(p);
    p = NULL;
}
inline void operator delete[](void *p)
{
    MemManage.Delete(p);
    free(p);
    p = NULL;
}

#define new new(__FILE__, __LINE__)
#define delete delete

#endif //CGL_DEBUG_MEMORY

}//end namespace cgl

#endif//CGL_DEBUG

//---------------------------------------------------------------------------
//
// 控制台操作
//

#if defined(__GNUC__) || defined(__clang__)

#include <cstdio>
#include <cstdlib>

void print(const char* str)
{
    printf("%s\n", str);
}

void print(const char* str1, const char* str2)
{
    printf("%s\t%s\n", str1, str2);
}

void print(int n)
{
    printf("%i\n", n);
}

void print(float a, float b)
{
    printf("%f\t%f\n", a, b);
}

//绘制一个字符图像
void char_image(unsigned char* data, int width, int height, int line_width = 0)
{
    if(!line_width)line_width = width;
    for (int j=0; j < height; ++j) {
        for (int i=0; i < width; ++i){
            putchar(" .:ioVM@"[data[j*width+i]/32]);
        }
        putchar('\n');
    }
}

#endif

//
// CGL_UNIT_TEST
//

#ifdef _DEBUG

#include <cgl/system/time.hpp>

#define CGL_UNIT_TEST_CLOCK (cgl::system::time::clock_nano())// (std::clock())

CGL_PUBLIC_DECLARE uint64_t CGL_UNIT_TEST_TIME = CGL_UNIT_TEST_CLOCK;

#define CGL_UNIT_TEST(function) \
    CGL_UNIT_TEST_TIME = CGL_UNIT_TEST_CLOCK;\
    function;\
    CGL_LOG("%8lld - %s", CGL_UNIT_TEST_CLOCK - CGL_UNIT_TEST_TIME, #function)

#else

#define CGL_UNIT_TEST(function) function

#endif

#endif//CGL_DEBUG_H_20060730191312

/*

#ifdef __STD_IOSTREAM__
std::cout 重定向

class custom_streambuf : public std::streambuf
{
private:
    std::streambuf * stdbuf;

public:
    custom_streambuf() : std::streambuf(), stdbuf(NULL)
    {
        this->attach();
    }
    
    ~custom_streambuf()
    {
        this->deattch();
    }

    virtual std::streamsize xsputn(const char* __s, std::streamsize __n)
    {
        print(__s);
    }

    void attach()
    {
        stdbuf = std::cout.rdbuf();
        std::cout.rdbuf(this);
    }

    void deattch()
    {
        if(stdbuf){
            std::cout.rdbuf(stdbuf);
            stdbuf = NULL;
        }
    }
}fileBuf;


//QT
class custom_streambuf : public std::basic_streambuf<char>
{
public:
    const static int MAX_BUFFER = 20;

private:
    std::streambuf * stdbuf;
    std::deque<std::string> buffer;

public:
    custom_streambuf() : std::streambuf(), stdbuf(NULL)
    {
        this->attach();
    }

    ~custom_streambuf()
    {
        this->deattch();
    }

    virtual int_type overflow(int_type __c  = traits_type::eof())
    {
        buffer.push_back(std::string());
        if(buffer.size() > MAX_BUFFER){
            buffer.pop_front();
        }
        return 0;
    }

    virtual std::streamsize xsputn(const char* __s, std::streamsize __n)
    {
        if(buffer.empty()){
            buffer.push_back(std::string());
        }
        buffer.back().append(std::string(__s, __n));
        return __n;
    }

public:
    void attach()
    {
        stdbuf = std::cout.rdbuf();
        std::cout.rdbuf(this);
    }

    void deattch()
    {
        if(stdbuf){
            std::cout.rdbuf(stdbuf);
            stdbuf = NULL;
        }
    }

    void paint(QPainter& dc, int x, int y)
    {
        int h = dc.fontMetrics().lineSpacing();
        for(size_t i=0; i<buffer.size(); ++i){
            dc.drawText(x, y + i * h, buffer[i].c_str());
        }
    }

}logBuf;

#endif
*/
