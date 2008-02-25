#ifndef COMMANDPAUSESOUND_H
#define COMMANDPAUSESOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandPauseSound : public Command
  {
    public:
      CommandPauseSound(const std::string& path);
      virtual ~CommandPauseSound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Path;
  };
}

#endif // COMMANDPAUSESOUND_H
