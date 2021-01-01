/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 zip_archive.hpp

 2016-07-17 21:07:39

*/
#ifndef ZIP_ARCHIVE_HPP_20160717210739
#define ZIP_ARCHIVE_HPP_20160717210739

#include <map>
#include <cgl/io/file.hpp>
#include <cgl/string.hpp>
#include <cgl/zlib.hpp>
#include "zip/typedef.hpp"

namespace cgl{
namespace io{

/*
zlibͷ��ѹ������
78 01    0 1
78 5E    2-5
78 9C    6
78 DA    7-9
*/

//#include <vcl.h>
//extern void print(String);
//zip ��ʽ
// PK34 �ļ�����
// PK12 Ŀ¼�б�(�����Ŀ¼)
// PK56 �ļ��б�


#pragma pack(push, 1)
struct zip_list
{
    const static uint32_t TYPE_FILE = 0x04034b50;       //zip identifier
    const static uint32_t TYPE_DATA = 0x08074b50;
    const static uint32_t TYPE_LIST = 0x02014b50;       //zip directory
    const static uint32_t TYPE_END  = 0x06054b50;
    const static uint32_t TYPE_EXTRA = 0x08064b50;      //Archive extra data record

    uint32_t type;
    union{
        //�ļ�ͷʵ�壨0x504B0304������ѡģ�飬��¼��ѹ���ĵ�ÿһ���ļ����ļ�ͷ��Ϣ��
        //�ļ�������Ϣ���Լ�����������Ϣ����Ҫ���ֱ��������������ݡ�
        //�����ѹ�����ݿ��Ӧ��ѹ�����а��������ļ������м���0x504B0304ģ�顣
        struct{
            uint16_t unzip_version;     //��ѹ������pkware��Ͱ汾
            uint16_t flag;              //ͨ�ñ��ر�־
            uint16_t compress_mode;     //ѹ����ʽ 0x00 û��ѹ�� 0x08 ѹ�� 0x63 ����
            uint32_t time;              //�ļ�����޸�ʱ�� WORD date/ WORD time
            uint32_t crc32;
            uint32_t compressed_size;   //ѹ�����ݴ�С
            uint32_t source_size;       //Դ�ļ���С
            uint16_t name_length;       //�ļ����Ƴ���
            uint16_t extra_size;        //��չ�ֶγ���
            //char    name[];
            //char    extra[];
        }file;

        //ѹ��������������0x504B0708������ѡģ�飬�����ڲ��ܶ������ ZIP �ļ����м�������Ŵ�����ʱʹ�á�
        //ֻ������Ӧ��local file header��ͨ�ñ���ֶεĵڣ�bit��Ϊ��ʱ�Ż���֣�����ÿһ���ļ�ͷģ��ĺ��档
        struct{
            uint32_t crc32;
            uint32_t compressed_size;
            uint32_t source_size;
        }data;

        //ѹ������Ŀ¼��0x504B0102������ѡģ�飬��¼��ѹ���ļ��ľ������ݣ�
        //һ�������ļ�ͷģ���Ӧ��һ����Ϊ�ļ����ݵ�ƫ��ģ�飬ѹ������Ѱַ��ʽ�����������¼�ġ�
        //��ģ���һЩ��ʶ��Ϣ��Ӧ�ļ�ͷģ�����Ϣ���������������͡�
        struct{
            uint16_t zip_version;       //ѹ���汾
            uint16_t unzip_version;     //��ѹ������pkware��Ͱ汾
            uint16_t flag;              //ͨ�ñ��ر�־
            uint16_t compress_type;     //ѹ����ʽ
            uint32_t time;
            uint32_t crc32;
            uint32_t compressed_size;   //ѹ�����ݴ�С  64
            uint32_t source_size;       //Դ�ļ���С    64
            uint16_t name_length;       //�ļ����Ƴ���
            uint16_t extra_size;        //��չ�ֶγ���
            uint16_t comment_size;      //�ļ�ע�ͳ���
            uint16_t disk;              //�ļ���ʼ�ķ־���
            uint16_t inner_attrib;      //�ļ��ڲ�����
            uint32_t attrib;            //�ļ��ⲿ����
            uint32_t local_offset;      //��Ӧ�ļ�ʵ�����ļ��е�ƫ��
            //char    name[];
            //char    extra[];
            //char    comment[];
        }list;

