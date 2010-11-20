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

#include "InjectionManager.h"
#include "DuskConstants.h"
#ifdef DUSK_EDITOR
  #include "NPCBase.h"
#endif
#include "Vehicle.h"

namespace Dusk
{

InjectionManager::InjectionManager()
{
  m_ReferenceMap.clear();
  m_RefCount = 0;
  m_DeletionObjects.clear();
}

InjectionManager::~InjectionManager()
{
  clearData();
  m_RefCount = 0;
}

InjectionManager& InjectionManager::getSingleton()
{
  static InjectionManager Instance;
  return Instance;
}

unsigned int InjectionManager::getNumberOfReferences() const
{
  return m_RefCount;
}

AnimatedObject* InjectionManager::addAnimatedReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Quaternion& rotation, const float scale)
{
  AnimatedObject * ObjectPointer = new AnimatedObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ObjectPointer);
  ++m_RefCount;
  return ObjectPointer;
}

NPC* InjectionManager::addNPCReference(const std::string& ID,
     const Ogre::Vector3& position, const Ogre::Quaternion& rot, const float Scale)
{
  NPC* NPCPointer = new NPC(ID, position, rot, Scale);
  m_ReferenceMap[ID].push_back(NPCPointer);
  ++m_RefCount;
  return NPCPointer;
}

Vehicle* InjectionManager::addVehicleReference(const std::string& ID, const Ogre::Vector3& position,
                                   const Ogre::Quaternion& rotation, const float scale)
{
  Vehicle* vehiPtr = new Vehicle(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(vehiPtr);
  ++m_RefCount;
  return vehiPtr;
}

WaypointObject* InjectionManager::addWaypointReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Quaternion& rotation, const float scale)
{
  WaypointObject* wpPointer = new WaypointObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(wpPointer);
  ++m_RefCount;
  return wpPointer;
}

Projectile* InjectionManager::addProjectileReference(const std::string& ID, const Ogre::Vector3& position,
                                   const Ogre::Quaternion& rotation, const float scale)
{
  Projectile* projPtr = new Projectile(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(projPtr);
  ++m_RefCount;
  return projPtr;
}

InjectionObject* InjectionManager::getInjectionObjectReference(const std::string& ID) const
{
  std::map<std::string, std::vector<InjectionObject*> >::const_iterator iter;
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

AnimatedObject* InjectionManager::getAnimatedObjectReference(const std::string& ID) const
{
  std::map<std::string, std::vector<InjectionObject*> >::const_iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter!=m_ReferenceMap.end())
  {
    if (!(iter->second.empty()))
    {
      if ((iter->second.at(0)->getDuskType()==otNPC) or
          (iter->second.at(0)->getDuskType()==otAnimated))
      {
        return (dynamic_cast<AnimatedObject*>(iter->second.at(0)));
      }
    }
  }
  return NULL;
}

NPC* InjectionManager::getNPCReference(const std::string& ID) const
{
  InjectionObject* ap = getInjectionObjectReference(ID);
  if (ap!=NULL)
  {
    if (ap->getDuskType() != otNPC)
    {
      return NULL;
    }
  }
  return (dynamic_cast<NPC*>(ap));
}

#ifdef DUSK_EDITOR
unsigned int InjectionManager::deleteReferencesOfAnimatedObject(const std::string& del_ID)
{
  std::map<std::string, std::vector<InjectionObject*> >::iterator iter;
  iter = m_ReferenceMap.find(del_ID);
  if (iter==m_ReferenceMap.end())
  { //nothing found
    return 0;
  }//if
  unsigned int deletedReferences = 0;
  long int i;
  for (i=iter->second.size()-1; i>=0; --i)
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

unsigned int InjectionManager::reenableReferencesOfObject(const std::string& ID, Ogre::SceneManager * scm)
{
  unsigned int re_enabled = 0;
  if (scm==NULL)
  {
    std::cout << "InjectionManager::reenableReferencesOfObject: ERROR: Scene "
              << "Manager is NULL pointer!";
    return 0;
  }
  if (!NPCBase::getSingleton().hasNPC(ID))
  {
    std::cout << "InjectionManager::reenableReferencesOfObject: ERROR: there is"
              << " no record about NPC with the new ID \""+ID+"\" within the "
              << "NPCBase class. Aborting.\n";
    return 0;
  }
  std::map<std::string, std::vector<InjectionObject*> >::iterator iter;
  iter = m_ReferenceMap.find(ID);
  if (iter==m_ReferenceMap.end())
  {
    return 0;
  }//if
  unsigned int position;
  for (position = 0; position<iter->second.size(); ++position)
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

unsigned int InjectionManager::updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm)
{
  if (oldID=="" or newID=="")
  {
    std::cout << "InjectionManager::updateReferencesAfterIDChange: ERROR: old "
              << "ID or new ID is empty string. We don't want empty ID "
              << "strings!\n";
    return 0;
  }
  if (oldID==newID)
  {
    std::cout << "InjectionManager::updateReferencesAfterIDChange: Hint: old ID"
              << " is the same as new ID. No need to change anything here.\n";
    return 0;
  }
  if (!NPCBase::getSingleton().hasNPC(newID))
  {
    std::cout << "InjectionManager::updateReferencesAfterIDChange: ERROR: there"
              << " is no record about object with the new ID \""+newID+"\" "
              << "within the NPCBase class. Aborting.\n";
    return 0;
  }
  if (scm==NULL)
  {
    std::cout << "InjectionManager::updateReferencesAfterIDChange: ERROR: got "
              << "NULL pointer for scene manager. Unable to update enabled "
              << "objects.\n";
    return 0;
  }
  std::map<std::string, std::vector<InjectionObject*> >::iterator iter;
  iter = m_ReferenceMap.find(oldID);
  if (iter==m_ReferenceMap.end())
  {
    return 0;
  }//if

  unsigned int updated_references = 0;
  //now update
  while (!(iter->second.empty()))
  {
    InjectionObject* objPtr = iter->second.back();
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
    }//if not NULL
    iter->second.pop_back();
    --m_RefCount;
    ++updated_references;
  }//while
  return updated_references;
}
#endif //ifdef DUSK_EDITOR

void InjectionManager::injectAnimationTime(const float TimePassed)
{
  if (!m_DeletionObjects.empty())
  {
    performRequestedDeletions();
  }
  unsigned int i;
  std::map<std::string, std::vector<InjectionObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for (i=0; i<iter->second.size(); i++)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->injectTime(TimePassed);
      }
    }//for
    ++iter;
  }//while
}

