#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "openal/al.h" //OpenAL header
#include "openal/alc.h"  //OpenAL header


class Sound
{
  public:
    Sound();
    virtual ~Sound();
    bool Init();//initializes OpenAL
    bool Exit();//deinitializes OpenAL
    bool Play(std::string FileName);
  protected:

  private:
    bool PlayWAV(std::string WAV_FileName);
    bool PlayOgg(std::string Ogg_FileName);
    
    ALCdevice *pDevice;
    bool AL_Ready;
    bool InitInProgress;
};

#endif // SOUND_H
