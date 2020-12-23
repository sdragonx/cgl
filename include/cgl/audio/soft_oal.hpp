/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 soft_oal.hpp

 2020-02-10 04:20:39

 ∂ØÃ¨º”‘ÿsoft_oal

*/
#ifndef SOFT_OAL_HPP_20200210042039
#define SOFT_OAL_HPP_20200210042039

#include <cgl/public.h>
#include <cgl/system/shared_obj.hpp>
#include <al/al.h>
#include <al/alc.h>
#include <al/alext.h>

extern "C"{

#ifndef AL_SOFT_map_buffer
#define AL_SOFT_map_buffer 1
typedef unsigned int ALbitfieldSOFT;
#define AL_MAP_READ_BIT_SOFT                     0x00000001
#define AL_MAP_WRITE_BIT_SOFT                    0x00000002
#define AL_MAP_PERSISTENT_BIT_SOFT               0x00000004
#define AL_PRESERVE_DATA_BIT_SOFT                0x00000008
typedef void (AL_APIENTRY*LPALBUFFERSTORAGESOFT)(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq, ALbitfieldSOFT flags);
typedef void* (AL_APIENTRY*LPALMAPBUFFERSOFT)(ALuint buffer, ALsizei offset, ALsizei length, ALbitfieldSOFT access);
typedef void (AL_APIENTRY*LPALUNMAPBUFFERSOFT)(ALuint buffer);
typedef void (AL_APIENTRY*LPALFLUSHMAPPEDBUFFERSOFT)(ALuint buffer, ALsizei offset, ALsizei length);
#ifdef AL_ALEXT_PROTOTYPES
AL_API void AL_APIENTRY alBufferStorageSOFT(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq, ALbitfieldSOFT flags);
AL_API void* AL_APIENTRY alMapBufferSOFT(ALuint buffer, ALsizei offset, ALsizei length, ALbitfieldSOFT access);
AL_API void AL_APIENTRY alUnmapBufferSOFT(ALuint buffer);
AL_API void AL_APIENTRY alFlushMappedBufferSOFT(ALuint buffer, ALsizei offset, ALsizei length);
#endif
#endif


typedef void* (oal_loader)(const char* name);

struct soft_oalAPI
{
    //AL
    LPALDOPPLERFACTOR        alDopplerFactor;
    LPALDOPPLERVELOCITY      alDopplerVelocity;
    LPALSPEEDOFSOUND         alSpeedOfSound;

    LPALDISTANCEMODEL        alDistanceModel;

    LPALENABLE               alEnable;
    LPALDISABLE              alDisable;
    LPALISENABLED            alIsEnabled;

    LPALGETSTRING            alGetString;
    LPALGETBOOLEANV          alGetBooleanv;
    LPALGETINTEGERV          alGetIntegerv;
    LPALGETFLOATV            alGetFloatv;
    LPALGETDOUBLEV           alGetDoublev;
    LPALGETBOOLEAN           alGetBoolean;
    LPALGETINTEGER           alGetInteger;
    LPALGETFLOAT             alGetFloat;
    LPALGETDOUBLE            alGetDouble;

    LPALGETERROR             alGetError;
    LPALISEXTENSIONPRESENT   alIsExtensionPresent;
    LPALGETPROCADDRESS       alGetProcAddress;
    LPALGETENUMVALUE         alGetEnumValue;

    LPALLISTENERF            alListenerf;
    LPALLISTENER3F           alListener3f;
    LPALLISTENERFV           alListenerfv;
    LPALLISTENERI            alListeneri;
    LPALLISTENER3I           alListener3i;
    LPALLISTENERIV           alListeneriv;

    LPALGETLISTENERF         alGetListenerf;
    LPALGETLISTENER3F        alGetListener3f;
    LPALGETLISTENERFV        alGetListenerfv;
    LPALGETLISTENERI         alGetListeneri;
    LPALGETLISTENER3I        alGetListener3i;
    LPALGETLISTENERIV        alGetListeneriv;

    LPALGENSOURCES           alGenSources;
    LPALDELETESOURCES        alDeleteSources;
    LPALISSOURCE             alIsSource;

    LPALSOURCEF              alSourcef;
    LPALSOURCE3F             alSource3f;
    LPALSOURCEFV             alSourcefv;
    LPALSOURCEI              alSourcei;
    LPALSOURCE3I             alSource3i;
    LPALSOURCEIV             alSourceiv;

    LPALGETSOURCEF           alGetSourcef;
    LPALGETSOURCE3F          alGetSource3f;
    LPALGETSOURCEFV          alGetSourcefv;
    LPALGETSOURCEI           alGetSourcei;
    LPALGETSOURCE3I          alGetSource3i;
    LPALGETSOURCEIV          alGetSourceiv;

    LPALSOURCEPLAYV          alSourcePlayv;
    LPALSOURCESTOPV          alSourceStopv;
    LPALSOURCEREWINDV        alSourceRewindv;
    LPALSOURCEPAUSEV         alSourcePausev;
    LPALSOURCEPLAY           alSourcePlay;
    LPALSOURCESTOP           alSourceStop;
    LPALSOURCEREWIND         alSourceRewind;
    LPALSOURCEPAUSE          alSourcePause;
    LPALSOURCEQUEUEBUFFERS   alSourceQueueBuffers;
    LPALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;

    LPALGENBUFFERS           alGenBuffers;
    LPALDELETEBUFFERS        alDeleteBuffers;
    LPALISBUFFER             alIsBuffer;

    LPALBUFFERDATA           alBufferData;

    LPALBUFFERF              alBufferf;
    LPALBUFFER3F             alBuffer3f;
    LPALBUFFERFV             alBufferfv;
    LPALBUFFERI              alBufferi;
    LPALBUFFER3I             alBuffer3i;
    LPALBUFFERIV             alBufferiv;

    LPALGETBUFFERF           alGetBufferf;
    LPALGETBUFFER3F          alGetBuffer3f;
    LPALGETBUFFERFV          alGetBufferfv;
    LPALGETBUFFERI           alGetBufferi;
    LPALGETBUFFER3I          alGetBuffer3i;
    LPALGETBUFFERIV          alGetBufferiv;

    //ALC
    LPALCCREATECONTEXT       alcCreateContext;
    LPALCMAKECONTEXTCURRENT  alcMakeContextCurrent;
    LPALCPROCESSCONTEXT      alcProcessContext;
    LPALCSUSPENDCONTEXT      alcSuspendContext;
    LPALCDESTROYCONTEXT      alcDestroyContext;
    LPALCGETCURRENTCONTEXT   alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE   alcGetContextsDevice;

    LPALCOPENDEVICE          alcOpenDevice;
    LPALCCLOSEDEVICE         alcCloseDevice;

    LPALCGETERROR            alcGetError;

    LPALCISEXTENSIONPRESENT  alcIsExtensionPresent;
    LPALCGETPROCADDRESS      alcGetProcAddress;
    LPALCGETENUMVALUE        alcGetEnumValue;
    LPALCGETSTRING           alcGetString;
    LPALCGETINTEGERV         alcGetIntegerv;
    LPALCCAPTUREOPENDEVICE   alcCaptureOpenDevice;
    LPALCCAPTURECLOSEDEVICE  alcCaptureCloseDevice;
    LPALCCAPTURESTART        alcCaptureStart;
    LPALCCAPTURESTOP         alcCaptureStop;
    LPALCCAPTURESAMPLES      alcCaptureSamples;

    //ALEXT
    //LPALGETBUFFERSAMPLESSOFT alGetBufferSamplesSOFT;

    LPALCLOOPBACKOPENDEVICESOFT alcLoopbackOpenDeviceSOFT;
    LPALCRENDERSAMPLESSOFT      alcRenderSamplesSOFT;

    LPALBUFFERSTORAGESOFT       alBufferStorageSOFT;
    LPALMAPBUFFERSOFT           alMapBufferSOFT;
    LPALUNMAPBUFFERSOFT         alUnmapBufferSOFT;
};

void AL_init(soft_oalAPI& oal, oal_loader load)
{
    oal.alDopplerFactor        = (LPALDOPPLERFACTOR)load("alDopplerFactor");
    oal.alDopplerVelocity      = (LPALDOPPLERVELOCITY)load("alDopplerVelocity");
    oal.alSpeedOfSound         = (LPALSPEEDOFSOUND)load("alSpeedOfSound");

    oal.alDistanceModel        = (LPALDISTANCEMODEL)load("alDistanceModel");

    oal.alEnable               = (LPALENABLE)load("alEnable");
    oal.alDisable              = (LPALDISABLE)load("alDisable");
    oal.alIsEnabled            = (LPALISENABLED)load("alIsEnabled");

    oal.alGetString            = (LPALGETSTRING)load("alGetString");
    oal.alGetBooleanv          = (LPALGETBOOLEANV)load("alGetBooleanv");
    oal.alGetIntegerv          = (LPALGETINTEGERV)load("alGetIntegerv");
    oal.alGetFloatv            = (LPALGETFLOATV)load("alGetFloatv");
    oal.alGetDoublev           = (LPALGETDOUBLEV)load("alGetDoublev");
    oal.alGetBoolean           = (LPALGETBOOLEAN)load("alGetBoolean");
    oal.alGetInteger           = (LPALGETINTEGER)load("alGetInteger");
    oal.alGetFloat             = (LPALGETFLOAT)load("alGetFloat");
    oal.alGetDouble            = (LPALGETDOUBLE)load("alGetDouble");

    oal.alGetError             = (LPALGETERROR)load("alGetError");

    oal.alIsExtensionPresent   = (LPALISEXTENSIONPRESENT)load("alIsExtensionPresent");
    oal.alGetProcAddress       = (LPALGETPROCADDRESS)load("alGetProcAddress");
    oal.alGetEnumValue         = (LPALGETENUMVALUE)load("alGetEnumValue");

    oal.alListenerf            = (LPALLISTENERF)load("alListenerf");
    oal.alListener3f           = (LPALLISTENER3F)load("alListener3f");
    oal.alListenerfv           = (LPALLISTENERFV)load("alListenerfv");
    oal.alListeneri            = (LPALLISTENERI)load("alListeneri");
    oal.alListener3i           = (LPALLISTENER3I)load("alListener3i");
    oal.alListeneriv           = (LPALLISTENERIV)load("alListeneriv");
    oal.alGetListenerf         = (LPALGETLISTENERF)load("alGetListenerf");
    oal.alGetListener3f        = (LPALGETLISTENER3F)load("alGetListener3f");
    oal.alGetListenerfv        = (LPALGETLISTENERFV)load("alGetListenerfv");
    oal.alGetListeneri         = (LPALGETLISTENERI)load("alGetListeneri");
    oal.alGetListener3i        = (LPALGETLISTENER3I)load("alGetListener3i");
    oal.alGetListeneriv        = (LPALGETLISTENERIV)load("alGetListeneriv");

    oal.alGenSources           = (LPALGENSOURCES)load("alGenSources");
    oal.alDeleteSources        = (LPALDELETESOURCES)load("alDeleteSources");
    oal.alIsSource             = (LPALISSOURCE)load("alIsSource");
    oal.alSourcef              = (LPALSOURCEF)load("alSourcef");
    oal.alSource3f             = (LPALSOURCE3F)load("alSource3f");
    oal.alSourcefv             = (LPALSOURCEFV)load("alSourcefv");
    oal.alSourcei              = (LPALSOURCEI)load("alSourcei");
    oal.alSource3i             = (LPALSOURCE3I)load("alSource3i");
    oal.alSourceiv             = (LPALSOURCEIV)load("alSourceiv");
    oal.alGetSourcef           = (LPALGETSOURCEF)load("alGetSourcef");
    oal.alGetSource3f          = (LPALGETSOURCE3F)load("alGetSource3f");
    oal.alGetSourcefv          = (LPALGETSOURCEFV)load("alGetSourcefv");
    oal.alGetSourcei           = (LPALGETSOURCEI)load("alGetSourcei");
    oal.alGetSource3i          = (LPALGETSOURCE3I)load("alGetSource3i");
    oal.alGetSourceiv          = (LPALGETSOURCEIV)load("alGetSourceiv");
    oal.alSourcePlayv          = (LPALSOURCEPLAYV)load("alSourcePlayv");
    oal.alSourceStopv          = (LPALSOURCESTOPV)load("alSourceStopv");
    oal.alSourceRewindv        = (LPALSOURCEREWINDV)load("alSourceRewindv");
    oal.alSourcePausev         = (LPALSOURCEPAUSEV)load("alSourcePausev");
    oal.alSourcePlay           = (LPALSOURCEPLAY)load("alSourcePlay");
    oal.alSourceStop           = (LPALSOURCESTOP)load("alSourceStop");
    oal.alSourceRewind         = (LPALSOURCEREWIND)load("alSourceRewind");
    oal.alSourcePause          = (LPALSOURCEPAUSE)load("alSourcePause");
    oal.alSourceQueueBuffers   = (LPALSOURCEQUEUEBUFFERS)load("alSourceQueueBuffers");
    oal.alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS)load("alSourceUnqueueBuffers");

    oal.alGenBuffers           = (LPALGENBUFFERS)load("alGenBuffers");
    oal.alDeleteBuffers        = (LPALDELETEBUFFERS)load("alDeleteBuffers");
    oal.alIsBuffer             = (LPALISBUFFER)load("alIsBuffer");
    oal.alBufferData           = (LPALBUFFERDATA)load("alBufferData");
    oal.alBufferf              = (LPALBUFFERF)load("alBufferf");
    oal.alBuffer3f             = (LPALBUFFER3F)load("alBuffer3f");
    oal.alBufferfv             = (LPALBUFFERFV)load("alBufferfv");
    oal.alBufferi              = (LPALBUFFERI)load("alBufferi");
    oal.alBuffer3i             = (LPALBUFFER3I)load("alBuffer3i");
    oal.alBufferiv             = (LPALBUFFERIV)load("alBufferiv");
    oal.alGetBufferf           = (LPALGETBUFFERF)load("alGetBufferf");
    oal.alGetBuffer3f          = (LPALGETBUFFER3F)load("alGetBuffer3f");
    oal.alGetBufferfv          = (LPALGETBUFFERFV)load("alGetBufferfv");
    oal.alGetBufferi           = (LPALGETBUFFERI)load("alGetBufferi");
    oal.alGetBuffer3i          = (LPALGETBUFFER3I)load("alGetBuffer3i");
    oal.alGetBufferiv          = (LPALGETBUFFERIV)load("alGetBufferiv");
}

