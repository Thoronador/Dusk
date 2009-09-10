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
        AnimatedObject(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
        virtual ~AnimatedObject();
        Ogre::Vector3 GetDirection() const;
        void SetDirection(const Ogre::Vector3& direc);
        float GetSpeed() const;
        void SetSpeed(const float v);
        void TravelToDestination(const Ogre::Vector3& dest);
        Ogre::Vector3 GetDestination() const;
        bool IsOnTravel() const;
        bool Enable(Ogre::SceneManager* scm);
        void PlayAnimation(const std::string& AnimName, const bool DoLoop);
        std::string GetAnimation() const;
        void Move(const float SecondsPassed);
    protected:
        Ogre::Vector3 m_Direction, m_Destination;
        float m_Speed;

        std::string m_Anim;
        bool m_DoPlayAnim;
        bool m_LoopAnim;
        bool m_Travel;
    };

    static const Ogre::Vector3 Gravitation(0.0, -9.81, 0.0);
}

#endif // ANIMATEDOBJECT_H
