/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 wavefile.hpp

 2018-08-11 08:27:52

 ADPCM

*/
#ifndef WAVEFILE_HPP_20180811082752
#define WAVEFILE_HPP_20180811082752

#include <cgl/public.h>
#include <cgl/io.hpp>
//#include <mmreg.h>    //windows

#ifndef AL_FORMAT_MONO4
    #define AL_FORMAT_MONO4     0x1300
    #define AL_FORMAT_STEREO4   0x1301
#endif

#ifndef AL_FORMAT_MONO8
    #define AL_FORMAT_MONO8     0x1100
    #define AL_FORMAT_MONO16    0x1101
    #define AL_FORMAT_STEREO8   0x1102
    #define AL_FORMAT_STEREO16  0x1103
#endif

#ifndef AL_FORMAT_MONO24
    #define AL_FORMAT_MONO24    0x10001104
    #define AL_FORMAT_STEREO24  0x10001105
#endif

namespace cgl{
namespace al{

/*
enum PCM_FORMAT{
    PCM_FORMAT_MENO4,
    PCM_FORMAT_MENO8,
    PCM_FORMAT_MENO16,
    PCM_FORMAT_MENO24,

    PCM_FORMAT_STEREO4,
    PCM_FORMAT_STEREO8,
    PCM_FORMAT_STEREO16,
    PCM_FORMAT_STEREO24,
};
*/


#include "adpcm.c"

/*
#define SAMPLE_RATE         22050  // sample rate��ÿ��22050��������
#define QUANTIZATION        0x10   // 16bit������
#define BYTES_EACH_SAMPLE   0x2    // QUANTIZATION / 8, ����ÿ����������short��ռ��2���ֽ�
#define CHANNEL_NUN         0x1    // ������
#define FORMAT_TAG          0x1    // ����PCM
*/

#pragma pack(push, 1)

struct RIFF_HEADER //12 bytes
{
    uint8_t  chunkID[4];    //"RIFF"
    uint32_t chunkSize;     //size not including chunkSize or chunkID
    uint8_t  format[4];     //"WAVE"
};

/*
format
    one of these: 1:linear, 6:a law, 7:u-law
blockAlign
    ÿ����������λ��(���ֽ���), ��ֵΪ:ͨ����*ÿ����������λֵ/8�����������Ҫһ�δ�
    ������ֵ��С���ֽ�����, �Ա㽫��ֵ���ڻ������ĵ���ÿ����ռ�����ֽ�:
    channels * bitsPerSample / 8
*/
struct WAVE_FORMAT //24 bytes
{
    uint8_t  chunkID[4];    //"fmt "
    uint32_t chunkSize;     //�洢���ӿ���ֽ���������ǰ���Subchunk1ID��Subchunk1Size��8���ֽڣ�
    uint16_t format;        //�洢��Ƶ�ļ��ı����ʽ��������ΪPCM����洢ֵΪ1����Ϊ������PCM��ʽ������һ����ѹ����
    uint16_t channels;      //ͨ��������ͨ��(Mono)ֵΪ1��˫ͨ��(Stereo)ֵΪ2���ȵ�
    uint32_t sampleRate;    //�����ʣ���8k��44.1k��
    uint32_t byteRate;      //ÿ��洢��bit������ֵ=SampleRate * NumChannels * BitsPerSample/8
    uint16_t blockAlign;    //������С����ֵ=NumChannels * BitsPerSample/8
    uint16_t bitsPerSample; //ÿ���������bit����һ��Ϊ8,16,32�ȡ�
};

struct WAVE_FACT //12 bytes
{
    uint8_t  fccID[4];  // must be "fact"
    uint32_t id;        // must be 0x4
    uint32_t size;
};

struct WAVE_DATA //8 bytes
{
    uint8_t  chunkID[4];    //"data" should contain the word data
    uint32_t chunkSize;     //Stores the size of the data block
};

#pragma pack(pop)


//���waveͨ������
int wave_channels(int format)
{
    switch(format){
    case AL_FORMAT_MONO4:
    case AL_FORMAT_MONO8:
    case AL_FORMAT_MONO16:
    case AL_FORMAT_MONO24:
        return 1;
    case AL_FORMAT_STEREO4:
    case AL_FORMAT_STEREO8:
    case AL_FORMAT_STEREO16:
    case AL_FORMAT_STEREO24:
        return 2;
    default:
        break;
    }
    return 0;
}

int wave_sample_bits(int format)
{
    switch(format){
    case AL_FORMAT_MONO4:
        return 4;
    case AL_FORMAT_MONO8:
        return 8;
    case AL_FORMAT_MONO16:
        return 16;
    case AL_FORMAT_STEREO4:
        return 4;
    case AL_FORMAT_STEREO8:
        return 8;
    case AL_FORMAT_STEREO16:
        return 16;
    case AL_FORMAT_MONO24:
    case AL_FORMAT_STEREO24:
        return 24;
    default:
        break;
    }
    return 0;
}

int wave_format(int channels, int bit_per_sample)
{
    if(channels == 1){
        switch(bit_per_sample){
        case 4://ADPCM
            return AL_FORMAT_MONO4;
        case 8:
            return AL_FORMAT_MONO8;
        case 16:
            return AL_FORMAT_MONO16;
        case 24:
            return AL_FORMAT_MONO24;
        default:
            break;
        }
    }
    else if(channels == 2){
        switch(bit_per_sample){
        case 4:
            return AL_FORMAT_STEREO4;
        case 8:
            return AL_FORMAT_STEREO8;
        case 16:
            return AL_FORMAT_STEREO16;
        case 24:
            return AL_FORMAT_STEREO24;
        default:
            break;
        }
    }
    return 0;
}

//pcm 8λת16λ
inline int16_t pcm_8bit_to_16bit(int8_t value)
{
    return static_cast<int16_t>(value) << 8;
}

void pcm_8bit_to_16bit(uint16_t* dest, const byte_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i] = (static_cast<uint16_t>(source[i]) - 128) << 8;
    }
}