        //Ŀ¼������ʶ����0x504B0506������ѡģ�飬���ڱ��ѹ����Ŀ¼���ݵĽ�����
        //ÿһ��ѹ���ļ�ֻ��һ��Ŀ¼������ʶ������¼ѹ�����ļ����Ϣ�Լ��ļ���ȡ�ȡ�
        struct{
            uint16_t disk;              //��ǰ�־���
            uint16_t disk_start;        //Central Directory�Ŀ�ʼ�־���
            uint16_t disk_count;        //��ǰ�־�Central Directory�ļ�¼����
            uint16_t dir_count;         //Central Directory���ܼ�¼����
            uint32_t dir_size;          //Central Directory�Ĵ�С (bytes)
            uint32_t dir_offset;        //Central Directory�Ŀ�ʼλ��ƫ��
            uint16_t comment_size;      //Zip�ļ�ע�ͳ���(n)
            //char    comment[0];
        }endlist;
    };
    cstring<char> name;
    cstring<char> extra;
    cstring<char> comment;
    uint32_t offset;                    //�ļ��е�ƫ����

    //�������ݿ�Ĵ�С
    size_t size()
    {
        switch(type){
        case ZIP_TYPE_LIST:
            return 4 + sizeof(list) + list.name_length + list.extra_size + list.comment_size;
        case ZIP_TYPE_FILE:
            return 4 + sizeof(file) + file.name_length + file.extra_size + file.compressed_size;
        case ZIP_TYPE_END:
            return 4 + sizeof(endlist) + endlist.comment_size;
        case ZIP_TYPE_DATA:
            return 4 + sizeof(data);
        default:
            CGL_LOG("unknown zip record type.");
            break;
        }
        return 0;
    }
};

#pragma pack(pop)

template<typename T>
class fzlib;

template<typename T>
class zip_archive : public archive_base<T>
{
private:
    std::map<cstring<char>, zip_list> m_ls;
    typedef std::map<cstring<char>, zip_list>::iterator iterator;
    iterator m_itr;
    friend class fzlib<T>;

public:
    zip_archive():archive_base<T>(), m_ls(), m_itr()
    {

    }

    ~zip_archive()
    {
        this->close();
    }

    int open(const char* file_name, int, int)
    {
        binfile f;
        f.open(file_name, io_read, sh_share);
        if(f.is_open()){
            archive_base<T>::m_name = file_name;
            archive_base<T>::m_mode = io_zip|io_read;
            on_load(&f);
        }
        else{
            archive_base<T>::m_name.clear();
            return -1;
        }
        return 0;
    }

    int open(const wchar_t* file_name, int, int)
    {
        binfile f;
        f.open(file_name, io_read, sh_share);
        if(f.is_open()){
            archive_base<T>::m_name = file_name;
            archive_base<T>::m_mode = io_zip|io_read;
            on_load(&f);
        }
        else{
            archive_base<T>::m_name.clear();
            return -1;
        }
        return 0;
    }

    int open_memory(const void* data, size_t size)
    {
        io::filelink f;
        f.link(data, 0, size);
        on_load(&f);

        archive_base<T>::m_name.clear();
        archive_base<T>::m_mode = io_zip|io_read|io_memory;
        archive_base<T>::m_link = data;
        archive_base<T>::m_size = size;

        return 0;
    }

    void close()
    {
        m_ls.clear();
        archive_base<T>::m_name.clear();
        archive_base<T>::m_mode = 0;
    }

    bool is_open()const
    {
        return !archive_base<T>::m_name.empty();
    }

    bool ls(ls_data<T>& data)
    {
        if(!data.address){
            m_itr = m_ls.begin();
        }
        if(m_itr != m_ls.end()){
            data.address = m_itr->second.offset;
            data.name = m_itr->second.name;
            ++m_itr;
            return true;
        }
        else{
            return false;
        }
    }

