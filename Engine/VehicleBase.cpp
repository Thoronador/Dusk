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

#include "VehicleBase.h"
#include "Messages.h"
#include "DuskConstants.h"
#include "DuskExceptions.h"
#include "DuskFunctions.h"

namespace Dusk
{

// I guess we won't have any vehicle that carries more than 20 passengers.
const unsigned int cMaxMountpointCount = 20;

VehicleBase& VehicleBase::getSingleton()
{
  static VehicleBase Instance;
  return Instance;
}

VehicleBase::~VehicleBase()
{
  m_Vehicles.clear();
}

void VehicleBase::addVehicle(const std::string& ID, const VehicleRecord& data)
{
  if (ID!="" and data.Mesh!="" and data.Name!="" and data.MaxSpeed>=0.0f
      and data.Mountpoints.size()>0 and data.Mountpoints.size()<=cMaxMountpointCount)
  {
    m_Vehicles[ID] = data;
    m_Vehicles[ID].Mesh = adjustDirectorySeperator(data.Mesh);
  }
  else
  {
    DuskLog() << "VehicleBase::addVehicle: ERROR: invalid vehicle data or empty"
              << " ID supplied!\n";
  }
}

bool VehicleBase::hasVehicle(const std::string& ID) const
{
  return m_Vehicles.find(ID)!=m_Vehicles.end();
}

const std::string& VehicleBase::getVehicleMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.Mesh;
  }
  //nothing found, if we get here
  if (UseMarkerOnError)
  {
    DuskLog() << "VehicleBase::getVehicleMesh: Warning! No vehicle with ID \""
              << ID << "\" found. Using error marker's mesh instead.\n";
    return cErrorMesh;
  }
  DuskLog() << "VehicleBase::getMeshName: ERROR! No vehicle with ID \""<<ID
            << "\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

const std::string& VehicleBase::getVehicleName(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.Name;
  }
  DuskLog() << "VehicleBase::getVehicleName: ERROR! No vehicle with ID \""<<ID
            << "\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

float VehicleBase::getVehicleSpeed(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.MaxSpeed;
  }
  DuskLog() << "VehicleBase::getVehicleSpeed: Hint: no vehicle with ID \""
            << ID<<"\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

VehicleBase::VehicleBase()
{
  m_Vehicles.clear();
  mp_null.offset = Ogre::Vector3::ZERO;
  mp_null.rotation = Ogre::Quaternion::IDENTITY;
}

unsigned int VehicleBase::getVehicleMountpoints(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.Mountpoints.size();
  }
  DuskLog() << "VehicleBase::getVehicleMountpoints: ERROR: no vehicle with ID \""
            << ID <<"\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

const Ogre::Vector3& VehicleBase::getMountpointOffset(const std::string& ID, const unsigned int idx) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    if (idx< cIter->second.Mountpoints.size()) return cIter->second.Mountpoints.at(idx).offset;
    DuskLog() << "VehicleBase::getMountpointOffset: ERROR: index is out of "
              << "range for vehicle \""<<ID<<"\". Returning zero vector.\n";
    return Ogre::Vector3::ZERO;
  }
  DuskLog() << "VehicleBase::getMountpointOffset: ERROR: no vehicle with ID \""
            << ID<<"\" found. Returning zero vector.\n";
  throw IDNotFound("VehicleBase", ID);
}

const Ogre::Quaternion& VehicleBase::getMountpointRotation(const std::string& ID, const unsigned int idx) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    if (idx< cIter->second.Mountpoints.size()) return cIter->second.Mountpoints.at(idx).rotation;
    DuskLog() << "VehicleBase::getMountpointRotation: ERROR: index is out of "
              << "range for vehicle \""<<ID<<"\". Returning identity quaternion.\n";
    return Ogre::Quaternion::IDENTITY;
  }
  DuskLog() << "VehicleBase::getMountpointRotation: ERROR: no vehicle with ID \""
            << ID<<"\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

const MountpointData& VehicleBase::getMountpointData(const std::string& ID, const unsigned int idx) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    if (idx< cIter->second.Mountpoints.size()) return cIter->second.Mountpoints.at(idx);
    DuskLog() << "VehicleBase::getMountpointData: ERROR: index is out of range."
              << " Returning predefined data.\n";
    return mp_null;
  }
  DuskLog() << "VehicleBase::getMountpointData: ERROR: no vehicle with ID \""
            << ID<<"\" found. Throwing exception.\n";
  throw IDNotFound("VehicleBase", ID);
}

bool VehicleBase::deleteVehicle(const std::string& ID)
{
  std::map<std::string, VehicleRecord>::iterator iter;
  iter = m_Vehicles.find(ID);
  if (iter==m_Vehicles.end())
  {
    return false;
  }
  m_Vehicles.erase(iter);
  return true;
}

