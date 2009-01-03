#include "CommandPlaySound.h"
#include "Sound.h"
namespace Dusk
{
    CommandPlaySound::CommandPlaySound(const std::string& NoiseID, const SoundOpCode op_code)
    {
      //ctor
      m_Noise = NoiseID;
      m_operation = op_code;
    }

    CommandPlaySound::~CommandPlaySound()
    {
      //dtor
    }

    bool CommandPlaySound::execute(Dusk::Scene* scene, int count)
    {
      switch(m_operation)
      {
        case sopPlay:
          return Sound::get().PlayNoise(m_Noise); break;
        case sopPause:
          return Sound::get().PauseNoise(m_Noise); break;
        case sopUnPause:
          return Sound::get().UnPauseNoise(m_Noise); break;
        case sopStop:
          return Sound::get().StopNoise(m_Noise); break;
        case sopReplay:
          //New version of Sound has no Replay method yet. Instead we are using
          //a combination of Stop and Play to simulate the former Replay method.
          if (Sound::get().StopNoise(m_Noise))
          {
            return Sound::get().PlayNoise(m_Noise);
          }
          return false; break;
      }//switch
      std::cout << "CommandPlaySound::execute: ERROR: unknown op code parameter.\n";
      return false;
    }
}

