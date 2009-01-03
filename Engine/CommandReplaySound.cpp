#include "CommandReplaySound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandReplaySound::CommandReplaySound(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandReplaySound::~CommandReplaySound()
  {

  }

  bool CommandReplaySound::execute(Dusk::Scene* scene, int count)
  {
    //New version of Sound has no Replay method yet. Instead we are using a
    // combination of Stop and Play to simulate the former Replay method.
    if (Sound::get().StopNoise(m_Noise))
    {
      return Sound::get().PlayNoise(m_Noise);
    }
    return false;
  }
}
