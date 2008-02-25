#include "CommandStopSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandStopSound::CommandStopSound(const std::string& path)
  {
    m_Path = path;
  }

  //destructor
  CommandStopSound::~CommandStopSound()
  {

  }

  bool CommandStopSound::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().Stop(m_Path);
  }
}
