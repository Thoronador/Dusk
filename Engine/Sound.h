#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#if defined(_WIN32)
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif
#include "openal/al.h" //OpenAL header
#include "openal/alc.h"  //OpenAL header
#include "oggvorbis/vorbisfile.h" //Vorbis header

//Types for wave format:
typedef struct riff_chunk
{
  char Riff[4];
  unsigned long len; //32 bit unsigned int; file size-8
  char Wave[4];
};
typedef riff_chunk TRiffChunk;
typedef struct fmt_chunk
{
  char fmt_[4];
  unsigned long chunk_size;//32 bit unsigned int;
  unsigned short FormatTag;//16 bit unsigned int;
  unsigned short Channels;
  unsigned long SamplesPerSecond;
  unsigned long BytesPerSecond;
  unsigned short BlockAlign;
  unsigned short BitsPerSample;
};
typedef fmt_chunk TFmtChunk;
typedef struct data_chunk
{
  char data[4];
  unsigned long length_of_data;
};
typedef data_chunk TDataChunk;

//buffer management type
typedef struct media_rec
{
  std::string MediaName; //unique name, case sensitive
  std::string FileName; //pro forma, not really needed after file is loaded
  ALuint num_buffers;
  ALuint * buffers;
  std::vector<std::string> attached_to;
  media_rec * next;
};
typedef media_rec TMediaRec;

//source management type
typedef struct noise_rec
{
  std::string NoiseName; //unique name, case sensitive
  ALuint sourceID;
  TMediaRec * attachedMedia;
  noise_rec * next;
};
typedef noise_rec TNoiseRec;

//type declaration for OggVorbis function pointers
typedef int (*P_ov_clear)(OggVorbis_File *vf);
typedef vorbis_comment* (*P_ov_comment)(OggVorbis_File *vf,int link);
//typedef int (*P_ov_fopen)(char *path,OggVorbis_File *vf);
typedef vorbis_info* (*P_ov_info)(OggVorbis_File *vf,int link);
typedef int (*P_ov_open)(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);
typedef int (*P_ov_open_callbacks)(void *datasource, OggVorbis_File *vf, char *initial, long ibytes, ov_callbacks callbacks);
typedef ogg_int64_t (*P_ov_pcm_total)(OggVorbis_File *vf,int i);
typedef long (*P_ov_read)(OggVorbis_File *vf, char *buffer, int length, int bigendianp, int word, int sgned, int *bitstream);
typedef int (*P_ov_test)(FILE *f,OggVorbis_File *vf,char *initial,long ibytes);


//Klasse Sound
class Sound
{
  public:
    virtual ~Sound();
    //(de-)initialization routines
    bool Init(std::string PathToLib_AL = "NULL", std::string PathToLib_Vorbisfile = "NULL", bool needVorbis = false);//initializes OpenAL
    bool Exit();//deinitializes OpenAL

    // **presence checks**
    bool IsMediaPresent(const std::string MediaIdentifier) const;
    bool IsNoisePresent(const std::string NoiseIdentifier) const;

    // **noise management routines**
    bool CreateNoise(const std::string NoiseIdentifier);
    bool DestroyNoise(const std::string NoiseIdentifier);
    //   Attach: associates existing Noise with a Media
    bool Attach(const std::string NoiseIdentifier, const std::string MediaIdentifier);
    //   Detach: revokes association between Noise and its attached Media
    bool Detach(const std::string NoiseIdentifier);
    //   noise playback management
    bool PlayNoise(const std::string NoiseIdentifier);
    bool PauseNoise(const std::string NoiseIdentifier);
    bool UnPauseNoise(const std::string NoiseIdentifier);
    bool StopNoise(const std::string NoiseIdentifier);
    bool LoopNoise(const std::string NoiseIdentifier, const bool DoLoop = true);
    //   state retrieval functions
    bool IsPlayingNoise(const std::string NoiseIdentifier) const;
    bool IsLoopingNoise(const std::string FileName) const;
    //   noise volume functions
    bool SetNoiseVolume(const std::string NoiseIdentifier, const float volume = 1.0f);
    float GetNoiseVolume(const std::string NoiseIdentifier, const bool consider_MinMax = false) const;

    // **media management routines**
    bool CreateMedia(const std::string MediaIdentifier, const std::string PathToMedia);
    bool DestroyMedia(const std::string MediaIdentifier);

