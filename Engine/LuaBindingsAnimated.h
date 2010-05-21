/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-21
 Purpose: Lua functions/bindings for use of AnimatedObject class in Lua Scripts

 History:
     - 2010-05-21 (rev 206) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSANIMATED_H
#define LUABINDINGSANIMATED_H

#include "LuaIncludes.h"

namespace Dusk
{

namespace Lua
{
  /* returns the Player

     return value(s) on stack: 1
         #1 (userdata) - pointer to the Player object

     expected stack parameters: 0
         nothing/ nil
  */
  int GetPlayer(lua_State *L);

  /* returns the requested AnimatedObject as pointer/light userdata

     return value(s) on stack: 1
         #1 (userdata) - pointer to the AnimatedObject

     expected stack parameters: 1
         #1 (string) - ID of the AnimatedObject
  */
  int GetAnimated(lua_State *L);

  /* returns the current loop state of the given AnimatedObject

     return value(s) on stack: 1
         #1 (boolean) - loop state

     expected stack parameters: 1
         #1 (userdata) - pointer to the AnimatedObject
  */
  int GetAnimatedLoop(lua_State *L);

  /* returns the current animation name of the given AnimatedObject

     return value(s) on stack: 1
         #1 (string) - animation name

     expected stack parameters: 1
         #1 (userdata) - pointer to the AnimatedObject
  */
  int GetAnimatedAnimation(lua_State *L);

  /* sets the animation name and loop state of the given AnimatedObject

     return value(s) on stack: 0
         nothing/ nil

     expected stack parameters: 3
         #1 (userdata) - pointer to the AnimatedObject
         #2 (string)   - desired animation name
         #3 (boolean)  - loop state
  */
  int AnimatedPlayAnimation(lua_State *L);

  //registers all of the above functions at Lua
  void registerAnimated(lua_State *L);

} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSANIMATED_H
