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
 Purpose: Lua functions/bindings for use of Sound class in Lua Scripts

 History:
     - 2010-02-09 (rev 170) - initial version (by thoronador)
     - 2010-11-10 (rev 250) - update for corrected function names in Sound

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
