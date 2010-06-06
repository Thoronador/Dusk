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
