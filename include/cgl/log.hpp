/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 log.hpp

 2018-02-24 23:05:28

*/
#ifndef LOG_HPP_20180224230528
#define LOG_HPP_20180224230528

#include <cgl/public.h>
#include <cgl/string.hpp>

#include <stdio.h>

#include <iostream>
#include <fstream>

#ifdef CGL_PLATFORM_VCL
    #include <vcl.h>
#endif

//__STD_IOSTREAM__

namespace cgl{

typedef void (*stdprint)(const char*);
typedef void (*stdwprint)(const wchar_t*);

class unistreambuf
{
public:
    handle_t handle;

public:
    virtual void clear() { }
    virtual int put( char ) { return 0; }
    virtual int put( wchar_t ) { return 0; }
    virtual int print( const char* ) { return 0; }
    virtual int print( const wchar_t* ) { return 0; }
    virtual void flush() { }
};

class unistdout : public unistreambuf
{
public:
    void clear()
    {
        using namespace std;
        #ifdef CGL_PLATFORM_WINDOWS
        ::system("cls");
        #else
        printf("\033c");
        #endif
    }

    int put(char ch)
    {
        using namespace std;
        return putchar(ch);
    }

    int put(wchar_t ch)
    {
        using namespace std;
        return putchar(ch);
    }

    int print(const char* str)
    {
        using namespace std;
        return printf(str);
    }

    int print(const wchar_t* str)
    {
        using namespace std;
        return wprintf(str);
    }

    void flush()
    {
        using namespace std;
        fflush(stdout);
    }
};

#ifdef CGL_PLATFORM_WINDOWS

class textbox_stmbuf: public unistreambuf
{
public:
    HWND handle;

public:
    textbox_stmbuf() : handle()
    {
    }

    void clear()
    {
        using namespace std;
        #ifdef CGL_PLATFORM_WINDOWS
        ::system("cls");
        #else
        printf("\033c");
        #endif
    }

    //这个暂时没有转换
    int put(char ch)
    {
        TCHAR buf[2] = {ch};
        return internal_print(buf);
    }

    int put(wchar_t ch)
    {
        TCHAR buf[2] = {ch};
        return internal_print(buf);
    }

    int print(const char* str)
    {
        #ifdef UNICODE
        return internal_print(unistring(str).c_str());
        #else
        return internal_print(str);
        #endif
    }

    int print(const wchar_t* str)
    {
        #ifdef UNICODE
        return internal_print(str);
        #else
        return internal_print(string_t(str).c_str());
        #endif
    }

    void flush()
    {
    }

public:
    int internal_print(const TCHAR* text)
    {
        SendMessage(handle, EM_SETSEL, -1, 0);
        return SendMessage(handle, EM_REPLACESEL, 0, (LPARAM)text);
    }
};

#endif

template<typename _char_t>
class unistream
{
public:
    typedef unistream this_type;
    typedef _char_t char_type;
    const static char_type camma[3];    //", "

    unistreambuf* m_stmbuf;

    static unistdout m_stdout;

public:
    handle_t textbox;

public:
    unistream() : m_stmbuf(&m_stdout), textbox() { }

    virtual void dispose()=0;

    virtual void clear()
    {
        m_stmbuf->clear();
    }

    virtual this_type& print(const char* text)
    {
        m_stmbuf->print(text);
        return *this;
    }

    virtual this_type& print(const wchar_t* text)
    {
        m_stmbuf->print(text);
    }

    this_type& operator<(const char* text)
    {
        this->clear();
        return this->print(text);
    }

    this_type& operator<(const wchar_t* text)
    {
        this->clear();
        return this->print(text);
    }

    this_type& operator<<(const char* text)
    {
        return this->print(text);
    }

    this_type& operator<<(const wchar_t* text)
    {
        return this->print(text);
    }

    void setbuf(unistreambuf* buf)
    {
        if(buf){
            m_stmbuf = buf;
        }
        else{
            m_stmbuf = &m_stdout;
        }
    }

private:
//    unistream(const unistream&);
//    unistream& operator=(const unistream&);
};

template<typename T>
CGL_PUBLIC_DECLARE const T unistream<T>::camma[3] = {',', ' ', '\0'};

template<typename T>
CGL_PUBLIC_DECLARE unistdout unistream<T>::m_stdout = unistdout();


class unilog : public unistream<char>
{
public:
    typedef unilog this_type;
    typedef char char_type;


    //bool stdout;                  //output to stdout
    cstring<char_type> filename;    //output to file
private:
    #ifdef CGL_PLATFORM_VCL
    TMemo* m_memo;
    #endif

    char_type m_streambuf[4096];

public:
    unilog() : unistream<char>()
    {
        #ifdef CGL_PLATFORM_VCL
        m_memo = null;
        #endif
    }

    unilog(const this_type& log)
    {
        #ifdef CGL_PLATFORM_VCL
        m_memo = log.m_memo;
        #endif
        filename = log.filename;
    }

