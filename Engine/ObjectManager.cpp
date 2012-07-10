/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012  thoronador

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

#include "ObjectManager.h"
#include "DuskConstants.h"
#include "Database.h"
#include "ContainerRecord.h"
#include "Messages.h"

namespace Dusk
{

ObjectManager::ObjectManager()
{
  m_ReferenceMap.clear();
  m_RefCount = 0;
}

ObjectManager::~ObjectManager()
{
  clearData();
  m_RefCount = 0;
}

ObjectManager& ObjectManager::getSingleton()
{
  static ObjectManager Instance;
  return Instance;
}

unsigned int ObjectManager::getNumberOfReferences() const
{
  return m_RefCount;
}

DuskObject* ObjectManager::addObjectReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const float scale)
{
  DuskObject * ObjectPointer = new DuskObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ObjectPointer);
  ++m_RefCount;
  return ObjectPointer;
}

Light* ObjectManager::addLightReference(const std::string& ID, const Ogre::Vector3& position)
{
  Light * LightPointer = new Light(ID, position);
  m_ReferenceMap[ID].push_back(LightPointer);
  ++m_RefCount;
  return LightPointer;
}

Container* ObjectManager::addContainerReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const float scale)
{
  Container* ContainerPointer = new Container(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ContainerPointer);
  ++m_RefCount;
  return ContainerPointer;
}

Item* ObjectManager::addItemReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const float scale)
{
  Item* ItemPointer = new Item(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ItemPointer);
  ++m_RefCount;
  return ItemPointer;
}

Weapon* ObjectManager::addWeaponReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const float scale)
{
  Weapon* WeaponPointer = new Weapon(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(WeaponPointer);
  ++m_RefCount;
  return WeaponPointer;
}

DuskObject* ObjectManager::getObjectByID(const std::string& ID) const
{
  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter!=m_ReferenceMap.end())
  {
    if (!(iter->second.empty()))
    {
      return iter->second.at(0);
    }
  }
  return NULL;
}

bool ObjectManager::isObjectPresent(const std::string& ID) const
{
  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter!=m_ReferenceMap.end())
  {
    return (!(iter->second.empty()));
  }
  return false;
}

bool ObjectManager::removeItemReference(Item* pItem)
{
  if (pItem==NULL) return false;
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.find(pItem->getID());
  if (iter!=m_ReferenceMap.end())
  {
    unsigned int i;
    for (i=0; i<iter->second.size(); ++i)
    {
      if (iter->second.at(i)==pItem)
      {
        //found it
        pItem->disable();
        delete pItem;
        pItem = NULL; //not really needed here
        iter->second.at(i)= iter->second.at(iter->second.size()-1);
        iter->second.at(iter->second.size()-1) = NULL;
        iter->second.pop_back();
        --m_RefCount;
        return true;
      }
    }//for
  }
  return false;
}

bool ObjectManager::saveAllToStream(std::ofstream& Stream) const
{
  unsigned int i;
  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for (i=0; i<iter->second.size(); i++)
    {
      if (iter->second.at(i)!=NULL)
      {
        if (!(iter->second.at(i)->saveToStream(Stream)) or (!Stream.good()))
        {
          DuskLog() << "ObjectManager::saveAllToStream: ERROR while writing reference data.\n";
          return false;
        }
      }//if
    }//for
    ++iter;
  }//while
  return true;
}

bool ObjectManager::loadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader)
{
  DuskObject * objPtr = NULL;
  switch(PrefetchedHeader)
  {
    case cHeaderRefO:
         objPtr = new DuskObject;
         if (objPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->getID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefL:
         objPtr = new Light;
         if (objPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->getID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefC:
         objPtr = new Container;
         if (objPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->getID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefI:
         objPtr = new Item;
         if (objPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->getID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRfWe:
         objPtr = new Weapon;
         if (objPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->getID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    default:
         DuskLog() << "ObjectManager::loadNextFromStream: ERROR: unexpected header.\n";
         break;
  }//swi
  return false;
}

void ObjectManager::enableAllObjects(Ogre::SceneManager * scm)
{
  unsigned int i;
  if (scm==NULL)
  {
    DuskLog() << "ObjectManager::enableAllObjects: ERROR: Scene Manager is NULL!\n";
    return;
  }

  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for(i=0; i<iter->second.size(); ++i)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->enable(scm);
      }
    }//for
    ++iter;
  }//while
}

void ObjectManager::disableAllObjects()
{
  unsigned int i;
  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for(i=0; i<iter->second.size(); ++i)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->disable();
      }
    }//for
    ++iter;
  }//while
}

