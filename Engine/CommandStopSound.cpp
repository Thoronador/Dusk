#include "CommandStopSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandStopSound::CommandStopSound(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandStopSound::~CommandStopSound()
  {

  }

  bool CommandStopSound::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().StopNoise(m_Noise);
  }
}
