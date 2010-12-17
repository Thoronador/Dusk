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

#include "LuaEngine.h"
#include <iostream>

#include "LuaBindingsSound.h"
#include "LuaBindingsWeather.h"
#include "LuaBindingsObject.h"
#include "LuaBindingsUniformMotion.h"
#include "LuaBindingsAnimated.h"
#include "LuaBindingsNPC.h"
#include "LuaBindingsQuestLog.h"
#include "Messages.h"

namespace Dusk
{

LuaEngine::LuaEngine()
{
  //empty
  m_Lua = NULL;
  m_Lua = lua_open();
  if (m_Lua!=NULL)
  {
    luaopen_base(m_Lua);
    luaopen_io(m_Lua);
    luaopen_string(m_Lua);
    luaopen_math(m_Lua);
    luaopen_loadlib(m_Lua);
    DuskLog() << "LuaEngine started, running " << LUA_VERSION << ".\n";
    runString("io.write('Lua says: this is ',_VERSION,'!\\n')");
    registerDusk();
  }
  else
  {
    DuskLog() << "LuaEngine: ERROR: lua_open() failed!\n";
  }
  m_ScriptQueue = std::deque<Script>();
}

LuaEngine::~LuaEngine()
{
  //empty
  lua_close(m_Lua);
  m_ScriptQueue.clear();
  DuskLog() << "LuaEngine stopped.\n";
}

LuaEngine& LuaEngine::getSingleton()
{
  static LuaEngine Instance;
  return Instance;
}

bool LuaEngine::runString(const std::string& line, std::string* err_msg)
{
  //load chunk and push it onto stack
  #ifdef DUSK_LUA51
  //Lua 5.1 stuff
  int errCode = luaL_loadstring(m_Lua, line.c_str());
  #elif defined(DUSK_LUA50)
  //Lua 5.0 stuff
  int errCode = luaL_loadbuffer(m_Lua, line.c_str(), line.length(), line.c_str());
  #else
    #error "LuaEngine could not detect a known Lua version!"
  #endif
  switch (errCode)
  {
    case 0: //all went fine here
         break;
    case LUA_ERRSYNTAX:
         DuskLog() << "LuaEngine::runString: ERROR during pre-compilation.\n";
         break;
    case LUA_ERRMEM:
         DuskLog() << "LuaEngine::runString: ERROR: memory allocation "
                   << "failed.\n";
         break;
    default:
         DuskLog() << "LuaEngine::runString: an unknown ERROR occured while "
                   << "loading the string.\n";
         break;
  } //swi
  /*get lua error message, which was pushed onto stack by luaL_loadstring() or
    luaL_loadbuffer() */
  if (errCode != 0)
  {
    DuskLog() << "Lua's error message: \"" << lua_tostring(m_Lua, -1) <<"\"\n";
    std::cout.flush();
    if (err_msg!=NULL)
    {
      *err_msg = std::string(lua_tostring(m_Lua, -1));
    }
    return false;
  }//if

  //call/execute the loaded chunk
  // call with zero arguments (0), push all results (MULTRET), and use the
  //  standard error function (0)
  errCode = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);
  switch (errCode)
  {
    case 0: //all went fine here
         return true;
         break;
    case LUA_ERRRUN:
         DuskLog() << "LuaEngine::runString: ERROR while running the chunk.\n";
         break;
    case LUA_ERRERR:
         DuskLog() << "LuaEngine::runString: ERROR while running the error "
                   << "handling function.\n";
         break;
    case LUA_ERRMEM:
         DuskLog() << "LuaEngine::runString: ERROR: memory allocation "
                   << "failed.\n";
         break;
    default:
         DuskLog() << "LuaEngine::runString: an unknown ERROR occured during "
                   << "protected function call.\n";
         break;
  } //swi
  /* get lua error message, which was pushed onto stack by lua_pcall() */
  DuskLog() << "Lua's error message: \"" << lua_tostring(m_Lua, -1) <<"\"\n";
  std::cout.flush();
  if (err_msg!=NULL)
  {
    *err_msg = std::string(lua_tostring(m_Lua, -1));
  }
  return false;
}

