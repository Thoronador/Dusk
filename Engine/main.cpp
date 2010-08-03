// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files, and
// not all of them are needed here, so just include OgreException.h. Other OGRE
// headers are included in other headers/ source files, where needed.
// ----------------------------------------------------------------------------
#include <OgreException.h>

#include "Application.h"

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
    catch( Ogre::Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else

        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }

    return 0;
}
