/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 al.hpp

 2018-08-11 14:49:46

 albuffer > albuffer_manager > albuffer_auto_manager
 alsound + allistener + albuffer_auto_manager > alcontext

 albuffer alsound 独自处理的音频数据，不在自动管理范围之内

 所以类和容器，都不自动释放资源。

*/
#ifndef AL_HPP_20180811144946
#define AL_HPP_20180811144946

//#define CGL_AUDIO_NO_OGG

#include <cgl/public.h>
#include <cgl/io.hpp>
#include <cgl/filesystem.hpp>
#include <cgl/string.hpp>
#include <cgl/vec.hpp>

#include <vector>
#include <map>

#ifdef CGL_PLATFORM_CB6
#define _fseeki64 fseeki64
#endif

#define AL_ALEXT_PROTOTYPES

#ifdef CGL_PLATFORM_MSVC
	#define AL_LIBTYPE_STATIC
#endif

#include <al/al.h>
#include <al/alc.h>

#define AL_ALEXT_PROTOTYPES
#include <al/alext.h>
//#include <al/alu.h>
//#include <al/alut.h>

#ifndef CGL_AUDIO_NO_OGG
#include "ogg.hpp"
#endif//CGL_AUDIO_NO_OGG

#include "soft_oal.hpp"
#include "wavefile.hpp"

/*
#ifdef CGL_NO_AUDIO
extern "C"{
AL_API void AL_APIENTRY alListenerfv(ALenum param, const ALfloat *values) { }

AL_API void AL_APIENTRY alSourcei(ALuint source, ALenum param, ALint value) { }
AL_API void AL_APIENTRY alGetSourcei(ALuint source,  ALenum param, ALint *value) { }
AL_API void AL_APIENTRY alDeleteSources(ALsizei n, const ALuint *sources){ }

ALC_API ALCcontext* ALC_APIENTRY alcCreateContext(ALCdevice *device, const ALCint* attrlist) { return NULL; }
AL_API void AL_APIENTRY alDeleteBuffers(ALsizei n, const ALuint *buffers) { }
ALC_API ALCboolean ALC_APIENTRY alcMakeContextCurrent(ALCcontext *context) { return false; }
ALC_API void ALC_APIENTRY alcDestroyContext(ALCcontext *context) { }

ALC_API ALCdevice* ALC_APIENTRY alcOpenDevice(const ALCchar *devicename) { return NULL; }
ALC_API ALCboolean ALC_APIENTRY alcCloseDevice(ALCdevice *device) { return false; }
}
#endif//CGL_NO_AUDIO
*/

/*
ALvoid alBufferDataStatic(const ALint bid, ALenum format, ALvoid* data, ALsizei size, ALsizei freq)
{
    static PFNALBUFFERDATASTATICPROC proc = NULL;
    if (proc == NULL){
        proc = (PFNALBUFFERDATASTATICPROC)alcGetProcAddress(NULL, (const ALCchar*) "alBufferDataStatic");
    }
    if (proc)
        proc(bid, format, data, size, freq);
}
*/

namespace cgl{
namespace al{

const char* al_error_string(ALenum err)
{
    switch(err){
    case AL_NO_ERROR          : return "AL_NO_ERROR";
    case AL_INVALID_NAME      : return "AL_INVALID_NAME";
    case AL_INVALID_ENUM      : return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE     : return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION : return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY     : return "AL_OUT_OF_MEMORY";
    default:
        return "unknown AL error.";
    };
}

void al_check(int err)
{
    CGL_LOG(al_error_string(err));
}

const char* alc_error_string(ALenum err)
{
    switch(err){
    case ALC_NO_ERROR        : return "ALC_NO_ERROR";
    case ALC_INVALID_DEVICE  : return "ALC_INVALID_DEVICE";
    case ALC_INVALID_CONTEXT : return "ALC_INVALID_CONTEXT";
    case ALC_INVALID_ENUM    : return "ALC_INVALID_ENUM";
    case ALC_INVALID_VALUE   : return "ALC_INVALID_VALUE";
    case ALC_OUT_OF_MEMORY   : return "ALC_OUT_OF_MEMORY";
    default:
        return "unknown ALC error.";
    };
}

void alc_check(int err)
{
    CGL_LOG(alc_error_string(err));
}

class alobject
{
protected:
    ALuint m_id;
public:
    alobject() : m_id()
    {

    }

