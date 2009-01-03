#include "CommandPauseSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandPauseSound::CommandPauseSound(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandPauseSound::~CommandPauseSound()
  {

  }

  bool CommandPauseSound::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().PauseNoise(m_Noise);
  }
}
