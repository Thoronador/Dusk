#ifndef COMMANDASSOCIATESOUND_H
#define COMMANDASSOCIATESOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandAssociateSoundMedia : public Command
  {
    public:
      CommandAssociateSoundMedia(const std::string& NoiseID, const std::string& MediaID);
      virtual ~CommandAssociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise, m_Media;
  };

  class CommandDeassociateSoundMedia : public Command
  {
    public:
      CommandDeassociateSoundMedia(const std::string& NoiseID);
      virtual ~CommandDeassociateSoundMedia();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
  };
}

#endif // COMMANDASSOCIATESOUND_H
