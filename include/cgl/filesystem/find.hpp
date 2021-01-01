/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 filefind.hpp

 2010-08-24 03:34:49

 14:30 2015/2/25

 class finder<char>
   bool find(path, find_file, find_directory)  //��ʼ����
   bool next()                      //������һ��
   bool finding()                   //�Ƿ������
   void close()                     //��������

*/
#ifndef FILEFIND_HPP_20100824033449
#define FILEFIND_HPP_20100824033449

#include <cgl/public.h>
#include <cgl/filesystem/dir.hpp>
#include <cgl/filesystem/filename.hpp>
#include <cgl/filesystem/path.hpp>
#include <cgl/std/string.hpp>

namespace cgl{

//�ļ����ҷ�ʽ
const static int ff_file      = 1;
const static int ff_directory = 2;
const static int ff_any       = 3;

namespace filesystem{

    template<typename _char_t>
    extern bool is_vtdir(const _char_t* path);

}//end namespace filesystem
}//end namespace cgl;

#ifdef _INC_WINDOWS

#ifndef CGL_PLATFORM_MSVC
#include <dir.h>
#endif

namespace cgl{
namespace filesystem{

//�б��ļ�
template<typename T>
int listfile(std::vector<string_t>& ls, string_t path, const string_t& ext = "*")
{
    WIN32_FIND_DATAA wfd;
    if(path.back() != '/' || path.back() != '\\')path.push_back('/');
    HANDLE h = FindFirstFileA((path + ext).c_str(), &wfd);
    if(!h || h == INVALID_HANDLE_VALUE)return -1;
    do{
        ls.push_back(wfd.cFileName);
    }while(FindNextFileA(h, &wfd));
    return 0;
}

//�о�Ŀ¼
//2020-02-20 00:01:32
typedef int (*ListFileCallBack)(const TCHAR* filename, const WIN32_FIND_DATA& wfd);

int listfile(const TCHAR* path, const TCHAR* ext, bool subdir, ListFileCallBack on_find)
{
    WIN32_FIND_DATA wfd;
    size_t size = cslen(path);
    size_t n;
    TCHAR filename[MAX_PATH] = {0};
    cscpy(filename, path, size);
    if(filename[size - 1] != '\\'){//����Ŀ¼
        filename[size] = '\\';
        ++size;
    }
    //filename[size] = '*';
    cscpy(filename + size, ext, cslen(ext));
    HANDLE hFind = FindFirstFile(filename, &wfd);
    if(hFind != INVALID_HANDLE_VALUE){
        do{
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
                if(!filesystem::is_vtdir(wfd.cFileName)){
                    n = cslen(wfd.cFileName);
                    cscpy(filename + size, wfd.cFileName, n);
                    filename[size + n] = '\\';
                    filename[size + n + 1] = '\0';
                    on_find(filename, wfd);
                    if(subdir){
                        listfile(filename, ext, subdir, on_find);
                    }
                }
            }
            else{
                n = cslen(wfd.cFileName);
                cscpy(filename + size, wfd.cFileName, n);
                on_find(filename, wfd);
            }
        }while(FindNextFile(hFind, &wfd));
    }
    return 0;
}

//
// finder<T>
//

template<typename _char_t>
class filefind
{
public:
    typedef TYPENAME ttype<_char_t, WIN32_FIND_DATAA, WIN32_FIND_DATAW>::type find_data;
    typedef int64_t time_type;

    handle_t handle;
    find_data data;
    uint32_t m_mode;

public:
    filefind() : handle(INVALID_HANDLE_VALUE), m_mode(ff_any)
    {
        memset(&data, 0, sizeof(find_data));
    }

    ~filefind()
    {
        this->close();
    }

    bool find(const _char_t* path, int mode = ff_any)
    {
        this->close();
        _char_t real_path[MAX_PATH] = {0};
        //֧��%appdata%�����Ļ�������
        tvalue<_char_t>(ExpandEnvironmentStringsA, ExpandEnvironmentStringsW)(path, real_path, MAX_PATH);

        //fixed
        /*
        size_t length = cslen(real_path);
        if( !is_dirbreak(real_path[length - 1]) ){
            real_path[length] = '\\';
            real_path[length + 1] = '*';
        }
        else{
            real_path[length] = '*';
        }
        */

        handle = tvalue<_char_t>(FindFirstFileA, FindFirstFileW)(real_path, &data);
        m_mode = mode;

        //�����һ���ļ����Բ�ƥ�䣬������һ��
        if(this->finding() && !check_attribute()){
            this->next();
        }
        return this->finding();
    }

    bool finding()const
    {
        return handle != INVALID_HANDLE_VALUE;
    }

    bool next()
    {//���û���ҵ����رղ���
        while(tvalue<_char_t>(FindNextFileA, FindNextFileW)(handle, &data)){
            if(this->check_attribute()){//����ƥ��ɹ�
                return true;
            }
        }
        this->close();
        return false;
    }

    void close()
    {
        if(this->finding()){
            FindClose(handle);
            handle = INVALID_HANDLE_VALUE;
        }
    }

    const _char_t* name()const
    {
        return data.cFileName;
    }

    uint32_t attrib()const
    {
        return data.dwFileAttributes;
    }

    int64_t size()const
    {
        return int64_t(data.nFileSizeHigh<<32 + data.nFileSizeLow);
    }

    ftime_t ctime()
    {
        return *(int64_t*)&data.ftCreationTime;
    }

