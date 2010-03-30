#ifndef DUSKTYPESH
#define DUSKTYPESH
// In this file global types are defined

namespace Dusk
{
  enum DIRECTION{FORWARD,BACKWARD,LEFT,RIGHT,JUMP_UP,DOWN, TURN_LEFT, TURN_RIGHT};
  typedef unsigned char uint8; //8 bit unsigned integer, 0-255
  struct ColourData
  {
     uint8 red, green, blue;
  };
}//namespace

#endif
