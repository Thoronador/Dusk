#ifndef COMMANDREPLAYSOUND_H
#define COMMANDREPLAYSOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandReplaySound : public Command
  {
    public:
      CommandReplaySound(const std::string& NoiseID);
      virtual ~CommandReplaySound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDREPLAYSOUND_H
