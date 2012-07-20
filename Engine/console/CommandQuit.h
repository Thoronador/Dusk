/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2010 DaSteph, walljumper, thoronador

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
 Authors: DaSteph, walljumper, thoronador
 Date:    2010-01-13
 Purpose: CommandQuit class
          command to shut down the game

 History:
     - 2008-01-07 (rev 20)  - initial version (by DaSteph)
     - 2008-01-17 (rev 23)  - } execute() implemented
     - 2008-01-17 (rev 24)  - }       (by walljumper)
     - 2008-02-22 (rev 39)  - fixed some bugs
     - 2010-01-13 (rev 152) - removed unneccessary includes;
                              documentation update (by thoronador)
     - 2010-08-15 (rev 228) - scene is now destroyed explicitly during command
                              execution (should have been this way earlier)
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging

 ToDo list:
     - I guess this one is finished.

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_COMMANDQUIT_H
#define DUSK_COMMANDQUIT_H
#include "Command.h"
#include "../Scene.h"

namespace Dusk
{
  class CommandQuit : public Dusk::Command
  {
    public:
      /* constructor */
      CommandQuit();

      /* destructor */
      virtual ~CommandQuit();

      virtual bool execute(Dusk::Scene* scene, int count = 1);
  };//class

}//namespace

#endif // DUSK_COMMANDQUIT_H
