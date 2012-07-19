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

#include "VehicleRecord.h"
#include "Messages.h"
#include "DuskConstants.h"

namespace Dusk
{

// I guess we won't have any vehicle that carries more than 20 passengers.
const unsigned int cMaxMountpointCount = 20;

uint32_t VehicleRecord::getRecordType() const
{
  return cHeaderVehi;
}

//record type identifier (usually the value returned by the above function)
const uint32_t VehicleRecord::RecordType = cHeaderVehi;


bool VehicleRecord::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "VehicleRecord::saveToStream: ERROR: stream contains errors!\n";
    return false;
  }

  uint32_t len = 0;
  float temp;

  //header "Vehi"
  outStream.write((const char*) &cHeaderVehi, sizeof(uint32_t));
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
  //MaxSpeed
  outStream.write((const char*) &MaxSpeed, sizeof(MaxSpeed));
  //number of Mountpoints
  len = Mountpoints.size();
  outStream.write((const char*) &len, sizeof(len));
  //write mountpoint data
  for (len=0; len<Mountpoints.size(); ++len)
  {
    // -- offset
    temp = Mountpoints.at(len).offset.x;
    outStream.write((const char*) &temp, sizeof(float));
    temp = Mountpoints.at(len).offset.y;
    outStream.write((const char*) &temp, sizeof(float));
    temp = Mountpoints.at(len).offset.z;
    outStream.write((const char*) &temp, sizeof(float));
    // -- rotation
    temp = Mountpoints.at(len).rotation.w;
    outStream.write((const char*) &temp, sizeof(float));
    temp = Mountpoints.at(len).rotation.x;
    outStream.write((const char*) &temp, sizeof(float));
    temp = Mountpoints.at(len).rotation.y;
    outStream.write((const char*) &temp, sizeof(float));
    temp = Mountpoints.at(len).rotation.z;
    outStream.write((const char*) &temp, sizeof(float));
  }//for

  return outStream.good();
}

bool VehicleRecord::loadFromStream(std::ifstream& inStream)
{
  if (!inStream.good())
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR: stream "
              << "contains errors!\n";
    return false;
  }
  uint32_t len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len != cHeaderVehi)
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR: stream "
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
    DuskLog() << "VehicleRecord::loadFromStream: ERROR: ID is longer "
              << "than 255 characters!\n";
    return false;
  }
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
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
    DuskLog() << "VehicleRecord::loadFromStream: ERROR: mesh path of "
              << "vehicle \""<<ID<<"\" is longer than 255 characters!\n";
    return false;
  }
  memset(Buffer, 0, 256);
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
              << "mesh path from stream!\n";
    return false;
  }
  Mesh = std::string(Buffer);
  //read name
  len = 0;
  inStream.read((char*) &len, sizeof(uint32_t));
  if (len>255)
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR: name of vehicle \""
              << ID <<"\" is longer than 255 characters!\n";
    return false;
  }
  memset(Buffer, 0, 256);
  inStream.read(Buffer, len);
  if (!inStream.good())
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
              << "name from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  Name = std::string(Buffer);
  //MaxSpeed
  inStream.read((char*) &MaxSpeed, sizeof(MaxSpeed));
  if (MaxSpeed<0.0f)
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
              << "data from stream. Vehicle has invalid maximum speed.\n";
    return false;
  }
  //number of Mountpoints
  len = 0;
  inStream.read((char*) &len, sizeof(len));
  if (!inStream.good())
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
              << "data from stream!\n";
    return false;
  }
  if (len>cMaxMountpointCount)
  {
    DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading "
              << "data from stream! Number of mountpoints is too big. Current "
              << "value is "<<len<<", but only up to "
              << cMaxMountpointCount << " are allowed.\n";
    return false;
  }
  //read the mountpoint data
  float w, x, y, z;
  MountpointData mpd;
  Mountpoints.clear();
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    // -- offset first
    inStream.read((char*) &x, sizeof(float));
    inStream.read((char*) &y, sizeof(float));
    inStream.read((char*) &z, sizeof(float));
    mpd.offset = Ogre::Vector3(x,y,z);
    // -- rotation
    inStream.read((char*) &w, sizeof(float));
    inStream.read((char*) &x, sizeof(float));
    inStream.read((char*) &y, sizeof(float));
    inStream.read((char*) &z, sizeof(float));
    mpd.rotation = Ogre::Quaternion(w,x,y,z);
    if (!inStream.good())
    {
      DuskLog() << "VehicleRecord::loadFromStream: ERROR while reading"
                << " mountpoint data from stream!\n";
      return false;
    }//if
    Mountpoints.push_back(mpd);
  }//for
  //Did all go well so far?
  return inStream.good();
}

} //namespace
