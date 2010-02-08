/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-08
 Purpose: LuaIncludes
          Summarizes all #includes specific to Lua, so we can simply do a
            #include "LuaIncludes.h"
          instead of including all the Lua headers every time a file needs
          stuff from Lua.
          If you don't see why this is a good idea, read the comments below.

 History:
     - 2010-02-08 (rev 168) - initial version (by thoronador)

 ToDo list:
     - make this work on Windows, too
     - ???

 Bugs:
     - Will most likely not compile /work with other Lua versions than 5.0.
     - will not work on Windows (yet), due to different paths
     - Untested.
       If you know what includes are needed for other Lua Versions and/or
       other platforms, feel free to tell us.
 --------------------------------------------------------------------------*/

#ifndef LUAINCLUDES_H
#define LUAINCLUDES_H

extern "C" {
  /* We use Lua 5.0 on Linux platforms.
     If you want to use a different Lua version, or use a different OS, you
     will have to change the following includes.
     These includes work on Debian 5.0 ("lenny") with package liblualib50-dev
     and its dependencies installed, but they might need to be changed for
     other systems.
  */
  #include <lua50/lua.h>
  #include <lua50/lualib.h>
  #include <lua50/lauxlib.h>
}

#endif // LUAINCLUDES_H
