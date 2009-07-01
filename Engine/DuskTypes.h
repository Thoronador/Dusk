#ifndef DUSKTYPESH
#define DUSKTYPESH
// In this file are global types (and constants) defined

namespace Dusk
{
  enum DIRECTION{FORWARD,BACKWARD,LEFT,RIGHT,UP,DOWN, TURN_LEFT, TURN_RIGHT};

  #if defined(_WIN32)
    const std::string path_sep = "\\";
  #else
    const std::string path_sep = "/";
  #endif
}//namespace

#endif
