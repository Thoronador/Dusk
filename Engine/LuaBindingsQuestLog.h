/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-08
 Purpose: Lua functions/bindings for use of QuestLog class in Lua Scripts

 History:
     - 2010-03-08 (rev 178) - initial version (by thoronador)

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
  int AddQuestEntry(lua_State *L);
  int HasQuest(lua_State *L);
  int IsQuestFinished(lua_State *L);
  int HasQuestEntry(lua_State *L);
  int GetMaximumPresentIndex(lua_State *L);

  void registerQuestLog(lua_State *L);
} //namespace

#endif // LUABINDINGSQUESTLOG_H
