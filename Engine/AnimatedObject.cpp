#include "AnimatedObject.h"

namespace Dusk
{

AnimatedObject::AnimatedObject()
{
  //ctor
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
  velocity = Ogre::Vector3::ZERO;
  acceleration = Ogre::Vector3::ZERO;
  m_ApplyGravitation = true;
}

AnimatedObject::~AnimatedObject()
{
  //dtor
}

Ogre::Vector3 AnimatedObject::GetVelocity() const
{
  return velocity;
}

void AnimatedObject::SetVelocity(const Ogre::Vector3 v)
{
  velocity = v;
}

Ogre::Vector3 AnimatedObject::GetAcceleration() const
{
  return acceleration;
}

void AnimatedObject::SetAcceleration(const Ogre::Vector3 a)
{
  acceleration = a;
}

void AnimatedObject::UseGravitation(const bool use)
{
  m_ApplyGravitation = use;
}

void AnimatedObject::Move(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  if (m_ApplyGravitation)
  {
    velocity = velocity + SecondsPassed * (acceleration+Gravitation);
  }
  else
  {
    velocity = velocity + SecondsPassed * acceleration;
  }
  position = position + SecondsPassed * velocity;
  //adjust position of scene node/ entity in Ogre
  if (IsEnabled())
  {
    SetPosition(position);
  }
}

}
