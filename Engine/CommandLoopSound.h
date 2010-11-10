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
 Purpose: CommandLoopSound class
          Command for setting loop mode of sounds via Console

 History:
     - 2008-03-28 (rev 58)  - initial version (by thoronador)
     - 2009-01-03 (rev 67)  - adjustments to work with new (rev. 66) Sound
                              class
     - 2010-05-06 (rev 197) - documentation update
     - 2010-11-10 (rev 250) - update for corrected function names in Sound

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDLOOPSOUND_H
#define COMMANDLOOPSOUND_H

#include "Command.h"
#include <string>

namespace Dusk
{
  class CommandLoopSound : public Command
  {
    public:
      /* constructor

         parameters:
             NoiseID - ID of Noise whose loop mode shall be changed
             DoLoop  - indicates whether noise shall be put into loop
                       mode (true) or not (false)
      */
      CommandLoopSound(const std::string& NoiseID, const bool DoLoop);
      /* destructor */
      virtual ~CommandLoopSound();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Noise;
      bool m_DoLoop;
  };
}

#endif // COMMANDLOOPSOUND_H