    // **general state query/set functions**
    float GetSpeedOfSound() const;
    bool SetSpeedOfSound(const float new_value);

    // **listener management functions**
    //   positioning
    bool SetListenerPostion(const float x, const float y, const float z);
    std::vector<float> GetListenerPosition() const;
    bool ListenerTranslatePostion(const float delta_x, const float delta_y, const float delta_z);
    //   velocity
    bool SetListenerVelocity(const float x, const float y, const float z);
    std::vector<float> GetListenerVelocity() const;
    //   orientation of listener
    std::vector<float> GetListenerOrientation() const;
    bool ListenerRotate(const float x_axis, const float y_axis=0.0f, const float z_axis=0.0f);





    //old or unimplemented/ non-working routines
    //routines for managing noise state
    bool ReplayNoise(const std::string NoiseIdentifier);
    //source postioning
    bool SetSoundPosition(const std::string FileName, const float x, const float y, const float z);
    std::vector<float> GetSoundPosition(const std::string FileName) const;
    //source velocity
    bool SetSoundVelocity(const std::string FileName, const float x, const float y, const float z);
    std::vector<float> GetSoundVelocity(const std::string FileName) const;
    //file management
    bool FreeFileResources(std::string FileName);//should possibly be private?
    std::vector<std::string> GetBufferedMedia() const;
    std::vector<std::string> GetNoiseList() const;
    static Sound& get();
  protected:

  private:
    Sound();
    Sound(const Sound& op){}
    //bool PlayWAV(std::string WAV_FileName);
    //bool PlayOgg(std::string Ogg_FileName);
    bool CreateWAVMedia(const std::string MediaIdentifier, const std::string PathToMedia);
    bool CreateOggMedia(const std::string MediaIdentifier, const std::string PathToMedia);
    void AllFuncPointersToNULL(void); //never ever call this one manually,
    // or you might render the whole class inoperative!

    TMediaRec * pMediaList;
    TNoiseRec * pNoiseList;

    ALCdevice *pDevice;
    ALCcontext *pContext;
    bool AL_Ready;
    bool Vorbis_Ready;
    bool InitInProgress;

    #if defined(_WIN32)
      HINSTANCE libHandleAL; //handle to OpenAL dynamic library
      HINSTANCE libHandleOV; //handle to OggVorbis dynamic library
    #else
      void * libHandleAL; //handle to OpenAL dynamic library
      void * libHandleOV; //handle to OggVorbis dynamic library
    #endif
    //****
    //**OpenAL function pointers
    //**** alc.h: AL context functions
    //**** Context Management
    LPALCCREATECONTEXT alcCreateContext;
    LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
    LPALCPROCESSCONTEXT alcProcessContext;
    LPALCSUSPENDCONTEXT alcSuspendContext;
    LPALCDESTROYCONTEXT alcDestroyContext;
    LPALCGETCURRENTCONTEXT alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE alcGetContextsDevice;

    //**** Device Management
    LPALCOPENDEVICE alcOpenDevice;
    LPALCCLOSEDEVICE alcCloseDevice;

    //**** Error handling
    LPALCGETERROR alcGetError;

    //**** Extension handling functions (ALC)
    /* Disabled for now
    LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
    LPALCGETPROCADDRESS alcGetProcAddress;
    LPALCGETENUMVALUE alcGetEnumValue;
    */

    //**** Query functions
    LPALCGETSTRING alcGetString;
    LPALCGETINTEGERV alcGetIntegerv;

    //****al.h: AL functions
    //**** Renderer State management
    LPALENABLE alEnable;
    LPALDISABLE alDisable;
    LPALISENABLED alIsEnabled;

    //**** State retrieval
    LPALGETSTRING alGetString;
    LPALGETBOOLEANV alGetBooleanv;
    LPALGETINTEGERV alGetIntegerv;
    LPALGETFLOATV alGetFloatv;
    LPALGETDOUBLEV alGetDoublev;
    LPALGETBOOLEAN alGetBoolean;
    LPALGETINTEGER alGetInteger;
    LPALGETFLOAT alGetFloat;
    LPALGETDOUBLE alGetDouble;

    //***** Error handling
    LPALGETERROR alGetError;

    //**** Extension handling func (AL)
    LPALISEXTENSIONPRESENT alIsExtensionPresent;
    LPALGETPROCADDRESS alGetProcAddress;
    LPALGETENUMVALUE alGetEnumValue;

