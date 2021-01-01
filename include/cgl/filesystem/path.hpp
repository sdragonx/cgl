/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 path.hpp

 2020-05-06 20:46:31

*/
#ifndef PATH_HPP_20200506204631
#define PATH_HPP_20200506204631

#include <cgl/public.h>

#include <fcntl.h>
#include <sys/stat.h>

#ifdef CGL_PLATFORM_MSVC
	#define S_ISDIR(mode) (mode & S_IFDIR)
#endif

namespace cgl{
namespace filesystem{


template<typename _char_t>
class path_traits
{
public:
    typedef _char_t char_type;

    const static char_type dirbreak = '/';
    const static char_type winbreak = '\\';
    const static char_type extbreak = '.';

    const static char_type cdir[2];
    const static char_type pdir[3];
    const static char_type dirbreaks[3];
};

template<typename _char_t>
const _char_t path_traits<_char_t>::cdir[2] = {'.'};

template<typename _char_t>
const _char_t path_traits<_char_t>::pdir[3] = {'.', '.'};

template<typename _char_t>
const _char_t path_traits<_char_t>::dirbreaks[3] = {'/', '\\'};


//判断文件是否存在。返回值：>0目录，<0文件，==0文件不存在
template<typename _char_t>
int file_exists_impl(const _char_t* filename)
{
    #ifdef CGL_PLATFORM_ANDROID
        //#pragma message file_exists<> error.
        return 0;
    #else
        struct _stat info;
        int n = tvalue<_char_t>(_stat, _wstat)(filename, &info);
        if(n == -1){
            return 0;
        }
        else if(S_ISDIR(info.st_mode)){
            return 1;
        }
        else{
            return -1;
        }
    #endif
}

template<typename _char_t>
bool file_exists(const _char_t* filename)               { return file_exists_impl(filename) < 0; }

template<typename _char_t>
bool file_exists(const cstring<_char_t>& filename)      { return file_exists_impl(filename.c_str()) < 0; }

template<typename _char_t>
bool directory_exists(const _char_t* filename)          { return file_exists_impl(filename) > 0; }

template<typename _char_t>
bool directory_exists(const cstring<_char_t>& filename) { return file_exists_impl(filename.c_str()) > 0; }

/*
template<typename _char_t>
bool is_file(const _char_t* filename)
{
    return file_exists<_char_t>(filename);
}

template<typename _char_t>
bool is_directory(const _char_t* filename)
{
    return directory_exists(filename);
}
*/

//判断目录是否是"."或".."
template<typename _char_t>
bool is_vtdir(const _char_t* path)
{
    if(!memcmp(path, path_traits<_char_t>::cdir, sizeof(path_traits<_char_t>::cdir))){
        return true;
    }
    if(!memcmp(path, path_traits<_char_t>::pdir, sizeof(path_traits<_char_t>::pdir))){
        return true;
    }
    return false;
}

//判断字符是否为目录分隔符
template<typename _char_t>
inline bool is_dirbreak(_char_t ch)
{
    return ch == path_traits<_char_t>::dirbreak || ch == path_traits<_char_t>::winbreak;
}

//把目录分隔符统一格式化成 '/'
template<typename _char_t>
void path_uniform(_char_t* path)
{
    for( ; path; ++path){
        if(*path == path_traits<_char_t>::winbreak){
            *path = path_traits<_char_t>::dirbreak;
        }
    }
}

template<typename _char_t>
void path_uniform(cstring<_char_t>& path)
{
    for(size_t i=0; i<path.length(); ++i){
        if(path[i] == path_traits<_char_t>::winbreak){
            path[i] = path_traits<_char_t>::dirbreak;
        }
    }
}

//路径后面保持有/
template<typename _char_t>
void path_fix(cstring<_char_t>& path, _char_t b = path_traits<char>::dirbreak)
{
    if(!path.empty()){
        if(!is_dirbreak(path.back())){
            path.push_back(b);
        }
    }
}

#ifdef CGL_PLATFORM_VCL
//vcl
String path_uniform(String& path)
{
    for(int i=1; i<=path.Length(); ++i){
        if(path[i] == path_traits<char>::winbreak)path[i] = path_traits<char>::dirbreak;
    }
    return path;
}

String path_fix(String& path, char b = path_traits<char>::dirbreak)
{
    if(!path.IsEmpty()){
        if(!is_dirbreak(path[path.Length()])){
            path += b;
        }
    }
    return path;
}
#endif//CGL_PLATFORM_VCL

//返回上层目录
template<typename _char_t>
void path_pop(cstring<_char_t>& path)
{
    if(path.empty())return ;
    for(size_t i=path.length()-1; i!=0; --i){
        if(is_dirbreak(path[i])){
            path.resize(i);
            break;
        }
    }
}

//获取文件的目录
template<typename T>
cstring<T> extract_filepath(const cstring<T>& path)
{
    size_t pos = path.find_last_of(path_traits<T>::dirbreaks);
    if(pos != cstring<T>::npos){
        ++pos;
        return path.substr(0, pos);
    }
    return cstring<T>();
}

//没有尾部的目录分隔符
template<typename T>
cstring<T> extract_directory(const cstring<T>& path)
{
    size_t pos = path.find_last_of(path_traits<T>::dirbreaks);
    if(pos != cstring<T>::npos){
        ++pos;
        return path.substr(0, pos-1);
    }
    return cstring<T>();
}

template<typename T>
cstring<T> extract_file_ext(const cstring<T>& filename)
{
    size_t pos = filename.find_last_of('.');
    if(pos != cstring<T>::npos){
        return filename.substr(pos, filename.length()-pos);
    }
    else{
        return cstring<T>();
    }
}

}//end namespace filesystem
}//end namespace cgl


/*


class path
{
public:
    typedef path this_type;
    typedef cstring<char> string_type;

    string_type m_path;

    path() : m_path()
    {
    }

    template<typename T>
    this_type& operator=(const T& str)
    {
        m_path = str;
        return *this;
    }


    string_t directory()
    {

    }

    string_t filename()
    {
    }

    string_t ext()
    {

    }

    string_t dirlist(int level)
    {
    }

    int dircount()//返回目录深度
    {
    }

    //string_type operator[](int index){}

    this_type& push(const string_type& name)
    {
        m_path.push_back('/');
        m_path += name;
        return *this;
    }

    pop()
    {
    }
};
*/

#endif //PATH_HPP_20200506204631

