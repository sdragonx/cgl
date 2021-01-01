/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 filelink.hpp

 2018-09-28 13:29:42

*/
#ifndef FILELINK_HPP_20180928132942
#define FILELINK_HPP_20180928132942

#include <cgl/io/unifile.hpp>

namespace cgl{
namespace io{

//
// 映射文件或者内存的一部分进行读写，不能改变其大小
//
class filelink : public unifile
{
private:
    fsize_t m_start;
    fsize_t m_size;
    fsize_t m_seekpos;

public:
    filelink() : unifile(),
        m_start(), m_size(), m_seekpos()
    {

    }

    int link(unifile* file, fsize_t start, fsize_t size)
    {
        m_file = file;
        m_start = start;
        m_size = size;
        m_seekpos = 0;
        return 0;
    }

    int link(void* data, fsize_t start, fsize_t size)
    {
        m_mode = io_memory;
        m_data = data;
        m_start = start;
        m_size = size;
        m_seekpos = 0;
        return 0;
    }

    int link(const void* data, fsize_t start, fsize_t size)
    {
        m_mode = io_memory;
        m_cdata = data;
        m_start = start;
        m_size = size;
        m_seekpos = 0;
        return 0;
    }

    void close()
    {
        m_file = null;
        m_start = m_size = m_seekpos = 0;
    }

    bool is_open()
    {
        if(m_mode & io_memory){
            return m_cdata;
        }
        else{
            return m_file;
        }
    }

    fsize_t seek(fsize_t pos, int whence)
    {
        internal::set_seekpos(m_seekpos, m_size, pos, whence);
        return m_seekpos;
    }

    fsize_t tell()
    {
        return m_seekpos;
    }

    fsize_t size()
    {
        return m_size;
    }

    void flush()
    {
        if(m_mode & ~io_memory){
            m_file->flush();
        }
    }

    fsize_t read(void* data, fsize_t size)
    {
        size = internal::check_read(m_seekpos, m_size, size);
        if(!size){
            return size;
        }
        if(m_mode & io_memory){
            //指针加int64会报可疑指针操作警告
            memcpy(data, static_cast<const byte_t*>(m_cdata) + size_t(m_start + m_seekpos), size);
            this->seek(size, seek_current);
        }
        else{
            m_file->seek(m_start + m_seekpos, seek_begin);
            size = m_file->read(data, size);
            m_seekpos += size;
        }
        return size;
    }

    fsize_t write(const void* data, fsize_t size)
    {
        size = internal::check_read(m_seekpos, m_size, size);
        if(!size){
            return size;
        }
        if(m_mode & io_memory){
            memcpy(static_cast<byte_t*>(m_data) + size_t(m_start + m_seekpos), data, size);
            this->seek(size, seek_current);
        }
        else{
            m_file->seek(m_start + m_seekpos, SEEK_SET);
            size = m_file->write(data, size);
            m_seekpos += size;
        }
        return size;
    }
};

}//end namespace io
}//end namespace cgl

#endif //FILELINK_HPP_20180928132942
