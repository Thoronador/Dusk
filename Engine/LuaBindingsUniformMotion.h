#ifndef LUABINDINGSUNIFORMMOTION_H
#define LUABINDINGSUNIFORMMOTION_H

#include "LuaIncludes.h"

namespace Dusk
{

namespace Lua
{

  /* returns the movement direction of the given object

     return value(s) on stack: 3
         #1 (number) - x-component of the direction
         #2 (number) - y-component of the direction
         #3 (number) - z-component of the direction

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetDirection(lua_State *L);

  /* sets the new direction of the given object

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 4
         #1 (userdata) - pointer to the object
         #2 (number)   - x-component of the new direction
         #3 (number)   - y-component of the new direction
         #4 (number)   - z-component of the new direction
  */
  int SetDirection(lua_State *L);

  /* returns the movement speed of the given object

     return value(s) on stack: 1
         #1 (number) - movement speed

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetSpeed(lua_State *L);

  /* sets the new speed of the given object

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 2
         #1 (userdata) - pointer to the object
         #2 (number)   - new movement speed of object
  */
  int SetSpeed(lua_State *L);

  /* returns the movement destination of the given object

     return value(s) on stack: 3
         #1 (number) - x-component of the destination
         #2 (number) - y-component of the destination
         #3 (number) - z-component of the destination

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int GetDestination(lua_State *L);

  /* sets the new destination of the given object and causes it to travel to
     that point

     return value(s) on stack: 0
         nothing / nil

     expected stack parameters: 4
         #1 (userdata) - pointer to the object
         #2 (number)   - x-component of the new destination
         #3 (number)   - y-component of the new destination
         #4 (number)   - z-component of the new destination
  */
  int TravelToDestination(lua_State *L);

  /* returns true, if the given object is currently travelling to a certain
     destination

     return value(s) on stack: 1
         #1 (boolean) - true, if object is travelling; false otherwise

     expected stack parameters: 1
         #1 (userdata) - pointer to the object
  */
  int IsOnTravel(lua_State *L);

  //registers all of the above functions at Lua
  void registerMotion(lua_State *L);
} //namespace Lua

} //namespace Dusk

#endif // LUABINDINGSUNIFORMMOTION_H
