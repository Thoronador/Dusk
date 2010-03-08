#include "LuaBindingsQuestLog.h"
#include "QuestLog.h"

namespace Dusk
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
      int_index = index;
    }
    if (QuestLog::GetSingleton().addQuestEntry(lua_tostring(L, 1), int_index))
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
    if (QuestLog::GetSingleton().hasQuest(lua_tostring(L, 1)))
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
    if (QuestLog::GetSingleton().isQuestFinished(lua_tostring(L, 1)))
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
      int_index = index;
    }
    if (QuestLog::GetSingleton().hasQuestEntry(lua_tostring(L, 1), int_index))
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
    lua_pushnumber(L, QuestLog::GetSingleton().getMaximumPresentIndex(lua_tostring(L, 1)));
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

} //namespace
