/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 walljumper, thoronador

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
 Authors: walljumper, thoronador
 Date:    2010-05-06
 Purpose: CommandMove class
          Command for moving camera (and player) via Console

 History:
     - 2008-03-22 (rev 51)  - initial version (by walljumper)
     - 2008-03-24 (rev 52)  - forward and backward movement
     - 2008-03-24 (rev 53)  - tried to fix bug
     - 2008-03-25 (rev 54)  - fixed mistake with direction type (thoronador)
     - 2008-03-25 (rev 55)  - left and right movement (by walljumper)
     - 2009-06-21 (rev 98)  - camera rotation added (by thoronador)
     - 2010-03-24 (rev 186) - adjustments to work with new Singleton based
                              Camera class
                            - jump added
     - 2010-05-06 (rev 197) - documentation update
     - 2010-08-27 (rev 236) - implementation of movement changed

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef DUSK_COMMANDMOVE_H
#define DUSK_COMMANDMOVE_H
#include "Command.h"
#include "../DuskTypes.h"

namespace Dusk
{
    class CommandMove : public Command
    {
        public:
            /* standard constructor */
            CommandMove(): m_Direction(FORWARD){}

            /* constructor with directions parameter

               parameters:
                   dir  -  direction of movement (symbolic constant)
            */
            CommandMove(const DIRECTION dir);

            /* destructor */
            virtual ~CommandMove();
            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
            static int moveConstant;
            static const int rotateDPS;
            DIRECTION m_Direction;
    };
}

#endif // DUSK_COMMANDMOVE_H
