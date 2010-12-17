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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-08
 Purpose: LuaEngine Singleton class
          This class will handle (i.e. run) all Lua code from scripts.

 History:
     - 2010-02-08 (rev 168) - initial version (by thoronador)
     - 2010-02-09 (rev 170) - bindings for Weather and Sound classes added
     - 2010-02-09 (rev 171) - bindings for DuskObject added
     - 2010-03-03 (rev 176) - extension of runString() and runFile() for error
                              messages
     - 2010-03-04 (rev 177) - small fix
                            - bindings for NPC added
     - 2010-03-08 (rev 178) - bindings for QuestLog added
     - 2010-05-21 (rev 206) - bindings for AnimatedObject added
     - 2010-05-21 (rev 207) - bindings for UniformMotionObject added
     - 2010-05-28 (rev 209) - adjustments for namespace in QuestLog's bindings
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2010-12-17 (rev 269) - new version of runString() now uses no more macros
     - 2010-12-17 (rev 270) - new version of runFile() now uses no more macros

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
    static LuaEngine& getSingleton();

    /* runs the passed string as a Lua command and returns true on success

       parameters:
           line    - string containig the Lua code which will be executed
           err_msg - pointer to a string which will contain the error message
                     in case of an error. If no error occured, the string will
                     not be changed.

       remarks:
           The parameter err_msg can be set to NULL (default), if no error
           message is required.
    */
    bool runString(const std::string& line, std::string* err_msg=NULL);

    /* runs the Lua script in file FileName and returns true on success

       parameters:
           FileName - file name of the Lua script which will be executed
           err_msg  - pointer to a string which will contain the error message
                      in case of an error. If no error occured, the string will
                      not be changed.

       remarks:
           The parameter err_msg can be set to NULL (default), if no error
           message is required.
    */
    bool runFile(const std::string& FileName, std::string* err_msg=NULL);

    /* adds a script to the script queue

       parameters:
           theScript - the script that will be added to the script queue
    */
    void addScript(const Dusk::Script& theScript);

   /* Processes the scripts on the internal queue

      parameters:
          maxEntries - The number of maximal entries to process. 0 means all.

      return value:
          the number of processed scripts
    */
    unsigned int processScripts(unsigned int maxEntries = 0);

    // implicitly act as a lua_State pointer - not sure, if we will ever need this
    inline operator lua_State*()
    {
      return m_Lua;
    }
  private:
    /* constructor */
    LuaEngine();

    /* copy constructor - empty as usual */
    LuaEngine(const LuaEngine& op){}

    /* will register all functions of Dusk's classes in Lua, so they can be used
       in Lua scripts
    */
    void registerDusk();

    /* the Lua interpreter */
    lua_State * m_Lua;

    /* Holds the queue of scripts to process. */
    std::deque<Script> m_ScriptQueue;
}; //class

} //namespace

#endif // LUAENGINE_H