    zip_list* find(const char* filename)
    {
        iterator itr = m_ls.find(filename);
        if(itr != m_ls.end()){
            return &itr->second;
        }
        return null;
    }

private:
    int on_load(unifile* f)
    {
        //��ȡ�ļ��б�
        for(fsize_t seek = 0; !f->eof(); ){
            zip_list ls;
            f->seek(seek, seek_begin);
            f->read(&ls.type, 4);
            if(ls.type == zip_list::TYPE_FILE){
                f->read(&ls.file, sizeof(ls.file));
                ls.name.resize(ls.file.name_length);
                f->read(&ls.name.front(), ls.name.length());
                ls.offset = f->tell() + ls.file.extra_size;
                m_ls[ls.name] = ls;
                seek = ls.offset + ls.file.compressed_size;
            }
            else{
                //f->read(&ls.file, sizeof(ls.file));
                //ls.name.resize(ls.file.name_length);
                break;
            }
        }
        return 0;
    }
};



template<typename T>
class fzlib : public unifile
{
public:
    file<char> m_file;
    zstream m_stream;
    byte_t m_buf[CGL_FILE_BUFFER_SIZE];
    fsize_t m_offset;
    fsize_t m_compressed_size;
    fsize_t m_size;
    fsize_t m_seekpos;

public:
    fzlib() : unifile(), m_file()
    {
        memset(&m_stream, 0, sizeof(m_stream));
        m_offset = m_compressed_size = m_size = m_seekpos = 0;
    }

    ~fzlib()
    {
        this->close();
    }

    int open(const char* fn, int, int)
    {
        return -1;
    }

    int open(const wchar_t* fn, int, int)
    {
        return -1;
    }

    int open(archive<T>& _archive, const char* file_name)
    {
        //using namespace zlib;

        const zip_archive<T>* pzip = dynamic_cast<const zip_archive<T>*>(_archive.handle());
        if(!pzip){
            return -1;
        }

        TYPENAME std::map<cstring<T>, zip_list>::const_iterator itr = pzip->m_ls.find(file_name);
        if(itr != pzip->m_ls.end()){
            const zip_list& ls = itr->second;

            if(pzip->mode() & io_memory){//���ڴ�����
                m_file.open_memory(pzip->m_link, pzip->m_size);
            }
            else{//�򿪴����ļ�
                m_file.open(pzip->m_name.c_str(), io_read, sh_share);
            }

            m_offset = ls.offset;
            m_compressed_size = ls.file.compressed_size;
            m_size = ls.file.source_size;
            m_seekpos = 0;
          }
        else{
            return -1;
        }

        memset(&m_stream, 0, sizeof(m_stream));
        int ret = inflateInit(&m_stream);//, ZLIB_VERSION, sizeof(mz_stream));
        if (ret != MZ_OK) return ret;
        return 0;
    }

    void close()
    {
        m_file.close();
        inflateEnd(&m_stream);
    }

    bool is_open()
    {
        return m_file.is_open();
    }

    size_t compressed_size()const
    {
        return m_compressed_size ? m_compressed_size : m_size;
    }

    fsize_t size()
    {
        return m_size;
    }

    fsize_t resize(fsize_t size)
    {
        return -1;
    }

    bool eof()
    {
        return !m_stream.avail_in && m_seekpos >= this->size();
    }

    fsize_t seek(fsize_t pos, int whence)
    {
        if(!pos && !whence){
            m_seekpos = 0;
            memset(&m_stream, 0, sizeof(m_stream));
            int ret = inflateInit(&m_stream);
            if (ret != MZ_OK){
                return -1;
            }
            return 0;
        }
        return -1;
    }

    fsize_t tell()
    {
        return m_seekpos;
    }