bool LuaEngine::runFile(const std::string& FileName, std::string* err_msg)
{
  int errCode = luaL_loadfile(m_Lua, FileName.c_str());
  switch (errCode)
  {
    case 0: //all went fine here
         break;
    case LUA_ERRSYNTAX:
         DuskLog() << "LuaEngine::runFile: ERROR during pre-compilation.\n";
         break;
    case LUA_ERRMEM:
         DuskLog() << "LuaEngine::runFile: ERROR: memory allocation failed.\n";
         break;
    case LUA_ERRFILE:
         DuskLog() << "LuaEngine::runFile: ERROR while opening the file \""
                   << FileName << "\".\n";
         break;
    default:
         DuskLog() << "LuaEngine::runFile: an unknown ERROR occured while "
                   << "loading the file \""<<FileName<<"\".\n";
         break;
  } //swi
  /*get Lua's error message, which was pushed onto stack by lua_load() */
  if (errCode!=0)
  {
    DuskLog() << "Lua's error message: \"" << lua_tostring(m_Lua, -1) <<"\"\n";
    if (err_msg!=NULL)
    {
      *err_msg = std::string(lua_tostring(m_Lua, -1));
    }
    return false;
  }//if

  //call/execute the loaded chunk
  // call with zero arguments (0), push all results (MULTRET), and use the
  //  standard error function (0)
  errCode = lua_pcall(m_Lua, 0, LUA_MULTRET, 0);
  switch (errCode)
  {
    case 0: //all went fine here
         return true;
         break;
    case LUA_ERRRUN:
         DuskLog() << "LuaEngine::runFile: ERROR while running the chunk.\n";
         break;
    case LUA_ERRERR:
         DuskLog() << "LuaEngine::runFile: ERROR while running the error "
                   << "handling function.\n";
         break;
    case LUA_ERRMEM:
         DuskLog() << "LuaEngine::runFile: ERROR: memory allocation failed.\n";
         break;
    default:
         DuskLog() << "LuaEngine::runFile: an unknown ERROR occured during "
                   << "protected function call.\n";
         break;
  } //swi
  /* get Lua's error message, which was pushed onto stack by lua_pcall() */
  DuskLog() << "Lua's error message: " << lua_tostring(m_Lua, -1) <<"\n";
  std::cout.flush();
  if (err_msg!=NULL)
  {
    *err_msg = std::string(lua_tostring(m_Lua, -1));
  }
  return false;
}

void LuaEngine::addScript(const Dusk::Script& theScript)
{
  m_ScriptQueue.push_back(theScript);
}

unsigned int LuaEngine::processScripts(unsigned int maxEntries)
{
  unsigned int toDo = m_ScriptQueue.size();
  if (maxEntries!=0 and maxEntries<toDo)
  {
    toDo = maxEntries;
  }
  maxEntries = toDo;
  while (maxEntries>0)
  {
    if (!runString(m_ScriptQueue.front().getStringRepresentation()))
    {
      m_ScriptQueue.pop_front();
      DuskLog() << "LuaEngine::processScripts: ERROR while processing script "
                << toDo-maxEntries << " of " << toDo << ". Aborting.\n";
      return toDo-maxEntries;
    }
    m_ScriptQueue.pop_front();
    --maxEntries;
  } //while
  return toDo;
}

void LuaEngine::registerDusk()
{
  registerSound(m_Lua);
  Lua::registerWeather(m_Lua);
  Lua::registerObject(m_Lua);
  Lua::registerMotion(m_Lua);
  Lua::registerAnimated(m_Lua);
  Lua::registerNPC(m_Lua);
  Lua::registerQuestLog(m_Lua);
}

} //namespace
