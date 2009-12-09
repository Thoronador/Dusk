#include "NPC.h"

namespace Dusk
{

NPC::NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale)
 : AnimatedObject(_ID, pos, rot, Scale)
{
  m_Health = 10.0f; //arbitrary value to make sure NPC has at least some health
                    //and is not dead from the beginning
}

NPC::~NPC()
{
  //empty
}

ObjectTypes NPC::GetType() const
{
  return otNPC;
}

float NPC::getHealth() const
{
  return m_Health;
}

void NPC::setHealth(const float new_health)
{
  m_Health = new_health;
  if (m_Health<0.0f) m_Health = 0.0f;
}
bool NPC::isAlive() const
{
  return (m_Health<=0.0f);
}

} //namespace
