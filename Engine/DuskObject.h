#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H

#include <string>
#include <fstream>
#include <OgreUserDefinedObject.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>

namespace Dusk{

enum ObjectTypes {otUndefined, otStatic, otItem, otAnimated, otLight, otContainer};

unsigned int GenerateUniqueObjectID();

class DuskObject: public Ogre::UserDefinedObject
{
    public:
        DuskObject();
        DuskObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);
        virtual ~DuskObject();
        Ogre::Vector3 GetPosition() const;
        Ogre::Vector3 GetRotation() const;
        void SetPosition(const Ogre::Vector3& pos);
        void SetRotation(const Ogre::Vector3& rot);
        float GetScale() const;
        bool SetScale(const float newScale);
        std::string GetID() const;
        bool ChangeID(const std::string& newID);
        virtual bool Enable(Ogre::SceneManager* scm);
        virtual bool Disable();
        virtual bool IsEnabled() const;
        ObjectTypes GetType() const;
        virtual bool SaveToStream(std::ofstream& OutStream) const;
        virtual bool LoadFromStream(std::ifstream& InStream);
    protected:
        std::string ID;
        ObjectTypes objectType;
        Ogre::Entity *entity;
        Ogre::Vector3 position, rotation;
        float m_Scale;
};

}//namespace

#endif // DUSKOBJECT_H
