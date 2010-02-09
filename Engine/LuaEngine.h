/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-09
 Purpose: LuaEngine Singleton class
          This class will handle (i.e. run) all Lua code from scripts.

 History:
     - 2010-02-08 (rev 168) - initial version (by thoronador)
     - 2010-02-09 (rev 170) - bindings for Weather and Sound classes added

 ToDo list:
     - ???

 Bugs:
     - Will most likely not compile /work with older Lua versions than 5.0.
     - Untested. If you find more bugs, then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LUAENGINE_H
#define LUAENGINE_H

#include "LuaIncludes.h"

#include <string>
#include <deque>
#include "Script.h"

namespace Dusk
{

class LuaEngine
{
  public:
    /* destructor */
    ~LuaEngine();

    /* singleton access */
    static LuaEngine& GetSingleton();

    /* runs the passed string as a Lua command and returns true on success */
    bool runString(const std::string& line);

    /* runs the Lua script in file FileName and returns true on success */
    bool runFile(const std::string& FileName);

    /* adds a script to the script queue */
    void addScript(Dusk::Script theScript);

   /* Processes the scripts on the internal queue

      parameters:
          maxEntries - The number of maximal entries to process. 0 means all.

      return value:
          the number of processed.scripts
    */
    unsigned int processScripts(unsigned int maxEntries = 0);
  private:
    /* constructor */
    LuaEngine();

    /* copy constructor - empty as usual */
    LuaEngine(const LuaEngine& op){}

    /* will register all functions of Dusk's classes in Lua, so they can be used
       in Lua scripts

       remarks:
           Not implemented yet.
    */
    void registerDusk();

    /* the Lua interpreter */
    lua_State * m_Lua;

    /* Holds the queue of scripts to process. */
    std::deque<Script> m_ScriptQueue;

  public:
    // implicitly act as a lua_State pointer - not sure, if we will ever need this
    inline operator lua_State*()
    {
      return m_Lua;
    }
}; //class

} //namespace

#endif // LUAENGINE_H
