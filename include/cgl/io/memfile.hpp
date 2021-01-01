/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 memfile.hpp

 2016-07-18 02:10:02

 默认空间是不连续的

*/
#ifndef IO_MEMORY_H_20160718021002
#define IO_MEMORY_H_20160718021002

#include <cgl/io/unifile.hpp>
#include <vector>

namespace cgl{
namespace io{

//内存文件
class memfile : public unifile
{
public:
    typedef memfile this_type;

private:
    typedef std::vector<byte_t> buffer;
    typedef std::vector<buffer> file_buf;

    file_buf m_buffer;
    fsize_t  m_seek;

public:
    memfile();
    ~memfile();
    int open(const char* fn, int mode, int share);
    int open(const wchar_t* fn, int mode, int share);
    int open(unifile&, int, int);
    void close();
    bool is_open() { return true; }
    fsize_t seek(fsize_t pos, int whence);
    fsize_t tell() {return m_seek;}
    fsize_t size();
    fsize_t resize(fsize_t size);
    fsize_t read(void* data, fsize_t size);
    fsize_t write(const void* data, fsize_t size);
};

memfile::memfile():m_seek(0)
{
}

memfile::~memfile()
{
    this->close();
}

int memfile::open(const char* fn, int mode, int share)
{
    unifile::m_name = fn;
    m_seek = 0;
    return 0;
}

int memfile::open(const wchar_t* fn, int mode, int share)
{
    unifile::m_name = fn;
    m_seek = 0;
    return 0;
}

int memfile::open(unifile&, int, int)
{
    return 0;
}

void memfile::close()
{
    file_buf temp;
    m_buffer.swap(temp);
    m_seek = 0;
}

fsize_t memfile::seek(fsize_t pos, int whence)
{
    switch(whence)
    {
    case seek_begin:
        m_seek = pos;
        break;
    case seek_current:
        m_seek += pos;
        break;
    case seek_end:
        m_seek = this->size() - pos;
        break;
    }
    return m_seek;
}

fsize_t memfile::size()
{
    fsize_t size = 0;
    for(size_t i=0; i < m_buffer.size(); ++i){
        size += m_buffer[i].size();
    }
    return size;
}

fsize_t memfile::resize(fsize_t size)
{
    if(!size){
        this->close();
    }
    else{
        fsize_t n = this->size();
        if(size == n){
            return size;
        }
        else if(size > n){
            //if(size - n < 1024)size = 1024;//块空间对齐
            m_buffer.push_back(buffer());
            m_buffer.back().resize(size-n);
        }
        else{
            do{
                if(m_buffer.back().size() > (n - size)){
                    m_buffer.back().resize(m_buffer.back().size() - (n - size));
                }
                else{
                    m_buffer.pop_back();
                }
                n = this->size();
            }while(n > size);
        }
    }

    return size;
}

fsize_t memfile::read(void* data, fsize_t size)
{
    if(!size)return 0;

    if(this->size() - this->tell() < size){
        size = this->size() - this->tell();
        if(!size)return 0;
    }

    fsize_t fpos = this->tell();
    fsize_t read = size;
    byte_t* pread = static_cast<byte_t*>(data);

    file_buf::iterator itr = m_buffer.begin();
    while(itr->size() <= fpos){
        fpos -= itr->size();
        ++itr;
    }

    do{
        if(read <= itr->size()-fpos){
            memcpy(pread, &itr->at(fpos), read);
            this->seek(read, seek_current);
            read = 0;//break;
        }
        else{
            fsize_t n = itr->size()-fpos;
            memcpy(pread, &itr->at(fpos), n);
            this->seek(n, seek_current);
            read -= n;
            pread += static_cast<int>(n);
            fpos = 0;
            ++itr;
        }
    }while(read);

    return size;
}

fsize_t memfile::write(const void* data, fsize_t size)
{
    if(!size) return 0;
    if(this->size() < this->tell() + size){
        this->resize(this->tell() + size);
    }

    fsize_t seek = m_seek;
    fsize_t write = size;
    const byte_t* pwrite = static_cast<const byte_t*>(data);

    file_buf::iterator itr = m_buffer.begin();
    //查找当前写入块
    while(itr->size() <= seek){
        seek -= itr->size();
        ++itr;
    }

    do{
        if(write <= itr->size() - seek){
            memcpy(&itr->at(seek), pwrite, write);
            this->seek(write, seek_current);
            write = 0;//break;
        }
        else{
            fsize_t n = itr->size() - seek;
            memcpy(&itr->at(seek), pwrite, n);
            this->seek(n, seek_current);
            write -= n;
            pwrite += static_cast<int>(n);
            seek = 0;
            ++itr;
        }
    }while(write);
    return size;
}


}//end namespace io
}//end namespace cgl

#endif //IO_MEMORY_H_20160718021002
