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

#include "LuaBindingsUniformMotion.h"
#include "UniformMotionObject.h"

namespace Dusk
{

namespace Lua
{

int GetDirection(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      const Ogre::Vector3 vec = umPtr->getDirection();
      lua_pushnumber(L, vec.x);
      lua_pushnumber(L, vec.y);
      lua_pushnumber(L, vec.z);
      return 3;
    }
    lua_pushstring(L, "GetUMODirection() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "GetUMODirection() expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetDirection(lua_State *L)
{
  if (lua_gettop(L)==4)
  {
    UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      umPtr->setDirection(Ogre::Vector3(lua_tonumber(L, 2), lua_tonumber(L, 3),
                                        lua_tonumber(L, 4)));
      return 0;
    }
    lua_pushstring(L, "SetDirection() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "SetDirection() expects exactly four arguments!\n");
  lua_error(L);
  return 0;
}

int GetSpeed(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      lua_pushnumber(L, umPtr->getSpeed());
      return 1;
    }
    lua_pushstring(L, "UMOGetSpeed() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "UMOGetSpeed() expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetSpeed(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      umPtr->setSpeed(lua_tonumber(L, 2));
      return 0;
    }
    lua_pushstring(L, "SetUMOSpeed() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "SetUMOSpeed() expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int GetDestination(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      const Ogre::Vector3 vec = umPtr->getDestination();
      lua_pushnumber(L, vec.x);
      lua_pushnumber(L, vec.y);
      lua_pushnumber(L, vec.z);
      return 3;
    }
    lua_pushstring(L, "GetUMODestination() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "GetUMODestination() expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int TravelToDestination(lua_State *L)
{
  if (lua_gettop(L)==4)
  {
    UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      umPtr->travelToDestination(Ogre::Vector3(lua_tonumber(L, 2), lua_tonumber(L, 3),
                                               lua_tonumber(L, 4)));
      return 0;
    }
    lua_pushstring(L, "TravelToDestination() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "TravelToDestination() expects exactly four arguments!\n");
  lua_error(L);
  return 0;
}

int IsOnTravel(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const UniformMotionObject* umPtr = static_cast<UniformMotionObject*> (lua_touserdata(L, 1));
    if (umPtr!=NULL)
    {
      if (umPtr->isOnTravel())
        lua_pushboolean(L, 1);
      else lua_pushboolean(L, 0);
      return 1;
    }
    lua_pushstring(L, "IsOnTravel() got NULL for object pointer!\n");
    lua_error(L);
    return 0;
  }
  lua_pushstring(L, "IsOnTravel() expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

void registerMotion(lua_State *L)
{
  lua_register(L, "GetMotionDirection", GetDirection);
  lua_register(L, "SetMotionDirection", SetDirection);
  lua_register(L, "GetMotionSpeed", GetSpeed);
  lua_register(L, "SetMotionSpeed", SetSpeed);
  lua_register(L, "GetMotionDestination", GetDestination);
  lua_register(L, "SetTravelDestination", TravelToDestination);
  lua_register(L, "IsOnTravel", IsOnTravel);
}

} //namespace Lua

} //namespace Dusk
