#include "UniformMotionObject.h"

namespace Dusk
{

UniformMotionObject::UniformMotionObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
  m_Scale = 1.0f;
  entity = NULL;
  m_Direction = Ogre::Vector3::ZERO;
  m_Destination = Ogre::Vector3::ZERO;
  m_Speed = 0.0f;
  m_Travel = false;
}

/* constructor with parameter list */
UniformMotionObject::UniformMotionObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
{
  ID = _ID;
  position = pos;
  rotation = rot;
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
  //dtor
  Disable();
}

Ogre::Vector3 UniformMotionObject::GetDirection() const
{
  return m_Direction;
}

void UniformMotionObject::SetDirection(const Ogre::Vector3& direc)
{
  m_Direction = direc;
  m_Direction.normalise();
}

float UniformMotionObject::GetSpeed() const
{
  return m_Speed;
}

void UniformMotionObject::SetSpeed(const float v)
{
  m_Speed = v;
  if (v<0.0f) m_Speed = 0.0f;
}

void UniformMotionObject::TravelToDestination(const Ogre::Vector3& dest)
{
  m_Destination = dest;
  m_Travel = true;
  SetDirection(dest-GetPosition());
}

Ogre::Vector3 UniformMotionObject::GetDestination() const
{
  return m_Destination;
}

bool UniformMotionObject::IsOnTravel() const
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
    const float Distance = Ogre::Vector3(m_Destination-GetPosition()).squaredLength();
    //are we moving to fast?
    if (Ogre::Math::Sqr(m_Speed*SecondsPassed)>=Distance)
    { //finished travelling
      SetPosition(m_Destination);
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
  if (IsEnabled())
  {
    SetPosition(position);
  }
}

bool UniformMotionObject::SaveUniformMotionObjectPart(std::ofstream& output) const
{
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

bool UniformMotionObject::LoadUniformMotionObjectPart(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    std::cout << "UniformMotionObject::LoadUniformMotionObjectPart: ERROR: bad"
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
