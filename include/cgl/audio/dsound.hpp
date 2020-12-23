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

//�ǲ��ǲ�ͬ��������ͨ��һ��buffer���ţ�
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

        //DSSCL_EXCLUSIVE   ��ǰ�����ռ�豸
        //DSSCL_PRIORITY    ������������DirectSound�豸��Ȩ��.�ڴ˱�־��,directsound��Ȼ�Ƕ�ռ��.
        //                  (�ɵ���SetFormat�������λ������Ĳ��Ÿ�ʽ)
        //DSSCL_NORMAL      ����Э������,��������ɹ��������豸���в���.
        //                  �˱�־��,�������޸����������ĸ�ʽ,���һ�Ĭ���������ڴ���
        //                  ���뵽һ��8bit��22KHz�ĵ�������������.
        //DSSCL_WRITEPRIMARY��д��������,��ʱ�����������ɲ��Ŵ���,����˵���ܰ����ݷŽ���������,�ٴ�����������.
        //                  ���еĴ���ֻ�������������н���.(������ȫ���������Ĳ���)
        hr = m_sound->SetCooperativeLevel(handle, DSSCL_NORMAL);
        if(FAILED(hr)){
            return FALSE;
        }

        //��ʼ��һ��wave��ʽ
        WAVEFORMATEX wavformat;
        init_format(wavformat, 2, 8000, 16);

        DSBUFFERDESC desc;
        memset(&desc, 0, sizeof(desc));
        desc.dwSize  = sizeof(desc);
        desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC |DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN ;
        desc.dwBufferBytes = MAX_AUDIO_BUF * BUFFERNOTIFYSIZE;
        desc.lpwfxFormat = &wavformat;

        //����buffer
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

    //���ܸ��ĸ�ʽ
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
            0,  //����Ȩ, δʹ��DSBCAPS_LOCDEFER ��������Ϊ0
            0   //DSBPLAY_LOOPING       ѭ��
        );      //DSBPLAY_LOCHARDWARE   ֻ��ͨ��Ӳ���еĻ���������
                //DSBPLAY_LOCSOFTWARE   ֻ��ͨ������еĻ��������в���
    }

    //����
    void set_volume(int value = DSBVOLUME_MAX)
    {
        m_soundbuf8->SetVolume(value);
    }

    //�������ò��Ÿ�ʽ
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
