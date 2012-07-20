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

#include "LuaBindingsNPC.h"
#include "../objects/NPC.h"
#include "../InjectionManager.h"

namespace Dusk
{
namespace Lua
{

int GetNPC(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    lua_pushlightuserdata(L, InjectionManager::getSingleton().getNPCReference(lua_tostring(L, 1)));
    return 1;
  }
  lua_pushstring(L, "GetNPC expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetHealth(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float health = npcPtr->getHealth();
      lua_pushnumber(L, health);
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetHealth expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetHealth(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      npcPtr->setHealth(lua_tonumber(L, 2));
    }
    return 0;
  }
  lua_pushstring(L, "SetHealth expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int GetLevel(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getLevel());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetLevel expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetStrength(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getStrength());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetStrength expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetAgility(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getAgility());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetAgility expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetVitality(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getVitality());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetVitality expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetIntelligence(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getIntelligence());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetIntelligence expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetWillpower(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getWillpower());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetWillpower expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetCharisma(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getCharisma());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetCharisma expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetLuck(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      lua_pushnumber(L, npcPtr->getLuck());
      return 1;
    }
    lua_pushnil(L);
    return 1;
  }
  lua_pushstring(L, "GetLuck expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetLevel(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float level = lua_tonumber(L, 2);
      if (level<0)
      {
        npcPtr->setLevel(0);
      }
      else if (level>255)
      {
        npcPtr->setLevel(255);
      }
      else
      {
        npcPtr->setLevel(static_cast<const uint8_t>(level));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetLevel expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetStrength(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float str = lua_tonumber(L, 2);
      if (str<0)
      {
        npcPtr->setStrength(0);
      }
      else if (str>255)
      {
        npcPtr->setStrength(255);
      }
      else
      {
        npcPtr->setStrength(static_cast<const uint8_t>(str));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetStrength expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetAgility(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float agi = lua_tonumber(L, 2);
      if (agi<0)
      {
        npcPtr->setAgility(0);
      }
      else if (agi>255)
      {
        npcPtr->setAgility(255);
      }
      else
      {
        npcPtr->setAgility(static_cast<const uint8_t>(agi));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetAgility expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetVitality(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float vit = lua_tonumber(L, 2);
      if (vit<0)
      {
        npcPtr->setVitality(0);
      }
      else if (vit>255)
      {
        npcPtr->setVitality(255);
      }
      else
      {
        npcPtr->setVitality(static_cast<const uint8_t>(vit));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetVitality expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetIntelligence(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float intelligence = lua_tonumber(L, 2);
      if (intelligence<0)
      {
        npcPtr->setIntelligence(0);
      }
      else if (intelligence>255)
      {
        npcPtr->setIntelligence(255);
      }
      else
      {
        npcPtr->setIntelligence(static_cast<const uint8_t>(intelligence));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetIntelligence expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetWillpower(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float will = lua_tonumber(L, 2);
      if (will<0)
      {
        npcPtr->setWillpower(0);
      }
      else if (will>255)
      {
        npcPtr->setWillpower(255);
      }
      else
      {
        npcPtr->setWillpower(static_cast<const uint8_t>(will));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetWillpower expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetCharisma(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float cha = lua_tonumber(L, 2);
      if (cha<0)
      {
        npcPtr->setCharisma(0);
      }
      else if (cha>255)
      {
        npcPtr->setCharisma(255);
      }
      else
      {
        npcPtr->setCharisma(static_cast<const uint8_t>(cha));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetCharisma expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int SetLuck(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const float luck = lua_tonumber(L, 2);
      if (luck<=0)
      {
        npcPtr->setLuck(0);
      }
      else if (luck>=255)
      {
        npcPtr->setLuck(255);
      }
      else
      {
        npcPtr->setLuck(static_cast<const uint8_t>(luck));
      }
    }
    return 0;
  }
  lua_pushstring(L, "SetLuck expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int IsFemale(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->isFemale())
        lua_pushboolean(L, 1);
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "IsFemale expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int AddItem(lua_State *L)
{
  if (lua_gettop(L)==3)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (lua_type(L, 2)!=LUA_TSTRING)
      {
        lua_pushstring(L, "AddItem expects second argument to be of type string!\n");
        lua_error(L);
        return 0;
      }
      if (lua_type(L, 3)!=LUA_TNUMBER)
      {
        lua_pushstring(L, "AddItem expects third argument to be of numerical type!\n");
        lua_error(L);
        return 0;
      }
      const std::string itemID = lua_tostring(L, 2);
      const unsigned int amount = static_cast<unsigned int>(lua_tonumber(L, 3));
      npcPtr->getInventory().addItem(itemID, amount);
    }
    return 0;
  }
  lua_pushstring(L, "AddItem expects exactly three arguments!\n");
  lua_error(L);
  return 0;
}


int RemoveItem(lua_State *L)
{
  if (lua_gettop(L)==3)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const std::string itemID = lua_tostring(L, 2);
      const unsigned int amount = static_cast<unsigned int> (lua_tonumber(L, 3));
      lua_pushnumber(L, npcPtr->getInventory().removeItem(itemID, amount));
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "RemoveItem expects exactly three arguments!\n");
  lua_error(L);
  return 0;
}

int GetItemCount(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      const std::string itemID = lua_tostring(L, 2);
      lua_pushnumber(L, npcPtr->getConstInventory().getItemCount(itemID));
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "GetItemCount expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int NPCEquip(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->equip(lua_tostring(L, 2)))
      {
        lua_pushboolean(L, 1);
      }
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "NPCEquip expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int NPCUnequip(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->unequip(lua_tostring(L, 2)))
      {
        lua_pushboolean(L, 1);
      }
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "NPCUnequip expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int NPCHasEquipped(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    const NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->hasEquipped(lua_tostring(L, 2)))
      {
        lua_pushboolean(L, 1);
      }
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "NPChasEquipped expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int NPCAttack(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->startAttack())
      {
        lua_pushboolean(L, 1);
      }
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "NPCAttack expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int NPCStopAttack(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    NPC* npcPtr = static_cast<NPC*> (lua_touserdata(L, 1));
    if (npcPtr!=NULL)
    {
      if (npcPtr->stopAttack())
      {
        lua_pushboolean(L, 1);
      }
      else lua_pushboolean(L, 0);
      return 1;
    }
    return 0;
  }
  lua_pushstring(L, "NPCStopAttack expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

void registerNPC(lua_State *L)
{
  lua_register(L, "GetNPC", GetNPC);

  lua_register(L, "GetHealth", GetHealth);
  lua_register(L, "SetHealth", SetHealth);

  lua_register(L, "GetLevel", GetLevel);
  lua_register(L, "GetStrength", GetStrength);
  lua_register(L, "GetAgility", GetAgility);
  lua_register(L, "GetVitality", GetVitality);
  lua_register(L, "GetIntelligence", GetIntelligence);
  lua_register(L, "GetWillpower", GetWillpower);
  lua_register(L, "GetCharisma", GetCharisma);
  lua_register(L, "GetLuck", GetLuck);

  lua_register(L, "SetLevel", SetLevel);
  lua_register(L, "SetStrength", SetStrength);
  lua_register(L, "SetAgility", SetAgility);
  lua_register(L, "SetVitality", SetVitality);
  lua_register(L, "SetIntelligence", SetIntelligence);
  lua_register(L, "SetWillpower", SetWillpower);
  lua_register(L, "SetCharisma", SetCharisma);
  lua_register(L, "SetLuck", SetLuck);

  lua_register(L, "IsFemale", IsFemale);

  lua_register(L, "Equip", NPCEquip);
  lua_register(L, "Unequip", NPCUnequip);
  lua_register(L, "HasEquipped", NPCHasEquipped);

  lua_register(L, "Attack", NPCAttack);
  lua_register(L, "StopAttack", NPCStopAttack);

  lua_register(L, "GetItemCount", GetItemCount);
  lua_register(L, "AddItem", AddItem);
  lua_register(L, "RemoveItem", RemoveItem);
}

} //namespace Lua
} //namespace Dusk