//pcm 16λת8λ
inline int8_t pcm_16bit_to_8bit(int16_t value)
{
    return static_cast<int8_t>(value >> 8);
}

void pcm_16bit_to_8bit(byte_t* dest, const uint16_t* source, size_t size)
{
    for(size_t i=0; i<size; ++i){
        dest[i] = static_cast<byte_t>((source[i] >> 8) + 128);
    }
}

//pcm 24λת16λ
void pcm_24bit_to_16bit(byte_t* dest, const byte_t* source, size_t channels, size_t samples)
{
    size_t size = channels * samples;
    for(size_t i=0; i < size; ++i){
        ++source;
        *dest++ = *source++;
        *dest++ = *source++;
    }
}

//˫����ת��������ת����ʽ�ǣ�����������ӳ�2
inline int stereo_to_mono(int left, int right)
{
    return (left + right) / 2;
}

template<typename T>
void stereo_to_mono(T* dest, const T* source, size_t size)
{
    for (size_t i = 0; i < size; ++i){
        dest[i] = static_cast<T>(stereo_to_mono(*source++, *source++));
    }
}

//������ת˫������ת����ʽ�ǣ���������ֵ��������������
template<typename T>
void mono_to_stereo(T* dest, const T* source, size_t size)
{
    for (size_t i = 0; i < size; ++i){
        *dest++ = *source;
        *dest++ = *source++;
    }
}

//
// wavfile
//

class wavfile
{
private:
    size_t m_size;
    int m_format;
    int m_frequency;
    memblock<byte_t> m_data;
    const byte_t* m_memlink;    //�򿪵����ڴ棬ָ���ڴ�����

public:
    wavfile() : m_size(), m_format(), m_frequency(), m_memlink(null)
    {

    }

    int open(const char* filename)
    {
        this->close();

        io::binfile f;

        f.open(filename, io_read);
        if (!f.is_open()){
            return -1;
        }

        return open(&f);
    }

    int open(const wchar_t* filename)
    {
        this->close();

        io::binfile f;

        f.open(filename, io_read);
        if (!f.is_open()){
            return -1;
        }

        return open(&f);
    }

    int open(io::unifile* f)
    {
        this->close();

        //����ļ���ʽ
        byte_t buf[1024];
        f->read(buf, 1024);
        size_t offset = check_wave_format(buf, 1024, &m_size, &m_format, &m_frequency);
        if(!offset){
            return -2;
        }

        //��ȡ��Ƶ����
        f->seek(offset, SEEK_SET);
        memblock<byte_t> pcm;
        pcm.resize(m_size);
        if(!f->read(pcm.data(), m_size)){
            return -3;
        }

        //����Ƿ���Ҫת����ʽ
        adpcm_state_t state = {0};
        switch(m_format){
        case AL_FORMAT_MONO4:
            m_data.resize(m_size * 4);
            //4λ��Ƶ��ѹ��16λ��Ƶ
            adpcm_decoder((const char*)pcm.data(), (int16_t*)m_data.data(), m_size, &state);
            m_size *= 4;
            m_format = AL_FORMAT_MONO16;
            break;
        case AL_FORMAT_MONO24:
            m_data.resize(m_size * 2 / 3);
            pcm_24bit_to_16bit(m_data.data(), pcm.data(), 1, m_size / 2 / 3);
            m_size = m_size * 2 / 3;
            m_format = AL_FORMAT_STEREO16;
            break;
        case AL_FORMAT_STEREO4:
            break;
        case AL_FORMAT_STEREO24:
            m_data.resize(m_size * 2 / 3);
            pcm_24bit_to_16bit(m_data.data(), pcm.data(), 2, m_size / 2 / 3);
            m_size = m_size * 2 / 3;
            m_format = AL_FORMAT_STEREO16;
            break;
        default:
            m_data.swap(pcm);
            break;
        }
        return 0;
    }

