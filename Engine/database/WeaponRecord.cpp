/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012 thoronador

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

#include "WeaponRecord.h"
#include <cstring>
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

uint32_t WeaponRecord::getRecordType() const
{
  return cHeaderWeap;
}

//record type identifier
const uint32_t WeaponRecord::RecordType = cHeaderWeap;

bool WeaponRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "WeaponRecord::saveToStream: ERROR: stream contains errors!\n";
    return false;
  }

  uint32_t len = 0;

  //header "Weap"
  outStream.write((const char*) &cHeaderWeap, sizeof(uint32_t));
  //ID
  len = ID.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(ID.c_str(), len);
  //Mesh
  len = Mesh.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Mesh.c_str(), len);
  //Name
  len = Name.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Name.c_str(), len);
  //value
  outStream.write((const char*) &value, sizeof(value));
  //weight
  outStream.write((const char*) &weight, sizeof(float));
  //type
  outStream.write((const char*) &Type, sizeof(WeaponType));
  //range
  outStream.write((const char*) &Range, sizeof(float));
  //time between attacks
  outStream.write((const char*) &TimeBetweenAttacks, sizeof(float));
  //projectile ID
  len = ProjectileID.length();
  outStream.write((const char*) &len, sizeof(len));
  outStream.write(ProjectileID.c_str(), len);
  //damage
  outStream.write((const char*) &DamageTimes, sizeof(uint8_t));
  outStream.write((const char*) &DamageDice, sizeof(uint8_t));

  return outStream.good();
}

bool WeaponRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: stream contains"
              << " errors!\n";
    return false;
  }
  uint32_t len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len != cHeaderWeap)
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: stream contains"
              << " unexpected header!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, 0, 256);
  //read ID
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: ID is longer "
              << "than 255 characters!\n";
    return false;
  }
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "ID from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  ID = std::string(Buffer);
  //read mesh
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: mesh path is "
              << "longer than 255 characters!\n";
    return false;
  }
  memset(Buffer, 0, 256);
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "mesh path from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  Mesh = std::string(Buffer);
  //read name
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: name is longer"
              << " than 255 characters!\n";
    return false;
  }
  memset(Buffer, 0, 256);
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "name from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  Name = std::string(Buffer);
  //value
  inStream.read((char*) &value, sizeof(value));
  //weight
  inStream.read((char*) &weight, sizeof(float));
  //type
  inStream.read((char*) &Type, sizeof(WeaponType));
  //range
  inStream.read((char*) &Range, sizeof(float));
  //time between attacks
  inStream.read((char*) &TimeBetweenAttacks, sizeof(float));
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "data from stream!\n";
    return false;
  }
  if ((Range<0.0f) or (TimeBetweenAttacks<0.0f))
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "data from stream. Weapon has invalid range or attack time.\n";
    return false;
  }
  //projectile ID
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR: projectile ID is"
              << " longer than 255 characters!\n";
    return false;
  }
  memset(Buffer, 0, 256);
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "WeaponRecord::loadFromStream: ERROR while reading "
              << "projectile from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  ProjectileID = std::string(Buffer);
  //damage
  inStream.read((char*) &DamageTimes, sizeof(uint8_t));
  inStream.read((char*) &DamageDice, sizeof(uint8_t));
  return inStream.good();
}

} //namespace
