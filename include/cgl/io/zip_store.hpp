/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 zip_store.hpp

 2018-08-08 07:19:29

 首先从外部读取文件，如果外部没有，就从zip库里面读取
 可以link多个zip文件，读取数据的时候，从最后连接的zip开始遍历

 iostore 计划支持安卓

*/
#ifndef ZIP_STORE_HPP_20180808071929
#define ZIP_STORE_HPP_20180808071929

#include <cgl/filesystem.hpp>
#include "zip_archive.hpp"

namespace cgl{
namespace io{

class zip_store
{
private:
    typedef io::archive<char> archive_type;
    typedef std::vector<archive_type> archive_list;

    archive_list storelist;
public:
    zip_store() : storelist()
    {
    }

    int link(const string_t& zip)
    {
        //check zip format
        if(filesystem::file_exists(zip.c_str())){
            binfile f;
            byte_t buf[32];
            f.open(zip.c_str(), io_read, sh_share);
            if(!f.is_open()){
                return FILE_OPEN_ERROR;
            }
            f.read(buf, 32);
            if(!filesystem::check_zip(buf, 32)){
                return FILE_INVALID_FORMAT;
            }
            f.close();
        }
        else{
            return FILE_NOT_EXISTS;
        }

        archive_type a;
        storelist.push_back(a);
        storelist.back().open(zip.c_str(), io_read|io_zip, sh_share);
        return 0;
    }

    void dispose()
    {
        for(size_t i=0; i<storelist.size(); ++i)
        {
            storelist[i].close();
        }
        storelist.clear();
    }

    bool empty()const
    {
        return storelist.empty();
    }

    size_t size()const
    {
        return storelist.size();
    }

    archive_type& operator[](size_t index)
    {
        return storelist[index];
    }

    fsize_t filesize(const char* filename)
    {
        io::file<char> f;
        this->openfile(f, filename);
        if(f.is_open()){
            return f.size();
        }
        return 0;
    }

    int readfile(std::vector<byte_t>& buf, const char* filename)
    {
        io::file<char> f;
        this->openfile(f, filename);

        if(f.is_open()){
            buf.resize(f.size());
            f.seek(0, seek_begin);
            return f.read(&buf[0], buf.size());
        }

        return 0;
    }

    int readfile(void* buf, size_t size, const char* filename)
    {
        io::file<char> f;
        this->openfile(f, filename);

        if(f.is_open()){
            size = std::min(size, (size_t)f.size());
            f.seek(0, seek_begin);
            return buf ? f.read(buf, size) : size;
        }

        return 0;
    }

private:
    int openfile(io::file<char>& f, const char* filename)
    {
        if(cgl::filesystem::file_exists<char>(filename)){
            f.open(filename, io_read, sh_share);
        }
        if(!f.is_open()){
            TYPENAME archive_list::reverse_iterator itr = storelist.rbegin();
            for(; itr != storelist.rend(); ++itr){
                if(!f.open_archive(*itr, filename)){
                    break;
                }
            }
        }
        return 0;
    }
};

}//end namespace io
}//end namespace cgl

#endif //ZIP_STORE_HPP_20180808071929