    alobject(ALuint id) : m_id(id)
    {

    }

    ALuint handle()const
    {
        return m_id;
    }

    bool is_null()const
    {
        return m_id != 0;
    }

    virtual void dispose()
    {
        m_id = 0;
    }
};

//
// albuffer al音频缓冲区
//

class albuffer : public alobject
{
public:
    albuffer() : alobject()
    {
    }

    albuffer(ALuint id) : alobject(id)
    {
    }

    ~albuffer()//non auto dispose
    {
    }

    static bool is_buffer(ALuint id)
    {
        return alIsBuffer(id);
    }

    //ogg wav
    int open(const string_t& filename);
    int open_memory(const byte_t* data, size_t size);

    void bind(ALuint id)
    {
        m_id = id;
    }

    ALenum format()const
    {
        //return get_int(AL_FORMAT);
    }

    ALsizei size()const
    {
        return get_int(AL_SIZE);
    }

    ALsizei frequency()const
    {
        return get_int(AL_FREQUENCY);
    }

    ALsizei bits()const
    {
        return get_int(AL_BITS);
    }

    ALsizei channels()const
    {
        return get_int(AL_CHANNELS);
    }

    void dispose()
    {
        if(m_id){
            alDeleteBuffers(1, &m_id);
            m_id = 0;
        }
    }

    int update(const byte_t* data, size_t size, ALenum format, ALsizei frequency);

private:
    int open_ogg(io::unifile* file);

    int get_int(ALenum e)const
    {
        if(m_id){
            ALsizei value;
            alGetBufferi(m_id, e, &value);
            return value;
        }
        return 0;
    }
};

//
// albuffer_manager buffer管理器
//

class albuffer_manager
{
private:
    std::vector<albuffer> m_buffers;

public:
    albuffer_manager() : m_buffers()
    {

    }
    ~albuffer_manager()
    {
    }

    void dispose()
    {
        for(size_t i=0; i<m_buffers.size(); ++i){
            m_buffers[i].dispose();
        }
        m_buffers.clear();
    }

    //ogg wav
    int open(const string_t& file)
    {
        albuffer b;
        if(b.open(file) == 0){
            m_buffers.push_back(b);
            return b.handle();
        }
        return 0;
    }

    int open_memory(const byte_t* data, size_t size)
    {
        albuffer b;
        if(b.open_memory(data, size) == 0){
            m_buffers.push_back(b);
            return b.handle();
        }
        return 0;
    }

    void erase(size_t id)
    {
        if(id < m_buffers.size()){
            m_buffers[id].dispose();
            m_buffers.erase(m_buffers.begin() + id);
        }
    }

    albuffer& operator[](size_t id){ return m_buffers[id]; }
    const albuffer& operator[](size_t id)const { return m_buffers[id]; }
};

//
// albuffer_store 自动加载buffer
//

class albuffer_store
{
public:
    typedef std::map<string_t, ALuint, csicmper> container_type;
    typedef TYPENAME container_type::iterator iterator;
    albuffer_manager buffers;
    container_type m_map;

public:
    albuffer_store() : buffers()
    {
    }

    ~albuffer_store()
    {
    }

    //ogg wav
    ALuint open(const string_t& filename)
    {
        iterator itr = m_map.find(filename);
        if(itr == m_map.end()){
            ALuint buf = buffers.open(filename);
            if(buf){
                m_map[filename] = buf;
                return buf;
            }
            else{
                return 0;
            }
        }
        else{
            return itr->second;
        }
    }

    ALuint open_memory(const string_t& filename, const byte_t* data, size_t size)
    {
        iterator itr = m_map.find(filename);
        if(itr == m_map.end()){
            ALuint buf = buffers.open_memory(data, size);
            if(buf){
                m_map[filename] = buf;
                return buf;
            }
            else{
                return 0;
            }
        }
        else{
            return itr->second;
        }
    }

    bool exists(const string_t& filename)const
    {
        return m_map.find(filename) != m_map.end();
    }

    void clear(){
        buffers.dispose();
        m_map.clear();
    }
};

//
// alsound 播放器
//

class alsound : public alobject
{
public:
    alsound() : alobject()
    {
    }

    ~alsound()//non auto dispose
    {
    }

    ALuint buffer()const
    {
        return attributei(AL_BUFFER);
    }

    bool bind_sound(ALuint id)
    {
        m_id = id;
        return this->is_null();
    }

