/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-01-19
 Purpose: CommandWeather class
          Commands for toggling, enabling and disabling fog, rain and snow
          via Console

 History:
     - 2010-01-03 (rev 149) - initial version; Fog and Snow (by thoronador)
     - 2010-01-19 (rev 157) - CommandSnow and CommandFog merged to
                              CommandWeather (should have been that way from
                              the start)
                            - Rain added
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - commands for other effects, such as wind(?)
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef COMMANDWEATHER_H
#define COMMANDWEATHER_H

#include "Command.h"

namespace Dusk
{
  class CommandWeather : public Command
  {
    public:
      /* enumeration type to specifiy weather effect */
      enum WeatherType {wtFog, wtRain, wtSnow};

      /* Constructor.
         Parameter wt indicates the weather feature (fog, rain or snow) that
         will be changed.
         If parameter toggle is true, then the feature will be toggled and the
         second parameter will be ignored. If toggle is false, the second
         parameter indicates, whether this feature will be turned on (true) or
         off (false).
      */
      CommandWeather(const WeatherType wt, const bool toggle, const bool turn_it_on=true);

      /* destructor */
      virtual ~CommandWeather();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_Toggle, m_TurnOn;
      WeatherType m_Type;
  };

} //namespace

#endif // COMMANDWEATHER_H
