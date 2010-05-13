/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-13
 Purpose: Lua functions/bindings for use of NPC class in Lua Scripts

 History:
     - 2010-03-04 (rev 177) - initial version (by thoronador)
     - 2010-05-13 (rev 201) - inventory-related functions added

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

  //inventory-related functions
  /* adds an arbitrary number of items to the NPC's inventory

     return value:
         nothing/ nil

     expected stack parameters: 3
         #1 (userdata) - pointer to NPC
         #2 (string)   - ID of items
         #3 (number)   - number of items to add
  */
  int AddItem(lua_State *L);

  /* removes an arbitrary number of items from the NPC's inventory

     return value(s) on stack: 1
         #1 (number) - number of items that were actually removed

     expected stack parameters: 3
         #1 (userdata) - pointer to NPC
         #2 (string)   - ID of items
         #3 (number)   - number of items to remove
  */
  int RemoveItem(lua_State *L);

  /* returns the number of items with given ID in the NPC's inventory

     return value(s) on stack: 1
         #1 (number) - number of items with that ID

     expected stack parameters: 2
         #1 (userdata) - pointer to NPC
         #2 (string)   - ID of item
  */
  int GetItemCount(lua_State *L);

  void registerNPC(lua_State *L);
} //namespace

#endif // LUABINDINGSNPC_H