void VehicleBase::clearAll()
{
  m_Vehicles.clear();
}

unsigned int VehicleBase::getVehicleNumber() const
{
  return m_Vehicles.size();
}

bool VehicleBase::saveAllToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "VehicleBase::saveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  std::map<std::string, VehicleRecord>::const_iterator iter;
  iter = m_Vehicles.begin();
  unsigned int len = 0;
  float temp;
  while (iter!=m_Vehicles.end() and OutStream.good())
  {
    //header "Vehi"
    OutStream.write((char*) &cHeaderVehi, sizeof(unsigned int));
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
    //MaxSpeed
    OutStream.write((char*) &(iter->second.MaxSpeed), sizeof(iter->second.MaxSpeed));
    //number of Mountpoints
    len = iter->second.Mountpoints.size();
    OutStream.write((char*) &len, sizeof(len));
    //write mountpoint data
    for (len=0; len<iter->second.Mountpoints.size(); ++len)
    {
      // -- offset
      temp = iter->second.Mountpoints.at(len).offset.x;
      OutStream.write((char*) &temp, sizeof(float));
      temp = iter->second.Mountpoints.at(len).offset.y;
      OutStream.write((char*) &temp, sizeof(float));
      temp = iter->second.Mountpoints.at(len).offset.z;
      OutStream.write((char*) &temp, sizeof(float));
      // -- rotation
      temp = iter->second.Mountpoints.at(len).rotation.w;
      OutStream.write((char*) &temp, sizeof(float));
      temp = iter->second.Mountpoints.at(len).rotation.x;
      OutStream.write((char*) &temp, sizeof(float));
      temp = iter->second.Mountpoints.at(len).rotation.y;
      OutStream.write((char*) &temp, sizeof(float));
      temp = iter->second.Mountpoints.at(len).rotation.z;
      OutStream.write((char*) &temp, sizeof(float));
    }//for
    ++iter;
  } //while
  return OutStream.good();
}

bool VehicleBase::loadNextVehicleFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR: stream "
              << "contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderVehi)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR: stream "
              << "contains unexpected header!\n";
    return false;
  }
  char Buffer[256];
  Buffer[0] = Buffer[255] = '\0';
  //read ID
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR: ID is longer "
              << "than 255 characters!\n";
    return false;
  }
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "ID from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  const std::string vehicleID = std::string(Buffer);
  //read mesh
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR: mesh path of "
              << "vehicle \""<<vehicleID<<"\" is longer than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "mesh path from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  VehicleRecord vRec;
  vRec.Mesh = std::string(Buffer);
  //read name
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR: name of vehicle \""
              << vehicleID <<"\" is longer than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "name from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  vRec.Name = std::string(Buffer);
  //MaxSpeed
  InStream.read((char*) &(vRec.MaxSpeed), sizeof(vRec.MaxSpeed));
  if (vRec.MaxSpeed<0.0f)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "data from stream. Vehicle has invalid maximum speed.\n";
    return false;
  }
  //number of Mountpoints
  len = 0;
  InStream.read((char*) &len, sizeof(len));
  if (!InStream.good())
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "data from stream!\n";
    return false;
  }
  if (len>cMaxMountpointCount)
  {
    DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "data from stream! Number of mountpoints is too big. Current "
              << "value is "<<len<<", but only up to "
              << cMaxMountpointCount << " are allowed.\n";
    return false;
  }
  //read the mountpoint data
  float w, x, y, z;
  MountpointData mpd;
  vRec.Mountpoints.clear(); //better be safe than sorry
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    // -- offset first
    InStream.read((char*) &x, sizeof(float));
    InStream.read((char*) &y, sizeof(float));
    InStream.read((char*) &z, sizeof(float));
    mpd.offset = Ogre::Vector3(x,y,z);
    // -- rotation
    InStream.read((char*) &w, sizeof(float));
    InStream.read((char*) &x, sizeof(float));
    InStream.read((char*) &y, sizeof(float));
    InStream.read((char*) &z, sizeof(float));
    mpd.rotation = Ogre::Quaternion(w,x,y,z);
    if (!InStream.good())
    {
      DuskLog() << "VehicleBase::loadNextVehicleFromStream: ERROR while reading"
                << " mountpoint data from stream!\n";
      return false;
    }//if
    vRec.Mountpoints.push_back(mpd);
  }//for
  //Did all go well so far?
  if (InStream.good())
  {
    addVehicle(vehicleID, vRec);
    return true;
  }//if
  return false;
}

#ifdef DUSK_EDITOR
VehicleBase::Iterator VehicleBase::getFirst() const
{
  return m_Vehicles.begin();
}

VehicleBase::Iterator VehicleBase::getEnd() const
{
  return m_Vehicles.end();
}
#endif


} //namespace
