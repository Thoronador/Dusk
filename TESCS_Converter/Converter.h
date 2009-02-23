#ifndef NO_OGRE_IN_LANDSCAPE
  //avoids using Ogre headers which we don't need for converter
  #define NO_OGRE_IN_LANDSCAPE
#endif
#include <string>

bool ScanESP(const std::string FileName, const std::string DuskFileName);
