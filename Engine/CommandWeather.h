/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-01-03
 Purpose: CommandFog and CommandSnow classes
          Commands for toggling, enabling and disabling fog and snow
          via Console

 History:
     - 2010-01-03 (rev 149) - initial version; Fog and Snow

 ToDo list:
     - commands for other effects, such as rain
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef COMMANDWEATHER_H
#define COMMANDWEATHER_H

#include "Command.h"

namespace Dusk
{
  class CommandFog : public Command
  {
    public:
      /* Constructor.
         If parameter toggle is true, then the fog will be toggled and the
         second parameter will be ignored. If toggle is false, the second
         parameter indicates, whether fog will be turned on (true) or off.
      */
      CommandFog(const bool toggle, const bool turn_it_on=true);
      virtual ~CommandFog();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_Toggle, m_TurnOn;
  };

  class CommandSnow : public Command
  {
    public:
      /* Constructor.
         If parameter toggle is true, then the snow will be toggled and the
         second parameter will be ignored. If toggle is false, the second
         parameter indicates, whether snow will be turned on (true) or off.
      */
      CommandSnow(const bool toggle, const bool turn_it_on=true);
      virtual ~CommandSnow();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_Toggle, m_TurnOn;
  };

} //namespace

#endif // COMMANDWEATHER_H
