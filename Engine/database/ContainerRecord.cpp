/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012 thoronador

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

#include "ContainerRecord.h"
#include "DuskConstants.h"
#include "Messages.h"
#include <cstring>

namespace Dusk{

bool ContainerRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "ContainerRecord::saveToStream: ERROR: stream contains errors!\n";
    return false;
  }

  uint32_t len = 0;
  //header "Cont"
  outStream.write((const char*) &cHeaderCont, sizeof(uint32_t));
  //ID
  len = ID.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(ID.c_str(), len);
  //Mesh
  len = Mesh.length();
  outStream.write((const char*) &len, sizeof(uint32_t));
  outStream.write(Mesh.c_str(), len);
  //Inventory
  if (!ContainerInventory.saveToStream(outStream))
  {
    DuskLog() << "ContainerRecord::saveStream: ERROR while writing "
              << "container's inventory.\n";
    return false;
  }//if
  return outStream.good();
}

bool ContainerRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR: stream contains errors!\n";
    return false;
  }
  uint32_t len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len != cHeaderCont)
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR: stream contains unexpected header!\n";
    return false;
  }
  char ID_Buffer[256];
  memset(ID_Buffer, 0, 256);
  //read ID
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR: ID is "
              << "longer than 255 characters!\n";
    return false;
  }
  inStream.read(ID_Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR while "
              << "reading ID from stream!\n";
    return false;
  }
  //read Mesh
  char Mesh_Buffer[256];
  memset(Mesh_Buffer, 0, 256);
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR: mesh path "
              << "is longer than 255 characters!\n";
    return false;
  }
  inStream.read(Mesh_Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR while "
              << "reading mesh path from stream!\n";
    return false;
  }
  Inventory temp;
  if (!temp.loadFromStream(inStream))
  {
    DuskLog() << "ContainerRecord::loadFromStream: ERROR while "
              << "reading inventory contens from stream!\n";
    return false;
  }
  //all right so far
  return inStream.good();
}

uint32_t ContainerRecord::getRecordType() const
{
  return cHeaderCont;
}

//record type identifier
const uint32_t ContainerRecord::RecordType = cHeaderCont;

}//namespace
