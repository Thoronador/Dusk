/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-15
 Purpose: Lua functions/bindings for use of DuskObject class in Lua Scripts

 History:
     - 2010-02-10 (rev 171) - initial version (by thoronador)
     - 2010-03-04 (rev 177) - GetNPC() moved to LuaBindingsNPC.h
     - 2010-05-15 (rev 204) - IsValidObject() added
                            - most object-related functions will now throw
                              errors, if they don't have a non-NULL pointer to
                              a DuskObject

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.

 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSOBJECT_H
#define LUABINDINGSOBJECT_H

#include "LuaIncludes.h"

namespace Dusk
{
  int GetObject(lua_State *L);
  int IsValidObject(lua_State *L);
  int IsObjectAvailable(lua_State *L);

  int Enable(lua_State *L);
  int Disable(lua_State *L);
  int IsEnabled(lua_State *L);

  int GetObjectPosition(lua_State *L);
  int SetObjectPosition(lua_State *L);
  int GetObjectRotation(lua_State *L);
  int SetObjectRotation(lua_State *L);

  int GetScale(lua_State *L);
  int SetScale(lua_State *L);

  void registerObject(lua_State *L);
} //namespace

#endif // LUABINDINGSOBJECT_H
