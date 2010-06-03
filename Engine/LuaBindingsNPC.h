/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-13
 Purpose: Lua functions/bindings for use of NPC class in Lua Scripts

 History:
     - 2010-03-04 (rev 177) - initial version (by thoronador)
     - 2010-05-13 (rev 201) - inventory-related functions added
     - 2010-05-21 (rev 206) - documentation updated and small improvements
     - 2010-06-03 (rev 214) - small improvement to get rid of compiler warnings

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

namespace Lua
{
  /* returns the requested NPC as pointer/light userdata

     return value(s) on stack: 1
         #1 (userdata) - pointer to the NPC

     expected stack parameters: 1
         #1 (string) - ID of the NPC
  */
  int GetNPC(lua_State *L);

  /* returns the current health of the given NPC

     return value(s) on stack: 1
         #1 (number) - health of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetHealth(lua_State *L);

  /* sets the current health of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new health value
  */
  int SetHealth(lua_State *L);

  /* returns the current level of the given NPC

     return value(s) on stack: 1
         #1 (number) - level of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetLevel(lua_State *L);

  /* returns the current strenght of the given NPC

     return value(s) on stack: 1
         #1 (number) - strength of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetStrength(lua_State *L);

  /* returns the current agility of the given NPC

     return value(s) on stack: 1
         #1 (number) - agility of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetAgility(lua_State *L);

  /* returns the current vitality of the given NPC

     return value(s) on stack: 1
         #1 (number) - vitality of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetVitality(lua_State *L);

  /* returns the current intelligence of the given NPC

     return value(s) on stack: 1
         #1 (number) - intelligence of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetIntelligence(lua_State *L);

  /* returns the current willpower of the given NPC

     return value(s) on stack: 1
         #1 (number) - willpower of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetWillpower(lua_State *L);

  /* returns the current charisma of the given NPC

     return value(s) on stack: 1
         #1 (number) - charisma of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetCharisma(lua_State *L);

  /* returns the current luck of the given NPC

     return value(s) on stack: 1
         #1 (number) - luck of NPC

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
  */
  int GetLuck(lua_State *L);

  /* sets the current level of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new level (0-255)
  */
  int SetLevel(lua_State *L);

  /* sets the strength of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new strength value (0-255)
  */
  int SetStrength(lua_State *L);

  /* sets the agility of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new agility value (0-255)
  */
  int SetAgility(lua_State *L);

  /* sets the vitality of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new vitality value (0-255)
  */
  int SetVitality(lua_State *L);

  /* sets the intelligence of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new intelligence value (0-255)
  */
  int SetIntelligence(lua_State *L);

  /* sets the willpower of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new willpower value (0-255)
  */
  int SetWillpower(lua_State *L);

  /* sets the charisma of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new charisma value (0-255)
  */
  int SetCharisma(lua_State *L);

  /* sets the luck value of the given NPC

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 1
         #1 (userdata) - pointer to the NPC
         #2 (number)   - new luck value (0-255)
  */
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

  //registers all of the above functions at Lua
  void registerNPC(lua_State *L);
} //namespace Lua
} //namespace Dusk

#endif // LUABINDINGSNPC_H
