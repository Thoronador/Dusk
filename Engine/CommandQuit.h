/*---------------------------------------------------------------------------
 Author:  DaSteph, walljumper
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

 ToDo list:
     - I guess this one is finished.

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef COMMANDQUIT_H
#define COMMANDQUIT_H
#include "Command.h"

namespace Dusk
{
    class Scene;
    class CommandQuit : public Dusk::Command
    {
        public:
            CommandQuit();
            virtual ~CommandQuit();

            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
        private:
    };
}

#endif // COMMANDQUIT_H
