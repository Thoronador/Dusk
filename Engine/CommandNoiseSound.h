#ifndef COMMANDNOISESOUND_H
#define COMMANDNOISESOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandCreateNoise : public Command
  {
    public:
      CommandCreateNoise(const std::string& NoiseID);
      virtual ~CommandCreateNoise();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };

  class CommandDestroyNoise : public Command
  {
    public:
      CommandDestroyNoise(const std::string& NoiseID);
      virtual ~CommandDestroyNoise();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDNOISESOUND_H
