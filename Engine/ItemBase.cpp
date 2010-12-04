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

#include "ItemBase.h"
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

ItemBase::ItemBase()
{
  m_ItemList = std::map<std::string,ItemRecord>();
}

ItemBase::~ItemBase()
{
  m_ItemList.clear();
}

ItemBase& ItemBase::getSingleton()
{
  static ItemBase Instance;
  return Instance;
}

bool ItemBase::hasItem(const std::string& ID_of_item) const
{
  return (m_ItemList.find(ID_of_item) != m_ItemList.end());
}

void ItemBase::addItem(const std::string& ID, const std::string& name,
                       const int value, const float weight,
                       const std::string& Mesh)
{
  if (ID=="" or name=="" or Mesh=="")
  {
    DuskLog() << "ItemBase::addItem: ERROR: ID, name or mesh name is empty.\n";
    return;
  }
  if (value<0 or weight<0.0f)
  {
    DuskLog() << "ItemBase::addItem: ERROR: invalid weight or value given.\n";
    return;
  }
  m_ItemList[ID] = (ItemRecord){name, value, weight, Mesh};
}

void ItemBase::addItem(const std::string& ID, const ItemRecord& record)
{
  addItem(ID, record.Name, record.value, record.weight, record.Mesh);
}

bool ItemBase::deleteItem(const std::string& ID_of_item)
{
  std::map<std::string, ItemRecord>::iterator iter;
  iter = m_ItemList.find(ID_of_item);
  if (iter == m_ItemList.end())
  {
    return false;
  }
  m_ItemList.erase(iter);
  return true;
}

void ItemBase::clearAllItems()
{
  m_ItemList.clear();
}

unsigned int ItemBase::numberOfItems() const
{
  return m_ItemList.size();
}

std::string ItemBase::getItemName(const std::string& itemID) const
{
  std::map<std::string, ItemRecord>::const_iterator iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.Name;
  }
  DuskLog() << "ItemBase::getItemName: ERROR: No item with ID \""<<itemID
            << "\" found. Returning empty string.\n";
  return "";
}

int ItemBase::getItemValue(const std::string& itemID) const
{
  std::map<std::string, ItemRecord>::const_iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.value;
  }
  DuskLog() << "ItemBase::getItemValue: ERROR: No item with ID \""<<itemID
            << "\" found. Returning -1.\n";
  return -1;
}

float ItemBase::getItemWeight(const std::string& itemID) const
{
  std::map<std::string, ItemRecord>::const_iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.weight;
  }
  DuskLog() << "ItemBase::getItemWeight: ERROR: No item with ID \""<<itemID
            << "\" found. Returning zero.\n";
  return 0.0;
}

std::string ItemBase::getMeshName(const std::string& itemID, const bool UseMarkerOnError) const
{
  std::map<std::string, ItemRecord>::const_iterator iter;
  iter = m_ItemList.find(itemID);
  if (iter != m_ItemList.end())
  {
    return iter->second.Mesh;
  }
  //item with given ID is not present
  if (UseMarkerOnError)
  {
    DuskLog() << "ItemBase::getMeshName: ERROR: No item with ID \""<<itemID
            << "\" found. Returning error marker's mesh instead.\n";
    return cErrorMesh;
  }
  else
  {
    DuskLog() << "ItemBase::getMeshName: ERROR: No item with ID \""<<itemID
            << "\" found. Returning empty string.\n";
    return "";
  }
}

bool ItemBase::saveToStream(std::ofstream& Stream) const
{
  if (!Stream.good())
  {
    DuskLog() << "ItemBase::saveToStream: ERROR: bad stream.\n";
    return false;
  }

  std::map<std::string, ItemRecord>::const_iterator iter;
  unsigned int len = 0;

  for(iter=m_ItemList.begin(); iter!=m_ItemList.end(); iter++)
  {
    //write header "Item"
    Stream.write((char*) &cHeaderItem, sizeof(unsigned int));
    //write ID
    len = iter->first.length();
    Stream.write((char*) &len, sizeof(unsigned int));
    Stream.write(iter->first.c_str(), len);
    //write name
    len = iter->second.Name.length();
    Stream.write((char*) &len, sizeof(unsigned int));
    Stream.write(iter->second.Name.c_str(), len);
    //write value
    Stream.write((char*) &(iter->second.value), sizeof(int));
    //write weight
    Stream.write((char*) &(iter->second.weight), sizeof(float));
    //write Mesh name
    len = iter->second.Mesh.length();
    Stream.write((char*) &len, sizeof(unsigned int));
    Stream.write(iter->second.Mesh.c_str(), len);

    if (!Stream.good())
    {
      DuskLog() << "ItemBase::saveToStream: ERROR while writing data.\n";
      return false;
    }//if
  }//for
  return true;
}

bool ItemBase::loadFromStream(std::ifstream& Stream)
{
  if (!Stream.good())
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR: bad stream.\n";
    return false;
  }//if
  unsigned int Header, len;
  //buffers declared static to avoid multiple allocation and deallocation
  //during multiple calls of LoadFromStream()
  static char ID_Buffer[256], Name_Buffer[256], Mesh_Buffer[256];
  int value;
  float weight;

  //read header "Item"
  Header = 0;
  Stream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderItem)
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR: Stream contains invalid "
              << "item record header.\n";
    return false;
  }//if

  //read length of ID
  Stream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  //read ID
  Stream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream.good()))
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR while reading data (ID).\n";
    return false;
  }

  //read length of item name
  Stream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR: item name cannot be longer "
              << "than 255 characters.\n";
    return false;
  }
  //read item name
  Stream.read(Name_Buffer, len);
  Name_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream.good()))
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR while reading data (name).\n";
    return false;
  }

  //read value
  Stream.read((char*) &value, sizeof(int));
  //read weight
  Stream.read((char*) &weight, sizeof(float));

  //read length of mesh name
  Stream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR: mesh name cannot be longer "
              << "than 255 characters.\n";
    return false;
  }
  //read mesh name
  Stream.read(Mesh_Buffer, len);
  Mesh_Buffer[len] = '\0'; //add terminating null character
  if (!(Stream.good()))
  {
    DuskLog() << "ItemBase::loadFromStream: ERROR while reading data (mesh).\n";
    return false;
  }
  addItem(std::string(ID_Buffer), std::string(Name_Buffer), value, weight,
          std::string(Mesh_Buffer));
  return true;
}

std::map<std::string, ItemRecord>::const_iterator ItemBase::getFirst() const
{
  return m_ItemList.begin();
}

std::map<std::string, ItemRecord>::const_iterator ItemBase::getEnd() const
{
  return m_ItemList.end();
}

}//namespace
