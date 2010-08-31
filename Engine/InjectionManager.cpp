#include "InjectionManager.h"
#include "DuskConstants.h"

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

unsigned int InjectionManager::numberOfReferences() const
{
  return m_RefCount;
}

AnimatedObject* InjectionManager::addAnimatedReference(const std::string& ID,
    const Ogre::Vector3& position, const Ogre::Vector3& rotation, const float scale)
{
  AnimatedObject * ObjectPointer = new AnimatedObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ObjectPointer);
  ++m_RefCount;
  return ObjectPointer;
}

NPC* InjectionManager::addNPCReference(const std::string& ID,
     const Ogre::Vector3& position, const Ogre::Vector3& rot, const float Scale)
{
  NPC* NPCPointer = new NPC(ID, position, rot, Scale);
  m_ReferenceMap[ID].push_back(static_cast<AnimatedObject*>(NPCPointer));
  ++m_RefCount;
  return NPCPointer;
}

WaypointObject* InjectionManager::addWaypointReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Vector3& rotation, const float scale)
{
  WaypointObject* wpPointer = new WaypointObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(static_cast<WaypointObject*>(wpPointer));
  ++m_RefCount;
  return wpPointer;
}

Projectile* InjectionManager::addProjectileReference(const std::string& ID, const Ogre::Vector3& position,
                                   const Ogre::Vector3& rotation, const float scale)
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
  InjectionObject * animPtr = NULL;
  switch(PrefetchedHeader)
  {
    case cHeaderRefA:
         animPtr = new AnimatedObject;
         if (animPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->getID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
         break;
    case cHeaderRefN:
         animPtr = new NPC;
         if (animPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->getID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
         break;
    case cHeaderRefP:
         animPtr = new Projectile;
         if (animPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->getID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
         break;
    case cHeaderRfWP:
         animPtr = new WaypointObject;
         if (animPtr->loadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->getID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
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

}//namespace
