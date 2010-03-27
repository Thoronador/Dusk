#include "ObjectData.h"
#include "DuskConstants.h"
#include "ObjectBase.h"
#include "LightBase.h"
#include "ContainerBase.h"
#include "ItemBase.h"

namespace Dusk
{

ObjectData::ObjectData()
{
  m_ReferenceMap.clear();
  m_RefCount = 0;
}

ObjectData::~ObjectData()
{
  ClearData();
  m_RefCount = 0;
}

ObjectData& ObjectData::GetSingleton()
{
  static ObjectData Instance;
  return Instance;
}

unsigned int ObjectData::NumberOfReferences() const
{
  return m_RefCount;
}

DuskObject* ObjectData::addObjectReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  DuskObject * ObjectPointer = new DuskObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ObjectPointer);
  ++m_RefCount;
  return ObjectPointer;
}

Light* ObjectData::addLightReference(const std::string& ID, const Ogre::Vector3& position)
{
  Light * LightPointer = new Light(ID, position);
  m_ReferenceMap[ID].push_back(LightPointer);
  ++m_RefCount;
  return LightPointer;
}

Container* ObjectData::addContainerReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  Container* ContainerPointer = new Container(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ContainerPointer);
  ++m_RefCount;
  return ContainerPointer;
}

Item* ObjectData::addItemReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  Item* ItemPointer = new Item(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ItemPointer);
  ++m_RefCount;
  return ItemPointer;
}

DuskObject* ObjectData::GetObjectByID(const std::string& ID) const
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

bool ObjectData::removeItemReference(Item* pItem)
{
  if (pItem==NULL) return false;
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.find(pItem->GetID());
  if (iter!=m_ReferenceMap.end())
  {
    unsigned int i;
    for (i=0; i<iter->second.size(); ++i)
    {
      if (iter->second.at(i)==pItem)
      {
        //found it
        pItem->Disable();
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

bool ObjectData::SaveAllToStream(std::ofstream& Stream) const
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
        if (!(iter->second.at(i)->SaveToStream(Stream)) or (!Stream.good()))
        {
          std::cout << "ObjectData::SaveAllToStream: ERROR while writing reference data.\n";
          return false;
        }
      }//if
    }//for
    ++iter;
  }//while
  return true;
}

bool ObjectData::LoadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader)
{
  DuskObject * objPtr = NULL;
  switch(PrefetchedHeader)
  {
    case cHeaderRefO:
         objPtr = new DuskObject;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->GetID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefL:
         objPtr = new Light;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->GetID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefC:
         objPtr = new Container;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->GetID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefI:
         objPtr = new Item;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[objPtr->GetID()].push_back(objPtr);
           ++m_RefCount;
           return true;
         }
         delete objPtr;
         break;
    default:
         std::cout << "ObjectData::LoadNextFromStream: ERROR: unexpected header.\n";
         break;
  }//swi
  return false;
}

void ObjectData::EnableAllObjects(Ogre::SceneManager * scm)
{
  unsigned int i;
  if (scm==NULL)
  {
    std::cout << "ObjectData::EnableAllObjects: ERROR: Scene Manager is NULL!\n";
    return;
  }

  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for(i=0; i<iter->second.size(); i++)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->Enable(scm);
      }
    }//for
    ++iter;
  }//while
}

void ObjectData::DisableAllObjects()
{
  unsigned int i;
  std::map<std::string, std::vector<DuskObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for(i=0; i<iter->second.size(); i++)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->Disable();
      }
    }//for
    ++iter;
  }//while
}

unsigned int ObjectData::deleteReferencesOfObject(const std::string& ID)
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
unsigned int ObjectData::reenableReferencesOfObject(const std::string& ID, Ogre::SceneManager * scm)
{
  unsigned int re_enabled = 0;
  unsigned int position;

  if (scm==NULL)
  {
    std::cout << "ObjectData::reenableReferencesOfObject: ERROR: Scene Manager is NULL pointer!";
    return 0;
  }
  if (!ObjectBase::GetSingleton().hasObject(ID) and
      !LightBase::GetSingleton().hasLight(ID) and
      !ContainerBase::GetSingleton().HasContainer(ID) and
      !ItemBase::GetSingleton().hasItem(ID))
  {
    std::cout << "ObjectData::reenableReferencesOfObject: ERROR: there is no "
              << "record about object with the new ID \""+ID+"\" within the"
              << "ObjectBase, LightBase, ContainerBase or ItemBase classes. "
              << "Aborting.\n";
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
      if (iter->second.at(position)->IsEnabled())
      {
        iter->second.at(position)->Disable();
        iter->second.at(position)->Enable(scm);
        re_enabled++;
      }//if
    }//if
  }//for
  return re_enabled;
}

/// method to update all references of an object after the ID was changed (by Editor application)
unsigned int ObjectData::updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm)
{
  unsigned int updated_references = 0;

  if (oldID=="" or newID=="")
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: ERROR: old ID or "
              << "new ID is empty string. We don't want empty ID strings!\n";
    return 0;
  }
  if (oldID==newID)
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: Hint: old ID is "
              << " the same as new ID. No need to change anything here.\n";
    return 0;
  }
  if (!ObjectBase::GetSingleton().hasObject(newID) and
      !LightBase::GetSingleton().hasLight(newID) and
      !ContainerBase::GetSingleton().HasContainer(newID) and
      !ItemBase::GetSingleton().hasItem(newID))
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: ERROR: there is "
              << "no record about object with the new ID \""+newID+"\" within "
              << "the ObjectBase, LightBase, ContainerBase or ItemBase classes."
              << " Aborting.\n";
    return 0;
  }
  if (scm==NULL)
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: ERROR: got NULL "
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
      if (objPtr->IsEnabled())
      { //we cannot change ID of enabled objects, so disable them first
        objPtr->Disable();
        objPtr->ChangeID(newID);
        objPtr->Enable(scm);
      }
      else
      { //not enabled, so simply change ID
        objPtr->ChangeID(newID);
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

void ObjectData::ClearData()
{
  DuskObject * ObjPtr;
  std::map<std::string, std::vector<DuskObject*> >::iterator iter;
  iter = m_ReferenceMap.begin();
  while(iter!=m_ReferenceMap.end())
  {
     while (!(iter->second.empty()))
     {
      ObjPtr = iter->second.back();
      ObjPtr->Disable();
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
