/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012 thoronador

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

#include "Resource.h"
#include "API.h"
#include "DuskConstants.h"
#include "Messages.h"
#include "ResourceBase.h"

namespace Dusk
{

Resource::Resource()
: DuskObject()
{
  m_Spawned = true;
  m_RespawnTime = 0.0f;
}

Resource::Resource(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
: DuskObject(_ID, pos, rot, Scale)
{
  m_Spawned = true;
  m_RespawnTime = 0.0f;
}

Resource::~Resource()
{

}

ObjectTypes Resource::getDuskType() const
{
  return otResource;
}

bool Resource::canCollide() const
{
  return true;
}

bool Resource::isSpawned() const
{
  return m_Spawned;
}

void Resource::injectTime(const float SecondsPassed)
{
  if (!m_Spawned)
  {
    m_RespawnTime -= SecondsPassed;
    if (m_RespawnTime<=0.0f)
    {
      m_Spawned = true;
      //update visualisation
      if (isEnabled())
      {
        disable();
        enable(getAPI().getOgreSceneManager());
      }
    }
  }
}

bool Resource::saveToStream(std::ofstream& outStream) const
{
  if (!outStream.good())
  {
    DuskLog() << "Resource::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefR" (reference of Resource)
  outStream.write((const char*) &cHeaderRefR, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(outStream))
  {
    DuskLog() << "Resource::saveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data from Resource
  outStream.write((const char*) &m_Spawned, sizeof(bool));
  outStream.write((const char*) &m_RespawnTime, sizeof(float));
  if (!outStream.good())
  {
    DuskLog() << "Resource::saveToStream: ERROR while writing data!\n";
    return false;
  }
  return true;
}

bool Resource::loadFromStream(std::ifstream& inStream)
{
  if (isEnabled())
  {
    DuskLog() << "Resource::loadFromStream: ERROR: Cannot load from stream "
              << "while object is enabled.\n";
    return false;
  }
  if (!inStream.good())
  {
    DuskLog() << "Resource::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefR"
  unsigned int Header = 0;
  inStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefR)
  {
    DuskLog() << "Resource::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(inStream))
  {
    DuskLog() << "Resource::loadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff from Resource
  inStream.read((char*) &m_Spawned, sizeof(bool));
  inStream.read((char*) &m_RespawnTime, sizeof(float));
  if ((m_RespawnTime!=m_RespawnTime) or !inStream.good())
  {
    DuskLog() << "Resource::loadFromToStream: ERROR while reading data!\n";
    return false;
  }
  return true;
}

const std::string& Resource::getObjectMesh() const
{
  if (m_Spawned) return ResourceBase::getSingleton().getResourceData(ID).meshSpawned;
  return ResourceBase::getSingleton().getResourceData(ID).meshHarvested;
}

} //namespace
