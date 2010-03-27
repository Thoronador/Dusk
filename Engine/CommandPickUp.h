#ifndef COMMANDPICKUP_H
#define COMMANDPICKUP_H

#include "Command.h"

namespace Dusk
{

  class CommandPickUp : public Command
  {
    public:
      CommandPickUp();
      virtual ~CommandPickUp();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
  };

} //namespace

#endif // COMMANDPICKUP_H
