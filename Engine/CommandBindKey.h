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
 Date:    2010-12-19
 Purpose: CommandBindKey and CommandUnbindKey classes
          Command for setting and deleting key bindings via console

 History:
     - 2010-12-19 (rev 273) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDBINDKEY_H
#define COMMANDBINDKEY_H

#include <string>
#include "Command.h"

namespace Dusk
{
  class CommandBindKey : public Command
  {
    public:
      /* Constructor.

         parameters:
             key    - string that identifies the key
             action - string that names the function that shall be bound to
                      that key
      */
      CommandBindKey(const std::string& key, const std::string& action);

      /* destructor */
      virtual ~CommandBindKey();

      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Key, m_Action;
  };//class

  class CommandUnbindKey : public Command
  {
    public:
      /* Constructor.

         parameter:
             key - string that identifies the key whose binding shall be deleted
      */
      CommandUnbindKey(const std::string& key);

      /* destructor */
      virtual ~CommandUnbindKey();

      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      std::string m_Key;
  };//class

} //namespace

#endif // COMMANDBINDKEY_H