    int open_memory(const byte_t* data, size_t size)
    {
        this->close();

        size_t offset = check_wave_format(data, size, &m_size, &m_format, &m_frequency);
        if(!offset){
            return -2;
        }

        if(m_format == AL_FORMAT_MONO4){
            memblock<byte_t> pcm;
            pcm.resize(m_size * 4);

            adpcm_state_t state = {0};
            //4λ��Ƶ��ѹ��16λ��Ƶ
            adpcm_decoder((const char*)(data + offset), (int16_t*)pcm.data(), m_size, &state);
            //openal buffer.c
//            for(int i=0; i<m_size; ++i)
//            DecodeIMA4Block(((int16_t*)pcm.data())+i, data+offset+i, 2);
            //memcpy(m_data.data(), pcm.data(), m_size);
            //m_format = WAVE_FORMAT_MONO16;
            m_size *= 2;
            m_data.resize(m_size);
            pcm_16bit_to_8bit(m_data.data(), (uint16_t*)pcm.data(), m_size);
            m_format = AL_FORMAT_MONO8;
        }
        else{
            m_memlink = data + offset;
        }

        return 0;
    }

    void close()
    {
        m_size = m_format = m_frequency = 0;
        m_data.dispose();
        m_memlink = 0;
    }

    int size()const      { return m_size;      }
    int format()const    { return m_format;    }
    int frequency()const { return m_frequency; }

    int channels()const
    {
        return wave_channels(m_format);
    }

    int sample_bits()const
    {
        return wave_sample_bits(m_format);
    }

    //�������ݲ�������
    int samples()const
    {
        return size() * 8 / sample_bits() / channels();
    }

    const byte_t* data()const
    {
        return m_memlink ? m_memlink : m_data.data();
    }

    static size_t check_wave_format(const byte_t* data, size_t data_size, size_t* size, int* format, int* frequency);
};

bool check_fourcc(const byte_t* data, byte_t a, byte_t b, byte_t c, byte_t d)
{
    return data[0] == a && data[1] == b && data[2] == c && data[3] == d;
}

//���wav��ʽ������dataƫ��λ��
size_t wavfile::check_wave_format(const byte_t* data, size_t data_size, size_t* size, int* format, int* frequency)
{
    size_t offset = 0;
    RIFF_HEADER *riff = (RIFF_HEADER*)(data + offset);
    //check for RIFF and WAVE tag
    if(!check_fourcc(riff->chunkID, 'R', 'I', 'F', 'F')
        //&& !check_fourcc(riff->format, 'W', 'A', 'V', 'E')
        )
    {
        CGL_LOG_ERROR("wavfile: invalid RIFF or WAVE header");
        return 0;
    }

    offset += sizeof(RIFF_HEADER);
    WAVE_FORMAT *wave = (WAVE_FORMAT*)(data + offset);
    //check for fmt tag
    if( !check_fourcc(wave->chunkID, 'f', 'm', 't', ' ')){
        CGL_LOG_ERROR("wavfile: invalid FORMAT header");
        return 0;
    }
    offset += 8 + wave->chunkSize;
    WAVE_FACT* fact = (WAVE_FACT*)(data + offset);

    //check for fact tag and skip it
    if(check_fourcc(fact->fccID, 'f', 'a', 'c', 't')){
        offset += sizeof(WAVE_FACT);
    }

    WAVE_DATA* wave_data = (WAVE_DATA*)(data + offset);

    //find 'data' flag

    //check for data tag
    while(!check_fourcc(wave_data->chunkID, 'd', 'a', 't', 'a')){
        CGL_LOG_DEBUG("wavfile: tag = %c%c%c%c", wave_data->chunkID[0], wave_data->chunkID[1], wave_data->chunkID[2], wave_data->chunkID[3]);
        offset += 8 + wave_data->chunkSize;
        if(offset >= data_size){
            CGL_LOG_ERROR("wavfile: invalid DATA header");
            return 0;
        }
        wave_data = (WAVE_DATA*)(data + offset);
    }

    /*
    if(check_fourcc(wave_data->chunkID, 'L', 'I', 'S', 'T')){
        offset += 8 + wave_data->chunkSize;
        wave_data = (WAVE_DATA*)(data + offset);
    }

    if(check_fourcc(wave_data->chunkID, 'b', 'e', 'x', 't')){
        offset += 8 + wave_data->chunkSize;
        wave_data = (WAVE_DATA*)(data + offset);
    }
    */

    if(size)*size = wave_data->chunkSize;
    if(frequency)*frequency = wave->sampleRate;
    if(format)*format = wave_format(wave->channels, wave->bitsPerSample);

    return offset + sizeof(WAVE_DATA);
}


}//end namespace al
}//end namespace cgl

#endif //WAVEFILE_HPP_20180811082752
