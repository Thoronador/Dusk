#ifndef COMMANDUNPAUSESOUND_H
#define COMMANDUNPAUSESOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandUnPauseSound : public Command
  {
    public:
      CommandUnPauseSound(const std::string& path);
      virtual ~CommandUnPauseSound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Path;
  };
}

#endif // COMMANDUNPAUSESOUND_H
