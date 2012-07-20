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

#include "UniformMotionObject.h"
#include "../Messages.h"

namespace Dusk
{

UniformMotionObject::UniformMotionObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  m_Rotation = Ogre::Quaternion::IDENTITY;
  m_Scale = 1.0f;
  entity = NULL;
  m_Direction = Ogre::Vector3::ZERO;
  m_Destination = Ogre::Vector3::ZERO;
  m_Speed = 0.0f;
  m_Travel = false;
}

/* constructor with parameter list */
UniformMotionObject::UniformMotionObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
{
  ID = _ID;
  position = pos;
  m_Rotation = rot;
  if (m_Scale>0.0f)
  {
    m_Scale = Scale;
  } else {
    m_Scale = 1.0f;
  }
  entity = NULL;
  m_Direction = Ogre::Vector3::ZERO;
  m_Destination = Ogre::Vector3::ZERO;
  m_Speed = 0.0f;
  m_Travel = false;
}

UniformMotionObject::~UniformMotionObject()
{
  //destructor
  disable();
}

Ogre::Vector3 UniformMotionObject::getDirection() const
{
  return m_Direction;
}

void UniformMotionObject::setDirection(const Ogre::Vector3& direc)
{
  m_Direction = direc;
  m_Direction.normalise();
}

float UniformMotionObject::getSpeed() const
{
  return m_Speed;
}

void UniformMotionObject::setSpeed(const float v)
{
  m_Speed = v;
  if (v<0.0f) m_Speed = 0.0f;
}

bool UniformMotionObject::isMoving() const
{
  return (m_Speed*m_Direction).squaredLength()>=0.0001;
}

void UniformMotionObject::travelToDestination(const Ogre::Vector3& dest)
{
  m_Destination = dest;
  m_Travel = true;
  setDirection(dest-getPosition());
}

Ogre::Vector3 UniformMotionObject::getDestination() const
{
  return m_Destination;
}

bool UniformMotionObject::isOnTravel() const
{
  return m_Travel;
}

void UniformMotionObject::injectTime(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  if (m_Travel)
  {
    const float Distance = Ogre::Vector3(m_Destination-getPosition()).squaredLength();
    //are we moving to fast?
    if (Ogre::Math::Sqr(m_Speed*SecondsPassed)>=Distance)
    { //finished travelling
      setPosition(m_Destination);
      m_Travel = false;
      m_Direction = Ogre::Vector3::ZERO;
      m_Speed = 0.0f;
    }
    else
    {
      position = position + SecondsPassed*m_Speed*m_Direction;
    }
  }
  else if (m_Direction!=Ogre::Vector3::ZERO)
  {
    position = position + SecondsPassed*m_Speed*m_Direction;
  }
  //adjust position of scene node/ entity in Ogre
  if (isEnabled())
  {
    setPosition(position);
  }
}

bool UniformMotionObject::saveUniformMotionObjectPart(std::ofstream& output) const
{
  if (!output.good())
  {
    DuskLog() << "UniformMotionObject::saveUniformMotionObjectPart: ERROR: bad"
              << " stream given!\n";
    return false;
  }
  //write direction and destination, and speed
  // -- direction
  float xyz = m_Direction.x;
  output.write((char*) &xyz, sizeof(float));
  xyz = m_Direction.y;
  output.write((char*) &xyz, sizeof(float));
  xyz = m_Direction.z;
  output.write((char*) &xyz, sizeof(float));
  // -- destination
  xyz = m_Destination.x;
  output.write((char*) &xyz, sizeof(float));
  xyz = m_Destination.y;
  output.write((char*) &xyz, sizeof(float));
  xyz = m_Destination.z;
  output.write((char*) &xyz, sizeof(float));
  // -- speed
  output.write((char*) &m_Speed, sizeof(float));
  // -- travelling flag
  output.write((char*) &m_Travel, sizeof(bool));
  return output.good();
}

bool UniformMotionObject::loadUniformMotionObjectPart(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "UniformMotionObject::loadUniformMotionObjectPart: ERROR: bad"
              << " stream given!\n";
    return false;
  }
  float f_temp;
  // -- direction
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.z = f_temp;
  // -- destination
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.z = f_temp;
  //  -- speed
  InStream.read((char*) &m_Speed, sizeof(float));
  // -- travelling flag
  InStream.read((char*) &m_Travel, sizeof(float));
  return InStream.good();
}

} //namespace
