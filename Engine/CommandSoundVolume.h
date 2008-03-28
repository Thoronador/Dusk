#ifndef COMMANDSOUNDVOLUME_H
#define COMMANDSOUNDVOLUME_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandSoundVolume : public Command
  {
    public:
      CommandSoundVolume(const std::string& path, const float vol);
      virtual ~CommandSoundVolume();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Path;
      float m_Volume;
  };
}

#endif // COMMANDSOUNDVOLUME_H
