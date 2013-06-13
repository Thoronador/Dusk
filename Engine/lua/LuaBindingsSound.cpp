/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "LuaBindingsSound.h"
#include "../sound/Sound.h"

namespace Dusk
{

namespace Lua
{

int CreateNoise(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    if (Sound::get().createSource(lua_tostring(L, 1)))
    { //push result
      lua_pushboolean(L, 1);
    }
    else
    { //push result
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "CreateNoise expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int DestroyNoise(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().destroySource(lua_tostring(L, 1));
    //push result
    if (success)
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "DestroyNoise expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int CreateMedia(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==2)
  {
    const bool success = Sound::get().createMedia(lua_tostring(L, 1), lua_tostring(L, 2));
    //push result
    if (success)
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "CreateMedia expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int DestroyMedia(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().destroyMedia(lua_tostring(L, 1));
    //push result
    if (success)
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "DestroyMedia expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int AttachMediaToNoise(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==2)
  {
    Source & src = Sound::get().getSource(lua_tostring(L, 1));
    Media & m = Sound::get().getMedia(lua_tostring(L, 2));
    //push result
    lua_pushboolean(L, src.attach(m) ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "AttachMediaToNoise expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int DetachMediaFromNoise(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).detach();
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "DetachMediaFromNoise expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int PlaySound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).play();
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "PlaySound expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int PauseSound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).pause();
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "PauseSound expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int UnPauseSound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).unPause();
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "UnPauseSound expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int StopSound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).stop();
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "StopSound expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int ReplaySound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    //use combination of Stop and Play
    Source & src = Sound::get().getSource(lua_tostring(L, 1));
    if (src.stop())
    {
      if (src.play())
      {
        lua_pushboolean(L, 1);
        return 1;
      }
    }
    lua_pushboolean(L, 0);
    return 1;
  }
  lua_pushstring(L, "ReplaySound expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int LoopSound(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==2)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).loop(lua_toboolean(L, 2)!=0);
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "LoopSound expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SoundIsPlaying(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    lua_pushboolean(L, Sound::get().getSource(lua_tostring(L, 1)).isPlaying() ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "SoundIsPlaying expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SoundIsLooping(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    lua_pushboolean(L, Sound::get().getSource(lua_tostring(L, 1)).isLooping() ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "SoundIsLooping expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetSoundVolume(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==2)
  {
    const bool success = Sound::get().getSource(lua_tostring(L, 1)).setVolume(lua_tonumber(L, 2));
    //push result
    lua_pushboolean(L, success ? 1 : 0);
    return 1;
  }
  lua_pushstring(L, "SetSoundVolume expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int GetSoundVolume(lua_State *L)
{
  const int top = lua_gettop(L);
  if (top==1)
  {
    lua_pushnumber(L, Sound::get().getSource(lua_tostring(L, 1)).getVolume());
    return 1;
  }
  lua_pushstring(L, "GetSoundVolume expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

void registerSound(lua_State *L)
{
  lua_register(L, "CreateNoise", CreateNoise);
  lua_register(L, "DestroyNoise", DestroyNoise);
  lua_register(L, "CreateMedia", CreateMedia);
  lua_register(L, "DestroyMedia", DestroyMedia);

  lua_register(L, "AttachMediaToNoise", AttachMediaToNoise);
  lua_register(L, "DetachMediaFromNoise", DetachMediaFromNoise);

  lua_register(L, "PlaySound", PlaySound);
  lua_register(L, "PauseSound", PauseSound);
  lua_register(L, "UnPauseSound", UnPauseSound);
  lua_register(L, "StopSound", StopSound);
  lua_register(L, "ReplaySound", ReplaySound);
  lua_register(L, "LoopSound", LoopSound);

  lua_register(L, "SoundIsPlaying", SoundIsPlaying);
  lua_register(L, "SoundIsLooping", SoundIsLooping);

  lua_register(L, "SetSoundVolume", SetSoundVolume);
  lua_register(L, "GetSoundVolume", GetSoundVolume);
}

} //namespace Lua

} //namespace Dusk
