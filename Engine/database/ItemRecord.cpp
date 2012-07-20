/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

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

#include "ItemRecord.h"
#include <cstring>
#include "../DuskConstants.h"
#include "../Messages.h"

namespace Dusk
{

const uint32_t ItemRecord::RecordType = cHeaderItem;

uint32_t ItemRecord::getRecordType() const
{
  return cHeaderItem;
}

bool ItemRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "ItemRecord::saveToStream: ERROR: bad stream.\n";
    return false;
  }

  //write header "Item"
  outStream.write((const char*) &cHeaderItem, sizeof(uint32_t));
  //write ID
  uint32_t len = ID.length();
  outStream.write((const char*) &len, sizeof(unsigned int));
  outStream.write(ID.c_str(), len);
  //write name
  len = Name.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Name.c_str(), len);
  //write value
  outStream.write((const char*) &value, sizeof(int));
  //write weight
  outStream.write((const char*) &weight, sizeof(float));
  //write Mesh name
  len = Mesh.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Mesh.c_str(), len);

  if (!outStream.good())
  {
    DuskLog() << "ItemRecord::saveToStream: ERROR while writing data.\n";
    return false;
  }//if
  return true;
}

bool ItemRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR: bad stream.\n";
    return false;
  }//if

  char ID_Buffer[256];
  //read header "Item"
  uint32_t Header = 0;
  inStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderItem)
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR: Stream contains invalid "
              << "item record header.\n";
    return false;
  }//if

  uint32_t len;
  //read length of ID
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  memset(ID_Buffer, 0, 256);
  inStream.read(ID_Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR while reading data (ID).\n";
    return false;
  }
  ID = std::string(ID_Buffer);

  //read length of item name
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR: item name cannot be "
              << "longer than 255 characters.\n";
    return false;
  }
  //read item name
  memset(ID_Buffer, 0, len+1);
  inStream.read(ID_Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR while reading data (name).\n";
    return false;
  }
  Name = std::string(ID_Buffer);

  //read value
  inStream.read((char*) &value, sizeof(int));
  //read weight
  inStream.read((char*) &weight, sizeof(float));
  if (!inStream.good())
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }

  //read length of mesh name
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR: mesh name cannot be longer"
              << " than 255 characters.\n";
    return false;
  }
  //read mesh name
  memset(ID_Buffer, 0, len+1);
  inStream.read(ID_Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ItemRecord::loadFromStream: ERROR while reading data (mesh).\n";
    return false;
  }
  return true;
}

}//namespace
