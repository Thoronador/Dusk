#ifndef LIGHT_H
#define LIGHT_H
#include "DuskObject.h"
#include <string>
#include <fstream>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include <OgreLight.h>

namespace Dusk
{

  class Light: public DuskObject
  {
    public:
      Light();
      Light(const std::string& ID, const Ogre::Vector3& pos = Ogre::Vector3::ZERO,
            const Ogre::Vector3& dir = Ogre::Vector3::ZERO);
      virtual bool Enable(Ogre::SceneManager* scm);
      virtual bool Disable();
      virtual ObjectTypes GetType() const;
      void SetDirection(const Ogre::Vector3& dir);
      Ogre::Vector3 GetDirection() const;
      virtual bool SaveToStream(std::ofstream& OutStream) const;
      virtual bool LoadFromStream(std::ifstream& InStream);
    protected:
      Ogre::Light* entity;
      Ogre::Vector3 m_Direction;
  };//class

}//namespace

#endif // LIGHT_H
