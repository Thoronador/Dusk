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

Ogre::Vector3 DuskObject::GetPosition() const
{
  return position;
}

Ogre::Vector3 DuskObject::GetRotation() const
{
  return rotation;
}

void DuskObject::SetPosition(const Ogre::Vector3 pos)
{
  position = pos;
}

void DuskObject::SetRotation(const Ogre::Vector3 rot)
{
  rotation = rot;
}

}
