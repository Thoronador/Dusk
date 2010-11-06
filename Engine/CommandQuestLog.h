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
