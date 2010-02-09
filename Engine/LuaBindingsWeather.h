/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-09
 Purpose: Lua functions/bindings for use of Weather class in Lua Scripts

 History:
     - 2010-02-09 (rev 170) - initial version (by thoronador)

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

int StartLinearFog(lua_State *L);
int StartExponentialFog(lua_State *L);
int StopFog(lua_State *L);
int IsFoggy(lua_State *L);
int IsLinearFog(lua_State *L);
int SetFogColour(lua_State *L);
int GetFogColour(lua_State *L);
int ToggleFog(lua_State *L);

int StartSnow(lua_State *L);
int StopSnow(lua_State *L);
int IsSnowing(lua_State *L);
int ToggleSnow(lua_State *L);

int StartRain(lua_State *L);
int StopRain(lua_State *L);
int IsRaining(lua_State *L);
int ToggleRain(lua_State *L);

void registerWeather(lua_State *L);

} //namespace

#endif // LUABINDINGSWEATHER_H
