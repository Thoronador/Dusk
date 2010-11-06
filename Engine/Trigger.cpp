/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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
  return m_Box.intersects(obj->getPosition());
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
    return m_Sphere.intersects(obj->getPosition());
  }
  //NULL objects are not within the sphere
  return false;
}

const Ogre::Sphere& SphereTrigger::getSphere() const
{
  return m_Sphere;
}

} //namespace
