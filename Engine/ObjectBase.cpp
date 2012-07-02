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

#include "ObjectBase.h"
#include <cstring>
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

uint32_t ObjectRecord::getRecordType() const
{
  return cHeaderObjS;
}

bool ObjectRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
     DuskLog() << "ObjectRecord::saveToStream: ERROR: Stream contains errors!\n";
     return false;
  }

  unsigned int len;
  //write header "ObjS"
  outStream.write((const char*) &cHeaderObjS, sizeof(uint32_t)); //Object, Static
  //write ID
  len = ID.length();
  outStream.write((const char*) &len, sizeof(unsigned int));
  outStream.write(ID.c_str(), len);
  //write mesh
  len = Mesh.length();
  outStream.write((const char*) &len, sizeof(unsigned int));
  outStream.write(Mesh.c_str(), len);
  //write collision flag
  outStream.write((const char*) &collide, sizeof(bool));
  //check
  if (!outStream.good())
  {
    DuskLog() << "ObjectRecord::saveToStream: Error while writing data to "
              << "stream. Current object is \""<<ID<<"\".\n";
    return false;
  }
  return outStream.good();
}

bool ObjectRecord::loadFromStream(std::ifstream& inStream)
{
  unsigned int len;
  uint32_t Header = 0;
  char ID_Buffer[256], Mesh_Buffer[256];

  //read header "ObjS" (Object, Static)
  inStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderObjS)
  {
    DuskLog() << "ObjectRecord::loadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ObjectRecord::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  memset(ID_Buffer, 0, 256);
  inStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0'; //add terminating null character
  if (!inStream.good())
  {
    DuskLog() << "ObjectRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  //read length of mesh name
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ObjectRecord::loadFromStream: ERROR: Name of Mesh cannot be "
              << "longer than 255 characters.\n";
    return false;
  }
  //read mesh
  memset(Mesh_Buffer, 0, 256);
  inStream.read(Mesh_Buffer, len);
  Mesh_Buffer[len] = '\0'; //add terminating null character
  //read collision flag
  bool collision_flag = true;
  inStream.read((char*) &collision_flag, sizeof(bool));
  if (!inStream.good())
  {
    DuskLog() << "ObjectRecord::loadFromStream: ERROR while reading data.\n";
    return false;
  }
  //now set the data
  ID = std::string(ID_Buffer);
  Mesh = std::string(Mesh_Buffer);
  collide = collision_flag;
  return true;
}

}//namespace
