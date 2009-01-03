#include "CommandUnPauseSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandUnPauseSound::CommandUnPauseSound(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandUnPauseSound::~CommandUnPauseSound()
  {

  }

  bool CommandUnPauseSound::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().UnPauseNoise(m_Noise);
  }
}