    bool bind_buffer(const ALuint buf)
    {
        if(buf == 0){
            return AL_FALSE;
        }

        if(this->buffer() == buf){
            return AL_TRUE;
        }

        if(m_id == 0){
            alGenSources(1, &m_id);
            //check()
        }

        alSourcei(m_id, AL_BUFFER, buf);
        alSourcef(m_id, AL_PITCH, 1.0f);
        set_volume(100.0f);
        //alSourcef(m_source, AL_SOURCE_TYPE, AL_STATIC);//AL_STREAMING

        if (alGetError() != AL_NO_ERROR){
            return AL_FALSE;
        }
        return AL_TRUE;
    }

    bool bind(const albuffer& buf)
    {
        return this->bind_buffer(buf.handle());
    }

    void dispose()
    {
        if(m_id){
            alSourcei(m_id, AL_BUFFER, 0);
            alDeleteSources(1, &m_id);
        }
        m_id = 0;
    }

    void set_loop(bool loop)
    {
        source(AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    }

    void play(bool loop = false)
    {
        if(m_id){
            set_loop(loop);
            alSourcePlay(m_id);
        }
    }

    void pause()
    {
        if(m_id){
            alSourcePause(m_id);
        }
    }

    void rewind()
    {
        if(m_id){
            alSourceRewind(m_id);
        }
    }

    void stop()
    {
        if(m_id){
            alSourceStop(m_id);
        }
    }

    //
    // 播放状态
    //

    ALenum state()const
    {
        if(m_id){
            ALenum state;
            alGetSourcei(m_id, AL_SOURCE_STATE, &state);
            return state;
        }
        return AL_NONE;
    }

    bool is_looping()const
    {
        return attributei(AL_LOOPING) == AL_TRUE;
    }

    bool is_playing()const
    {
        return this->state() == AL_PLAYING;
    }

    bool is_paused()const
    {
        return this->state() == AL_PAUSED;
    }

    bool is_stop()const
    {
        return this->state() == AL_STOPPED;
    }

    //
    // 播放进度（秒）
    //

    float offset()
    {
        return attributef(AL_SEC_OFFSET);
    }

    void set_offset(float value)
    {
        source(AL_SEC_OFFSET, value);
    }

    int sample_offset()const
    {
        return attributei(AL_SAMPLE_OFFSET);
    }

    int byte_offset()const
    {
        return attributei(AL_BYTE_OFFSET);
    }

    //计算总时间，单位秒
    float length()
    {
        ALuint buf = this->buffer();
        if(buf){
            int size;
            int frequency;
            int bits;
            int channels;

            alGetBufferi(buf, AL_SIZE, &size);
            alGetBufferi(buf, AL_FREQUENCY, &frequency);
            alGetBufferi(buf, AL_BITS, &bits);
            alGetBufferi(buf, AL_CHANNELS, &channels);
            return float(size) / (frequency * (bits / 8) * channels);
        }
        return 0.0f;
    }

    //
    // 音量
    //

    float volume()const
    {
        const float scale = 100.0f;
        return attributef(AL_GAIN) * scale;
    }

    void set_volume(float volume)
    {
        const float scale = 1.0f / 100.0f;
        source(AL_GAIN, volume * scale);
    }

    //
    // 3D音效
    //

    /*

    vec3f position;       //源声音的位置
    vec3f vel;            // 源声音的速度 volume
    vec3f Listener;       // 听者的位置
    vec3f ListenerVel;    // 听者的速度
    vec3f ListenerOri;    // 听者的方向



    alSource3f(source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (source, AL_SOURCE_RELATIVE, AL_TRUE      );

    alSpeedOfSound(1.0);
    alDopplerVelocity(1.0);
    alDopplerFactor(1.0);
    alSourcef(Source, AL_PITCH, 1.0f);
    alSourcef(Source, AL_SOURCE_TYPE, AL_STREAMING);
    */

    vec3f position()
    {
        vec3f p;
        if(m_id){
            alGetSource3f(m_id, AL_POSITION, &p.x, &p.y, &p.z);
        }
        return p;
    }

    //X,Y,Z 源声音的位置
    void set_position(vec3f v)
    {
        if(m_id){
            alSourcefv(m_id, AL_POSITION, v.data);
        }
    }

    void set_position(float x, float y, float z)
    {
        if(m_id){
            float v[3] = {x, y, z};
            alSourcefv(m_id, AL_POSITION, v);
        }
    }

    //速度矢量 源声音的速度
    void set_velocity(vec3f v)
    {
        if(m_id){
            alSourcefv(m_id, AL_VELOCITY, v.data);
        }
    }

    //声源方向
    void set_direction(vec3f v)
    {
        if(m_id){
            alSourcefv(m_id, AL_DIRECTION, v.data);
        }
    }
private:
    void source(ALenum e, int value)
    {
        if(m_id){
            alSourcei(m_id, e, value);
        }
    }

    void source(ALenum e, float value)
    {
        if(m_id){
            alSourcef(m_id, e, value);
        }
    }

    void sourcefv(ALenum e, float values[])
    {
        if(m_id){
            alSourcefv(m_id, e, values);
        }
    }

    int attributei(ALenum e)const
    {
        if(m_id){
            ALenum value;
            alGetSourcei(m_id, e, &value);
            return value;
        }
        return AL_NONE;
    }

    float attributef(ALenum e)const
    {
        if(m_id){
            float value;
            alGetSourcef(m_id, e, &value);
            return value;
        }
        return 0.0f;
    }
};

//
// allistener 倾听者位置
//

class allistener
{
public:
    vec3f position;
    vec3f velocity;
    vec3f at;
    vec3f up;
public:
    allistener() : position(), velocity(), at(), up()
    {

    }

