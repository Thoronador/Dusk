#include "Sound.h"

//constructor
Sound::Sound()
{
  pDevice = NULL;
  AL_Ready = false;
  InitInProgress = false;
}

//destructor
Sound::~Sound()
{

}

//returns true, if initialization of OpenAL was successful; false otherwise
bool Sound::Init()
{

  return false;
}

//returns true, if deinitialization of OpenAL was successful; false otherwise
bool Sound::Exit()
{

  return false;
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
