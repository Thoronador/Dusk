#include "LuaBindingsWeather.h"
#include "Weather.h"

namespace Dusk
{

int StartLinearFog(lua_State *L)
{
  if (lua_gettop(L)==2)
  {
    Weather::getSingelton().startLinearFog(lua_tonumber(L, 1), lua_tonumber(L, 2));
    return 0;
  }
  lua_pushstring(L, "StartLinearFog expects exactly two arguments!\n");
  lua_error(L);
  return 0;
}

int StartExponentialFog(lua_State *L)
{
  if (lua_gettop(L)==1)
  {
    Weather::getSingelton().startExponentialFog(lua_tonumber(L, 1));
    return 0;
  }
  lua_pushstring(L, "StartExponentialFog expects exactly one argument!\n");
  lua_error(L);
  return 0;
}

int StopFog(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    Weather::getSingelton().stopFog();
    return 0;
  }
  lua_pushstring(L, "IsFoggy expects exactly no argument!\n");
  lua_error(L);
  return 0;
}

int IsFoggy(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if ( Weather::getSingelton().isFoggy())
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "IsFoggy expects no argument!\n");
  lua_error(L);
  return 0;
}

int IsLinearFog(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isLinearFog())
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "IsLinearFog expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int SetFogColour(lua_State *L)
{
  if (lua_gettop(L)==3)
  {
    Weather::getSingelton().setFogColour(lua_tonumber(L, 1), lua_tonumber(L, 2),
                                         lua_tonumber(L, 3));
    return 0;
  }
  lua_pushstring(L, "SetFogColour expects exactly three arguments!\n");
  lua_error(L);
  return 0;
}

int GetFogColour(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    float r,g,b;
    Weather::getSingelton().getFogColour(&r, &g, &b);
    lua_pushnumber(L, r);
    lua_pushnumber(L, g);
    lua_pushnumber(L, b);
    return 3;
  }
  lua_pushstring(L, "GetFogColour expects no arguments!\n");
  lua_error(L);
  return 0;
}

int ToggleFog(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isFoggy())
    {
      Weather::getSingelton().stopFog();
    }
    else
    {
      Weather::getSingelton().startLinearFog(50.0, 500.0);
    }
    return 0;
  }
  lua_pushstring(L, "ToggleFog expects no arguments!\n");
  lua_error(L);
  return 0;
}

int StartSnow(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    Weather::getSingelton().startSnow();
    return 0;
  }
  lua_pushstring(L, "StartSnow expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int StopSnow(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    Weather::getSingelton().stopSnow();
    return 0;
  }
  lua_pushstring(L, "StopSnow expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int IsSnowing(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isSnowing())
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "IsSnowing expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int ToggleSnow(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isSnowing())
    {
      Weather::getSingelton().stopSnow();
    }
    else
    {
      Weather::getSingelton().startSnow();
    }
    return 0;
  }
  lua_pushstring(L, "ToggleSnow expects no arguments!\n");
  lua_error(L);
  return 0;
}

int StartRain(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    Weather::getSingelton().startRain();
    return 0;
  }
  lua_pushstring(L, "StartRain expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int StopRain(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    Weather::getSingelton().stopRain();
    return 0;
  }
  lua_pushstring(L, "StopRain expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int IsRaining(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isRaining())
    {
      lua_pushboolean(L, 1);
    }
    else
    {
      lua_pushboolean(L, 0);
    }
    return 1;
  }
  lua_pushstring(L, "IsRaining expects no arguments at all!\n");
  lua_error(L);
  return 0;
}

int ToggleRain(lua_State *L)
{
  if (lua_gettop(L)==0)
  {
    if (Weather::getSingelton().isRaining())
    {
      Weather::getSingelton().stopRain();
    }
    else
    {
      Weather::getSingelton().startRain();
    }
    return 0;
  }
  lua_pushstring(L, "ToggleRain expects no arguments!\n");
  lua_error(L);
  return 0;
}

void registerWeather(lua_State *L)
{
  lua_register(L, "StartLinearFog", StartLinearFog);
  lua_register(L, "StartExponentialFog", StartExponentialFog);
  lua_register(L, "StopFog", StopFog);
  lua_register(L, "IsFoggy", IsFoggy);
  lua_register(L, "IsLinearFog", IsLinearFog);
  lua_register(L, "toggleFog", ToggleFog);

  lua_register(L, "SetFogColour", SetFogColour);
  lua_register(L, "GetFogColour", GetFogColour);

  lua_register(L, "StartSnow", StartSnow);
  lua_register(L, "StopSnow", StopSnow);
  lua_register(L, "IsSnowing", IsSnowing);
  lua_register(L, "toggleSnow", ToggleSnow);

  lua_register(L, "StartRain", StartRain);
  lua_register(L, "StopRain", StopRain);
  lua_register(L, "IsRaining", IsRaining);
  lua_register(L, "toggleRain", ToggleRain);
}

} //namespace
