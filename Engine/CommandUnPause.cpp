#include "CommandUnPauseSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandUnPauseSound::CommandUnPauseSound(const std::string& path)
  {
    m_Path = path;
  }

  //destructor
  CommandUnPauseSound::~CommandUnPauseSound()
  {

  }

  bool CommandUnPauseSound::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().UnPause(m_Path);
  }
}