    void init()
    {
        velocity = vec3f(0.0f, 0.0f, -1.0f);
        at = vec3f(0.0f, 0.0f, -1.0f);
        up = vec3f(0.0f, 1.0f,  0.0f);
        alListenerfv(AL_POSITION, position.data);
        alListenerfv(AL_VELOCITY, velocity.data);
        alListenerfv(AL_ORIENTATION, at.data);
    }

    void set_position(float x, float y, float z = 0.0f)
    {
        position.x = x;
        position.y = y;
        position.z = z;
        alListenerfv(AL_POSITION, position.data);
    }

    void set_position(const vec3f& pos)
    {
        position = pos;
        alListenerfv(AL_POSITION, position.data);
    }

    void set_velocity(const vec3f& vel)
    {
        velocity = vel;
        alListenerfv(AL_VELOCITY, velocity.data);
    }

    void set_direction(const vec3f& _at, const vec3f& _up)
    {
        at = _at;
        up = _up;
        alListenerfv(AL_ORIENTATION, at.data);
    }

    //AL_SEC_OFFSET f, fv, i, iv 播放的位置，秒
    //方向矢量
    // 听者的方向 (first 3 elements are "at", second 3 are "up")
    //ALfloat ListenerOri[6];
    //AL_GAIN f, fv 主增益，应为正数
    //AL_POSITION fv, 3f, iv, 3i X，Y，Z位置
    //fv, 3f, iv, 3i速度向量    AL_ORIENTATION fv, iv 由"at"和"up"向量描述的方向
    //float t[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
    //AL_ORIENTATION
    //alListener
    //fv, iv 由"at"和"up"向量描述的方向
    void set_direction(float n[6])
    {
        alListenerfv(AL_ORIENTATION, n);
    }
};

//
// alcontext al设备环境
//

class alcontext
{
public:
    static ALCdevice* device;
    albuffer_store buffers;
    allistener listener;

private:
    ALCcontext *m_context;
    std::vector<alsound> m_sounds;

    static shared_obj so;

public:
    alcontext() : buffers(), listener(), m_context(null), m_sounds()
    {

    }

    ~alcontext()
    {
    }

    bool is_init()const
    {
        return m_context;
    }

    static void* AL_Loader(const char* name)
    {
        return (void*)so.load(name);
    }

    static ALCdevice* open_device()
    {
        if(!device){
            AL_init(soft_oal, AL_Loader);
            ALC_init(soft_oal, AL_Loader);
            ALEXT_init(soft_oal, AL_Loader);

            #if 0 //def CGL_PLATFORM_WINDOWS
            ALCdevice *dev;
            if(dev = alcOpenDevice("DirectSound3D")){
                const_write(device) = dev;
                CGL_LOG("openal device: DirectSound3D.");
            }
            else if(dev = alcOpenDevice("DirectSound")){
                const_write(device) = dev;
                CGL_LOG("openal device: DirectSound.");
            }
            else if(dev = alcOpenDevice("MMSYSTEM")){
                const_write(device) = dev;
                CGL_LOG("openal device: mmsystem.");
            }
            else if(dev = alcOpenDevice("openal-soft")){
                const_write(device) = dev;
                CGL_LOG_ERROR("openal device: openal-soft.");
            }
            else if(dev = alcOpenDevice(null)){
                const_write(device) = dev;
                CGL_LOG_ERROR("openal device: default.");
            }
            else{
                CGL_LOG_ERROR("openal device create failed.");
            }
            #else
            //const char * devicename = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
            const_write(device) = alcOpenDevice(NULL);
//            const_write(device) = alcLoopbackOpenDeviceSOFT(NULL);
            #endif

            atexit(close_device);    //close device on program exit
        }
        return device;
    }