    ftime_t wtime()
    {
        return *(int64_t*)&data.ftLastWriteTime;
    }

    ftime_t atime()
    {
        return *(int64_t*)&data.ftLastAccessTime;
    }

    bool is_directory()const
    {
        return data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
    }

private:
    bool check_attribute()
    {
        if(this->attrib() & FILE_ATTRIBUTE_DIRECTORY){
            if(m_mode & ff_directory && !is_vtdir(data.cFileName)){
                return true;
            }
        }
        else{
            if(m_mode & ff_file){
                return true;
            }
        }
        return false;
    }
};

}//end namespace filesystem
}//end namespace cgl

#else//CGL_PLATFORM_LINUX CGL_PLATFORM_ANDROID

#include <dirent.h>

/*
 dirent.d_type
 DT_FIFO   1  �ܵ��ļ�     (p)
 DT_CHR    2  �ַ��豸�ļ� (c)
 DT_DIR    4  Ŀ¼�ļ�     (d)
 DT_BLK    6  ���豸�ļ�   (b)
 DT_REG    8  ��ͨ�ļ�     (-)
 DT_LNK   10  �����ļ�     (l)
 DT_SOCK  12  �׽����ļ�   (s)
 DT_WHT   14
*/

namespace cgl{
namespace filesystem{

class filefind
{
public:
    typedef DIR *handle_type;
    typedef int64_t time_type;

    handle_type handle;
    dirent* dir;
    int m_mode;

public:
    filefind() : handle(null), m_mode(ff_any)
    {
    }

    ~filefind()
    {
        this->close();
    }

    //Ŀ¼���ֲ�����ͨ���*?
    bool find(const char* path, int mode)
    {
        this->close();
        m_mode = mode;
        handle = opendir(path);
        if(this->finding() && !this->check_attribute()){
            this->next();
        }
        this->next();
        return this->finding();
    }

    bool finding()const
    {
        return handle;
    }

    bool next()
    {
        while(dir = readdir(handle)){
            if(this->check_attribute()){
                return true;
            }
        }
        this->close();
        return false;
    }

    void close()
    {
        if(this->finding()){
            closedir(handle);
            handle = null;
        }
    }

    const char* name()const
    {
        //return handle-> .ff_name;
        return dir->d_name;
    }

    uint32_t attrib()const
    {
        return dir->d_type;
    }

    int64_t size()const
    {
        return 0;
    }

    time_type ctime()
    {
        return 0;
    }

    time_type wtime()
    {
        return 0;
    }

    time_type atime()
    {
        return 0;
    }

private:
    bool check_attribute()
    {
        if(dir->d_type == DT_DIR){
            if(m_mode & ff_directory){
                return true;
            }
        }
        else if(dir->d_type == DT_REG){
            if(m_mode & ff_file){
                return true;
            }
        }
        else{
            //...
        }
        return false;
    }
};

}//end namespace filesystem
}//end namespace cgl

#endif//_INC_WINDOWS

namespace cgl{

namespace filesystem{

int find_files(unistring_list& ls, unistring path, const unistring& filter)
{
    //ls.clear();

    if(path.empty()){
        return -1;
    }

    filesystem::path_fix(path, L'\\');

    filesystem::filefind<wchar_t> ff;
    //ff.find((path + L"*").c_str(), ff_any);
    ff.find(path.c_str(), ff_any);
    while(ff.finding()){
        if(ff.is_directory()){
            if(!filesystem::is_vtdir(ff.name())){
                find_files(ls, path + ff.name(), filter);
            }
        }
        else{
            unistring ext = filesystem::extract_file_ext<wchar_t>(ff.name());
            if(filesystem::check_file_ext(filter, ext)){
                ls.push_back(path + ff.name());
			}
        }

        ff.next();
    }
	return 0;
}

}
}

#endif //FILEFIND_HPP_20100824033449


/*
//#include <dir.h>
//ʹ�ñ�׼��
template<typename _char_t>
class finder
{
public:
    typedef ttype<_char_t, ffblk, _wffblk>::type find_data;
    typedef int64_t time_type;

    int handle;
    find_data data;
    int m_mode;

public:
    finder() : handle(-1), m_mode(ff_any)
    {
    }

    ~finder()
    {
        this->close();
    }

    bool find(const _char_t* path, int mode)
    {
        this->close();
        handle = tvalue<_char_t>(::findfirst, ::_wfindfirst)(fn, &data, a);
        return this->finding();
    }

    bool finding()const
    {
        return handle != -1;
    }

    bool next()
    {
        if(tvalue<_char_t>(::findnext, ::_wfindnext)(&data)){
            this->close();
            return false;
        }
        return true;
    }

    void close()
    {
        if(this->finding()){
            tvalue<_char_t>(::findclose, ::_wfindclose)(&data);
            handle = -1;
        }
    }

    const _char_t* name()const
    {
        return data.ff_name;
    }

    uint32_t attrib()const
    {
        return data.ff_attrib;
    }

    int64_t size()const
    {
        return data.ff_size;
    }

    time_type ctime()
    {
        return 0;
    }

    time_type wtime()
    {
        return 0;
    }

    time_type atime()
    {
        return 0;
    }

private:
    bool check_attribute()
    {
        if(this->attrib() & fa_directory){
            if(mode & ff_directory){
                return true;
            }
        }
        else{
            if(mode & ff_file){
                return true;
            }
        }
        return false;
    }
};
*/