/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 shared_obj.hpp

 2020-01-28 01:39:11

 windows:
   ���dll�����ڣ�ò��Ҳ��������Ȼ��ʲôҲ����

 linux:
 dlopen("true path", mode)
 mode��
����RTLD_LAZY   �ݻ�������������Ҫʱ�ٽ������
����RTLD_NOW    ��������������ǰ�������δ�����ķ��š�
����RTLD_LOCAL
����RTLD_GLOBAL ������������
����RTLD_GROUP
����RTLD_WORLD

*/
#ifndef DYNAMIC_LIB_HPP_20200128013911
#define DYNAMIC_LIB_HPP_20200128013911

#ifndef CGL_PLATFORM_WINDOWS
    #if defined(_WIN32) || defined(_WIN64) || defined(__NT__) || defined(_XBOX)
        #define CGL_PLATFORM_WINDOWS
    #endif
#endif

#ifdef CGL_PLATFORM_WINDOWS
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace cgl{

class shared_obj
{
public:
    #ifdef CGL_PLATFORM_WINDOWS
    typedef HMODULE handle_type;
    typedef FARPROC function_type;
    #else
    typedef void* handle_type;
    //typedef void (*function_type)(void);
    typedef void* function_type;
    #endif

    handle_type handle;

public:
    shared_obj() : handle(NULL)
    {
    }

    shared_obj(const char* filename)
    {
        this->open(filename);
    }

    ~shared_obj()
    {
        this->close();
    }

    int open(const char* filename)
    {
        this->close();
        #ifdef CGL_PLATFORM_WINDOWS
        handle = LoadLibraryA(filename);
        #else
        handle = dlopen(filename, RTLD_LAZY);
        #endif
        return 0;
    }

    void close()
    {
        #ifdef CGL_PLATFORM_WINDOWS
        FreeLibrary(handle);
        #else
        dlclose(handle);
        #endif
        handle = NULL;
    }

    inline function_type load(const char* name)
    {
        #ifdef CGL_PLATFORM_WINDOWS
        return GetProcAddress(handle, name);
        #else
        return dlsym(handle, name);
        #endif
    }
};

}//end namespace cgl

#endif //DYNAMIC_LIB_HPP_20200128013911