    static void close_device()
    {
        if(device){
            alcCloseDevice(device);
            const_cast<ALCdevice*&>(device) = null;
        }
    }

    int init()
    {
        open_device();
        if(device){
            ALCint attrs[16];
            attrs[0] = ALC_FORMAT_CHANNELS_SOFT;
            attrs[1] = ALC_STEREO_SOFT;
            attrs[2] = ALC_FORMAT_TYPE_SOFT;
            attrs[3] = ALC_SHORT_SOFT;
            attrs[4] = ALC_FREQUENCY;
            attrs[5] = 44100;
            attrs[6] = 0;
            m_context = alcCreateContext(device, null);
            alcMakeContextCurrent(m_context);
            listener.init();
            return 0;
        }
        return -1;
    }

    bool is_enabled()const
    {
        return m_context;
    }

    void dispose()
    {
        buffers.clear();
        if(m_context){
            alcMakeContextCurrent(null);
            alcDestroyContext(m_context);
            m_context = null;
        }
    }

    //主音量(0.0f ~ 100.0f)
    void set_volume(float volume)
    {
        alListenerf(AL_GAIN, volume / 100.0f);
    }

    //得到设备说明
    //default : "Generic Software"
    const char* specifier()const
    {
        return alcGetString(device, ALC_DEVICE_SPECIFIER);
    }

    #if 0 //没效果
    void stop()
    {
        alcSuspendContext(m_context); // 终止pContext.
    }

    void reset()
    {
        alcProcessContext(m_context); // 重置pContext.
    }
    #endif

//sounds manager
    //这两个函数产生的句柄，自动回收
    //播放一个声音，并返回句柄
    ALuint playsound(const string_t& filename)
    {
        alsound s;
        ALuint buf = buffers.open(filename);
        if(buf){
            s.bind_buffer(buf);
            s.play();
            m_sounds.push_back(s);
            return s.handle();
        }
        return 0;
    }

