/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 filesystem.hpp

 2010-08-24 03:31:55

S_ISLNK(st_mode)：是否是一个连接.
S_ISREG(st_mode)：是否是一个常规文件.
S_ISDIR(st_mode)：是否是一个目录
S_ISCHR(st_mode)：是否是一个字符设备.
S_ISBLK(st_mode)：是否是一个块设备
S_ISFIFO(st_mode)：是否 是一个FIFO文件.
S_ISSOCK(st_mode)：是否是一个SOCKET文件 

*/
#ifndef FILESYSTEM_HPP_20100824033155
#define FILESYSTEM_HPP_20100824033155

#include <cgl/string.hpp>
#include <cgl/io.hpp>
#include "filesystem/filename.hpp"
#include "filesystem/filetype.hpp"
//#include "filesystem/find.hpp"
//#include "filesystem/operation.hpp"
#include "filesystem/path.hpp"
#include "filesystem/temp.hpp"

#ifdef CGL_PLATFORM_VCL
#include <vcl.h>
#endif

namespace cgl{
namespace filesystem{

#if 0
//---------------------------------------------------------------------------
//xcopy
//xcopy mode
const static int XCOPY_MODE_SUBDIRECTORY = 1;//拷贝子目录
const static int XCOPY_MODE_SYSTEM       = 2;//系统文件
const static int XCOPY_MODE_HIDDEN       = 4;
const static int XCOPY_MODE_READONLY     = 8;
const static int XCOPY_MODE_ATTRIB       = 16;//复制全部属性

/*
UINT XCopy(
    string srcPath,         //源目录
    const string& filter,   //文件过滤
    string dstPath,         //目标目录
    bool subdir = true)     //是否拷贝子目录
{
    if(srcPath[srcPath.length()-1]!='\\')srcPath.push_back('\\');
    if(dstPath[dstPath.length()-1]!='\\')dstPath.push_back('\\');

    WIN32_FIND_DATA wfd;
    char ext[_MAX_EXT];
    HANDLE h = FindFirstFile((srcPath+"*.*").c_str(), &wfd);
    if(h!= (HANDLE)-1)
    {
        mkdir(dstPath.c_str()); //创建目标目录
        do
        {
            if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if(subdir && strcmp(wfd.cFileName, ".") && strcmp(wfd.cFileName, ".."))
                {
                    XCopy(srcPath + wfd.cFileName, filter, dstPath + wfd.cFileName);
                }
            }
            else
            {
                _splitpath(wfd.cFileName, NULL, NULL, NULL, ext);
                if( ( strlen(ext) && filter.find(ext)!=string::npos ) ||
                    filter.find("*.*")!=string::npos)
                {
                    CopyFile((srcPath + wfd.cFileName).c_str(),
                        (dstPath + wfd.cFileName).c_str(), 0);
                    Memo1->Lines->Add((dstPath + wfd.cFileName).c_str());
                }
            }
        }while(FindNextFile(h, &wfd));
        FindClose(h);
        return 0;
    }
    return -1;
}
*/

/*
xcopy a  b        a\*.*    b\*.*    创建b，复制
xcopy a\  b        a\*.*    b\*.*    创建b，复制

xcopy a  b\        a\*.*    b\a\*.*    创建b\a
xcopy a\ b\        a\*.*    b\*.*    直接复制
*/

const static int CHAR_NULL = '\0';

//#include <cgl/stdc.hpp>

template<typename _char_t>
UINT xcopy(const _char_t src_path[], const _char_t dst_path[], const _char_t filter[], int attrib)
{
    cgl::filefind<char> ff;
    _char_t src[MAX_PATH] = {0};
    _char_t dst[MAX_PATH] = {0};
    _char_t buf[MAX_PATH] = {0};

    size_t src_length = cslen<_char_t>(src_path);
    size_t dst_length = cslen<_char_t>(dst_path);

    //std::copy(src_path, src_path+src_length, src);
    //std::copy(dst_path, dst_path+dst_length, dst);
    cscpy<_char_t>(src, src_path);//, src_length);
    cscpy<_char_t>(dst, dst_path);//, dst_length);

    if(dst[dst_length-1]!='\\')
    {
        if(src[src_length-1]!='\\')
        {
            src[src_length] = '\\';
            src[++src_length] = CHAR_NULL;
        }
        mkdir(dst);
        dst[dst_length] = '\\';
        dst[++dst_length] = CHAR_NULL;
    }
    else
    {
        if(src[src_length-1]!='\\')
        {
            _splitpath(src, NULL, NULL, buf, NULL);
            strcpy(dst+dst_length, buf);
            mkdir(dst);
            src[src_length] = '\\';
            src[++src_length] = CHAR_NULL;
            dst[dst_length] = CHAR_NULL;
        }
    }

    if(attrib & fa_directory)
    {
        strcpy(src+src_length, filter);
        ff.open(src, attrib);
        src[src_length] = CHAR_NULL;
        for(; ff.is_find(); ff.next())
        {
            if(ff.attrib() & fa_directory)
            {
                if(strcmp(ff.name(), ".") && strcmp(ff.name(), ".."))
                {
                    strcpy(src+src_length, ff.name());
                    strcpy(dst+dst_length, ff.name());
                    xcopy(src, dst, filter, attrib);
                    src[src_length] = CHAR_NULL;
                    dst[dst_length] = CHAR_NULL;
                }
            }
        }
        ff.close();
    }

    attrib &= ~fa_directory;
    strcpy(src+src_length, filter);
    print(src);
    ff.open(src, attrib);
    src[src_length] = CHAR_NULL;
    for(; ff.is_find(); ff.next())
    {
        strcpy(src+src_length, ff.name());
        strcpy(dst+dst_length, ff.name());
        CopyFile(src, dst, !(attrib & fa_overwrite));
        src[src_length] = CHAR_NULL;
        dst[dst_length] = CHAR_NULL;
    }
    return 0;
}

#endif

///////////////////////////////////////////////////
#if 0
UINT XCopy(std::string src, std::string dst, const std::string& filter, int attrib)
{
    ffind<char> ff;
    char buf[MAX_PATH] = {0};

    if(dst[dst.length()-1]!='\\')
    {
        if(src[src.length()-1]!='\\')
        {
            src.push_back('\\');
        }
        mkdir(dst.c_str());
        dst.push_back('\\');
    }
    else
    {
        if(src[src.length()-1]!='\\')
        {
            _splitpath(src.c_str(), NULL, NULL, buf, NULL);
            mkdir((dst+buf).c_str());
            src.push_back('\\');
        }
    }

    if(attrib & fa_directory)
    {
        ff.find((src+filter).c_str(), attrib);
        for(; ff.is_find(); ff.next())
        {
            if(ff.attrib() & fa_directory)
            {
                if(strcmp(ff.name(), ".") && strcmp(ff.name(), ".."))
                {
                    XCopy(src + ff.name(), dst + ff.name(), filter, attrib);
                }
            }
        }
        ff.close();
    }

    attrib &= ~fa_directory;
    ff.open((src+filter).c_str(), attrib);
    for(; ff.is_find(); ff.next())
    {
        //CopyFile((src + ff.name()).c_str(), (dst + ff.name()).c_str(), !(attrib & fa_overwrite));
    }
    return 0;
}

#endif



/*
//删不掉的保护文件夹
int ProtextCreate(String Path)
{
    if(!IsPath(Path))Path+='/';
    if(!DirectoryExists(Path+L"sdragonx/any../"))
    {
        CreateDir(Path+L"/sdragonx");
        CreateDir(Path+L"/sdragonx/any../");
        FileSetAttr(Path + L"/sdragonx", faHidden|faSysFile|faReadOnly);
    }
    return 0;
}

int ProtextDelete(String Path)
{
    if(!DirectoryExists(Path+L"sdragonx/any../"))
    {
        FileSetAttr(Path + L"/sdragonx", 0);
        RemoveDir(Path+L"/sdragonx/any../");
        RemoveDir(Path+L"/sdragonx");
    }
    return 0;
}

bool ProtextExist(String Path)
{
    return DirectoryExists(Path+"/sdragonx/any../");
}
*/


}//end namesapce filesystem
}//end namespace cgl

#endif //FILESYSTEM_HPP_20100824033155
