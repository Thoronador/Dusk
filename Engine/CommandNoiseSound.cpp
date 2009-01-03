#include "CommandNoiseSound.h"
#include "Sound.h"
namespace Dusk
{
  //noise creation command
  //constructor
  CommandCreateNoise::CommandCreateNoise(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandCreateNoise::~CommandCreateNoise()
  {

  }

  bool CommandCreateNoise::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().CreateNoise(m_Noise);
  }


  //noise deletion command

  //constructor
  CommandDestroyNoise::CommandDestroyNoise(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandDestroyNoise::~CommandDestroyNoise()
  {

  }

  bool CommandDestroyNoise::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().DestroyNoise(m_Noise);
  }
}
