/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-09
 Purpose: Lua functions/bindings for use of Sound class in Lua Scripts

 History:
     - 2010-02-09 (rev 170) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find any bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUABINDINGSSOUND_H
#define LUABINDINGSSOUND_H

#include "LuaIncludes.h"

namespace Dusk
{

int CreateNoise(lua_State *L);
int DestroyNoise(lua_State *L);
int CreateMedia(lua_State *L);
int DestroyMedia(lua_State *L);
int AttachMediaToNoise(lua_State *L);
int DetachMediaFromNoise(lua_State *L);

int PlaySound(lua_State *L);
int PauseSound(lua_State *L);
int UnPauseSound(lua_State *L);
int StopSound(lua_State *L);
int ReplaySound(lua_State *L);
int LoopSound(lua_State *L);

int SoundIsPlaying(lua_State *L);
int SoundIsLooping(lua_State *L);

int SetSoundVolume(lua_State *L);
int GetSoundVolume(lua_State *L);

//called to register all of the above functions
void registerSound(lua_State *L);

} //namespace

#endif // LUABINDINGSSOUND_H
