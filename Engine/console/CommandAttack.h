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
 Date:    2010-06-06
 Purpose: CommandAttack class
          Command for letting the player attack/ stop attack via Console

 History:
     - 2010-06-06 (rev 215)  - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDATTACK_H
#define COMMANDATTACK_H

#include "Command.h"

namespace Dusk
{
  class CommandAttack : public Command
  {
    public:
      /* constructor

         parameters:
             start  - indicates whether attack shall be started (true) or
                      stopped (false)
      */
      CommandAttack(const bool start);
      /* destructor */
      virtual ~CommandAttack();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_DoStart;
  };
}

#endif // COMMANDATTACK_H
