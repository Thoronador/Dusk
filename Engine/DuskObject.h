#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H

#include <string>
#include <fstream>
#include <OgreUserDefinedObject.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>

namespace Dusk{

enum ObjectTypes {otUndefined, otStatic, otItem, otAnimated, otLight, otContainer, otNPC};

unsigned int GenerateUniqueObjectID();

class DuskObject: public Ogre::UserDefinedObject
{
    public:
        DuskObject();
        DuskObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);
        virtual ~DuskObject();

        /* Retrieves object's position */
        Ogre::Vector3 GetPosition() const;

        /* Retrieves object's rotation */
        Ogre::Vector3 GetRotation() const;

        /* Sets the position of the object. */
        void SetPosition(const Ogre::Vector3& pos);

        /* Sets rotation of the object. */
        void SetRotation(const Ogre::Vector3& rot);

        /* Retrieves scaling factor of the object. */
        float GetScale() const;

        /* Sets scaling factor of the object. Returns true on success.

           remarks:
               The scaling factor of an already enabled object cannot be changed.
               In such a case you have to call Disable() first, then set the new
               scaling factor and then re-enable it by calling Enable() again.
        */
        bool SetScale(const float newScale);

        /* Retrieves ID of the object. */
        std::string GetID() const;

        /* Changes ID of the object and returns true on success, false otherwise.

           remarks:
               Trying to change the ID to an empty string or changing the ID of
               a currently enabled object will always fail.
        */
        bool ChangeID(const std::string& newID);

        /* Enables the object, i.e. tells the SceneManager to display it.
           Returns true on success, false on error.

           remarks:
               Derived classes potentially implement their individual versions
               of this function. The same applies for Disable() and IsEnabled().
        */
        virtual bool Enable(Ogre::SceneManager* scm);

        /* Disables the object, i.e. tells the SceneManager not to display it.
           Returns true on success, false on error. */
        virtual bool Disable();

        /* Returns true, if the object is currently enabled, or false otherwise. */
        virtual bool IsEnabled() const;

        /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
        virtual ObjectTypes GetType() const;

        /* Saves the object to the given stream. Returns true on success, false
           otherwise.

           remark:
               Every derived class has to have its own implementation of this
               function to ensure the object is saved properly.
        */
        virtual bool SaveToStream(std::ofstream& OutStream) const;

        /* Tries to load an object from the given stream. Returns true on
           success, false otherwise.

           remarks:
               Every derived class has to have its own implementation of this
               function to ensure the object is saved properly.
               If the function returns false, the data within the object may be
               corrupted. It's advised not to use the object in this case.
        */
        virtual bool LoadFromStream(std::ifstream& InStream);
    protected:
        bool SaveDuskObjectPart(std::ofstream& output) const;
        bool LoadDuskObjectPart(std::ifstream& InStream);
        std::string ID;
        Ogre::Entity *entity;
        Ogre::Vector3 position, rotation;
        float m_Scale;
};

}//namespace

#endif // DUSKOBJECT_H
