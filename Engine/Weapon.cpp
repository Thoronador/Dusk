#include "Weapon.h"
#include "WeaponBase.h"
#include "DuskConstants.h"

namespace Dusk
{

Weapon::Weapon()
  : Item()
{
  m_Equipped = false;
}

Weapon::Weapon(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : Item(_ID, pos, rot, Scale)
{
  m_Equipped = false;
}

Weapon::~Weapon()
{
  disable();
}

std::string Weapon::getObjectMesh() const
{
  return WeaponBase::getSingleton().getWeaponMesh(ID);
}

ObjectTypes Weapon::getDuskType() const
{
  return otWeapon;
}

bool Weapon::canPickUp() const
{
  //we don't want to pick up equipped weapons
  return !m_Equipped;
}

bool Weapon::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Weapon::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RfWe" (reference of Weapon)
  OutStream.write((char*) &cHeaderRfWe, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    std::cout << "Weapon::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data inherited from Item
  if (!saveItemPart(OutStream))
  {
    std::cout << "Weapon::SaveToStream: ERROR while writing item data!\n";
    return false;
  }
  //Weapon has no new data members, so return here
  return OutStream.good();
}

bool Weapon::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Cannot load from stream while"
              << " weapon is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RfWe"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRfWe)
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    std::cout << "Weapon::LoadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff inherited from item
  if (!loadItemPart(InStream))
  {
    std::cout << "Weapon::LoadFromStream: ERROR while reading item data.\n";
    return false;
  }//if
  //class Weapon has no new data members, so return here
  return InStream.good();
}

} //namespace
