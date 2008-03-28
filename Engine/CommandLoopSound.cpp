#include "CommandLoopSound.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandLoopSound::CommandLoopSound(const std::string& path, const bool Looping)
  {
    m_Path = path;
    m_DoLoop = Looping;
  }

  //destructor
  CommandLoopSound::~CommandLoopSound()
  {

  }

  bool CommandLoopSound::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().Loop(m_Path, m_DoLoop);

    //should return true; or return false; here
  }
}
