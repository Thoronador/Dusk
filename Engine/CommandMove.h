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
     - 2008-03-25 (rev 98)  - camera rotation added (by thoronador)
     - 2010-03-24 (rev 186) - adjustments to work with new Singleton based
                              Camera class
                            - jump added
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDMOVE_H
#define COMMANDMOVE_H
#include "Command.h"
#include "DuskTypes.h"

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

#endif // COMMANDMOVE_H
