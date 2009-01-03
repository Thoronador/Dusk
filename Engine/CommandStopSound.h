#ifndef COMMANDSTOPSOUND_H
#define COMMANDSTOPSOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandStopSound : public Command
  {
    public:
      CommandStopSound(const std::string& NoiseID);
      virtual ~CommandStopSound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDSTOPSOUND_H
