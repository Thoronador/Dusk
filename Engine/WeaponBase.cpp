/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011 thoronador

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

#include "WeaponBase.h"
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

WeaponBase::WeaponBase()
{
  //empty
}

WeaponBase::~WeaponBase()
{
  m_Weapons.clear();
}

WeaponBase& WeaponBase::getSingleton()
{
  static WeaponBase Instance;
  return Instance;
}

void WeaponBase::addWeapon(const std::string& ID, const WeaponRecord& data)
{
  if (ID=="" or data.Mesh=="" or data.Name=="" or data.value<0
      or data.weight<0.0f or data.Range<0.0f)
  {
    return;
  }
  m_Weapons[ID] = data;
}

bool WeaponBase::hasWeapon(const std::string& ID) const
{
  return (m_Weapons.find(ID) != m_Weapons.end());
}

std::string WeaponBase::getWeaponMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter;
  iter = m_Weapons.find(ID);
  if (iter!=m_Weapons.end())
  {
    return iter->second.Mesh;
  }
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  return "";
}

std::string WeaponBase::getWeaponName(const std::string& ID) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter = m_Weapons.find(ID);
  if (iter!=m_Weapons.end())
  {
    return iter->second.Name;
  }
  return "";
}

int WeaponBase::getWeaponValue(const std::string& weaponID) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter = m_Weapons.find(weaponID);
  if (iter!=m_Weapons.end())
  {
    return iter->second.value;
  }
  return -1;
}

float WeaponBase::getWeaponWeight(const std::string& weaponID) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter = m_Weapons.find(weaponID);
  if (iter!=m_Weapons.end())
  {
    return iter->second.weight;
  }
  return 0.0f;
}

WeaponRecord WeaponBase::getWeaponData(const std::string& ID) const
{
  std::map<std::string, WeaponRecord>::const_iterator iter = m_Weapons.find(ID);
  if (iter!=m_Weapons.end())
  {
    return iter->second;
  }
  WeaponRecord temp;
  temp.Mesh = temp.Name = "";
  temp.value = -1;
  temp.weight = 0.0f;
  temp.Type = wtMelee;
  temp.Range = -1.0f;
  temp.TimeBetweenAttacks = 1.0e20; //long time, because weapon cannot attack
  temp.ProjectileID = "";
  temp.DamageTimes = 0;
  temp.DamageDice = 0;
  return temp;
}

bool WeaponBase::deleteWeapon(const std::string& ID)
{
  std::map<std::string, WeaponRecord>::iterator iter;
  iter = m_Weapons.find(ID);
  if (iter==m_Weapons.end())
  {
    return false;
  }
  m_Weapons.erase(iter);
  return true;
}

void WeaponBase::clearAll()
{
  m_Weapons.clear();
}

unsigned int WeaponBase::getNumberOfWeapons() const
{
  return m_Weapons.size();
}

bool WeaponBase::saveAllToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "WeaponBase::saveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  std::map<std::string, WeaponRecord>::const_iterator iter;
  iter = m_Weapons.begin();
  unsigned int len = 0;
  while (iter!=m_Weapons.end())
  {
    //header "Weap"
    OutStream.write((char*) &cHeaderWeap, sizeof(unsigned int));
    //ID
    len = iter->first.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->first.c_str(), len);
    //Mesh
    len = iter->second.Mesh.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->second.Mesh.c_str(), len);
    //Name
    len = iter->second.Name.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->second.Name.c_str(), len);
    //value
    OutStream.write((char*) &(iter->second.value), sizeof(iter->second.value));
    //weight
    OutStream.write((char*) &(iter->second.weight), sizeof(float));
    //type
    OutStream.write((char*) &(iter->second.Type), sizeof(WeaponType));
    //range
    OutStream.write((char*) &(iter->second.Range), sizeof(float));
    //time between attacks
    OutStream.write((char*) &(iter->second.TimeBetweenAttacks), sizeof(float));
    //projectile ID
    len = iter->second.ProjectileID.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(iter->second.ProjectileID.c_str(), len);
    //damage
    OutStream.write((char*) &(iter->second.DamageTimes), sizeof(uint8));
    OutStream.write((char*) &(iter->second.DamageDice), sizeof(uint8));
    ++iter;
  } //while
  return OutStream.good();
}

bool WeaponBase::loadNextWeaponFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: stream contains"
              << " errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderWeap)
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: stream contains"
              << " unexpected header!\n";
    return false;
  }
  char Buffer[256];
  Buffer[0] = Buffer[255] = '\0';
  //read ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: ID is longer "
              << "than 255 characters!\n";
    return false;
  }
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "ID from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  const std::string weapID = std::string(Buffer);
  //read mesh
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: mesh path is "
              << "longer than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "mesh path from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  WeaponRecord wRec;
  wRec.Mesh = std::string(Buffer);
  //read name
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: name is longer"
              << " than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "name from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  wRec.Name = std::string(Buffer);
  //value
  InStream.read((char*) &(wRec.value), sizeof(wRec.value));
  //weight
  InStream.read((char*) &(wRec.weight), sizeof(float));
  //type
  InStream.read((char*) &(wRec.Type), sizeof(WeaponType));
  //range
  InStream.read((char*) &(wRec.Range), sizeof(float));
  //time between attacks
  InStream.read((char*) &(wRec.TimeBetweenAttacks), sizeof(float));
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "data from stream!\n";
    return false;
  }
  if ((wRec.Range<0.0f) or (wRec.TimeBetweenAttacks<0.0f))
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "data from stream. Weapon has invalid range or attack time.\n";
    return false;
  }
  //projectile ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR: projectile ID is"
              << " longer than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "WeaponBase::loadNextWeaponFromStream: ERROR while reading "
              << "projectile from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  wRec.ProjectileID = std::string(Buffer);
  //damage
  InStream.read((char*) &(wRec.DamageTimes), sizeof(uint8));
  InStream.read((char*) &(wRec.DamageDice), sizeof(uint8));
  if (InStream.good())
  {
    addWeapon(weapID, wRec);
    return true;
  }//if
  return false;
}

#ifdef DUSK_EDITOR
WeaponBaseIterator WeaponBase::getFirst() const
{
  return m_Weapons.begin();
}

WeaponBaseIterator WeaponBase::getEnd() const
{
  return m_Weapons.end();
}
#endif

} //namespace
