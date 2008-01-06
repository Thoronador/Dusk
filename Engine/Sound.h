#ifndef SOUND_H
#define SOUND_H

#include <string>
#include "Framework.h"  //OpenAL Framework


class Sound
{
  public:
    Sound();
    virtual ~Sound();
    bool Play(std::string FileName);
  protected:

  private:
    bool PlayWAV(std::string WAV_FileName);
    bool PlayOgg(std::string Ogg_FileName);
    
    ALCDevice *pDevice;
    bool AL_Ready;
};

#endif // SOUND_H
