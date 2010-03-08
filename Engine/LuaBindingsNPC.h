/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-04
 Purpose: Lua functions/bindings for use of NPC class in Lua Scripts

 History:
     - 2010-03-04 (rev 177) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSNPC_H
#define LUABINDINGSNPC_H

#include "LuaIncludes.h"

namespace Dusk
{
  int GetNPC(lua_State *L);

  int GetHealth(lua_State *L);
  int SetHealth(lua_State *L);

  int GetLevel(lua_State *L);
  int GetStrength(lua_State *L);
  int GetAgility(lua_State *L);
  int GetVitality(lua_State *L);
  int GetIntelligence(lua_State *L);
  int GetWillpower(lua_State *L);
  int GetCharisma(lua_State *L);
  int GetLuck(lua_State *L);

  int SetLevel(lua_State *L);
  int SetStrength(lua_State *L);
  int SetAgility(lua_State *L);
  int SetVitality(lua_State *L);
  int SetIntelligence(lua_State *L);
  int SetWillpower(lua_State *L);
  int SetCharisma(lua_State *L);
  int SetLuck(lua_State *L);

  void registerNPC(lua_State *L);
} //namespace

#endif // LUABINDINGSNPC_H
