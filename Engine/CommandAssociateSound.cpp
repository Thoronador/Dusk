#include "CommandAssociateSound.h"
#include "Sound.h"
namespace Dusk
{
  //command for associating noise with media
  //constructor
  CommandAssociateSoundMedia::CommandAssociateSoundMedia(const std::string& NoiseID, const std::string& MediaID)
  {
    m_Noise = NoiseID;
    m_Media = MediaID;
  }

  //destructor
  CommandAssociateSoundMedia::~CommandAssociateSoundMedia()
  {

  }

  bool CommandAssociateSoundMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().Attach(m_Noise, m_Media);
  }


  //command for revoking associating between noise and media
  //constructor
  CommandDeassociateSoundMedia::CommandDeassociateSoundMedia(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandDeassociateSoundMedia::~CommandDeassociateSoundMedia()
  {

  }

  bool CommandDeassociateSoundMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().Detach(m_Noise);
  }
}
