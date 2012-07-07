/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2012 thoronador

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

#include "Weapon.h"
#include "WeaponRecord.h"
#include "Database.h"
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

Weapon::Weapon()
  : Item()
{
  m_Equipped = false;
}

Weapon::Weapon(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
  : Item(_ID, pos, rot, Scale)
{
  m_Equipped = false;
}

Weapon::~Weapon()
{
  disable();
}

const std::string& Weapon::getObjectMesh() const
{
  return Database::getSingleton().getTypedRecord<WeaponRecord>(ID).Mesh;
}

ObjectTypes Weapon::getDuskType() const
{
  return otWeapon;
}

bool Weapon::canCollide() const
{
  return true;
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
    DuskLog() << "Weapon::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RfWe" (reference of Weapon)
  OutStream.write((char*) &cHeaderRfWe, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Weapon::saveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data inherited from Item
  if (!saveItemPart(OutStream))
  {
    DuskLog() << "Weapon::saveToStream: ERROR while writing item data!\n";
    return false;
  }
  //Weapon has no new data members, so return here
  return OutStream.good();
}

bool Weapon::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Weapon::loadFromStream: ERROR: Cannot load from stream while"
              << " weapon is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Weapon::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RfWe"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRfWe)
  {
    DuskLog() << "Weapon::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Weapon::loadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff inherited from item
  if (!loadItemPart(InStream))
  {
    DuskLog() << "Weapon::loadFromStream: ERROR while reading item data.\n";
    return false;
  }//if
  //class Weapon has no new data members, so return here
  return InStream.good();
}

} //namespace
