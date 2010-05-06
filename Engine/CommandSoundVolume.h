/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: CommandSound class
          Command for setting volume of sounds via Console

 History:
     - 2008-03-28 (rev 58)  - initial version (by thoronador)
     - 2009-01-03 (rev 67)  - adjustments to work with new (rev. 66) Sound
                              class
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDSOUNDVOLUME_H
#define COMMANDSOUNDVOLUME_H

#include "Command.h"
#include <string>
namespace Dusk
{
  class CommandSoundVolume : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of the noise whose volume level has to be set
             vol     - new volume for that noise, shall be in [0;1], where
                       zero means muted and 1 is full volume.
                       You can also specify values greater than 1.0, but some
                       OpenAL implementations cut such values down to 1.0.
      */
      CommandSoundVolume(const std::string& NoiseID, const float vol);

      /* destructor */
      virtual ~CommandSoundVolume();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
      float m_Volume;
  };
}

#endif // COMMANDSOUNDVOLUME_H
