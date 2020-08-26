/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 typedef.hpp

 2006-10-11 21:02:22

*/
#ifndef TYPEDEF_HPP_20061011210222
#define TYPEDEF_HPP_20061011210222

#include <cgl/base/platform.h>
#include <cgl/base/byteorder.h>
#include <cgl/base/config.h>
#include <cgl/base/char.h>
#include <cgl/base/fixedpoint.h>
#include <cgl/base/float.h>
#include <cgl/base/int.h>
#include <cgl/base/is_zero.h>
#include <cgl/base/null.h>
#include <cgl/base/xtype.h>

#include <cstdio>
#include <cstdlib>
#include <limits>
#include <memory>

//handle_t
typedef void* handle_t;

//byte_t
typedef uint8_t byte_t;
#ifndef BYTE_MAX
#define BYTE_MIN 0
#define BYTE_MAX 255
#endif

//size_type
typedef uint32_t size32_t;
typedef uint64_t size64_t;
#define SIZE32_MIN UINT32_MIN
#define SIZE32_MAX UINT32_MAX
#define SIZE64_MIN UINT64_MIN
#define SIZE64_MAX UINT64_MAX

#define TEXT_MAX SIZE32_MAX
#define INDEX_MAX SIZE32_MAX

//2020/1/31 18:27
#define CGL_SWITCH_CASE(a) case a: return #a

//2020-05-09 11:52:35
#define sizeof_array(a) (sizeof(a) / sizeof(a[0]))

namespace cgl{

//ftype_t
typedef uint64_t ftype_t;

//fsize_t
typedef int64_t fsize_t;
#define FILE_SIZE_MAX INT64_MAX

//ftime_t
typedef int64_t ftime_t;

//identifier
typedef int64_t commptr_t;

/*
inline commptr_t commptr_init(int32_t x, int32_t y)
{
    union {
        struct {
            int32_t x, y;
        };
        int64_t value;
    }n = {x, y};
    return n.value;
}

inline commptr_t commptr_init(void* p)
{
    return reinterpret_cast<commptr_t>(p);
}

template<typename T>
T commptr_cast(const commptr_t& ptr)
{
    return T(ptr);
}

struct uniptr_t
{
    union{
        int64_t value;
        void*   pvoid;
    };

    void set(int x)
    {
        value = x;
    }

    void* get()
    {
        return pvoid;
    }

    operator int()const
    {
        return n;
    }

    operator void*()const
    {
        return (void*)n;
    }

    template<typename T>
    T* cast()
    {
        return reinterpret_cast<T*>(n);
    }
};

template<typename T>
uniptr_t& operator>>(uniptr_t& a, T& b)
{
    b = T(a.n);
    return a;
}
*/

//result_t
typedef int result_t;

//bool to integer
inline int bool_toint(bool value)
{
    return value ? 1 : 0;
}

//gen_code32
template<byte_t a, byte_t b, byte_t c, byte_t d>
class gen_code32
{
private:
    //根据enum的特性，变量值必须在编译期间计算出来
    //可以参与switch case条件
    enum{
        _value = (a << 0) | (b << 8) | (c << 16) | (d << 24)
    };
public:
    //保证64位也是32位值
    const static uint32_t value = _value;
};

//platform native type
#if defined(CGL_PLATFORM_WINDOWS)
    struct platform
    {
        typedef handle_t instance;
        typedef HDC      display;
        typedef HWND     window;
        typedef HBITMAP  bitmap;
    };
#elif defined(CGL_PLATFORM_ANDROID)
    struct platform
    {
        typedef handle_t instance;
        typedef handle_t display;
        typedef ANativeWindow * window;
        typedef handle_t bitmap;
    };
#elif defined(__unix__) || defined(__APPLE__)
    struct platform
    {
        typedef handle_t instance;
        typedef Display* display;
        typedef Window   window;
        typedef Pixmap   bitmap;
    };
#else
    struct platform
    {
        typedef handle_t instance;
        typedef handle_t display;
        typedef handle_t window;
        typedef handle_t bitmap;
    };
#endif

//int type attribute
bool get_attribute(int attrib, int value)
{
    return attrib & value;
}

void set_attribute(int& attrib, int value)
{
    attrib |= value;
}

void reset_attribute(int& attrib, int value)
{
    attrib &= ~value;
}

class attribute_t
{
protected:
    int attribute;

public:
    attribute_t() : attribute()
    {
    }

