#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#if defined(_WIN32)
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif
#include "openal/al.h" //OpenAL header
#include "openal/alc.h"  //OpenAL header
//#include "oggvorbis/vorbisfile.h" //Vorbis header

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

//Type for internal file&buffer/source management
typedef struct bufsrc_rec
{
  std::string FileName;
  ALuint sourceID;
  ALuint num_buffers;
  ALuint * buffers;
  bufsrc_rec * next;
};
typedef bufsrc_rec TBufSrcRecord;

//Klasse Sound
class Sound
{
  public:
    virtual ~Sound();
    bool Init(std::string PathToLib_AL = "NULL", std::string PathToLib_Vorbisfile = "NULL");//initializes OpenAL
    bool Exit();//deinitializes OpenAL
    bool Play(std::string FileName);
    bool IsPlaying(std::string FileName) const;
    bool Pause(std::string FileName);
    bool UnPause(std::string FileName);
    bool Stop(std::string FileName);
    bool Replay(std::string FileName);
    bool FreeFileResources(std::string FileName);//should possibly be private?
    std::vector<std::string> GetBufferedFiles() const;
    static Sound& get();
  protected:

  private:
    Sound();
    Sound(const Sound& op){}
    bool PlayWAV(std::string WAV_FileName);
    bool PlayOgg(std::string Ogg_FileName);
    void AllFuncPointersToNULL(void); //never ever call this one manually!

    TBufSrcRecord * pFileList;//pointer to list

    ALCdevice *pDevice;
    ALCcontext *pContext;
    bool AL_Ready;
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
    /* Disable for now
    LPALISEXTENSIONPRESENT alIsExtensionPresent;
    LPALGETPROCADDRESS alGetProcAddress;
    LPALGETENUMVALUE alGetEnumValue;
    */

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
    /* Disabled for now
    LPALDOPPLERFACTOR alDopplerFactor;
    LPALDOPPLERVELOCITY alDopplerVelocity;
    LPALSPEEDOFSOUND alSpeedOfSound;
    LPALDISTANCEMODEL alDistanceModel;
    */
};

#endif // SOUND_H
