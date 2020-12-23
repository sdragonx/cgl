/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 ogg.hpp

 2020-02-21 17:55:01

 class libogg
 {
 };

 ogg_stream ogg;
 ogg.open(io::unifile*)

 ogg.read(void* data, size);
 ogg.size();


*/
#ifndef OGG_HPP_20200221175501
#define OGG_HPP_20200221175501

#include <cgl/system/shared_obj.hpp>

#define OV_EXCLUDE_STATIC_CALLBACKS

#include <ogg/ogg.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

template<typename T>
class ogg_instance
{
public:
    static cgl::shared_obj libogg;
    static cgl::shared_obj libvorbis;
    static cgl::shared_obj libvorbisfile;
};

template<typename T>
cgl::shared_obj ogg_instance<T>::libogg("libogg.dll");

template<typename T>
cgl::shared_obj ogg_instance<T>::libvorbis("libvorbis.dll");

template<typename T>
cgl::shared_obj ogg_instance<T>::libvorbisfile("libvorbisfile.dll");

void* ogg_load(const char* fn)
{
    using namespace cgl;
    //single<shared_obj> libogg("libogg.dll");
    //single<shared_obj> libvorbis("libvorbis.dll");
    //shared_obj libvorbisfile("libvorbisfile.dll");
    return (void*) ogg_instance<int>::libvorbisfile.load(fn);
}

int ov_clear(OggVorbis_File *vf)
{
    typedef int (*LPOV_CLEAR)(OggVorbis_File *);
    LPOV_CLEAR lpfunc = (LPOV_CLEAR)ogg_load("ov_clear");
    if(lpfunc){
        return lpfunc(vf);
    }
    return OV_FALSE;
}

int ov_open_callbacks(void *datasource, OggVorbis_File *vf,
                const char *initial, long ibytes, ov_callbacks callbacks)
{
    typedef int (*LPOV_OPEN_CALLBACKS)(void *, OggVorbis_File *, const char *, long , ov_callbacks );
    LPOV_OPEN_CALLBACKS lpfunc = (LPOV_OPEN_CALLBACKS)ogg_load("ov_open_callbacks");
    if(lpfunc){
        return lpfunc(datasource, vf, initial, ibytes, callbacks);
    }
    return OV_FALSE;
}

vorbis_info *ov_info(OggVorbis_File *vf,int link)
{
    typedef vorbis_info * (*LPOV_INFO)(OggVorbis_File *vf,int link);
    LPOV_INFO lpfunc = (LPOV_INFO)ogg_load("ov_info");
    if(lpfunc){
        return lpfunc(vf, link);
    }
    return NULL;
}

long ov_read(OggVorbis_File *vf,char *buffer,int length,
                    int bigendianp,int word,int sgned,int *bitstream)
{
    typedef long (*LPOV_READ)(OggVorbis_File *,char *,int , int , int , int , int *);
    LPOV_READ lpfunc = (LPOV_READ)ogg_load("ov_read");
    if(lpfunc){
        return lpfunc(vf, buffer, length, bigendianp, word, sgned, bitstream);
    }
    return 0;
}

ogg_int64_t ov_pcm_total(OggVorbis_File *vf,int i)
{
    typedef ogg_int64_t (*LPOV_PCM_TOTAL)(OggVorbis_File *,int );
    LPOV_PCM_TOTAL lpfunc = (LPOV_PCM_TOTAL)ogg_load("ov_pcm_total");
    if(lpfunc){
        return lpfunc(vf, i);
    }
    return 0;
}

namespace cgl{
namespace al{

const char* ov_error_string(int code)
{
    switch(code)
    {
    case OV_EREAD:
        return "Read from media.";
    case OV_ENOTVORBIS:
        return "Not Vorbis data.";
    case OV_EVERSION:
        return "Vorbis version mismatch.";
    case OV_EBADHEADER:
        return "Invalid Vorbis header.";
    case OV_EFAULT:
        return "Internal logic fault (bug or heap/stack) corruption.";
    default:
        return "Unknown Ogg error.";
    }
}

void ov_check(int code)
{
    CGL_LOG(ov_error_string(code));
}

//ogg decode
size_t DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, size_t ulBufferSize, size_t ulChannels)
{
    int current_section;
    long lDecodeSize;
    size_t ulSamples;
    short *pSamples;

    size_t ulBytesDone = 0;
    while (1)
    {
        lDecodeSize = ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
        if (lDecodeSize > 0){
            ulBytesDone += lDecodeSize;
            if (ulBytesDone >= ulBufferSize)
                break;
        }
        else{
            break;
        }
    }

    // Mono, Stereo and 4-Channel files decode into the same channel order as WAVEFORMATEXTENSIBLE,
    // however 6-Channels files need to be re-ordered
    if (ulChannels == 6)
    {
        pSamples = (short*)pDecodeBuffer;
        for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6){
            // WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
            // OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
            std::swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
            std::swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
            std::swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
        }
    }

    return ulBytesDone;
}


size_t al_fread(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    io::unifile *f = static_cast<io::unifile*>(datasource);
    return f->read(ptr, size * nmemb);
}

int al_fseek(void *datasource, ogg_int64_t offset, int whence)
{
    io::unifile *f = static_cast<io::unifile*>(datasource);
    return f->seek(offset, whence);
}

int al_fclose(void *datasource)
{
    io::unifile *f = static_cast<io::unifile*>(datasource);
    f->close();
    return 0;
}

long al_ftell(void *datasource)
{
    io::unifile *f = static_cast<io::unifile*>(datasource);
    return f->tell();
}

}//end namespace al
}//end namespace cgl

#endif //OGG_HPP_20200221175501
