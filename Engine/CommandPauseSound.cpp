#include "CommandPauseSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandPauseSound::CommandPauseSound(const std::string& path)
  {
    m_Path = path;
  }

  //destructor
  CommandPauseSound::~CommandPauseSound()
  {

  }

  bool CommandPauseSound::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().Pause(m_Path);
  }
}
