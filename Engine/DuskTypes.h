#ifndef DUSKTYPESH
#define DUSKTYPESH
#include <string>
// In this file global types (and constants) are defined

namespace Dusk
{
  enum DIRECTION{FORWARD,BACKWARD,LEFT,RIGHT,UP,DOWN, TURN_LEFT, TURN_RIGHT};

  #if defined(_WIN32)
    const std::string path_sep = "\\";
  #else
    const std::string path_sep = "/";
  #endif

  //standard mesh name for non-existing objects/ items
  const std::string cErrorMesh = "ErrorMarker.mesh";
}//namespace

#endif
