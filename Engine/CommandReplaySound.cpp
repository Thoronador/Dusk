#include "CommandReplaySound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandReplaySound::CommandReplaySound(const std::string& path)
  {
    m_Path = path;
  }

  //destructor
  CommandReplaySound::~CommandReplaySound()
  {

  }

  bool CommandReplaySound::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().Replay(m_Path);
  }
}
