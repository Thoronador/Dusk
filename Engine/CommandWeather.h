/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
