#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H
#include <Ogre.h>
#include "DuskObject.h"

namespace Dusk
{
    class AnimatedObject : public Dusk::DuskObject
    {
    public:
        AnimatedObject();
        virtual ~AnimatedObject();
        Ogre::Vector3 GetVelocity();
        void SetVelocity(const Ogre::Vector3 v);
        Ogre::Vector3 GetAcceleration();
        void SetAcceleration(const Ogre::Vector3 a);
        void UseGravitation(const bool use);
        void Move(const float SecondsPassed);
    protected:
        Ogre::Vector3 velocity, acceleration;
        bool m_ApplyGravitation;
    };
    
    static const Ogre::Vector3 Gravitation(0.0, -9.81, 0.0);
}

#endif // ANIMATEDOBJECT_H
