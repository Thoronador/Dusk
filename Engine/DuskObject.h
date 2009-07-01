#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H

#include <string>
#include <OgreEntity.h>
#include <OgreVector3.h>

namespace Dusk{

enum ObjectTypes {otUndefined, otStatic};

class DuskObject
{
    public:
        DuskObject();
        DuskObject(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
        virtual ~DuskObject();
        Ogre::Vector3 GetPosition() const;
        Ogre::Vector3 GetRotation() const;
        void SetPosition(const Ogre::Vector3 pos);
        void SetRotation(const Ogre::Vector3 rot);
        float GetScale() const;
        bool SetScale(const float newScale);
        std::string GetID() const;
        bool ChangeID(const std::string newID);
        bool Enable();
        bool Disable();
        bool IsEnabled();
    protected:
        std::string ID;
        ObjectTypes objectType;
        Ogre::Entity *entity;
        Ogre::Vector3 position, rotation;
        float m_Scale;
};

}

#endif // DUSKOBJECT_H
