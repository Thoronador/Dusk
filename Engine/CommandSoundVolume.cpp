#include "CommandSoundVolume.h"
#include "Sound.h"
namespace Dusk
{
  //constructor
  CommandSoundVolume::CommandSoundVolume(const std::string& path, const float vol)
  {
    m_Path = path;
    m_Volume = vol;
  }

  //destructor
  CommandSoundVolume::~CommandSoundVolume()
  {

  }

  bool CommandSoundVolume::execute(Dusk::Scene* scene, int count)
  {
    Sound::get().SetVolume(m_Path, m_Volume);

    //should return true; or return false; here
  }
}
