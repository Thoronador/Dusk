#include "Sound.h"

//constructor
Sound::Sound()
{
  pDevice = NULL;
  AL_Ready = false;
}

//destructor
Sound::~Sound()
{

}

bool Sound::Play(std::string FileName)
{
  if (FileName.substr(FileName.length()-4)==".wav")
  {
    return PlayWAV(FileName);
  }
  else if (FileName.substr(FileName.length()-4)==".ogg")
  {
    return PlayOgg(FileName);
  }
  else return false;
}

bool Sound::PlayWAV(std::string WAV_FileName)
{

}

bool Sound::PlayOgg(std::string Ogg_FileName)
{

}
