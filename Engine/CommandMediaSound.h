#ifndef COMMANDMEDIASOUND_H
#define COMMANDMEDIASOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandCreateMedia : public Command
  {
    public:
      CommandCreateMedia(const std::string& MediaID, const std::string& MediaPath);
      virtual ~CommandCreateMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Media, m_MediaPath;
  };

  class CommandDestroyMedia : public Command
  {
    public:
      CommandDestroyMedia(const std::string& MediaID);
      virtual ~CommandDestroyMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Media;
  };
}

#endif // COMMANDMEDIASOUND_H
