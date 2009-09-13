#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

#include "AnimatedObject.h"
#include <vector>

namespace Dusk
{
  class AnimationData
  {
    public:
      virtual ~AnimationData();
      static AnimationData& GetSingleton();
      unsigned int NumberOfReferences() const;
      AnimatedObject* addReference(const std::string ID, const Ogre::Vector3 position,
                               const Ogre::Vector3 rotation, const float scale);
      void InjectAnimationTime(const float TimePassed);
      void ClearData();
    private:
      AnimationData();
      AnimationData(const AnimationData& op) {}
      std::vector<AnimatedObject*> m_ReferenceList;
  };//class
}//namespace

#endif // ANIMATIONDATA_H
