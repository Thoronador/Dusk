#include "VehicleBase.h"
#include <iostream>
#include "DuskConstants.h"

namespace Dusk
{

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
  if (ID!="" and data.Mesh!="" and data.Name!="" and data.MaxSpeed>=0.0f)
  {
    m_Vehicles[ID] = data;
  }
  else
  {
    std::cout << "VehicleBase::addVehicle: ERROR: invalid vehicle data or empty"
              << " ID supplied!\n";
  }
}

bool VehicleBase::hasVehicle(const std::string& ID) const
{
  return m_Vehicles.find(ID)!=m_Vehicles.end();
}

std::string VehicleBase::getVehicleMesh(const std::string& ID, const bool UseMarkerOnError) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.Mesh;
  }
  //nothing found, if we get here
  if (UseMarkerOnError) return cErrorMesh;
  return "";
}

std::string VehicleBase::getVehicleName(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.Name;
  }
  return "";
}

float VehicleBase::getVehicleSpeed(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.MaxSpeed;
  }
  return 0.0f;
}

VehicleBase::VehicleBase()
{
  m_Vehicles.clear();
}

unsigned int VehicleBase::getVehicleMountpoints(const std::string& ID) const
{
  const std::map<std::string, VehicleRecord>::const_iterator cIter =
      m_Vehicles.find(ID);
  if (cIter!=m_Vehicles.end())
  {
    return cIter->second.MountpointCount;
  }
  return 0;
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
    std::cout << "VehicleBase::SaveAllToStream: ERROR: stream contains errors!\n";
    return false;
  }
  std::map<std::string, VehicleRecord>::const_iterator iter;
  iter = m_Vehicles.begin();
  unsigned int len = 0;
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
    OutStream.write((char*) &(iter->second.MountpointCount), sizeof(iter->second.MountpointCount));
    ++iter;
  } //while
  return OutStream.good();
}

bool VehicleBase::loadNextVehicleFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR: stream "
              << "contains errors!\n";
    return false;
  }
  unsigned int len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len != cHeaderVehi)
  {
    std::cout << "VehicleBase::LoadNextVehicleFromStream: ERROR: stream "
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
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR: ID is longer "
              << "than 255 characters!\n";
    return false;
  }
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
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
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR: mesh path is "
              << "longer than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
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
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR: name is longer"
              << " than 255 characters!\n";
    return false;
  }
  Buffer[0] = Buffer[255] = '\0';
  InStream.read(Buffer, len);
  if (!InStream.good())
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "name from stream!\n";
    return false;
  }
  Buffer[len] = '\0';
  vRec.Name = std::string(Buffer);
  //MaxSpeed
  InStream.read((char*) &(vRec.MaxSpeed), sizeof(vRec.MaxSpeed));
  //number of Mountpoints
  InStream.read((char*) &(vRec.MountpointCount), sizeof(vRec.MountpointCount));
  if (!InStream.good())
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "data from stream!\n";
    return false;
  }
  if (vRec.MaxSpeed<0.0f)
  {
    std::cout << "VehicleBase::loadNextVehicleFromStream: ERROR while reading "
              << "data from stream. Vehicle has invalid maximum speed.\n";
    return false;
  }
  //Did all go well so far?
  if (InStream.good())
  {
    addVehicle(vehicleID, vRec);
    return true;
  }//if
  return false;
}

} //namespace