    ~unilog()
    {
        this->dispose();
    }

    void dispose()
    {
        #ifdef CGL_PLATFORM_VCL
        m_memo = null;
        #endif
        filename.clear();
    }

public://basic
    void clear()
    {
        #ifdef CGL_PLATFORM_VCL
        if(m_memo)m_memo->Lines->Clear();
        #endif

        if(!filename.empty()){
            std::basic_ofstream<TCHAR> f(filename.c_str(), std::ios::trunc);
        }
    }

    this_type& print(const char* text)    { return m_out(text); }
    this_type& print(const wchar_t* text) { return m_out(text); }

    #ifdef CGL_PLATFORM_VCL
    __property TMemo* Memo={read=m_memo, write=m_memo};

    this_type& m_out(String text)
    {
        if(m_memo){
            //m_memo->SelLength = 0;
            m_memo->SelStart = m_memo->Text.Length();
            m_memo->SelText = text;
        }
        else{
            debug_out(text.c_str());
        }

        if(!filename.empty()){
            std::basic_ofstream<wchar_t> f(filename.c_str(), std::ios::app|std::ios::ate);
            f<<text.c_str();
        }
        return *this;
    }

    #else
    this_type& m_out(const cstring<char_type>& text)
    {
        using namespace std;
        printf(text.c_str());
        if(!filename.empty()){
            basic_ofstream<wchar_t> f(filename.c_str(), ios::app|ios::ate);
            f<<text.c_str();
        }
        return *this;
    }
    #endif

    //output to ide messagebox
    void debug_out(const char* str);
    void debug_out(const wchar_t* str);
};

//---------------------------------------------------------------------------
//debug_out
#ifdef CGL_PLATFORM_WINDOWS
void unilog::debug_out(const char* str)
{
    OutputDebugStringA(str);
}

void unilog::debug_out(const wchar_t* str)
{
    OutputDebugStringW(str);
}
#else
template<typename T>
void unilog<T>::debug_out(const char* str)
{
    printf(str);
}

template<typename T>
void unilog<T>::debug_out(const wchar_t* str)
{
    wprintf(str);
}
#endif//CGL_PLATFORM_WINDOWS

//---------------------------------------------------------------------------
//std

//支持std::endl
template<typename T>
unistream<T>& operator<<(unistream<T>& log, std::ostream& (*fun)(std::ostream&) )
{
    return log.print(endline_t::c_str<unistream<T>::char_type>());
}

//支持std::string
template<typename T>
unistream<T>& operator<<(unistream<T>& log, const std::string& text)
{
    return log.print(text.c_str());
}

//支持std::wstring
template<typename T>
unistream<T>& operator<<(unistream<T>& log, const std::wstring& text)
{
    return log.print(text.c_str());
}

//---------------------------------------------------------------------------
//cgl::endline
template<typename T>
unistream<T>& operator<<(unistream<T>& log, endline_t)
{//endline
    return log.print(endline_t::c_str<unistream<T>::char_type>());
}

//---------------------------------------------------------------------------
#ifdef CGL_PLATFORM_VCL
template<typename T>
unistream<T>& operator<<(unistream<T>& log, String text)
{
    return log.print(text.c_str());
}

template<typename T>
unistream<T>& operator<(unistream<T>& log, String text)
{
    log.clear();
    return log.print(text.c_str());
}

/*
unistream<T>& operator<<(int n)
{
    this->m_out(String(n));
}

unistream<T>& operator<<(float n)
{
    this->m_out(FormatFloat("0.0000000", n));
}
*/

template<typename T>
unistream<T>& operator<<(unistream<T>& log, const TPoint& p)
{
    char buf[64];
    STD::snprintf(buf, 64, "(%i, %i)", p.x, p.y);
    return log.print(buf);
}

template<typename T>
unistream<T>& operator<<(unistream<T>& log, const TRect& rc)
{
    char buf[128];
    STD::snprintf(buf, 256, "[%i, %i]-[%i, %i]", rc.left, rc.top, rc.Width(), rc.Height());
    return log.print(buf);
}

#else
template<typename T>
unistream<T>& operator<<(unistream<T>& log, const string_t& text)
{
    return log.print(text.c_str());
}
#endif

//---------------------------------------------------------------------------

CGL_PUBLIC_DECLARE unilog mlog = unilog();

//---------------------------------------------------------------------------
#ifdef _DEBUG
inline void _debug_log(const char* title, const char* text, const char* file, int line)
{
    using namespace std;
    char buf[1024];
    snprintf(buf, 1024, "%08X %s\t%s\t%s(%i)", clock(), title, text, file, line);
    mlog<<buf<<endline;
}
#define debug_log(title, text) _debug_log(title, text, __FILE__, __LINE__)
#else
#define debug_log(title, text)
#endif

}//end namespace cgl

#endif //LOG_HPP_20180224230528