    void set(int value)
    {
        set_attribute(attribute, value);
    }

    void reset(int value)
    {
        reset_attribute(attribute, value);
    }

    bool get(int value)const
    {
        return get_attribute(attribute, value);
    }

    operator bool()const
    {
        return attribute;
    }
};

//min max
#ifndef CGL_PLATFORM_MSVC
template<typename T>
inline T max(T a, T b, T c)
{
    return a < b ? (b < c ? c : b) : (a < c ? c : a);
}

template<typename T>
inline T min(T a, T b, T c)
{
    return a < b ? (a < c ? a : c) : (b < c ? b : c);
}
#endif

template<typename T1, typename T2>
struct pairT
{
    T1 first;
    T2 second;
};

template<typename T, typename V>
struct simple
{
    union{
        T id;
        T name;
    };
    union{
        V value;
        V text;
    };
};

//
// static object
//

template<typename T>
class single
{
public:
    typedef T value_type;
    static T value;

protected:
    single() {/*void*/}
    virtual ~single() { /*void*/ }

private:
    single(const single&);
    single& operator=(const single&);
};

template<typename T>
T single<T>::value = T();


}//end namespace cgl

//CGL_PROPERTY(get=getXXX,put=setXXX) int value;
#if defined(__BORLANDC__) || defined(_MSC_VER)
    #ifndef CGL_PLATFORM_CB6
        #define CGL_PROPERTY1(...) __declspec(property(__VA_ARGS__))
    #endif
#else
    #define CGL_PROPERTY1(...)
#endif

/*

//enum class
#define enum_class struct

#define enum_class_type(type)\
    typedef type value_type;\
    operator type()const{ return value; }\
    void operator=(type n){ value = n; }\
    type value

#define enum_value const static value_type

//enum_class ABC
//{
//    enum_class_type(int);
//    enum_value A = 0;
//    enum_value B = 1;
//};

*/

//
// __cgl_property
//

#if defined(__BORLANDC__)  || defined(_MSC_VER)

#define __cgl_property(Class, Type, Name, Getter, Setter)\
    __declspec(property(get=Getter, put=Setter)) Type Name;

#define __cgl_property_readonly(Class, Type, Name, Getter)\
    __declspec(property(get=Getter)) Type Name;

#else //defined(__BORLANDC__)  || defined(_MSC_VER)

//模拟的property是占用空间的
#define __cgl_property(Class, Type, Name, Getter, Setter)\
    class __property_##Name\
    {\
    public:\
        char data[0];\
        friend class Class;\
        inline size_t offset() { return offsetof(Class, Name); }\
        inline Class* parent() { return reinterpret_cast<Class*>(size_t(this)-offset()); }\
        template<typename T> inline operator T() { return T(parent()->Getter()); }\
        inline __property_##Name & operator=(const Type& n) { parent()->Setter(n); return *this; }\
    }Name;

#define __cgl_property_readonly(Class, Type, Name, Getter)\
    class __property_##Name\
    {\
    public:\
        char data[0];\
        friend class Class;\
        inline size_t offset() { return offsetof(Class, Name); }\
        inline Class* parent() { return reinterpret_cast<Class*>(size_t(this)-offset()); }\
        template<typename T>inline operator T() { return T(parent()->Getter()); }\
    private:\
        __property_##Name & operator=(const Type& n);\
    }Name;

#endif//defined(__BORLANDC__) || defined(_MSC_VER)

#endif //TYPEDEF_HPP_20061011210222
