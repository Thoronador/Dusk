/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2008, 2010, 2012 ssj5000, DaSteph, walljumper, thoronador

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

// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files, and
// not all of them are needed here, so just include OgreException.h. Other OGRE
// headers are included in other headers/ source files, where needed.
// ----------------------------------------------------------------------------
#include <OgreException.h>

#include "Application.h"
#include "DuskExceptions.h"

// ----------------------------------------------------------------------------
// Main function, just boots the application object
// ----------------------------------------------------------------------------

int main(int argc, char **argv)
{
    std::string pluginFileName = "";

    if (argc>1)
    {
      pluginFileName = std::string(argv[1]);
    }
    else
    {
      //no plugin file name passed, so let's have a wild guess
      // (that might be wrong, but is still better than nothing)
      #if defined(_WIN32)
      pluginFileName = "plugins-windows-release.cfg";
      #elif defined(__linux__) || defined(linux)
      pluginFileName = "plugins-linux.cfg";
      #else
        #error "Unknown operating system!"
      #endif
    }

    // Create application object
    Dusk::Application app;

    try
    {
        app.go(pluginFileName);
    }
    catch(Dusk::IDNotFound& e)
    {
      #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox(NULL, e.what().c_str(), "A Dusk-specific exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
      #else
      std::cerr << "A Dusk-specific exception has occured: " << e.what();
      #endif
    }
    catch( Ogre::Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An Ogre exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else

        std::cerr << "An Ogre exception has occured: " << e.getFullDescription();
#endif
    }
    catch(...)
    {
      #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox(NULL, "unknown exception", "An unknown exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
      #else
      std::cerr << "An unknown exception has occured!\n";
      #endif
    }

    return 0;
}
