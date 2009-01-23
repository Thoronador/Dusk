#include "DuskObject.h"

namespace Dusk{

DuskObject::DuskObject()
{
  //ctor
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
}

DuskObject::~DuskObject()
{
    //dtor
}

}
