/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 thoronador

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
