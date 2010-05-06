#include "CommandSoundVolume.h"
#include "Sound.h"

namespace Dusk
{
  //constructor
  CommandSoundVolume::CommandSoundVolume(const std::string& NoiseID, const float vol)
  {
    m_Noise = NoiseID;
    m_Volume = vol;
  }

  //destructor
  CommandSoundVolume::~CommandSoundVolume()
  {
    //empty
  }

  bool CommandSoundVolume::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().SetNoiseVolume(m_Noise, m_Volume);
  }
}
