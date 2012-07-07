/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

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

#include "ResourceRecord.h"
#include "DuskConstants.h"
#include "Messages.h"
#include <cstring>

namespace Dusk
{

uint32_t ResourceRecord::getRecordType() const
{
  return cHeaderRsrc;
}

//record type identifier
const uint32_t ResourceRecord::RecordType = cHeaderRsrc;


bool ResourceRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
     DuskLog() << "ResourceRecord::saveToStream: ERROR: Stream contains errors!\n";
     return false;
  }

  uint32_t len = 0;

  //write header "Rsrc"
  outStream.write((const char*) &cHeaderRsrc, 4); //Resource
  //write ID
  len = ID.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(ID.c_str(), len);
  //write mesh (spawned state)
  len = meshSpawned.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(meshSpawned.c_str(), len);
  //write mesh (harvested state)
  len = meshHarvested.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(meshHarvested.c_str(), len);
  //write item ID
  len = harvestItem.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(harvestItem.c_str(), len);
  //write respawn interval
  outStream.write((const char*) &respawnInterval, sizeof(float));
  //write harvest sound
  len = harvestSound.length();
  outStream.write((const char*) &len, sizeof(unsigned int));
  outStream.write(harvestSound.c_str(), len);
  //check
  if (!outStream.good())
  {
    DuskLog() << "ResourceRecord::saveToStream: Error while writing data to "
              << "stream. Current object is \""<<ID<<"\".\n";
    return false;
  }

  return outStream.good();
}

bool ResourceRecord::loadFromStream(std::ifstream& inStream)
{
  uint32_t Header = 0;

  //read header "Rsrc" (Resource)
  inStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderRsrc)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  uint32_t len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  char buffer[256];
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  ID = std::string(buffer);
  //read length of mesh (spawned)
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: Mesh path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh (spawned)
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  meshSpawned = std::string(buffer);

  //read length of mesh (harvested)
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: Mesh path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh (harvested)
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  meshHarvested = std::string(buffer);

  //read length of item ID
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: Item ID cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read item ID
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  harvestItem = std::string(buffer);

  //read respawn interval
  inStream.read((char*) &respawnInterval, sizeof(float));
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }

  //read length of sound ID
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR: Sound ID cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read sound ID
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ResourceRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  harvestSound = std::string(buffer);

  return true;
}

} //namespace
