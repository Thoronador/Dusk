#include "ObjectData.h"
#include "DuskConstants.h"
#include "ObjectBase.h"
#include "LightBase.h"
#include "ContainerBase.h"

namespace Dusk
{

ObjectData::ObjectData()
{
  m_ReferenceList.clear();
}

ObjectData::~ObjectData()
{
  ClearData();
}

ObjectData& ObjectData::GetSingleton()
{
  static ObjectData Instance;
  return Instance;
}

unsigned int ObjectData::NumberOfReferences() const
{
  return m_ReferenceList.size();
}

DuskObject* ObjectData::addObjectReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  DuskObject * ObjectPointer = new DuskObject(ID, position, rotation, scale);
  m_ReferenceList.push_back(ObjectPointer);
  return ObjectPointer;
}

Light* ObjectData::addLightReference(const std::string& ID, const Ogre::Vector3& position)
{
  Light * LightPointer = new Light(ID, position);
  m_ReferenceList.push_back(LightPointer);
  return LightPointer;
}

Container* ObjectData::addContainerReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  Container* ContainerPointer = new Container(ID, position, rotation, scale);
  m_ReferenceList.push_back(ContainerPointer);
  return ContainerPointer;
}

Item* ObjectData::addItemReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  Item* ItemPointer = new Item(ID, position, rotation, scale);
  m_ReferenceList.push_back(ItemPointer);
  return ItemPointer;
}

bool ObjectData::SaveAllToStream(std::ofstream& Stream) const
{
  unsigned int i;

  for (i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      if (!(m_ReferenceList.at(i)->SaveToStream(Stream)) or (!Stream.good()))
      {
        std::cout << "ObjectData::SaveAllToStream: ERROR while writing reference data.\n";
        return false;
      }
    }//if
  }//for
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
           m_ReferenceList.push_back(objPtr);
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefL:
         objPtr = new Light;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceList.push_back(objPtr);
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefC:
         objPtr = new Container;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceList.push_back(objPtr);
           return true;
         }
         delete objPtr;
         break;
    case cHeaderRefI:
         objPtr = new Item;
         if (objPtr->LoadFromStream(Stream))
         {
           m_ReferenceList.push_back(objPtr);
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
  for(i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      m_ReferenceList.at(i)->Enable(scm);
    }
  }//for
}

void ObjectData::DisableAllObjects()
{
  unsigned int i;
  for(i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      m_ReferenceList.at(i)->Disable();
    }
  }//for
}

unsigned int ObjectData::deleteReferencesOfObject(const std::string& ID)
{
  unsigned int deletedReferences = 0;
  long int i;
  for (i=m_ReferenceList.size()-1; i>=0; i=i-1)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      if (m_ReferenceList.at(i)->GetID()==ID)
      {
        delete m_ReferenceList.at(i);
        deletedReferences++;
        //swap position of deleted element mith one at the end
        // (needed later do do only one call to erase)
        m_ReferenceList.at(i) = m_ReferenceList.at(m_ReferenceList.size()-deletedReferences);
        m_ReferenceList.at(m_ReferenceList.size()-deletedReferences) = NULL;
      }//if
    }//if
  }//for
  if (deletedReferences >0)
  {
    m_ReferenceList.erase(m_ReferenceList.begin()+m_ReferenceList.size()-deletedReferences, m_ReferenceList.end());
  }//if
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
      !ContainerBase::GetSingleton().HasContainer(ID))
  {
    std::cout << "ObjectData::reenableReferencesOfObject: ERROR: there is no "
              << "record about object with the new ID \""+ID+"\" within the"
              << "ObjectBase, LightBase or ContainerBase classes. Aborting.\n";
    return 0;
  }
  for (position = 0; position<m_ReferenceList.size(); position++)
  {
    if (m_ReferenceList.at(position)!=NULL)
    {
      if (m_ReferenceList.at(position)->GetID()==ID && m_ReferenceList.at(position)->IsEnabled())
      {
        m_ReferenceList.at(position)->Disable();
        m_ReferenceList.at(position)->Enable(scm);
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
  unsigned int position;

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
      !ContainerBase::GetSingleton().HasContainer(newID))
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: ERROR: there is "
              << "no record about object with the new ID \""+newID+"\" within "
              << "the ObjectBase, LightBase or ContainerBase classes. "
              << "Aborting.\n";
    return 0;
  }
  if (scm==NULL)
  {
    std::cout << "ObjectData::updateReferencesAfterIDChange: ERROR: got NULL "
              << "pointer for scene manager. Unable to update enabled objects.\n";
    return 0;
  }

  //now search and update
  for (position = 0; position<m_ReferenceList.size(); position++)
  {
    if (m_ReferenceList.at(position)!=NULL)
    {
      if (m_ReferenceList.at(position)->GetID()==oldID)
      { //got reference of object
        if (m_ReferenceList.at(position)->IsEnabled())
        { //we cannot change ID of enabled objects, so disable them first
          m_ReferenceList.at(position)->Disable();
          m_ReferenceList.at(position)->ChangeID(newID);
          m_ReferenceList.at(position)->Enable(scm);
        }
        else
        { //not enabled, so simply change ID
          m_ReferenceList.at(position)->ChangeID(newID);
        }
        updated_references++;
      }//if
    }//if
  }//for
  return updated_references;
}

void ObjectData::ClearData()
{
  DuskObject * ObjPtr;
  while(!m_ReferenceList.empty())
  {
    ObjPtr = m_ReferenceList.back();
    ObjPtr->Disable();
    delete ObjPtr;
    m_ReferenceList.pop_back();
  }//while
  m_ReferenceList.clear();
}//clear data

}//namespace
