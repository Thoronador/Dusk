#ifndef SOUND_H
#define SOUND_H

#include <string>
#if defined(_WIN32)
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif
#include "openal/al.h" //OpenAL header
#include "openal/alc.h"  //OpenAL header


class Sound
{
  public:
    Sound();
    virtual ~Sound();
    bool Init(std::string PathToLibrary = "NULL");//initializes OpenAL
    bool Exit();//deinitializes OpenAL
    bool Play(std::string FileName);
  protected:

  private:
    bool PlayWAV(std::string WAV_FileName);
    bool PlayOgg(std::string Ogg_FileName);
    
    ALCdevice *pDevice;
    ALCcontext *pContext;
    bool AL_Ready;
    bool InitInProgress;
    
    #if defined(_WIN32)
      HINSTANCE libHandle;
    #else
      void * libHandle;
    #endif
    //****
    //**OpenAL function pointers
    //****
    //****Context Management
    LPALCCREATECONTEXT alcCreateContext;
    LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
    LPALCPROCESSCONTEXT alcProcessContext;
    LPALCSUSPENDCONTEXT alcSuspendContext;
    LPALCDESTROYCONTEXT alcDestroyContext;
    LPALCGETCURRENTCONTEXT alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE alcGetContextsDevice;

    //****Device Management
    LPALCOPENDEVICE alcOpenDevice;
    LPALCCLOSEDEVICE alcCloseDevice;
    
    //****Error handling
    LPALCGETERROR alcGetError;
    
    //****Extension handling functions
    /* Disabled for now
    LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
    LPALCGETPROCADDRESS alcGetProcAddress;
    LPALCGETENUMVALUE alcGetEnumValue;
    */
    
    //****Query functions
    LPALCGETSTRING alcGetString;
    LPALCGETINTEGERV alcGetIntegerv;
};

#endif // SOUND_H
