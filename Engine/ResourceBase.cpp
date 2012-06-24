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

#include "ResourceBase.h"
#include "DuskConstants.h"
#include "DuskExceptions.h"
#include "Messages.h"
#include <cstring>

namespace Dusk
{

ResourceBase::ResourceBase()
{
  //empty
}

ResourceBase::~ResourceBase()
{
  clearAll();
}

ResourceBase& ResourceBase::getSingleton()
{
  static ResourceBase Instance;
  return Instance;
}

void ResourceBase::addResource(const std::string& ID, const ResourceRecord& data)
{
  if (!ID.empty() and !data.meshSpawned.empty() and data.respawnInterval>0.0f)
  {
    m_Resources[ID] = data;
  }
}

bool ResourceBase::hasResource(const std::string& ID) const
{
  return (m_Resources.find(ID)!=m_Resources.end())   ;
}

      /* Returns the data of resource with given ID, if present. If no record
         for the requested ID is present, the function will throw an exception.

         parameter(s):
             ID - ID of the resource
      */
const ResourceRecord& ResourceBase::getResourceData(const std::string& ID) const
{
  const std::map<std::string, ResourceRecord>::const_iterator iter = m_Resources.find(ID);
  if (iter!=m_Resources.end())
  {
    return iter->second;
  }
  DuskLog() << "ResourceBase::getResourceData: ERROR! No object with ID \""<<ID
            << "\" found. Throwing exception.\n";
  throw IDNotFound("ResourceBase", ID);
}

bool ResourceBase::deleteResource(const std::string& ID)
{
  std::map<std::string, ResourceRecord>::iterator iter = m_Resources.find(ID);
  if (iter!=m_Resources.end())
  {
    m_Resources.erase(iter);
    return true;
  }
  return false;
}

void ResourceBase::clearAll()
{
  m_Resources.clear();
}

unsigned int ResourceBase::getNumberOfResources() const
{
  return m_Resources.size();
}

bool ResourceBase::saveAllToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
     DuskLog() << "ResourceBase::saveAllToStream: ERROR: Stream contains errors!\n";
     return false;
  }

  std::map<std::string, ResourceRecord>::const_iterator iter;
  unsigned int len;
  iter = m_Resources.begin();
  while (iter != m_Resources.end())
  {
    //write header "Rsrc"
    outStream.write((const char*) &cHeaderRsrc, 4); //Resource
    //write ID
    len = iter->first.length();
    outStream.write((const char*) &len, sizeof(unsigned int));
    outStream.write(iter->first.c_str(), len);
    //write mesh (spawned state)
    len = iter->second.meshSpawned.length();
    outStream.write((const char*) &len, sizeof(unsigned int));
    outStream.write(iter->second.meshSpawned.c_str(), len);
    //write mesh (harvested state)
    len = iter->second.meshHarvested.length();
    outStream.write((const char*) &len, sizeof(unsigned int));
    outStream.write(iter->second.meshHarvested.c_str(), len);
    //write item ID
    len = iter->second.harvestItem.length();
    outStream.write((const char*) &len, sizeof(unsigned int));
    outStream.write(iter->second.harvestItem.c_str(), len);
    //write respawn interval
    outStream.write((const char*) &(iter->second.respawnInterval), sizeof(float));
    //write harvest sound
    len = iter->second.harvestSound.length();
    outStream.write((const char*) &len, sizeof(unsigned int));
    outStream.write(iter->second.harvestSound.c_str(), len);
    //check
    if (!outStream.good())
    {
      DuskLog() << "ResourceBase::saveAllToStream: Error while writing data to "
                << "stream. Current object is \""<<iter->first<<"\".\n";
      return false;
    }
    ++iter;
  }//while
  return outStream.good();
}

bool ResourceBase::loadNextResourceFromStream(std::ifstream& inStream)
{
  unsigned int len;
  unsigned int Header = 0;
  char buffer[256];

  ResourceRecord temp;

  //read header "Rsrc" (Resource)
  inStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRsrc)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }
  const std::string tempID = std::string(buffer);
  //read length of mesh (spawned)
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: Mesh path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh (spawned)
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }
  temp.meshSpawned = std::string(buffer);

  //read length of mesh (harvested)
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: Mesh path cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read mesh (harvested)
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }
  temp.meshHarvested = std::string(buffer);

  //read length of item ID
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: Item ID cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read item ID
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }
  temp.harvestItem = std::string(buffer);

  //read respawn interval
  inStream.read((char*) &temp.respawnInterval, sizeof(float));
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }

  //read length of sound ID
  inStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR: Sound ID cannot "
              << "be longer than 255 characters.\n";
    return false;
  }
  //read sound ID
  memset(buffer, 0, 256);
  inStream.read(buffer, len);
  if (!(inStream.good()))
  {
    DuskLog() << "ResourceBase::loadNextResourceFromStream: ERROR while reading data.\n";
    return false;
  }
  temp.harvestSound = std::string(buffer);

  //now add it to the data
  addResource(tempID, temp);
  return true;
}

#ifdef DUSK_EDITOR
ResourceBase::Iterator ResourceBase::getFirst() const
{
  return m_Resources.begin();
}

ResourceBase::Iterator ResourceBase::getEnd() const
{
  return m_Resources.end();
}
#endif

} //namespace
