/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-08-20
 Purpose: CommandQuestLog classes
          Commands for switching visibility of QuestLog via Console

 History:
     - 2010-08-20 (rev 231) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDQUESTLOG_H
#define COMMANDQUESTLOG_H

#include "Command.h"

namespace Dusk
{

  class CommandQuestLog: public Command
  {
    public:
      /* enumeration value to identify operation */
      enum QuestLogOperation {qloToggle, qloNext, qloPrev};
      /* constructor */
      CommandQuestLog(const QuestLogOperation op);
      /* destructor */
      virtual ~CommandQuestLog();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    protected:
      QuestLogOperation m_Operation;
  };//class

}//namespace

#endif // COMMANDQUESTLOG_H
