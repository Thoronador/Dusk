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
 Date:    2010-03-08
 Purpose: Lua functions/bindings for use of QuestLog class in Lua Scripts

 History:
     - 2010-03-08 (rev 178) - initial version (by thoronador)
     - 2010-05-28 (rev 209) - documentation update; namespace

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSQUESTLOG_H
#define LUABINDINGSQUESTLOG_H

#include "LuaIncludes.h"

namespace Dusk
{

namespace Lua
{

  /* adds a new quest entry to the quest log

     return value(s) on stack: 1
         #1 (boolean) - returns true on success (i.e. a new entry was added),
                        false on failure

     expected stack parameters: 2
         #1 (string) - ID of quest entry
         #2 (number) - index of quest entry
  */
  int AddQuestEntry(lua_State *L);

  /* checks for the presence of a quest within the quest log

     return value(s) on stack: 1
         #1 (boolean) - returns true, if at least one entry for that quest is
                        present

     expected stack parameters: 1
         #1 (string) - ID of quest
  */
  int HasQuest(lua_State *L);

  /* checks if a quest is finished

     return value(s) on stack: 1
         #1 (boolean) - returns true, if the given quest is finished

     expected stack parameters: 1
         #1 (string) - ID of quest
  */
  int IsQuestFinished(lua_State *L);

  /* checks for the presence of a quest entry within the quest log

     return value(s) on stack: 1
         #1 (boolean) - returns true, if the quest entry is present

     expected stack parameters: 2
         #1 (string) - ID of quest
         #2 (number) - index of quest entry
  */
  int HasQuestEntry(lua_State *L);

  /* returns the index of the quest entry with the highest index of that quest
     line present in the QuestLog

     return value(s) on stack: 1
         #1 (boolean) - returns true, if the quest entry is present

     expected stack parameters: 1
         #1 (string) - ID of quest
  */
  int GetMaximumPresentIndex(lua_State *L);

  //register the above functions at Lua
  void registerQuestLog(lua_State *L);
} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSQUESTLOG_H
