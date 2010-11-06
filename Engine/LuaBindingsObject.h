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
 Date:    2010-05-15
 Purpose: Lua functions/bindings for use of DuskObject class in Lua Scripts

 History:
     - 2010-02-10 (rev 171) - initial version (by thoronador)
     - 2010-03-04 (rev 177) - GetNPC() moved to LuaBindingsNPC.h
     - 2010-05-15 (rev 204) - IsValidObject() added
                            - most object-related functions will now throw
                              errors, if they don't have a non-NULL pointer to
                              a DuskObject
     - 2010-05-21 (rev 207) - documentation updated; Lua namespace added

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

namespace Lua
{
  /* returns the object with the given ID, if present

     return value(s) on stack: 1
         #1 (userdata) - pointer to the static object

     expected stack parameters: 0
         #1 (string) - ID of the object
  */
  int GetObject(lua_State *L);

  /* returns true, if the passed parameter is a valid object

     return value(s) on stack: 1
         #1 (boolean) - true, if parameter #1 is a valid object,
                        false otherwise

     expected stack parameters: 1
         #1 (any type) - suspected pointer to the object
  */
  int IsValidObject(lua_State *L);

  /* returns true, if a static(!) object with the given ID is available.
     Note that you cannot use this function to check for the availability of
     a non-static object.

     return value(s) on stack: 1
         #1 (boolean) - true, if a static object with the given ID exists,
                        false otherwise

     expected stack parameters: 1
         #1 (string) - ID of the object to check for
  */
  int IsObjectAvailable(lua_State *L);

  /* tries to enable an object (be it static or not), i.e. display it in-game,
     and returns whether or not the object could be enabled

     return value(s) on stack: 1
         #1 (boolean) - true, if the object was successfully enabled,
                        false otherwise

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int Enable(lua_State *L);

  /* tries to disable an object (be it static or not), i.e. hide it in-game,
     and returns whether or not the object could be disabled

     return value(s) on stack: 1
         #1 (boolean) - true, if the object was successfully disabled,
                        false otherwise

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int Disable(lua_State *L);

  /* returns whether or not the given object is enabled (and thus visible)

     return value(s) on stack: 1
         #1 (boolean) - true, if the object is enabled,
                        false otherwise

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int IsEnabled(lua_State *L);

  /* returns the position of the given object

     return value(s) on stack: 3
         #1 (number) - x-component of the object's position
         #2 (number) - y-component of the object's position
         #3 (number) - z-component of the object's position

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetObjectPosition(lua_State *L);

  /* sets the new position of the given object

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 4
         #1 (userdata) - pointer to the object
         #2 (number)   - x-component of the object's position
         #3 (number)   - y-component of the object's position
         #4 (number)   - z-component of the object's position
  */
  int SetObjectPosition(lua_State *L);

  /* returns the rotation of the given object

     return value(s) on stack: 3
         #1 (number) - x-component of the object's rotation
         #2 (number) - y-component of the object's rotation
         #3 (number) - z-component of the object's rotation

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetObjectRotation(lua_State *L);

  /* sets the new rotation of the given object

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 4
         #1 (userdata) - pointer to the object
         #2 (number)   - x-component of the object's rotation
         #3 (number)   - y-component of the object's rotation
         #4 (number)   - z-component of the object's rotation
  */
  int SetObjectRotation(lua_State *L);

  /* returns the scale of the given object.
     A scaling factor of exactly 1.0 means "normal" size, 0.5 means half size,
     2.0 means double size, and so on.

     return value(s) on stack: 1
         #1 (number) - scaling factor of the object

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetScale(lua_State *L);

  /* sets the new scale of the given object and returns true on success,
     i.e. if the scaling factor could be changed to the new value.
     Note that you normally cannot set the scale of an object as long as it is
     enabled, that is, the function will fail in that case.
     Use IsEnabled() to check whether an object is enabled or not.

     return value(s) on stack: 0
         #1 (boolean) - indicates success (true) or failure (false)

     expected stack parameters: 2
         #1 (userdata) - pointer to the object
         #2 (number)   - new scaling factor of the object
  */
  int SetScale(lua_State *L);

  void registerObject(lua_State *L);
} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSOBJECT_H
