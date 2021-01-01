/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fileop.hpp

 2020-05-26 11:06:05

 gccû�п��ַ���������

*/
#ifndef OPERATION_HPP_20200526110605
#define OPERATION_HPP_20200526110605

#include <cgl/public.h>

#ifdef CGL_PLATFORM_ANDROID
    #include <stdio.h>      //remove
    #include <unistd.h>     //rmdir
    #include <sys/stat.h>   //mkdir
#else
    #include <io.h>
#endif

namespace cgl{
namespace filesystem{

template<typename _char_t>
extern bool is_dirbreak(_char_t ch);

//�������ļ�
template<typename _char_t>
int renamefile(const _char_t* source, const _char_t* dest)
{
    return tvalue<_char_t>(STD::rename, STD::_wrename)(source, dest);
}

template<typename _char_t>
int renamefile(const cstring<_char_t>& source, const cstring<_char_t>& dest)
{
    return renamefile(source.c_str(), dest.c_str());
}

//ɾ���ļ�
int removefile(const char* filename)
{
    return STD::remove(filename);
}

int removefile(const wchar_t* filename)
{
    #if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)
        return STD::_wremove(filename);
    #else
        return ::_wremove(filename);
    #endif
}

template<typename _char_t>
int removefile(const cstring<_char_t>& filename)
{
    return removefile(filename.c_str());
}

//����Ŀ¼
template<typename _char_t>
int makedir(const _char_t* dir)
{
    #if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)
        return tvalue<_char_t>(STD::mkdir, STD::_wmkdir)(dir);
    #else
        return ::mkdir(dir, 0);
    #endif
}

//ɾ��Ŀ¼
template<typename _char_t>
int removedir(const _char_t* dir)
{
    #if defined(CGL_PLATFORM_CBUILDER) || defined(CGL_PLATFORM_MSVC)
        return tvalue<_char_t>(rmdir, _wrmdir)(dir);
    #else
        return rmdir(dir);
    #endif
}

template<typename _char_t>
inline int removedir(const cstring<_char_t>& dir)
{
    return removedir(dir.c_str());
}

//�����ļ�
template<typename _char_t>
int copyfile(const _char_t* src, const _char_t* dst, bool overwrite)//true�����ļ�
{
    #ifdef CGL_PLATFORM_WINDOWS

    return tvalue<_char_t>(CopyFileA, CopyFileW)(src, dst, !overwrite);

    #else

    FILE *in, *out;
    char buf[1024];

    int exist = file_exists(dst);

    if(exist < 0){//�ļ�����
        if(!overwrite){
            return FILE_EXISTS;
        }
    }
    else if(exist > 0){//�ļ�����Ŀ¼
        return FILE_IS_DIRECTORY;
    }

    in = tvalue<_char_t>(fopen, _wfopen)(src, T_TEXT(_char_t, "r+"));
    if(!in){
        return FILE_OPEN_ERROR;
    }

    out = tvalue<_char_t>(fopen, _wfopen)(dst, T_TEXT(_char_t, "w+"));
    if(!out){
        fclose(in);
        return FILE_OPEN_ERROR;
    }

    int len;
    while(len = fread(buf, 1, sizeof(buf), in))
    {
        fwrite(buf, 1, len, out);
    }

    fclose(in);
    fclose(out);

    //����ʱ�䡢Ȩ�ޡ�����
    //...

    return 0;

    #endif
}

//�������Ŀ¼
template<typename _char_t>
int xmkdir(const _char_t* dir)
{
    _char_t buf[260] = {0};
    size_t length = cslen(dir);
    size_t i = 0;

    //"C:/"
    if(dir[1]==':' && is_dirbreak<_char_t>(dir[2])){
        cscpy(buf, dir, 3);
        i = 3;
    }

    for( ; i < length; ++i){
        if( is_dirbreak<_char_t>(dir[i]) ){
            makedir<_char_t>(buf);
            buf[i] = '/';
        }
        else{
            buf[i] = dir[i];
        }
    }

    if( !is_dirbreak<_char_t>(buf[i - 1]) ){
        makedir<_char_t>(buf);
    }

    return 0;
}

//
// xdel(path, name, attribute)
//
// ɾ���༶�ļ���Ŀ¼
// xdel("aa/bb/cc", "*.txt", fa_any);
template<typename _char_t>
int xdel(const cstring<_char_t>& path, const cstring<_char_t>& name, int attribute = fa_any)
{
    //ɾ���ļ�
    if(filesystem::file_exists(path)){
        return filesystem::removefile(path.c_str());
    }
    else{//ɾ��Ŀ¼
        filesystem::filefind<_char_t> ff;
        cstring<_char_t> fn;

        fn = path + '/' + name;
        print(fn.c_str());
        ff.find(fn.c_str(), cgl::fa_any);
        while(ff.finding()){
            fn = path + '/' + ff.name();
            print(fn.c_str());
            if(ff.attrib() & cgl::fa_directory){//dir
                if(attribute & fa_child){//�Ƿ�ɾ����Ŀ¼
                    if( !filesystem::is_vtdir(ff.name()) ){
                        xdel(fn, name, attribute);
                    }
                }
            }
            else{//file
                //ȥ��ֻ�����ԣ�windows��ֻ���ļ�ɾ����
                if(ff.attrib() & cgl::fa_readonly){
                    if(attribute & fa_readonly){
                        chmod(fn.c_str(), S_IRWXU);
                        filesystem::removefile(fn.c_str());
                    }
                }
                else{
                    filesystem::removefile(fn.c_str());
                }
            }
            if(!ff.next())break;
        }
        ff.close();
        if(attribute & fa_directory){//�Ƿ�ɾ��Ŀ¼
            filesystem::removedir(path);
        }
        return 0;
    }
}


}//end namespace filesystem
}//end namespace cgl

#endif //OPERATION_HPP_20200526110605
