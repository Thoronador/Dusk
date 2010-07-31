/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-21
 Purpose: Lua functions/bindings for use of AnimatedObject class in Lua Scripts

 History:
     - 2010-05-21 (rev 206) - initial version (by thoronador)
     - 2010-05-27 (rev 209) - bindings for new methods of AnimatedObject
     - 2010-07-31 (rev 219) - update to reflect changes of AnimatedObject

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

  /* returns the current loop state of an animation of the given AnimatedObject

     return value(s) on stack: 1
         #1 (boolean) - loop state

     expected stack parameters: 2
         #1 (userdata) - pointer to the AnimatedObject
         #2 (string)   - animation name
  */
  int GetAnimatedLoop(lua_State *L);

  /* returns the list of all current animations of the given AnimatedObject

     return value(s) on stack: 0+
         #1 (string) - animation name of first enabled animation
         #2 (string) - animation name of second enabled animation
         #3 (string) - animation name of third enabled animation
         ...

     expected stack parameters: 1
         #1 (userdata) - pointer to the AnimatedObject
  */
  int GetAnimatedAnimations(lua_State *L);

  /* starts the named animation with the given loop state

     return value(s) on stack: 1
         #1 (boolean) - returns true on success, false on failure

     expected stack parameters: 3
         #1 (userdata) - pointer to the AnimatedObject
         #2 (string)   - name of animation
         #3 (boolean)  - loop state of animation
  */
  int AnimatedStartAnimation(lua_State *L);

  /* stops the named animation

     return value(s) on stack: 1
         #1 (boolean) - returns true on success, false on failure

     expected stack parameters: 2
         #1 (userdata) - pointer to the AnimatedObject
         #2 (string)   - name of animation
  */
  int AnimatedStopAnimation(lua_State *L);

  /* stops all animations of the given object

     return value(s) on stack: 1
         #1 (number) - number of stopped animations

     expected stack parameters: 1
         #1 (userdata) - pointer to the AnimatedObject
  */
  int AnimatedStopAllAnimations(lua_State *L);

  /* stops the named animation

     return value(s) on stack: 1
         #1 (boolean) - returns true if animation is active, false otherwise

     expected stack parameters: 2
         #1 (userdata) - pointer to the AnimatedObject
         #2 (string)   - name of animation
  */
  int AnimatedIsAnimationActive(lua_State *L);

  //registers all of the above functions at Lua
  void registerAnimated(lua_State *L);

} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSANIMATED_H