    //**** Set Listener parameters
    LPALLISTENERF alListenerf;
    LPALLISTENER3F alListener3f;
    LPALLISTENERFV alListenerfv;
    LPALLISTENERI alListeneri;
    LPALLISTENER3I alListener3i;
    LPALLISTENERIV alListeneriv;

    //**** Get Listener parameters
    LPALGETLISTENERF alGetListenerf;
    LPALGETLISTENER3F alGetListener3f;
    LPALGETLISTENERFV alGetListenerfv;
    LPALGETLISTENERI alGetListeneri;
    LPALGETLISTENER3I alGetListener3i;
    LPALGETLISTENERIV alGetListeneriv;

    //**** Create Source objects
    LPALGENSOURCES alGenSources;
    //**** Delete Source objects
    LPALDELETESOURCES alDeleteSources;
    //***Verify a handle is a valid source
    LPALISSOURCE alIsSource;

    //**** Set Source parameters
    LPALSOURCEF alSourcef;
    LPALSOURCE3F alSource3f;
    LPALSOURCEFV alSourcefv;
    LPALSOURCEI alSourcei;
    LPALSOURCE3I alSource3i;
    LPALSOURCEIV alSourceiv;

    //**** Get Source parameters
    LPALGETSOURCEF alGetSourcef;
    LPALGETSOURCE3F alGetSource3f;
    LPALGETSOURCEFV alGetSourcefv;
    LPALGETSOURCEI alGetSourcei;
    LPALGETSOURCE3I alGetSource3i;
    LPALGETSOURCEIV alGetSourceiv;

    //**** Source vector based playback calls
    LPALSOURCEPLAYV alSourcePlayv; // Play, replay, or resume (if paused) a list of Sources
    LPALSOURCESTOPV alSourceStopv; // Stop a list of Sources
    LPALSOURCEREWINDV alSourceRewindv; // Rewind a list of Sources
    LPALSOURCEPAUSEV alSourcePausev; // Pause a list of Sources

    //**** Source based playback calls
    LPALSOURCEPLAY alSourcePlay; // Play, replay, or resume a Source
    LPALSOURCESTOP alSourceStop; // Stop a Source
    LPALSOURCEREWIND alSourceRewind; // Rewind a Source (set playback postiton to beginning)
    LPALSOURCEPAUSE alSourcePause; // Pause a Source

    //**** Source Queuing
    LPALSOURCEQUEUEBUFFERS alSourceQueueBuffers;
    LPALSOURCEUNQUEUEBUFFERS alSourceUnqueueBuffers;

    //**** Create Buffer objects
    LPALGENBUFFERS alGenBuffers;
    //**** Delete Buffer objects
    LPALDELETEBUFFERS alDeleteBuffers;
    //**** Verify a handle is a valid Buffer
    LPALISBUFFER alIsBuffer;
    //**** Specify the data to be copied into a buffer
    LPALBUFFERDATA alBufferData;

    //**** Set Buffer parameters
    LPALBUFFERF alBufferf;
    LPALBUFFER3F alBuffer3f;
    LPALBUFFERFV alBufferfv;
    LPALBUFFERI alBufferi;
    LPALBUFFER3I alBuffer3i;
    LPALBUFFERIV alBufferiv;

    //**** Get Buffer parameters
    LPALGETBUFFERF alGetBufferf;
    LPALGETBUFFER3F alGetBuffer3f;
    LPALGETBUFFERFV alGetBufferfv;
    LPALGETBUFFERI alGetBufferi;
    LPALGETBUFFER3I alGetBuffer3i;
    LPALGETBUFFERIV alGetBufferiv;

    //**** Global Parameters
    /* Partially disabled for now
    LPALDOPPLERFACTOR alDopplerFactor;
    LPALDOPPLERVELOCITY alDopplerVelocity;*/
    LPALSPEEDOFSOUND alSpeedOfSound;
    /*LPALDISTANCEMODEL alDistanceModel;
    */

    //**** OggVorbis function pointers
    P_ov_clear ov_clear;
    P_ov_comment ov_comment;
    //P_ov_fopen ov_fopen;
    P_ov_info ov_info;
    //better don't use ov_open on Windows
    P_ov_open ov_open;
    P_ov_open_callbacks ov_open_callbacks;
    P_ov_pcm_total ov_pcm_total;
    P_ov_read ov_read;
    P_ov_test ov_test;
};

#endif // SOUND_H
