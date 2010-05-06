#include "CommandLoopSound.h"
#include "Sound.h"

namespace Dusk
{
  //constructor
  CommandLoopSound::CommandLoopSound(const std::string& NoiseID, const bool Looping)
  {
    m_Noise = NoiseID;
    m_DoLoop = Looping;
  }

  //destructor
  CommandLoopSound::~CommandLoopSound()
  {
    //empty
  }

  bool CommandLoopSound::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().LoopNoise(m_Noise, m_DoLoop);
  }
}