void ALC_init(soft_oalAPI& oal, oal_loader load)
{
    oal.alcCreateContext      = (LPALCCREATECONTEXT)load("alcCreateContext");
    oal.alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT)load("alcMakeContextCurrent");
    oal.alcProcessContext     = (LPALCPROCESSCONTEXT)load("alcProcessContext");
    oal.alcSuspendContext     = (LPALCSUSPENDCONTEXT)load("alcSuspendContext");
    oal.alcDestroyContext     = (LPALCDESTROYCONTEXT)load("alcDestroyContext");
    oal.alcGetCurrentContext  = (LPALCGETCURRENTCONTEXT)load("alcGetCurrentContext");
    oal.alcGetContextsDevice  = (LPALCGETCONTEXTSDEVICE)load("alcGetContextsDevice");

    oal.alcOpenDevice         = (LPALCOPENDEVICE)load("alcOpenDevice");
    oal.alcCloseDevice        = (LPALCCLOSEDEVICE)load("alcCloseDevice");

    oal.alcGetError           = (LPALCGETERROR)load("alcGetError");

    oal.alcIsExtensionPresent = (LPALCISEXTENSIONPRESENT)load("alcIsExtensionPresent");
    oal.alcGetProcAddress     = (LPALCGETPROCADDRESS)load("alcGetProcAddress");
    oal.alcGetEnumValue       = (LPALCGETENUMVALUE)load("alcGetEnumValue");

    oal.alcGetString          = (LPALCGETSTRING)load("alcGetString");
    oal.alcGetIntegerv        = (LPALCGETINTEGERV)load("alcGetIntegerv");

    oal.alcCaptureOpenDevice  = (LPALCCAPTUREOPENDEVICE)load("alcCaptureOpenDevice");
    oal.alcCaptureCloseDevice = (LPALCCAPTURECLOSEDEVICE)load("alcCaptureCloseDevice");
    oal.alcCaptureStart       = (LPALCCAPTURESTART)load("alcCaptureStart");
    oal.alcCaptureStop        = (LPALCCAPTURESTOP)load("alcCaptureStop");
    oal.alcCaptureSamples     = (LPALCCAPTURESAMPLES)load("alcCaptureSamples");
}

