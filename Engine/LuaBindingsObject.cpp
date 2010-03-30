#include "LuaBindingsObject.h"
#include "DuskObject.h"
#include "ObjectData.h"
#include "API.h"

namespace Dusk
{

int GetObject(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    lua_pushlightuserdata(L, ObjectData::GetSingleton().GetObjectByID(lua_tostring(L, 1)));
    return 1;
  }
  lua_pushstring(L, "GetObject expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int Enable(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      if (objPtr->Enable(getAPI().getOgreSceneManager()))
      {
        lua_pushboolean(L, 1);
        return 1;
      }
    }
    lua_pushboolean(L, 0);
    return 1;
  }
  lua_pushstring(L, "EnableObject expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int Disable(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      if (objPtr->Disable())
      {
        lua_pushboolean(L, 1);
        return 1;
      }
    }
    lua_pushboolean(L, 0);
    return 1;
  }
  lua_pushstring(L, "DisableObject expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int IsEnabled(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      if (objPtr->IsEnabled())
      {
        lua_pushboolean(L, 1);
        return 1;
      }
    }
    lua_pushboolean(L, 0);
    return 1;
  }
  lua_pushstring(L, "IsEnabled expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int GetObjectPosition(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      const Ogre::Vector3 vec = objPtr->GetPosition();
      lua_pushnumber(L, vec.x);
      lua_pushnumber(L, vec.y);
      lua_pushnumber(L, vec.z);
      return 3;
    }
    lua_pushnil(L);
    lua_pushnil(L);
    lua_pushnil(L);
    return 3;
  }
  lua_pushstring(L, "GetObjectPosition expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetObjectPosition(lua_State *L)
{
  if (lua_gettop(L)==4)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      objPtr->SetPosition(Ogre::Vector3(lua_tonumber(L, 2), lua_tonumber(L, 3),
                                        lua_tonumber(L, 4)));
    }
    return 0;
  }
  lua_pushstring(L, "SetObjectPosition expects exactly four arguments!\n");
  lua_error(L);
  return 0;
}

int GetObjectRotation(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      const Ogre::Vector3 vec = objPtr->GetRotation();
      lua_pushnumber(L, vec.x);
      lua_pushnumber(L, vec.y);
      lua_pushnumber(L, vec.z);
      return 3;
    }
    lua_pushnil(L);
    lua_pushnil(L);
    lua_pushnil(L);
    return 3;
  }
  lua_pushstring(L, "GetObjectRotation expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int SetObjectRotation(lua_State *L)
{
  if (lua_gettop(L)==4)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      objPtr->SetRotation(Ogre::Vector3(lua_tonumber(L, 2), lua_tonumber(L, 3),
                                        lua_tonumber(L, 4)));
    }
    return 0;
  }
  lua_pushstring(L, "SetObjectRotation expects exactly four arguments!\n");
  lua_error(L);
  return 0;
}

int GetScale(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      lua_pushnumber(L, objPtr->GetScale());
    }
    else
    {
      lua_pushnil(L);
    }
    return 1;
  }
  lua_pushstring(L, "GetScale expects exactly one argument!\n");
  lua_error(L);
  return 0;
}


int SetScale(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    DuskObject* objPtr = static_cast<DuskObject*> (lua_touserdata(L, 1));
    if (objPtr!=NULL)
    {
      if (objPtr->SetScale(lua_tonumber(L, 2)))
      {
        lua_pushboolean(L, 1);
        return 1;
      }
    }
    lua_pushboolean(L, 0);
    return 0;
  }
  lua_pushstring(L, "SetScale expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

void registerObject(lua_State *L)
{
  lua_register(L, "GetObject", GetObject);

  lua_register(L, "EnableObject", Enable);
  lua_register(L, "DisableObject", Disable);
  lua_register(L, "IsEnabled", IsEnabled);

  lua_register(L, "GetObjectPosition", GetObjectPosition);
  lua_register(L, "SetObjectPosition", SetObjectPosition);
  lua_register(L, "GetObjectRotation", GetObjectRotation);
  lua_register(L, "SetObjectRotation", SetObjectRotation);

  lua_register(L, "GetObjectScale", GetScale);
  lua_register(L, "SetObjectScale", SetScale);
}

} //namespace