#include "CommandMediaSound.h"
#include "Sound.h"
namespace Dusk
{
  //media creation command
  //constructor
  CommandCreateMedia::CommandCreateMedia(const std::string& MediaID, const std::string& MediaPath)
  {
    m_Media = MediaID;
    m_MediaPath = MediaPath;
  }

  //destructor
  CommandCreateMedia::~CommandCreateMedia()
  {

  }

  bool CommandCreateMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().CreateMedia(m_Media, m_MediaPath);
  }


  //media deletion command
  //constructor
  CommandDestroyMedia::CommandDestroyMedia(const std::string& MediaID)
  {
    m_Media = MediaID;
  }

  //destructor
  CommandDestroyMedia::~CommandDestroyMedia()
  {

  }

  bool CommandDestroyMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().DestroyMedia(m_Media);
  }
}
