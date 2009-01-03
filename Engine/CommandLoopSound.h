#ifndef COMMANDLOOPSOUND_H
#define COMMANDLOOPSOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandLoopSound : public Command
  {
    public:
      CommandLoopSound(const std::string& NoiseID, const bool DoLoop);
      virtual ~CommandLoopSound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
      bool m_DoLoop;
  };
}

#endif // COMMANDLOOPSOUND_H
