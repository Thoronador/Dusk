// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------
#include <Ogre.h>

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
      #else
      pluginFileName = "plugins-linux.cfg";
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
