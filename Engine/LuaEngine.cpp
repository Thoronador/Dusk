#include "LuaEngine.h"
#include <iostream>

#include "LuaBindingsSound.h"
#include "LuaBindingsWeather.h"
#include "LuaBindingsObject.h"
#include "LuaBindingsUniformMotion.h"
#include "LuaBindingsAnimated.h"
#include "LuaBindingsNPC.h"
#include "LuaBindingsQuestLog.h"

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
    std::cout << "LuaEngine started, running " << LUA_VERSION << ".\n";
    lua_dostring(m_Lua, "io.write('Lua says: this is ',_VERSION,'!\\n')");
    registerDusk();
  }
  else
  {
    std::cout << "LuaEngine: ERROR: lua_open() failed!\n";
  }
  m_ScriptQueue = std::deque<Script>();
}

LuaEngine::~LuaEngine()
{
  //empty
  lua_close(m_Lua);
  m_ScriptQueue.clear();
  std::cout << "LuaEngine stopped.\n";
}

LuaEngine& LuaEngine::getSingleton()
{
  static LuaEngine Instance;
  return Instance;
}

bool LuaEngine::runString(const std::string& line, std::string* err_msg)
{
  const int errCode = lua_dostring(m_Lua, line.c_str());
  std::cout.flush();
  switch (errCode)
  {
    case 0: //all went fine here
         return true;
         break;
    case LUA_ERRRUN:
         std::cout << "LuaEngine::runString: ERROR while running the chunk.\n";
         break;
    case LUA_ERRSYNTAX:
         std::cout << "LuaEngine::runString: ERROR during pre-compilation.\n";
         break;
    case LUA_ERRMEM:
         std::cout << "LuaEngine::runString: ERROR: memory allocation "
                   << "failed.\n"; break;
    default:
         std::cout << "LuaEngine::runString: an ERROR occured.\n"; break;
  } //swi
  /*get lua error message, which was pushed onto stack by lua_load() or
    lua_pcall() (they are called by macro lua_dofile)
  */
  std::cout << "Lua's error message: " << lua_tostring(m_Lua, -1) <<"\n";
  std::cout.flush();
  if (err_msg!=NULL)
  {
    *err_msg = std::string(lua_tostring(m_Lua, -1));
  }
  return false;
}

bool LuaEngine::runFile(const std::string& FileName, std::string* err_msg)
{
  const int errCode = lua_dofile(m_Lua, FileName.c_str());
  switch (errCode)
  {
    case 0: //all went fine here
         return true;
         break;
    case LUA_ERRRUN:
         std::cout << "LuaEngine::runFile: ERROR while running the "
                   << "chunk.\n"; break;
    case LUA_ERRSYNTAX:
         std::cout << "LuaEngine::runFile: ERROR during pre-compilation.\n";
         break;
    case LUA_ERRMEM:
         std::cout << "LuaEngine::runFile: ERROR: memory allocation failed.\n";
         break;
    case LUA_ERRFILE:
         std::cout << "LuaEngine::runFile: ERROR while opening the file \""
                   << FileName << "\".\n"; break;
    default:
         std::cout << "LuaEngine::runFile: an ERROR occured.\n"; break;
  } //swi
  /*get lua error message, which was pushed onto stack by lua_load() or
    lua_pcall() (they are called by macro lua_dofile)
  */
  std::cout << "Lua's error message: " << lua_tostring(m_Lua, -1) <<"\n";
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
      std::cout << "LuaEngine::processScripts: ERROR while processing script "
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
