/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 pkcrypt.hpp

 2020-05-28 15:44:55

 zip��ͳ���ܽ���

*/
#ifndef ZIP_PKCRYPT_HPP_20200528154455
#define ZIP_PKCRYPT_HPP_20200528154455

#include <cgl/crypto/pkcrypt.hpp>
#include "typedef.hpp"

namespace cgl{
namespace io{

const int PKCRYPT_HEAD_SIZE = 12;


//
// pkdecrypt
//

//��������У����
void calc_verify(uint8_t& a, uint8_t& b, ziplist* z)
{
    const int MZ_ZIP_FLAG_DATA_DESCRIPTOR = 8;// = 8;
    if(z->file32.flag & MZ_ZIP_FLAG_DATA_DESCRIPTOR){
        uint32_t date = dostime_to_unixtime(z->file32.time);
        a = (uint8_t)((date >> 16) & 0xff);
        b = (uint8_t)((date >> 8) & 0xff);
    }
    else{
        a = (uint8_t)((z->file32.crc32 >> 16) & 0xff);
        b = (uint8_t)((z->file32.crc32 >> 24) & 0xff);
    }
}

//�������ͷ��������벻ƥ�䣬����false
void decript_head(byte_t *buffer, pkcrypt& key)
{
    //char buf[1024];
    //CGL_LOG("in buffer: %s", tohex(buf, buffer, ZIP_PKCRYPT_HEAD));

    #if 0
    int i;
    for(i = 0; i<10; ++i){
        buffer[i] = key.decode(buffer[i]);
    }

    int verify1 = key.decode(buffer[i++]);
    int verify2 = key.decode(buffer[i++]);
    CGL_LOG("verify: %X %X", verify1, verify2);

    #else

    for(int i=0; i<PKCRYPT_HEAD_SIZE; ++i){
        buffer[i] = key.decode(buffer[i]);
    }

    uint32_t crc;
    char c;

    crc  = uint32_t(buffer[11])<<8;
    crc |= buffer[10];
    //CGL_LOG("crc calc: %X", crc);

    #endif

    //CGL_LOG("final buffer: %s", tohex(buf, buffer, PKCRYPT_HEAD_SIZE));
}

//�ж������Ƿ�ƥ��
bool check_password(byte_t buffer[PKCRYPT_HEAD_SIZE], ziplist* z)
{
    uint8_t a = 0;
    uint8_t b = 0;

    calc_verify(a, b, z);
    //CGL_LOG("verify calc: %X %X", int(a), int(b));

    return buffer[11] == b;
}

//�ļ�f��ָ��λ��������λ
int pkunzip(std::vector<byte_t>& data, io::unifile* f, ziplist& z, const char* password)
{
    char text[1024] = {0};
    byte_t buffer[CGL_FILE_BUFFER_SIZE];
    bool correct;   //�����Ƿ���ȷ
    size_t compressed_size;
    size_t size;
    std::vector<byte_t> zbuffer;    //��������

    pkcrypt key;
    key.init(password);

    //CGL_LOG("crc32: %s", tohex(text, (char*)&z.file.crc32, 4));
    //CGL_LOG("KEY��ʼ����%s", tohex(text, (char*)key.keys, 12));

    //��ȡ����ͷ
    byte_t head[PKCRYPT_HEAD_SIZE];
    f->read(head, PKCRYPT_HEAD_SIZE);

    decript_head(head, key);

    correct = check_password(head, &z);
    //CGL_LOG_DEBUG("����У�飺%s", correct ? "��ȷ" : "����");

    if(!correct){
        CGL_LOG("zip password error.");
        return ZIP_ERROR_PASSWORD;
    }

    //����
    compressed_size = z.file32.compressed_size;
    if(z.file32.compress_mode == ZIP_STORED){
        data.reserve(z.file32.source_size);

        compressed_size = z.file32.compressed_size - PKCRYPT_HEAD_SIZE;
        while(compressed_size){
            size = std::min(compressed_size, CGL_FILE_BUFFER_SIZE);
            size = f->read(buffer, size);
            if(size == 0){
                return ZIP_ERROR_DATA;
            }
            for(int i=0; i<size; ++i){
                data.push_back(key.decode(buffer[i]));
            }
            compressed_size -= size;
        }
    }
    else{
        zbuffer.reserve(z.file32.compressed_size + 2);
        zbuffer.push_back(0x78);    //zlibͷ
        zbuffer.push_back(0x01);

        compressed_size = z.file32.compressed_size;
        while(compressed_size){
            size = std::min(compressed_size, CGL_FILE_BUFFER_SIZE);
            size = f->read(buffer, size);
            if(size == 0){
                return ZIP_ERROR_DATA;
            }
            for(int i=0; i<size; ++i){
                zbuffer.push_back(key.decode(buffer[i]));
            }

            //z_stream

            compressed_size -= size;
        }

        //��ѹ
        size = z.file32.source_size;
        data.resize(size);
        zlib_uncompress(&data[0], &size, &zbuffer[0], zbuffer.size());
    }

    //CGL_LOG("��ѹ���ݴ�С��%u", data.size());
    //CGL_LOG("%s", (char*)&data[0]);

    return 0;
}

}//end namespace io
}//end namespace cgl

#endif //ZIP_PKCRYPT_HPP_20200528154455