/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 dsound.hpp

 2020-06-16 10:47:52

*/
#ifndef DSOUND_HPP_20200616104752
#define DSOUND_HPP_20200616104752

#include <cgl/public.h>

#include <dsound.h>
#pragma comment(lib,"dsound.lib")

#include "wavefile.hpp"

#ifndef INITGUID
    #error not defined INITGUID
#endif

namespace cgl{
namespace al{


#define MAX_AUDIO_BUF 4
#define BUFFERNOTIFYSIZE 192000

//是不是不同声音可以通过一个buffer播放？
//

class dxsound
{
public:
    IDirectSound8* m_sound;
    IDirectSoundBuffer*  m_soundbuf;//used to manage sound buffers.
    IDirectSoundBuffer8* m_soundbuf8;
    //IDirectSoundNotify8* m_notify;
    //DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];

public:
    int create(HWND handle)
    {
        HRESULT hr;
        //Init DirectSound
        hr = DirectSoundCreate8(NULL, &m_sound, NULL);
        if(FAILED(hr)){
            return FALSE;
        }

        //DSSCL_EXCLUSIVE   当前程序独占设备
        //DSSCL_PRIORITY    具有优先设置DirectSound设备的权限.在此标志下,directsound必然是独占的.
        //                  (可调用SetFormat设置主次缓冲区的播放格式)
        //DSSCL_NORMAL      正常协调级别,其他程序可共享声卡设备进行播放.
        //                  此标志下,不允许修改主缓冲区的格式,而且会默认在声卡内存中
        //                  申请到一个8bit和22KHz的单声道主缓冲区.
        //DSSCL_WRITEPRIMARY可写主缓冲区,此时主缓冲区不可播放处理,就是说不能把数据放进混声器中,再传到主缓冲区.
        //                  所有的处理只能在主缓冲区中进行.(可以完全控制声音的播放)
        hr = m_sound->SetCooperativeLevel(handle, DSSCL_NORMAL);
        if(FAILED(hr)){
            return FALSE;
        }

        //初始化一个wave格式
        WAVEFORMATEX wavformat;
        init_format(wavformat, 2, 8000, 16);

        DSBUFFERDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize  = sizeof(desc);
        desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC |DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN ;
        desc.dwBufferBytes = MAX_AUDIO_BUF * BUFFERNOTIFYSIZE;
        desc.lpwfxFormat = &wavformat;

        //创建buffer
        if( FAILED(m_sound->CreateSoundBuffer(&desc, &m_soundbuf, NULL))){
            return FALSE;
        }
        if( FAILED(m_soundbuf->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&m_soundbuf8))){
            return FALSE ;
        }

        //Get IDirectSoundNotify8
        /*
        if(FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))){
            return FALSE ;
        }

        for(int i =0;i<MAX_AUDIO_BUF;i++){
            m_pDSPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;
            m_event[i]=::CreateEvent(NULL,false,false,NULL);
            m_pDSPosNotify[i].hEventNotify=m_event[i];
        }
        m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_pDSPosNotify);
        m_pDSNotify->Release();
        */
        return 0;
    }

    void update(const void* data, size_t size)
    {
        const DWORD offset = 0;
        LPVOID buf     = NULL;
        DWORD  bufsize = 0;

        m_soundbuf8->Lock(offset, size, &buf, &bufsize, NULL, NULL, 0);
        memcpy(buf, data, bufsize);

        m_soundbuf8->Unlock(buf, bufsize, NULL, 0);
    }

    //不能更改格式
    void update(const al::wavfile& wav)
    {
//        int n = set_format(wav.channels(), wav.frequency(), wav.sample_bits());
//        update(wav.data(), wav.size());
    }

    int set_position(size_t position)
    {
        return m_soundbuf8->SetCurrentPosition(position);
    }

    size_t position()const
    {
        DWORD dwPlay;
        m_soundbuf8->GetCurrentPosition(&dwPlay, null);
        return dwPlay;
    }

    void play()
    {
        m_soundbuf8->SetCurrentPosition(0);
        m_soundbuf8->Play(
            0,
            0,  //优先权, 未使用DSBCAPS_LOCDEFER 必须设置为0
            0   //DSBPLAY_LOOPING       循环
        );      //DSBPLAY_LOCHARDWARE   只能通过硬件中的缓冲区播放
                //DSBPLAY_LOCSOFTWARE   只能通过软件中的缓冲区进行播放
    }

    //音量
    void set_volume(int value = DSBVOLUME_MAX)
    {
        m_soundbuf8->SetVolume(value);
    }

    //重新设置播放格式
    int set_format(const WAVEFORMATEX& wavformat)
    {
        return m_soundbuf8->SetFormat(&wavformat);
    }

    int set_format(int channels, int sampleRate, int sampleBits)
    {
        WAVEFORMATEX wavformat;
        init_format(wavformat, channels, sampleRate, sampleBits);
        return set_format(wavformat);
    }

private:
    void init_format(WAVEFORMATEX& wavformat, int channels, int sampleRate, int sampleBits)
    {
        wavformat.wFormatTag      = WAVE_FORMAT_PCM;
        wavformat.nChannels       = channels;
        wavformat.nSamplesPerSec  = sampleRate;
        wavformat.nAvgBytesPerSec = sampleRate * (sampleBits / 8) * channels;
        wavformat.nBlockAlign     = (sampleBits / 8) * channels;
        wavformat.wBitsPerSample  = sampleBits;
        wavformat.cbSize = 0;
    }

};


}//end namespace al
}//end namespace cgl

#endif //DSOUND_HPP_20200616104752