#ifdef DUSK_EDITOR
unsigned int ObjectManager::deleteReferencesOfObject(const std::string& ID)
{
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter==m_ReferenceMap.end())
  {
    return 0;
  }//if
  unsigned int deletedReferences = 0;
  long int i;
  for (i=iter->second.size()-1; i>=0; i=i-1)
  {
    if (iter->second.at(i)!=NULL)
    {
      delete (iter->second.at(i));
      iter->second.at(i) = NULL;
      ++deletedReferences;
    }//if
  }//for
  iter->second.clear();
  m_ReferenceMap.erase(iter);
  return deletedReferences;
}

///method to update all enabled objects of one ID after the mesh path has changed
/// currently only used by Editor
unsigned int ObjectManager::reenableReferencesOfObject(const std::string& ID, Ogre::SceneManager * scm)
{
  unsigned int re_enabled = 0;
  unsigned int position;

  if (scm==NULL)
  {
    DuskLog() << "ObjectManager::reenableReferencesOfObject: ERROR: Scene Manager is NULL pointer!";
    return 0;
  }
  if (!Database::getSingleton().hasRecord(ID))
  {
    DuskLog() << "ObjectManager::reenableReferencesOfObject: ERROR: there is no"
              << " record about object with the new ID \""+ID+"\" within the"
              << "Database or class. Aborting.\n";
    return 0;
  }
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter==m_ReferenceMap.end())
  {
    return 0;
  }//if
  for (position = 0; position<iter->second.size(); position++)
  {
    if (iter->second.at(position)!=NULL)
    {
      if (iter->second.at(position)->isEnabled())
      {
        iter->second.at(position)->disable();
        iter->second.at(position)->enable(scm);
        ++re_enabled;
      }//if
    }//if
  }//for
  return re_enabled;
}

/// method to update all references of an object after the ID was changed (by Editor application)
unsigned int ObjectManager::updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm)
{
  unsigned int updated_references = 0;

  if (oldID.empty() or newID.empty())
  {
    DuskLog() << "ObjectManager::updateReferencesAfterIDChange: ERROR: old ID "
              << "or new ID is empty string. We don't want empty ID strings!\n";
    return 0;
  }
  if (oldID==newID)
  {
    DuskLog() << "ObjectManager::updateReferencesAfterIDChange: Hint: old ID "
              << "is the same as new ID. No need to change anything here.\n";
    return 0;
  }
  if (!Database::getSingleton().hasRecord(newID))
  {
    DuskLog() << "ObjectManager::updateReferencesAfterIDChange: ERROR: there is "
              << "no record about object with the new ID \""+newID+"\" within "
              << "the Database class. Aborting.\n";
    return 0;
  }
  if (scm==NULL)
  {
    DuskLog() << "ObjectManager::updateReferencesAfterIDChange: ERROR: got NULL "
              << "pointer for scene manager. Unable to update enabled objects.\n";
    return 0;
  }
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.find(oldID);
  if (iter==m_ReferenceMap.end())
  {
    return 0;
  }//if
  //now update
  while (!(iter->second.empty()))
  {
    DuskObject* objPtr = iter->second.back();
    if (objPtr!=NULL)
    {
      if (objPtr->isEnabled())
      { //we cannot change ID of enabled objects, so disable them first
        objPtr->disable();
        objPtr->changeID(newID);
        objPtr->enable(scm);
      }
      else
      { //not enabled, so simply change ID
        objPtr->changeID(newID);
      }
      m_ReferenceMap[newID].push_back(objPtr);
      ++m_RefCount;
    }//if
    iter->second.pop_back();
    --m_RefCount;
    ++updated_references;
  }//while
  return updated_references;
}
#endif //DUSK_EDITOR

void ObjectManager::clearData()
{
  DuskObject * ObjPtr;
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.begin();
  while(iter!=m_ReferenceMap.end())
  {
     while (!(iter->second.empty()))
     {
      ObjPtr = iter->second.back();
      ObjPtr->disable();
      delete ObjPtr;
      iter->second.pop_back();
     }//while
     m_ReferenceMap.erase(iter);
     iter = m_ReferenceMap.begin();
  }//while
  m_ReferenceMap.clear();
  m_RefCount = 0;
}//clear data

}//namespace
