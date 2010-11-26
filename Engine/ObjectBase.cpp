/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
#include "DuskConstants.h"
#include <iostream>

namespace Dusk
{

ObjectBase::ObjectBase()
{
  m_ObjectList = std::map<std::string, ObjectRecord>();
}

ObjectBase::~ObjectBase()
{
  m_ObjectList.clear();
}

ObjectBase& ObjectBase::getSingleton()
{
  static ObjectBase Instance;
  return Instance;
}

bool ObjectBase::hasObject(const std::string& IDOfObject) const
{
  return (m_ObjectList.find(IDOfObject) != m_ObjectList.end());
}

void ObjectBase::addObject(const std::string& ID, const std::string& Mesh, const bool collision)
{
  if (ID=="" or Mesh=="")
  {
    std::cout << "ObjectBase::addObject: ERROR: ID or Mesh is empty string.\n";
    return;
  }
  ObjectRecord temp;
  temp.Mesh = Mesh;
  temp.collide = collision;
  m_ObjectList[ID] = temp;
}

bool ObjectBase::deleteObject(const std::string& ID_of_Object)
{
  std::map<std::string, ObjectRecord>::iterator iter;
  iter = m_ObjectList.find(ID_of_Object);
  if (iter==m_ObjectList.end())
  {
    return false;
  }
  m_ObjectList.erase(iter);
  return true;
}

void ObjectBase::clearAllObjects()
{
  m_ObjectList.clear();
}

unsigned int ObjectBase::numberOfObjects() const
{
  return m_ObjectList.size();
}

std::string ObjectBase::getMeshName(const std::string& ID, const bool UseMarkerOnError) const
{
  std::map<std::string, ObjectRecord>::const_iterator iter = m_ObjectList.find(ID);
  if (iter!=m_ObjectList.end())
  {
    return iter->second.Mesh;
  }
  //if we get to this point, object is not present in list
  if (UseMarkerOnError)
  {
    return cErrorMesh;
  }
  return "";
}

bool ObjectBase::getObjectCollision(const std::string& ID) const
{
  const std::map<std::string, ObjectRecord>::const_iterator iter = m_ObjectList.find(ID);
  if (iter!=m_ObjectList.end())
  {
    return iter->second.collide;
  }
  //nothing found here, so throw?
  throw 42;
  return true;
}

bool ObjectBase::saveToStream(std::ofstream& Stream) const
{
  std::map<std::string, ObjectRecord>::const_iterator iter;
  unsigned int len;

  for (iter = m_ObjectList.begin(); iter != m_ObjectList.end(); iter++)
  {
    //write header "ObjS"
    Stream.write((char*) &cHeaderObjS, sizeof(unsigned int)); //Object, Static
    //write ID
    len = iter->first.length();
    Stream.write((char*) &len, sizeof(unsigned int));
    Stream.write(iter->first.c_str(), len);
    //write mesh
    len = iter->second.Mesh.length();
    Stream.write((char*) &len, sizeof(unsigned int));
    Stream.write(iter->second.Mesh.c_str(), len);
    //write collision flag
    Stream.write((char*) &(iter->second.collide), sizeof(bool));
    //check
    if (!Stream.good())
    {
      std::cout << "ObjectBase::SaveToStream: Error while writing data to stream.\n";
      return false;
    }
  }//for
  return Stream.good();
}

bool ObjectBase::loadFromStream(std::ifstream& Stream)
{
  unsigned int len;
  unsigned int Header = 0;
  static char ID_Buffer[256], Mesh_Buffer[256];

  //read header "ObjS" (Object, Static)
  Stream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderObjS)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: Stream contains invalid "
              << "record header.\n";
    return false;
  }//if
  //read length of ID
  Stream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  Stream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream.good()))
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR while reading data.\n";
    return false;
  }
  //read length of mesh name
  Stream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR: Name of Mesh cannot be "
              << "longer than 255 characters.\n";
    return false;
  }
  //read mesh
  Stream.read(Mesh_Buffer, len);
  Mesh_Buffer[len] = '\0'; //add terminating null character
  //read collision flag
  bool collision_flag = true;
  Stream.read((char*) &collision_flag, sizeof(bool));
  if (!(Stream.good()))
  {
    std::cout << "ObjectBase::LoadFromStream: ERROR while reading data.\n";
    return false;
  }
  //now add it to the data
  addObject(std::string(ID_Buffer), std::string(Mesh_Buffer), collision_flag);
  return true;
}

std::map<std::string, ObjectRecord>::const_iterator ObjectBase::getFirst() const
{
  return m_ObjectList.begin();
}

std::map<std::string, ObjectRecord>::const_iterator ObjectBase::getEnd() const
{
  return m_ObjectList.end();
}

}//namespace