    fsize_t read(void* data, fsize_t size)
    {
        int result = 0;
        fpos_t in_read;
        if(m_size != m_compressed_size){//ѹ������
            m_stream.avail_out = size;
            m_stream.next_out = reinterpret_cast<byte_t*>(data);
            do {
                if(!m_stream.avail_in){//��ȡѹ������
                    m_file.seek(m_offset + m_seekpos, seek_begin);
                    if(!m_seekpos){
                        m_buf[0] = 0x78;//zlib��ѹǰ׺
                        m_buf[1] = 0x9C;
                        in_read = m_file.read(m_buf+2, std::min<size_t>(CGL_FILE_BUFFER_SIZE-2, this->compressed_size()-m_seekpos));
                    }
                    else{
                        in_read = m_file.read(m_buf, std::min<size_t>(CGL_FILE_BUFFER_SIZE, this->compressed_size()-m_seekpos));
                    }
                    if(in_read == 0){//eof
                        return size - m_stream.avail_out;
                    }
                    m_stream.avail_in = in_read;
                    if(!m_seekpos){
                        m_stream.avail_in += 2;
                    }
                    m_stream.next_in = m_buf;
                    m_seekpos += in_read;
                }
                //��ѹ
                result = inflate(&m_stream, MZ_NO_FLUSH);
                if(result != MZ_OK){
                    break;
                }
            }while(m_stream.avail_out);

            if(result != MZ_OK){
                return result;
            }
            else{
                return size - m_stream.avail_out;
            }
        }
        else{//δѹ������
            m_file.seek(m_offset + m_seekpos, seek_begin);
            in_read = m_file.read(data, std::min<size_t>(size, this->size()-m_seekpos));
            m_seekpos += in_read;
            return in_read;
        }
    }

    fsize_t write(const void* data, fsize_t size)
    {
        return -1;
    }

};

//typedef zip_file

}//end namespace io
}//end namespace cgl

namespace cgl{

int zip_debug(io::unifile* f)
{
    io::zip_list ls;
    size_t size;
    char data[1024];
    char buf[1024];

    for(fsize_t seek = 0; !f->eof(); ){
        f->seek(seek, seek_begin);
        f->read(&ls.type, 4);
        seek += 4;

        switch(ls.type){
        case io::zip_list::TYPE_FILE:
            size = sizeof(ls.file);
            f->read(&ls.file, size);
            ls.name.resize(ls.file.name_length);
            f->read(&ls.name.front(), ls.name.length());
            ls.offset = f->tell() + ls.file.extra_size;
            CGL_LOG("file: %-16s size: %4i compressed: %4i extra: %4i  %c",
                ls.name.c_str(),
                ls.file.source_size,
                ls.file.compressed_size,
                ls.file.extra_size,
                ls.file.flag & 0x08 ? '*' : ' ');

            f->read(data, ls.file.extra_size);
            CGL_LOG("    extra: %s", tohex(buf, data, ls.file.extra_size));
            f->read(data, 12);
            CGL_LOG("     data: %s", tohex(buf, data, 12));
            seek += size + ls.file.name_length + ls.file.extra_size + ls.file.compressed_size;
            break;
        case io::zip_list::TYPE_DATA:
            seek += sizeof(ls.data);
            CGL_LOG("data");
            break;
        case io::zip_list::TYPE_LIST:
            size = sizeof(ls.list);
            f->read(&ls.list, size);
            ls.name.resize(ls.list.name_length);
            f->read(&ls.name.front(), ls.name.length());
            ls.offset = f->tell() + ls.list.extra_size + ls.list.comment_size;
            CGL_LOG("list: %-16s extra: %i comment: %i", ls.name.c_str(), ls.list.extra_size, ls.list.comment_size);
            seek += size + ls.list.name_length + ls.list.extra_size + ls.list.comment_size;;
            break;
        case io::zip_list::TYPE_END:
            size = sizeof(ls.endlist);
            f->read(&ls.endlist, size);
            ls.comment.resize(ls.endlist.comment_size);
            //f->read(&ls.comment.front(), ls.comment.length());
            size = f->read(data, ls.endlist.comment_size);
            data[size] = '\0';
            //ls.endlist.
            CGL_LOG("zipEnd: comment: %u text: %s", ls.endlist.comment_size, data);//tohex(buf, data, size));
            break;
        default:
            CGL_LOG("unknown list type : %s", tohex(buf, &ls, sizeof(ls)) );
            return 0;
        }
    }
    return 0;
}

}//end namespace cgl

#endif //ZIP_ARCHIVE_HPP_20160717210739