void ALEXT_init(soft_oalAPI& oal, oal_loader load)
{
    //oal.alGetBufferSamplesSOFT    = (LPALGETBUFFERSAMPLESSOFT)load("alGetBufferSamplesSOFT");
    oal.alcLoopbackOpenDeviceSOFT   = (LPALCLOOPBACKOPENDEVICESOFT)load("alcLoopbackOpenDeviceSOFT");
    oal.alcRenderSamplesSOFT        = (LPALCRENDERSAMPLESSOFT)load("alcRenderSamplesSOFT");

           ;
    oal.alBufferStorageSOFT         = (LPALBUFFERSTORAGESOFT)load("alBufferStorageSOFT");
    oal.alMapBufferSOFT             = (LPALMAPBUFFERSOFT)load("alMapBufferSOFT");
    oal.alUnmapBufferSOFT           = (LPALUNMAPBUFFERSOFT)load("alUnmapBufferSOFT");
}

};

CGL_PUBLIC_DECLARE soft_oalAPI soft_oal = cgl::single<soft_oalAPI>::value;

#define soft_oal_exec(func) if(soft_oal.func)return soft_oal.func

extern "C"{

//
// AL
//

void AL_APIENTRY alDopplerFactor(ALfloat value)
{
    soft_oal_exec(alDopplerFactor)(value);
}

void AL_APIENTRY alDopplerVelocity(ALfloat value)
{
    soft_oal_exec(alDopplerVelocity)(value);
}

void AL_APIENTRY alSpeedOfSound(ALfloat value)
{
    soft_oal_exec(alSpeedOfSound)(value);
}


void AL_APIENTRY alDistanceModel(ALenum distanceModel)
{
    soft_oal_exec(alDistanceModel)(distanceModel);
}


void AL_APIENTRY alEnable(ALenum capability)
{
    soft_oal_exec(alEnable)(capability);
}

void AL_APIENTRY alDisable(ALenum capability)
{
    soft_oal_exec(alDisable)(capability);
}

ALboolean AL_APIENTRY alIsEnabled(ALenum capability)
{
    soft_oal_exec(alIsEnabled)(capability);
    return AL_FALSE;
}


/** State retrieval. */
const ALchar* AL_APIENTRY alGetString(ALenum param)
{
    soft_oal_exec(alGetString)(param);
    return "";
}

void AL_APIENTRY alGetBooleanv(ALenum param, ALboolean *values)
{
    soft_oal_exec(alGetBooleanv)(param, values);
}

void AL_APIENTRY alGetIntegerv(ALenum param, ALint *values)
{
    soft_oal_exec(alGetIntegerv)(param, values);
}

void AL_APIENTRY alGetFloatv(ALenum param, ALfloat *values)
{
    soft_oal_exec(alGetFloatv)(param, values);
}

void AL_APIENTRY alGetDoublev(ALenum param, ALdouble *values)
{
    soft_oal_exec(alGetDoublev)(param, values);
}

ALboolean AL_APIENTRY alGetBoolean(ALenum param)
{
    soft_oal_exec(alGetBoolean)(param);
    return AL_FALSE;
}

ALint AL_APIENTRY alGetInteger(ALenum param)
{
    soft_oal_exec(alGetInteger)(param);
    return 0;
}

ALfloat AL_APIENTRY alGetFloat(ALenum param)
{
    soft_oal_exec(alGetFloat)(param);
    return 0.0f;
}

ALdouble AL_APIENTRY alGetDouble(ALenum param)
{
    soft_oal_exec(alGetDouble)(param);
    return 0.0;
}

ALenum AL_APIENTRY alGetError(void)
{
    soft_oal_exec(alGetError)();
    return 0;
}


ALboolean AL_APIENTRY alIsExtensionPresent(const ALchar *extname)
{
    soft_oal_exec(alIsExtensionPresent)(extname);
    return AL_FALSE;
}

void* AL_APIENTRY alGetProcAddress(const ALchar *fname)
{
    soft_oal_exec(alGetProcAddress)(fname);
    return NULL;
}

ALenum AL_APIENTRY alGetEnumValue(const ALchar *ename)
{
    soft_oal_exec(alGetEnumValue)(ename);
    return 0;
}

/** Set Listener parameters */
void AL_APIENTRY alListenerf(ALenum param, ALfloat value)
{
    soft_oal_exec(alListenerf)(param, value);
}

void AL_APIENTRY alListener3f(ALenum param, ALfloat value1, ALfloat value2, ALfloat value3)
{
    soft_oal_exec(alListener3f)(param, value1, value2, value3);
}

void AL_APIENTRY alListenerfv(ALenum param, const ALfloat *values)
{
    soft_oal_exec(alListenerfv)(param, values);
}

void AL_APIENTRY alListeneri(ALenum param, ALint value)
{
    soft_oal_exec(alListeneri)(param, value);
}

void AL_APIENTRY alListener3i(ALenum param, ALint value1, ALint value2, ALint value3)
{
    soft_oal_exec(alListener3i)(param, value1, value2, value3);
}

void AL_APIENTRY alListeneriv(ALenum param, const ALint *values)
{
    soft_oal_exec(alListeneriv)(param, values);
}


/** Get Listener parameters */
void AL_APIENTRY alGetListenerf(ALenum param, ALfloat *value)
{
    soft_oal_exec(alGetListenerf)(param, value);
}

void AL_APIENTRY alGetListener3f(ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
    soft_oal_exec(alGetListener3f)(param, value1, value2, value3);
}

void AL_APIENTRY alGetListenerfv(ALenum param, ALfloat *values)
{
    soft_oal_exec(alGetListenerfv)(param, values);
}

void AL_APIENTRY alGetListeneri(ALenum param, ALint *value)
{
    soft_oal_exec(alGetListeneri)(param, value);
}

void AL_APIENTRY alGetListener3i(ALenum param, ALint *value1, ALint *value2, ALint *value3)
{
    soft_oal_exec(alGetListener3i)(param, value1, value2, value3);
}

void AL_APIENTRY alGetListeneriv(ALenum param, ALint *values)
{
    soft_oal_exec(alGetListeneriv)(param, values);
}



/** Create Source objects. */
void AL_APIENTRY alGenSources(ALsizei n, ALuint *sources)
{
    soft_oal_exec(alGenSources)(n, sources);
}

/** Delete Source objects. */
void AL_APIENTRY alDeleteSources(ALsizei n, const ALuint *sources)
{
    soft_oal_exec(alDeleteSources)(n, sources);
}

/** Verify a handle is a valid Source. */
ALboolean AL_APIENTRY alIsSource(ALuint source)
{
    soft_oal_exec(alIsSource)(source);
    return AL_FALSE;
}


/** Set Source parameters. */
void AL_APIENTRY alSourcef(ALuint source, ALenum param, ALfloat value)
{
    soft_oal_exec(alSourcef)(source, param, value);
}

void AL_APIENTRY alSource3f(ALuint source, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3)
{
    soft_oal_exec(alSource3f)(source, param, value1, value2, value3);
}

void AL_APIENTRY alSourcefv(ALuint source, ALenum param, const ALfloat *values)
{
    soft_oal_exec(alSourcefv)(source, param, values);
}

void AL_APIENTRY alSourcei(ALuint source, ALenum param, ALint value)
{
    soft_oal_exec(alSourcei)(source, param, value);
}

void AL_APIENTRY alSource3i(ALuint source, ALenum param, ALint value1, ALint value2, ALint value3)
{
    soft_oal_exec(alSource3i)(source, param, value1, value2, value3);
}

void AL_APIENTRY alSourceiv(ALuint source, ALenum param, const ALint *values)
{
    soft_oal_exec(alSourceiv)(source, param, values);
}


/** Get Source parameters. */
void AL_APIENTRY alGetSourcef(ALuint source, ALenum param, ALfloat *value)
{
    soft_oal_exec(alGetSourcef)(source, param, value);
}

void AL_APIENTRY alGetSource3f(ALuint source, ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
    soft_oal_exec(alGetSource3f)(source, param, value1, value2, value3);
}

void AL_APIENTRY alGetSourcefv(ALuint source, ALenum param, ALfloat *values)
{
    soft_oal_exec(alGetSourcefv)(source, param, values);
}

void AL_APIENTRY alGetSourcei(ALuint source,  ALenum param, ALint *value)
{
    soft_oal_exec(alGetSourcei)(source, param, value);
}

void AL_APIENTRY alGetSource3i(ALuint source, ALenum param, ALint *value1, ALint *value2, ALint *value3)
{
    soft_oal_exec(alGetSource3i)(source, param, value1, value2, value3);
}

void AL_APIENTRY alGetSourceiv(ALuint source,  ALenum param, ALint *values)
{
    soft_oal_exec(alGetSourceiv)(source, param, values);
}



/** Play, replay, or resume (if paused) a list of Sources */
void AL_APIENTRY alSourcePlayv(ALsizei n, const ALuint *sources)
{
    soft_oal_exec(alSourcePlayv)(n, sources);
}

/** Stop a list of Sources */
void AL_APIENTRY alSourceStopv(ALsizei n, const ALuint *sources)
{
    soft_oal_exec(alSourceStopv)(n, sources);
}

/** Rewind a list of Sources */
void AL_APIENTRY alSourceRewindv(ALsizei n, const ALuint *sources)
{
    soft_oal_exec(alSourceRewindv)(n, sources);
}

/** Pause a list of Sources */
void AL_APIENTRY alSourcePausev(ALsizei n, const ALuint *sources)
{
    soft_oal_exec(alSourcePausev)(n, sources);
}


/** Play, replay, or resume a Source */
void AL_APIENTRY alSourcePlay(ALuint source)
{
    soft_oal_exec(alSourcePlay)(source);
}

/** Stop a Source */
void AL_APIENTRY alSourceStop(ALuint source)
{
    soft_oal_exec(alSourceStop)(source);
}

/** Rewind a Source (set playback postiton to beginning) */
void AL_APIENTRY alSourceRewind(ALuint source)
{
    soft_oal_exec(alSourceRewind)(source);
}

/** Pause a Source */
void AL_APIENTRY alSourcePause(ALuint source)
{
    soft_oal_exec(alSourcePause)(source);
}


/** Queue buffers onto a source */
void AL_APIENTRY alSourceQueueBuffers(ALuint source, ALsizei nb, const ALuint *buffers)
{
    soft_oal_exec(alSourceQueueBuffers)(source, nb, buffers);
}

/** Unqueue processed buffers from a source */
void AL_APIENTRY alSourceUnqueueBuffers(ALuint source, ALsizei nb, ALuint *buffers)
{
    soft_oal_exec(alSourceUnqueueBuffers)(source, nb, buffers);
}

/** Create Buffer objects */
void AL_APIENTRY alGenBuffers(ALsizei n, ALuint *buffers)
{
    soft_oal_exec(alGenBuffers)(n, buffers);
}

/** Delete Buffer objects */
void AL_APIENTRY alDeleteBuffers(ALsizei n, const ALuint *buffers)
{
    soft_oal_exec(alDeleteBuffers)(n, buffers);
}

/** Verify a handle is a valid Buffer */
ALboolean AL_APIENTRY alIsBuffer(ALuint buffer)
{
    soft_oal_exec(alIsBuffer)(buffer);
    return AL_FALSE;
}


/** Specifies the data to be copied into a buffer */
void AL_APIENTRY alBufferData(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
{
    soft_oal_exec(alBufferData)(buffer, format, data, size, freq);
}


/** Set Buffer parameters, */
void AL_APIENTRY alBufferf(ALuint buffer, ALenum param, ALfloat value)
{
    soft_oal_exec(alBufferf)(buffer, param, value);
}

void AL_APIENTRY alBuffer3f(ALuint buffer, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3)
{
    soft_oal_exec(alBuffer3f)(buffer, param, value1, value2, value3);
}

void AL_APIENTRY alBufferfv(ALuint buffer, ALenum param, const ALfloat *values)
{
    soft_oal_exec(alBufferfv)(buffer, param, values);
}

void AL_APIENTRY alBufferi(ALuint buffer, ALenum param, ALint value)
{
    soft_oal_exec(alBufferi)(buffer, param, value);
}

void AL_APIENTRY alBuffer3i(ALuint buffer, ALenum param, ALint value1, ALint value2, ALint value3)
{
    soft_oal_exec(alBuffer3i)(buffer, param, value1, value2, value3);
}

void AL_APIENTRY alBufferiv(ALuint buffer, ALenum param, const ALint *values)
{
    soft_oal_exec(alBufferiv)(buffer, param, values);
}


/** Get Buffer parameters. */
void AL_APIENTRY alGetBufferf(ALuint buffer, ALenum param, ALfloat *value)
{
    soft_oal_exec(alGetBufferf)(buffer, param, value);
}

void AL_APIENTRY alGetBuffer3f(ALuint buffer, ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
    soft_oal_exec(alGetBuffer3f)(buffer, param, value1, value2, value3);
}

void AL_APIENTRY alGetBufferfv(ALuint buffer, ALenum param, ALfloat *values)
{
    soft_oal_exec(alGetBufferfv)(buffer, param, values);
}

void AL_APIENTRY alGetBufferi(ALuint buffer, ALenum param, ALint *value)
{
    soft_oal_exec(alGetBufferi)(buffer, param, value);
}

void AL_APIENTRY alGetBuffer3i(ALuint buffer, ALenum param, ALint *value1, ALint *value2, ALint *value3)
{
    soft_oal_exec(alGetBuffer3i)(buffer, param, value1, value2, value3);
}

void AL_APIENTRY alGetBufferiv(ALuint buffer, ALenum param, ALint *values)
{
    soft_oal_exec(alGetBufferiv)(buffer, param, values);
}


//
// ALC
//

/** Context management. */
ALCcontext* ALC_APIENTRY alcCreateContext(ALCdevice *device, const ALCint* attrlist)
{
    soft_oal_exec(alcCreateContext)(device, attrlist);
    return NULL;
}

ALCboolean ALC_APIENTRY alcMakeContextCurrent(ALCcontext *context)
{
    soft_oal_exec(alcMakeContextCurrent)(context);
    return AL_FALSE;
}

void ALC_APIENTRY alcProcessContext(ALCcontext *context)
{
    soft_oal_exec(alcProcessContext)(context);
}

void ALC_APIENTRY alcSuspendContext(ALCcontext *context)
{
    soft_oal_exec(alcSuspendContext)(context);
}

void ALC_APIENTRY alcDestroyContext(ALCcontext *context)
{
    soft_oal_exec(alcDestroyContext)(context);
}

ALCcontext* ALC_APIENTRY alcGetCurrentContext(void)
{
    soft_oal_exec(alcGetCurrentContext)();
    return NULL;
}

ALCdevice* ALC_APIENTRY alcGetContextsDevice(ALCcontext *context)
{
    soft_oal_exec(alcGetContextsDevice)(context);
    return NULL;
}


/** Device management. */
ALCdevice* ALC_APIENTRY alcOpenDevice(const ALCchar *devicename)
{
    soft_oal_exec(alcOpenDevice)(devicename);
    return NULL;
}

ALCboolean ALC_APIENTRY alcCloseDevice(ALCdevice *device)
{
    soft_oal_exec(alcCloseDevice)(device);
    return AL_FALSE;
}

/**
 * Error support.
 *
 * Obtain the most recent Device error.
 */
ALCenum ALC_APIENTRY alcGetError(ALCdevice *device)
{
    soft_oal_exec(alcGetError)(device);
    return 0;
}

/**
 * Extension support.
 *
 * Query for the presence of an extension, and obtain any appropriate
 * function pointers and enum values.
 */
ALCboolean ALC_APIENTRY alcIsExtensionPresent(ALCdevice *device, const ALCchar *extname)
{
    soft_oal_exec(alcIsExtensionPresent)(device, extname);
    return AL_FALSE;
}

void* ALC_APIENTRY alcGetProcAddress(ALCdevice *device, const ALCchar *funcname)
{
    soft_oal_exec(alcGetProcAddress)(device, funcname);
    return NULL;
}

ALCenum ALC_APIENTRY alcGetEnumValue(ALCdevice *device, const ALCchar *enumname)
{
    soft_oal_exec(alcGetEnumValue)(device, enumname);
    return 0;
}

/** Query function. */
const ALCchar* ALC_APIENTRY alcGetString(ALCdevice *device, ALCenum param)
{
    soft_oal_exec(alcGetString)(device, param);
    return "";
}

void ALC_APIENTRY alcGetIntegerv(ALCdevice *device, ALCenum param, ALCsizei size, ALCint *values)
{
    soft_oal_exec(alcGetIntegerv)(device, param, size, values);
}

/** Capture function. */
ALCdevice* ALC_APIENTRY alcCaptureOpenDevice(const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize)
{
    soft_oal_exec(alcCaptureOpenDevice)(devicename, frequency, format, buffersize);
    return NULL;
}

ALCboolean ALC_APIENTRY alcCaptureCloseDevice(ALCdevice *device)
{
    soft_oal_exec(alcCaptureCloseDevice)(device);
    return AL_FALSE;
}

void ALC_APIENTRY alcCaptureStart(ALCdevice *device)
{
    soft_oal_exec(alcCaptureStart)(device);
}

void ALC_APIENTRY alcCaptureStop(ALCdevice *device)
{
    soft_oal_exec(alcCaptureStop)(device);
}

void ALC_APIENTRY alcCaptureSamples(ALCdevice *device, ALCvoid *buffer, ALCsizei samples)
{
    soft_oal_exec(alcCaptureSamples)(device, buffer, samples);
}

//
// ALEXT
//

//AL_API void AL_APIENTRY alGetBufferSamplesSOFT(ALuint buffer, ALsizei offset, ALsizei samples, ALenum channels, ALenum type, ALvoid *data)
//{
//    soft_oal_exec(alGetBufferSamplesSOFT)(buffer, offset, samples, channels, type, data);
//}

ALCdevice* ALC_APIENTRY alcLoopbackOpenDeviceSOFT(const ALCchar *deviceName)
{
    soft_oal_exec(alcLoopbackOpenDeviceSOFT)(deviceName);
    return NULL;
}

void ALC_APIENTRY alcRenderSamplesSOFT(ALCdevice *device, ALCvoid *buffer, ALCsizei samples)
{
    soft_oal_exec(alcRenderSamplesSOFT)(device, buffer, samples);
}

void AL_APIENTRY alBufferStorageSOFT(ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq, ALbitfieldSOFT flags)
{
    soft_oal_exec(alBufferStorageSOFT)(buffer, format, data, size, freq, flags);
}

void* AL_APIENTRY alMapBufferSOFT(ALuint buffer, ALsizei offset, ALsizei length, ALbitfieldSOFT access)
{
    soft_oal_exec(alMapBufferSOFT)(buffer, offset, length, access);
    return NULL;
}

void AL_APIENTRY alUnmapBufferSOFT(ALuint buffer)
{
    soft_oal_exec(alUnmapBufferSOFT)(buffer);
}

}//end extern "C"

#undef soft_oal_exec

#endif //SOFT_OAL_HPP_20200210042039
