#include "NPC.h"
#include "Settings.h"

namespace Dusk
{

NPC::NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale)
 : AnimatedObject(_ID, pos, rot, Scale)
{
  m_Health = 10.0f; //arbitrary value to make sure NPC has at least some health
                    //and is not dead from the beginning
  //all attributes to one
  m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Charisma=m_Luck=1;
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

uint8 NPC::getLevel() const
{
  return m_Level;
}

uint8 NPC::getStrength() const
{
  return m_Strength;
}

uint8 NPC::getAgility() const
{
  return m_Strength;
}

uint8 NPC::getVitality() const
{
  return m_Vitality;
}

uint8 NPC::getIntelligence() const
{
  return m_Intelligence;
}

uint8 NPC::getCharisma() const
{
  return m_Charisma;
}

uint8 NPC::getLuck() const
{
  return m_Luck;
}

void NPC::setLevel(const uint8 new_level)
{
  m_Level = new_level;
}


void NPC::setStrength(const uint8 str)
{
  m_Strength = str;
}

void NPC::setAgility(const uint8 agi)
{
  m_Agility = agi;
}

void NPC::setVitality(const uint8 vit)
{
  m_Vitality = vit;
}

void NPC::setIntelligence(const uint8 intelligence)
{
  m_Intelligence = intelligence;
}

void NPC::setCharisma(const uint8 cha)
{
  m_Charisma = cha;
}

void NPC::setLuck(const uint8 luck)
{
  m_Luck = luck;
}

Inventory& NPC::getInventory()
{
  return m_Inventory;
}

unsigned int NPC::getMaxEncumbrance() const
{
  return cBaseEncumbrance + cEncumbranceStrengthCoefficient*m_Strength;
}

bool NPC::SaveToStream(std::ofstream& OutStream) const
{
  //not implemented yet
  std::cout << "NPC::SaveToStream: Hint: not implemented yet.\n";
  return false;
}

bool NPC::LoadFromStream(std::ifstream& InStream)
{
  //not implemented yet
  std::cout << "NPC::LoadFromStream: Hint: not implemented yet.\n";
  return false;
}

} //namespace
