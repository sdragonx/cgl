/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 sector.hpp

 2016-07-18 02:26:50

 文件即时读写，自定义缓冲区
 以扇区模式读写，每次读写最小单位，一个扇区

*/
#ifndef IO_SECTOR_H_20160718022650
#define IO_SECTOR_H_20160718022650

#ifdef CGL_PLATFORM_WINDOWS

#include <cgl/io/unifile.hpp>
#include <cgl/memory/lru_buffer.hpp>

namespace cgl{
namespace io{

class fsector : public unifile
{
public:
    const static int SECTOR_SIZE = CGL_FILE_SECTOR_SIZE;
    typedef byte_t buffer[SECTOR_SIZE];
    typedef lru_cache<buffer> cache_type;
    typedef TYPENAME cache_type::buffer_type buffer_type;
private:
    handle_t m_handle;
    fsize_t  m_seekpos;
    cache_type m_buffer;
public:
    fsector():m_handle(NULL), m_seekpos(0), m_buffer(8)
    {
        m_buffer.on_update = update_buffer;
    }
    ~fsector()
    {
        this->close();
    }

    int open(const char* fname, int, int)
    {
        return this->open(fname, null, 0, 0);
    }

    int open(const wchar_t* fname, int, int)
    {
        return this->open(null, fname, 0, 0);
    }

    void close()
    {
        if(m_handle){
            this->flush();
            CloseHandle(m_handle);
            m_handle = NULL;
        }
    }

    bool is_open() { return m_handle; }

    fsize_t read(void* data, fsize_t size)
    {
        byte_t *pdata = reinterpret_cast<byte_t*>(data);
        size_t sector = m_seekpos / SECTOR_SIZE;
        size_t offset = 0;
        size_t result = 0;
        size_t n      = 0;
        buffer_type *buffer = NULL;

        while(size)
        {
            buffer = get_buffer(sector);
            offset = m_seekpos % SECTOR_SIZE;

            if(offset+size < SECTOR_SIZE){
                n = size;
            }
            else{
                n = SECTOR_SIZE - offset;
                ++sector;
            }
            memcpy(pdata, buffer->value + offset, n);
            pdata     += n;
            size      -= n;
            result    += n;
            m_seekpos += n;
        }
        return result;
    }

    fsize_t write(const void* data, fsize_t size)
    {
        const byte_t *pdata = reinterpret_cast<const byte_t*>(data);
        size_t sector = m_seekpos / SECTOR_SIZE;
        size_t offset = 0;
        size_t result = 0;
        size_t n      = 0;
        buffer_type *buffer = NULL;

        while(size)
        {
            buffer = get_buffer(sector);
            offset = m_seekpos % SECTOR_SIZE;

            if(offset+size < SECTOR_SIZE){
                n = size;
            }
            else{
                n = SECTOR_SIZE - offset;
                ++sector;
            }
            memcpy(buffer->value + offset, pdata, n);
            pdata     += n;
            size      -= n;
            result    += n;
            m_seekpos += n;

            buffer->user = CGL_ONE;
        }
        return result;
    }

    fsize_t seek(fsize_t seekpos, int whence)
    {
        switch(whence)
        {
        case FILE_BEGIN:
            m_seekpos = seekpos;
            break;
        case FILE_CURRENT:
            m_seekpos += seekpos;
            break;
        case FILE_END:
            m_seekpos = size();
            break;
        default:
            break;
        }
        return m_seekpos;
    }

    fsize_t tell()
    {
        return m_seekpos;
    }

    fsize_t size()
    {
        return FileLength(m_handle);
    }

    fsize_t resize(fsize_t size)
    {
        return 0;
    }

    void flush()
    {
        //flush all buffer
        for(size_t i=0; i<8; ++i){
            flush(&m_buffer[i]);
        }
    }

    fsize_t set_buffer(fsize_t size)
    {
        this->flush();
        m_buffer.init(size / SECTOR_SIZE);
        return size;
    }
private:
    int open(const char* afn, const wchar_t* wfn, int, int)
    {
        this->close();
        if(afn){
            m_handle = CreateFileA(afn,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
                NULL);
        }
        else if(wfn){
            m_handle = CreateFileW(wfn,
                GENERIC_READ | GENERIC_WRITE,
                FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_NO_BUFFERING | FILE_FLAG_WRITE_THROUGH,
                NULL);
        }
        if(m_handle == INVALID_HANDLE_VALUE){
            m_handle = NULL;
        }
        return 0;
    }

    buffer_type* get_buffer(fsize_t sector)
    {
        size_t n = m_buffer.allocate(sector);
        return &m_buffer[n];
    }

    fsize_t buffer_read(fsize_t address, buffer_type* buffer)
    {
        DWORD dwRead = 0;
        SetFilePointer(m_handle, buffer->address() * SECTOR_SIZE, NULL, FILE_BEGIN);
        ReadFile(m_handle, buffer->value, SECTOR_SIZE, &dwRead, NULL);
        return dwRead;
    }

    fsize_t buffer_write(buffer_type* buffer)
    {
        DWORD dwWritten = 0;
        SetFilePointer(m_handle, buffer->address() * SECTOR_SIZE, NULL, FILE_BEGIN);
        WriteFile(m_handle, buffer->value, SECTOR_SIZE, &dwWritten, NULL);
        return dwWritten;
    }

    int flush(buffer_type* buffer)
    {
        if(buffer->user){
            buffer_write(buffer);
            buffer->user = CGL_ZERO;
        }
        return 0;
    }

    static int update_buffer(void *object, buffer_type* buffer, fsize_t address = -1);
};

int fsector::update_buffer(void *object, buffer_type* buffer, fsize_t address)
{
    fsector* f = static_cast<fsector*>(object);
    if(buffer->user){
        f->buffer_write(buffer);
    }
    if(address != FILE_SIZE_MAX){
        if(buffer->address() != address){
            f->flush(buffer);
            //buffer->address = address;
            f->buffer_read(address, buffer);
        }
    }
    return 0;
}

/*
    if(buffer->address != sector)
    {
        this->flush(buffer);
        buffer->address = sector;
        buffer_read(buffer);
    }

    if(buffer->address != sector)
    {
        this->flush(buffer);
        buffer->address = sector;
        memset(buffer->data, 0, FILE_SECTOR_SIZE);
        if(offset || (offset+size < FILE_SECTOR_SIZE) )
        {//如果写入的不是一个完整的页，要先读取源数据内容
            buffer_read(buffer);
        }
    }
*/

}//end namespace io
}//end namespace cgl

#endif//CGL_PLATFORM_WINDOWS

#endif //IO_SECTOR_H_20160718022650
