#include "Trigger.h"

namespace Dusk
{

Trigger::~Trigger()
{
  //empty
}

/* ---- AABoxTrigger methods ---- */
AABoxTrigger::AABoxTrigger()
{
  m_Box.setExtents(0, 0, 0, 0, 0, 0);
  m_Box.setNull();
}

AABoxTrigger::AABoxTrigger(const Ogre::AxisAlignedBox& box)
{
  m_Box = box;
}

AABoxTrigger::~AABoxTrigger()
{
  //empty
}

void AABoxTrigger::setBox(const Ogre::AxisAlignedBox& box)
{
  m_Box = box;
}

const Ogre::AxisAlignedBox& AABoxTrigger::getBox() const
{
  return m_Box;
}

bool AABoxTrigger::isWithin(const TriggerObject* obj) const
{
  if (obj==NULL or m_Box.isNull())
  {
    return false;
  }
  return m_Box.intersects(obj->GetPosition());
}

/* ---- SphereTrigger methods ---- */

SphereTrigger::SphereTrigger()
{
  //set zero radius sphere, i.e. point, at origin
  m_Sphere = Ogre::Sphere(Ogre::Vector3(0,0,0), 0);
}

SphereTrigger::SphereTrigger(const Ogre::Sphere& sphere)
{
  m_Sphere = sphere;
}

SphereTrigger::~SphereTrigger()
{
  //empty
}

void SphereTrigger::setSphere(const Ogre::Sphere& sphere)
{
  m_Sphere = sphere;
}

bool SphereTrigger::isWithin(const TriggerObject* obj) const
{
  if (obj!=NULL)
  {
    return m_Sphere.intersects(obj->GetPosition());
  }
  //NULL objects are not within the sphere
  return false;
}

const Ogre::Sphere& SphereTrigger::getSphere() const
{
  return m_Sphere;
}

} //namespace
