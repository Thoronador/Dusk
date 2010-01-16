#include "AnimationData.h"
#include "DuskConstants.h"

namespace Dusk
{

AnimationData::AnimationData()
{
  m_ReferenceList.clear();
}

AnimationData::~AnimationData()
{
  ClearData();
}

AnimationData& AnimationData::GetSingleton()
{
  static AnimationData Instance;
  return Instance;
}

unsigned int AnimationData::NumberOfReferences() const
{
  return m_ReferenceList.size();
}

AnimatedObject* AnimationData::addAnimatedReference(const std::string ID,
    const Ogre::Vector3 position, const Ogre::Vector3 rotation, const float scale)
{
  AnimatedObject * ObjectPointer = new AnimatedObject(ID, position, rotation, scale);
  m_ReferenceList.push_back(ObjectPointer);
  return ObjectPointer;
}

NPC* AnimationData::addNPCReference(const std::string ID,
     const Ogre::Vector3 position, const Ogre::Vector3 rot, const float Scale)
{
  NPC* NPCPointer = new NPC(ID, position, rot, Scale);
  m_ReferenceList.push_back(NPCPointer);
  return NPCPointer;
}

void AnimationData::InjectAnimationTime(const float TimePassed)
{
  unsigned int i;
  for (i=0; i<m_ReferenceList.size(); i++)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      m_ReferenceList.at(i)->Move(TimePassed);
    }
  }//for
}

void AnimationData::ClearData()
{
  AnimatedObject * ObjPtr;
  while(!m_ReferenceList.empty())
  {
    ObjPtr = m_ReferenceList.back();
    ObjPtr->Disable();
    delete ObjPtr;
    m_ReferenceList.pop_back();
  }//while
  m_ReferenceList.clear();
}//clear data

bool AnimationData::SaveAllToStream(std::ofstream& output) const
{
  if (!(output.good()))
  {
    std::cout << "AnimationData::SaveToStream: ERROR: Bad stream given.\n";
    return false;
  }
  unsigned int i;
  const unsigned int len = m_ReferenceList.size();
  for (i=0; i<len; i=i+1)
  {
    if (m_ReferenceList.at(i)!=NULL)
    {
      if (!( m_ReferenceList.at(i)->SaveToStream(output)))
      {
        std::cout << "AnimationData::SaveToStream: ERROR while saving reference.\n";
        return false;
      } //if
    }//if
  }//for
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
           m_ReferenceList.push_back(animPtr);
           return true;
         }
         delete animPtr;
         break;
    case cHeaderRefN:
         animPtr = new NPC;
         if (animPtr->LoadFromStream(Stream))
         {
           m_ReferenceList.push_back(animPtr);
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
