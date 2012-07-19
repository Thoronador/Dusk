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
 Date:    2010-02-09
 Purpose: Lua functions/bindings for use of Weather class in Lua Scripts

 History:
     - 2010-02-09 (rev 170) - initial version (by thoronador)
     - 2010-05-28 (rev 209) - namespace and documentation update

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSWEATHER_H
#define LUABINDINGSWEATHER_H

#include "LuaIncludes.h"

namespace Dusk
{

namespace Lua
{

  /* creates linear fog

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 2
         #1 (number) - fog start distance (>=0.0f)
         #2 (number) - maximum fog distance (>#1)
  */
  int StartLinearFog(lua_State *L);

  /* creates exponential fog

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 1
         #1 (number) - fog density
  */
  int StartExponentialFog(lua_State *L);

  /* clears any fog

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int StopFog(lua_State *L);

  /* checks for presence of fog

     return value(s) on stack: 1
         #1 (boolean) - true, if fog is present

     expected stack parameters: 0
         nothing / nil
  */
  int IsFoggy(lua_State *L);

  /* checks fog mode for linear fog

     return value(s) on stack: 1
         #1 (boolean) - true, if fog mode is linear fog

     expected stack parameters: 0
         nothing / nil
  */
  int IsLinearFog(lua_State *L);

  /* sets the fog colour

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 3
         #1 (number) - red colour component (in range [0;1])
         #2 (number) - green colour component (in range [0;1])
         #3 (number) - blue colour component (in range [0;1])
  */
  int SetFogColour(lua_State *L);

  /* returns the fog colour

     return value(s) on stack: 3
         #1 (number) - red colour component (in range [0;1])
         #2 (number) - green colour component (in range [0;1])
         #3 (number) - blue colour component (in range [0;1])

     expected stack parameters: 0
         nothing /nil
  */
  int GetFogColour(lua_State *L);

  /* toggles the fog, i.e. turns it on/off

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int ToggleFog(lua_State *L);


  /* let it snow!

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int StartSnow(lua_State *L);

  /* clears any snow

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int StopSnow(lua_State *L);

  /* checks for presence of snow

     return value(s) on stack: 1
         #1 (boolean) - true, if it snows

     expected stack parameters: 0
         nothing / nil
  */
  int IsSnowing(lua_State *L);

  /* toggles the snow, i.e. turns it on/off

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int ToggleSnow(lua_State *L);

  /* starts rain

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int StartRain(lua_State *L);

  /* stops the rain

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int StopRain(lua_State *L);

  /* checks for presence of rains

     return value(s) on stack: 1
         #1 (boolean) - true, if it is raining

     expected stack parameters: 0
         nothing / nil
  */
  int IsRaining(lua_State *L);

  /* toggles the rain, i.e. turns it on/off

     return value(s) on stack: 0
         nothing /nil

     expected stack parameters: 0
         nothing /nil
  */
  int ToggleRain(lua_State *L);

  //register the above functions at Lua
  void registerWeather(lua_State *L);

} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSWEATHER_H