    //执行句柄回收
    void update(){
        if(!m_sounds.empty())
        for(size_t i=m_sounds.size() - 1 ; i < SIZE_MAX; --i)
        {
            if(!m_sounds[i].is_playing()){
                m_sounds[i].dispose();
                m_sounds.erase(m_sounds.begin() + i);
            }
        }
    }
};

CGL_PUBLIC_DECLARE ALCdevice* alcontext::device = null;
CGL_PUBLIC_DECLARE shared_obj alcontext::so("soft_oal.dll");

//---------------------------------------------------------------------------
// albuffer

int albuffer::open(const string_t& filename)
{
    io::binfile file;
    file.open(filename.c_str(), io_read, sh_share);
    if(file.is_open()){
        byte_t buf[64];
        file.read(buf, 64);
        if(filesystem::check_wav(buf, 64)){
            wavfile wav;
            file.close();
            if(wav.open(filename.c_str()) == CGL_OK){
                return update(wav.data(), wav.size(), wav.format(), wav.frequency());
            }
        }
        else if(filesystem::check_ogg(buf, 64)){
            file.seek(0, seek_begin);
            return this->open_ogg(&file);
        }
        //format error
    }

    CGL_LOG_ERROR("albuffer : open file open error.");
    return -1;
}

int albuffer::open_memory(const byte_t* data, size_t size)
{
    if(filesystem::check_wav(data, size)){
        wavfile wav;
        if(wav.open_memory(data, size) == CGL_OK){
            return update(wav.data(), wav.size(), wav.format(), wav.frequency());
        }
    }
    else if(filesystem::check_ogg(data, size)){
        io::filelink file;
        file.link(data, 0, size);
        return this->open_ogg(&file);
    }
    return -1;
}

int albuffer::open_ogg(io::unifile* file)
{
    #ifndef CGL_AUDIO_NO_OGG
    OggVorbis_File  ogg_file;
    ov_callbacks    ogg_callback;
    vorbis_info       *psVorbisInfo;

    ogg_callback.read_func = al_fread;
    ogg_callback.seek_func = al_fseek;
    ogg_callback.close_func = al_fclose;
    ogg_callback.tell_func = al_ftell;

    if(ov_open_callbacks(file, &ogg_file, NULL, 0, ogg_callback) < 0)
    //if(ov_open(f, &ogg_file, NULL, 0) < 0)
    {
        return -1;
    }

    psVorbisInfo = ov_info(&ogg_file, -1);

    size_t    ulFrequency = 0;
    size_t    ulFormat = 0;
    size_t    ulChannels = 0;
    size_t    ulBufferSize;
    size_t    ulBytesWritten;

    if (psVorbisInfo)
    {
        ulFrequency = psVorbisInfo->rate;
        ulChannels = psVorbisInfo->channels;
        if (psVorbisInfo->channels == 1){
            ulFormat = AL_FORMAT_MONO16;
        }
        else if (psVorbisInfo->channels == 2){
            //ulFormat = AL_FORMAT_MONO16;
            //如果文件是双通道，STEREO16无法开启立体声特效
            ulFormat = AL_FORMAT_STEREO16;
        }
        else if (psVorbisInfo->channels == 4){
            ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
        }
        else if (psVorbisInfo->channels == 6){
            ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
        }
        //double time = ov_time_total(&ogg_file, 0);    //获取媒体文件时间，乘以比特率
        //ulBufferSize = ulFrequency * psVorbisInfo->channels * sizeof(int16_t) * time;
        //ulBufferSize = (ulBufferSize + 15) & ~15;    //直接对齐到16字节
        ulBufferSize = ov_pcm_total(&ogg_file, -1);
        ulBufferSize *= psVorbisInfo->channels * sizeof(int16_t);
    }

    if (ulFormat != 0)
    {
        memblock<char> buffer;
        buffer.resize(ulBufferSize);

        ulBytesWritten = DecodeOggVorbis(&ogg_file, buffer.data(), ulBufferSize * 20, ulChannels);

        //双声道转单声道，双声道无法产生立体声效果
        if(ulFormat == AL_FORMAT_STEREO16){
            int16_t* p = (int16_t*)buffer.data();
            stereo_to_mono(p, p, ulBufferSize / 4);
            ulFormat = AL_FORMAT_MONO16;
            ulBytesWritten /= 2;
            //ulFrequency *= 2;    //声音速率
        }

        if (ulBytesWritten){
            //auto create handle and update data
            update((byte_t*)buffer.data(), ulBytesWritten, ulFormat, ulFrequency);
        }
    }
    ov_clear(&ogg_file);
    return 0;
    #else
    return -1;
    #endif
}

const char* al_format_string(ALenum format)
{
    switch(format){
    case AL_FORMAT_MONO8:
        return "AL_FORMAT_MONO8";
    case AL_FORMAT_MONO16:
        return "AL_FORMAT_MONO16";
    case AL_FORMAT_STEREO8:
        return "AL_FORMAT_STEREO8";
    case AL_FORMAT_STEREO16:
        return "AL_FORMAT_STEREO16";
    default:
        return "unknonw format";
    }
}

int albuffer::update(const byte_t* data, size_t size, ALenum format, ALsizei frequency)
{
    int err;
    if(!m_id){
        alGenBuffers(1, &m_id);
        err = alGetError();
        if (err != AL_NO_ERROR){
            return err;
        }
    }

    CGL_LOG("albuffer::update size=%d, format=%s, frequency=%d",
        size, al_format_string(format), frequency);


    //AL_FORMAT_[MONO8, MONO16, STEREO8, STEREO16]
    //alBufferData(m_id, format, data, size, frequency);
    //alBufferDataStatic(m_id, format, (void*)data, size, frequency);
    alBufferStorageSOFT(m_id, format, (void*)data, size, frequency, AL_MAP_READ_BIT_SOFT|AL_MAP_PERSISTENT_BIT_SOFT);
    err = alGetError();
    if (err != AL_NO_ERROR){
        al_check(err);
        return err;
    }

    return err;
}

}//end namespace al
}//end namespace cgl

#endif //AL_HPP_20180811144946

