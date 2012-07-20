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

#include "LuaBindingsQuestLog.h"
#include "../QuestLog.h"

namespace Dusk
{

namespace Lua
{

int AddQuestEntry(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    const double index = lua_tonumber(L, 2);
    unsigned int int_index;
    if (index<=0.0f)
    {
      int_index = 0;
    }
    else
    {
      int_index = static_cast<unsigned int>(index);
    }
    if (QuestLog::getSingleton().addQuestEntry(lua_tostring(L, 1), int_index))
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "AddQuestEntry expects two arguments!\n");
  lua_error(L);
  return 0;
}

int HasQuest(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    if (QuestLog::getSingleton().hasQuest(lua_tostring(L, 1)))
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "HasQuest expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int IsQuestFinished(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    if (QuestLog::getSingleton().isQuestFinished(lua_tostring(L, 1)))
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "IsQuestFinished expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int HasQuestEntry(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    const double index = lua_tonumber(L, 2);
    unsigned int int_index;
    if (index<=0.0f)
    {
      int_index = 0;
    }
    else
    {
      int_index = static_cast<unsigned int>(index);
    }
    if (QuestLog::getSingleton().hasQuestEntry(lua_tostring(L, 1), int_index))
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "HasQuestEntry expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int GetMaximumPresentIndex(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    lua_pushnumber(L, QuestLog::getSingleton().getMaximumPresentIndex(lua_tostring(L, 1)));
    return 1;
  }
  lua_pushstring(L, "GetMaximumPresentIndex expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

void registerQuestLog(lua_State *L)
{
  lua_register(L, "AddQuestEntry", AddQuestEntry);
  lua_register(L, "HasQuest", HasQuest);
  lua_register(L, "IsQuestFinished", IsQuestFinished);
  lua_register(L, "HasQuestEntry", HasQuestEntry);
  lua_register(L, "GetMaximumPresentIndex", GetMaximumPresentIndex);
}

} //namespace Lua

} //namespace Dusk
