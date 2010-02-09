#include "AnimationData.h"
#include "DuskConstants.h"

namespace Dusk
{

AnimationData::AnimationData()
{
  m_ReferenceMap.clear();
  m_RefCount = 0;
}

AnimationData::~AnimationData()
{
  ClearData();
  m_RefCount = 0;
}

AnimationData& AnimationData::GetSingleton()
{
  static AnimationData Instance;
  return Instance;
}

unsigned int AnimationData::NumberOfReferences() const
{
  return m_RefCount;
}

AnimatedObject* AnimationData::addAnimatedReference(const std::string ID,
    const Ogre::Vector3 position, const Ogre::Vector3 rotation, const float scale)
{
  AnimatedObject * ObjectPointer = new AnimatedObject(ID, position, rotation, scale);
  m_ReferenceMap[ID].push_back(ObjectPointer);
  ++m_RefCount;
  return ObjectPointer;
}

NPC* AnimationData::addNPCReference(const std::string ID,
     const Ogre::Vector3 position, const Ogre::Vector3 rot, const float Scale)
{
  NPC* NPCPointer = new NPC(ID, position, rot, Scale);
  m_ReferenceMap[ID].push_back(NPCPointer);
  ++m_RefCount;
  return NPCPointer;
}

AnimatedObject* AnimationData::GetAnimatedObjectReference(const std::string& ID) const
{
  std::map<std::string, std::vector<AnimatedObject*> >::const_iterator iter;
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

NPC* AnimationData::GetNPCReference(const std::string& ID) const
{
  AnimatedObject* ap = GetAnimatedObjectReference(ID);
  if (ap!=NULL)
  {
    if (ap->GetType() != otNPC)
    {
      ap = NULL;
    }
  }
  return (NPC*)ap;
}

void AnimationData::InjectAnimationTime(const float TimePassed)
{
  unsigned int i;
  std::map<std::string, std::vector<AnimatedObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    for (i=0; i<iter->second.size(); i++)
    {
      if (iter->second.at(i)!=NULL)
      {
        iter->second.at(i)->Move(TimePassed);
      }
    }//for
    ++iter;
  }//while
}

void AnimationData::ClearData()
{
  AnimatedObject * ObjPtr;
  std::map<std::string, std::vector<AnimatedObject*> >::iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    while(!(iter->second.empty()))
    {
      ObjPtr = iter->second.back();
      ObjPtr->Disable();
      delete ObjPtr;
      iter->second.pop_back();
    }//while
    iter->second.clear();
    m_ReferenceMap.erase(iter);
    iter = m_ReferenceMap.begin();
  }//while
  m_RefCount = 0;
}//clear data

bool AnimationData::SaveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "AnimationData::SaveToStream: ERROR: Bad stream given.\n";
    return false;
  }
  unsigned int i;
  std::map<std::string, std::vector<AnimatedObject*> >::const_iterator iter;
  iter = m_ReferenceMap.begin();
  while (iter!=m_ReferenceMap.end())
  {
    const unsigned int len = iter->second.size();
    for (i=0; i<len; i=i+1)
    {
      if (iter->second.at(i)!=NULL)
      {
        if (!(iter->second.at(i)->SaveToStream(output)))
        {
          std::cout << "AnimationData::SaveToStream: ERROR while saving reference.\n";
          return false;
        } //if
      }//if
    }//for
    ++iter;
  }//while
  return output.good();
}

bool AnimationData::LoadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader)
{
  AnimatedObject * animPtr = NULL;
  switch(PrefetchedHeader)
  {
    case cHeaderRefA:
         animPtr = new AnimatedObject;
         if (animPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->GetID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
         break;
    case cHeaderRefN:
         animPtr = new NPC;
         if (animPtr->LoadFromStream(Stream))
         {
           m_ReferenceMap[animPtr->GetID()].push_back(animPtr);
           ++m_RefCount;
           return true;
         }
         delete animPtr;
         break;
    default:
         std::cout << "AnimationData::LoadNextFromStream: ERROR: unexpected "
                   << "header found.\n";
         break;
  }//swi
  return false;
}

}//namespace
