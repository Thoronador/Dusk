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

#include "ProjectileRecord.h"
#include <cstring>
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

uint32_t ProjectileRecord::getRecordType() const
{
  return cHeaderProj;
}

//record type identifier
const uint32_t ProjectileRecord::RecordType = cHeaderProj;

bool ProjectileRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "ProjectileRecord::saveToStream: ERROR: stream contains errors!\n";
    return false;
  }

  uint32_t len = 0;

  //header "Proj"
  outStream.write((const char*) &cHeaderProj, sizeof(uint32_t));
  //ID
  len = ID.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(ID.c_str(), len);
  //Mesh
  len = Mesh.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Mesh.c_str(), len);
  //TTL
  outStream.write((const char*) &DefaultTTL, sizeof(float));
  //velocity
  outStream.write((const char*) &DefaultVelocity, sizeof(float));
  //times
  outStream.write((const char*) &times, sizeof(uint8_t));
  //dice
  outStream.write((const char*) &dice, sizeof(uint8_t));
  //check
  if (!outStream.good())
  {
    DuskLog() << "ProjectileRecord::saveToStream: Error while writing data to "
              << "stream. Current projectile is \""<<ID<<"\".\n";
    return false;
  }

  return outStream.good();
}

bool ProjectileRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR: stream contains errors!\n";
    return false;
  }
  uint32_t len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len != cHeaderProj)
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR: stream "
              << "contains unexpected header!\n";
    return false;
  }
  char Buffer[256];
  memset(Buffer, 0, 256);
  //read ID
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR: ID is "
              << "longer than 255 characters!\n";
    return false;
  }
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR while "
              << "reading ID from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  ID = std::string(Buffer);

  //read Mesh
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR: mesh "
              << "path is longer than 255 characters!\n";
    return false;
  }
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR while "
              << "reading mesh path from stream!\n";
    return false;
  }
  Buffer[len] = '\0';

  Mesh = std::string(Buffer);
  //TTL
  inStream.read((char*) &DefaultTTL, sizeof(float));
  //velocity
  inStream.read((char*) &DefaultVelocity, sizeof(float));
  //times
  inStream.read((char*) &times, sizeof(uint8_t));
  //dice
  inStream.read((char*) &dice, sizeof(uint8_t));
  if (!inStream.good())
  {
    DuskLog() << "ProjectileRecord::loadFromStream: ERROR while "
              << "reading projectile data from stream!\n";
    return false;
  }
  return true;
}

} //namespace