void InjectionManager::clearData()
{
  InjectionObject * ObjPtr;
  std::map<std::string, std::vector<InjectionObject*> >::iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    while(!(iter->second.empty()))
    {
      ObjPtr = iter->second.back();
      ObjPtr->disable();
      delete ObjPtr;
      iter->second.pop_back();
    }//while
    iter->second.clear();
    m_ReferenceMap.erase(iter);
    iter = m_ReferenceMap.begin();
  }//while
  m_RefCount = 0;
}//clear data

bool InjectionManager::saveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "InjectionManager::saveAllToStream: ERROR: Bad stream given.\n";
    return false;
  }
  unsigned int i;
  std::map<std::string, std::vector<InjectionObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    const unsigned int len = iter->second.size();
    for (i=0; i<len; i=i+1)
    {
      if (iter->second.at(i)!=NULL)
      {
        if (!(iter->second.at(i)->saveToStream(output)))
        {
          std::cout << "InjectionManager::saveAllToStream: ERROR while saving "
                    << "reference.\n";
          return false;
        } //if
      }//if
    }//for
    ++iter;
  }//while
  return output.good();
}

bool InjectionManager::loadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader)
{
  InjectionObject * injectPtr = NULL;
  switch(PrefetchedHeader)
  {
    case cHeaderRefA:
         injectPtr = new AnimatedObject;
         if (injectPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[injectPtr->getID()].push_back(injectPtr);
           ++m_RefCount;
           return true;
         }
         delete injectPtr;
         break;
    case cHeaderRefN:
         injectPtr = new NPC;
         if (injectPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[injectPtr->getID()].push_back(injectPtr);
           ++m_RefCount;
           return true;
         }
         delete injectPtr;
         break;
    case cHeaderRefP:
         injectPtr = new Projectile;
         if (injectPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[injectPtr->getID()].push_back(injectPtr);
           ++m_RefCount;
           return true;
         }
         delete injectPtr;
         break;
    case cHeaderRefV:
         injectPtr = new Vehicle;
         if (injectPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[injectPtr->getID()].push_back(injectPtr);
           ++m_RefCount;
           return true;
         }
         delete injectPtr;
         break;
    case cHeaderRfWP:
         injectPtr = new WaypointObject;
         if (injectPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[injectPtr->getID()].push_back(injectPtr);
           ++m_RefCount;
           return true;
         }
         delete injectPtr;
         break;
    default:
         std::cout << "InjectionManager::LoadNextFromStream: ERROR: unexpected"
                   << " header found.\n";
         break;
  }//swi
  return false;
}

void InjectionManager::requestDeletion(InjectionObject* objPtr)
{
  //we don't want NULL pointers
  if (objPtr!=NULL)
  {
    if (m_DeletionObjects.empty())
    {
      m_DeletionObjects.push_back(objPtr);
    }
    //and we don't want to push the same object twice in a row
    else if (m_DeletionObjects.back()!=objPtr)
    {
      m_DeletionObjects.push_back(objPtr);
    }
  }
}

void InjectionManager::performRequestedDeletions()
{
  std::map<std::string, std::vector<InjectionObject*> >::iterator iter;
  while (!m_DeletionObjects.empty())
  {
    iter = m_ReferenceMap.find(m_DeletionObjects.back()->getID());
    if (iter!=m_ReferenceMap.end())
    {
      unsigned int i;
      for (i=0; i<iter->second.size(); ++i)
      {
        if (iter->second.at(i)==m_DeletionObjects.back())
        {
          //found it
          m_DeletionObjects.back()->disable();
          delete m_DeletionObjects.back();
          m_DeletionObjects.back() = NULL; //not really needed here
          iter->second.at(i)= iter->second.at(iter->second.size()-1);
          iter->second.at(iter->second.size()-1) = NULL;
          iter->second.pop_back();
          --m_RefCount;
        }
      }//for
    }
    m_DeletionObjects.pop_back();
  } //while
}

InjectionManager::ConstMapIterator InjectionManager::getBegin() const
{
  return m_ReferenceMap.begin();
}

InjectionManager::ConstMapIterator InjectionManager::getEnd() const
{
  return m_ReferenceMap.end();
}

}//namespace
