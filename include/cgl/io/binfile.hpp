/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 binfile.hpp

 2016-07-18 02:20:01

 2018/9/24 16:17

*/
#ifndef BINFILE_H_20160718022001
#define BINFILE_H_20160718022001

#include <cgl/io/unifile.hpp>
#include <cgl/io/fsync.hpp>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
vc cb supperted
#define O_DENYALL   0x10
#define O_DENYWRITE 0x20
#define O_DENYREAD  0x30
#define O_DENYNONE  0x40

#define O_DSYNC 0x0010
#define __O_SYNC 0x4000
#define O_SYNC (__O_SYNC | O_DSYNC)
#define O_DIRECT 0x8000
#define O_LARGEFILE 0x2000

读写权限
S_IRUSR/S_IWUSR/S_IXUSR：Owner的读、写、执行权限
S_IRGRP/S_IWGRP/S_IXGRP：Owner所在组中成员的读、写、执行权限
S_IROTH/S_IWOTH/S_IXOTH：其他任何用户的读、写、执行权限
S_IREAD|S_IWRITE|S_IEXEC：一些平台不支持
*/

namespace cgl{
namespace io{

//普通文件操作
class binfile : public unifile
{
public:
    binfile();
    ~binfile();
    int open(const char* fn, int mode, int share = sh_share);
    int open(const wchar_t* fn, int mode, int share = sh_share);
    void close();
    bool is_open();
    fsize_t read(void* data, fsize_t size);
    fsize_t write(const void* data, fsize_t size);
    fsize_t seek(fsize_t pos, int whence);
    fsize_t tell();
    fsize_t size();
    fsize_t resize(fsize_t size);
    void flush();
private:
    int open(const char* afn, const wchar_t* wfn, int mode, int share);
};

binfile::binfile() : unifile()
{

}

binfile::~binfile()
{
    this->close();
}

int binfile::open(const char* fn, int mode, int share)
{
    return this->open(fn, NULL, mode, share);
}

int binfile::open(const wchar_t* fn, int mode, int share)
{
    return this->open(NULL, fn, mode, share);
}

int binfile::open(const char* afn, const wchar_t* wfn, int mode, int share)
{
    this->close();

    int imode;
    int ishare = 0;

    imode = (mode & io_write) ? O_RDWR : O_RDONLY;
    if(mode & io_create){
        imode |= O_CREAT;
    }

    if(mode & io_append){
        imode |= O_APPEND;
    }

    if(mode & io_trunc){
        imode |= O_TRUNC;
    }

    #ifdef O_LARGEFILE
    imode |= O_LARGEFILE;
    #endif

    #ifdef O_BINARY
    imode |= O_BINARY;
    #endif

    //C++Builder && VC only, lock file
    //g++下面会删除文件!!
    #ifdef O_DENYALL
    switch(share){
    case sh_none:
        imode |= O_DENYALL;
        break;
    case sh_read:
        imode |= O_DENYWRITE;
        break;
    case sh_write:
        imode |= O_DENYREAD;
        break;
    case sh_share:
    default:
        imode |= O_DENYNONE;
        break;
    }
    #endif//O_DENYALL

	#ifndef CGL_PLATFORM_MSVC
    switch(share){
    case sh_none:
        ishare = 0;
        break;
    case sh_read:
        ishare = S_IRUSR|S_IXUSR;
        break;
    case sh_write:
        ishare = S_IWUSR|S_IXUSR;
        break;
    case sh_share:
    default:
        ishare = S_IRUSR|S_IWUSR|S_IXUSR;
        break;
    }
	#endif
	//_SH_DENYRW

    if(afn){
        m_id = ::open(afn, imode, ishare);
        unifile::m_name  = afn;
    }
    else if(wfn){
        #ifdef CGL_PLATFORM_ANDROID
        m_id = ::open(string_t(wfn).c_str(), imode, ishare);
        #else
        m_id = ::_wopen(wfn, imode, ishare);
        #endif
        unifile::m_name = wfn;
    }
    else{
        return FILE_ERROR;
    }

    if(this->is_open()){
        return FILE_SUCCESS;
    }
    else{
        m_id = 0;
        unifile::m_name.clear();
        return FILE_ERROR;
    }
}

void binfile::close()
{
    if(this->is_open())
    {
        ::close(m_id);
        m_id = 0;
    }
}

bool binfile::is_open()
{
    return m_id && (m_id != -1);
}

fsize_t binfile::seek(fsize_t pos, int whence)
{
    CGL_ASSERT(this->is_open());
    return seeki64(m_id, pos, whence);
}

fsize_t binfile::tell()
{
    CGL_ASSERT(this->is_open());
    return telli64(m_id);
}

fsize_t binfile::size()
{
    CGL_ASSERT(this->is_open());
    return filelengthi64(m_id);
}

fsize_t binfile::resize(fsize_t size)
{
    CGL_ASSERT(this->is_open());
    return chsizei64(m_id, size);
}

void binfile::flush()
{
    CGL_ASSERT(this->is_open());
    fdatasync(m_id);
}

fsize_t binfile::read(void* data, fsize_t size)
{
    CGL_ASSERT(this->is_open());
    return ::read(m_id, data, static_cast<size_t>(size));
}

fsize_t binfile::write(const void* data, fsize_t size)
{
    CGL_ASSERT(this->is_open());
    return ::write(m_id, data, static_cast<size_t>(size));
}

}//end namespace io
}//end namespace cgl

#endif //BINFILE_H_20160718022001
