/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: CommandPickUp class
          Command for letting the player pick up the nearest possible item
          via Console

 History:
     - 2010-03-27 (rev 188) - initial version (by thoronador)
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDPICKUP_H
#define COMMANDPICKUP_H

#include "Command.h"

namespace Dusk
{

  class CommandPickUp : public Command
  {
    public:
      /* constructor */
      CommandPickUp();
      /* destructor */
      virtual ~CommandPickUp();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
  };

} //namespace

#endif // COMMANDPICKUP_H
