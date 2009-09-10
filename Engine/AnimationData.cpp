#include "AnimationData.h"

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

AnimatedObject* AnimationData::addReference(const std::string ID,
    const Ogre::Vector3 position, const Ogre::Vector3 rotation, const float scale)
{
  AnimatedObject * ObjectPointer = new AnimatedObject(ID, position, rotation, scale);
  m_ReferenceList.push_back(ObjectPointer);
  return ObjectPointer;
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

}//namespace
