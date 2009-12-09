#ifndef NPC_H
#define NPC_H

#include <string>
#include <OgreVector3.h>
#include "AnimatedObject.h"
#include "Container.h"

namespace Dusk
{
  class NPC: public AnimatedObject, public Container
  {
    public:
      NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
      virtual ~NPC();
      virtual ObjectTypes GetType() const;

      float getHealth() const;
      void setHealth(const float new_health);
      bool isAlive() const;
      /* still to do:
         -Enable, and pissibly Disable and isEnabled
         -LoadFromStream, SaveToStream (diamond problem)
      */
    protected:
      float m_Health;
  }; //class

} //namespace

#endif // NPC_H
