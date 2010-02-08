#include "LuaEngine.h"
#include <iostream>

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
    lua_dostring(m_Lua, "io.write('Lua says: this is ',_VERSION,'!\n')");
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

LuaEngine& LuaEngine::GetSingleton()
{
  static LuaEngine Instance;
  return Instance;
}

bool LuaEngine::runString(const std::string& line)
{
  const int errCode = lua_dostring(m_Lua, line.c_str());
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
  return false;
}

bool LuaEngine::runFile(const std::string& FileName)
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
  return false;
}

void LuaEngine::addScript(Dusk::Script theScript)
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
    const Script nextScript = m_ScriptQueue.front();
    m_ScriptQueue.pop_front();
    if (!runString(nextScript.getStringRepresentation()))
    {
      std::cout << "LuaEngine::processScripts: ERROR while processing script "
                << toDo-maxEntries << " of " << toDo << ". Aborting.\n";
      return toDo-maxEntries;
    }
    --maxEntries;
  } //while
  return toDo;
}

void LuaEngine::registerDusk()
{
  //empty, not implemented yet
}

} //namespace
