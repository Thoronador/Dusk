/*---------------------------------------------------------------------------
 Authors: ssj5000, thoronador
 Date:    2010-05-05
 Purpose: DuskObject class
          This class represents a single, static object in the game and
          serves as ancestor for many other objects in the game.

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2009-01-19 (rev 71)  - missing header includes added (by thoronador)
     - 2009-01-23 (rev 73)  - class moved into namespace Dusk
                            - position and rotation added
     - 2009-01-23 (rev 74)  - get and set methods for rotation and position
     - 2009-07-01 (rev 101) - Ogre Entity can now be created/ destroyed via
                              Enable() and Disable() methods
                            - check for active entity via IsEnabled()
                            - scale, GetScale() and SetScale() added
                            - GetID() and ChangeID() introduced
                            - constructor with parameter list added
                            - object type changed from string to enumeration
     - 2009-07-02 (rev 102) - enumeration value for items added
     - 2009-07-13 (rev 104) - Enable() can now work with arbitrary SceneManager
     - 2009-07-16 (rev 107) - rotation and position is now applied to entity
                            - lower bound for scaling factor implemented
     - 2009-08-30 (rev 123) - DuskObject is now descendant of UserDefinedObject
                              from Ogre, and entity gets its DuskObject set as
                              UserDefinedObject (serves as sort of a link)
     - 2009-08-30 (rev 124) - rotation is now set to a value in 0-360°, if out
                              of that range
     - 2009-09-08 (rev 126) - error in SetPosition() fixed
     - 2009-09-10 (rev 127) - enumeration value for animated objects added
     - 2009-09-18 (rev 129) - GetType() added
                            - enumeration value for lights added
                            - minor improvements in class method declarations
     - 2009-09-22 (rev 130) - methods to load and save object added
     - 2009-09-24 (rev 131) - enumeration value for containers added
     - 2009-12-05 (rev 139) - member objectType removed; GetType() only
                              depends on class/type and not on instance
                            - comments to document functions added
     - 2009-12-08 (rev 140) - enumeration value for NPCs added
     - 2010-01-17 (rev 156) - load and save methods improved, methods
                              SaveDuskObjectPart() + LoadDuskObjectPart() added
     - 2010-02-06 (rev 165) - canPickUp() added
     - 2010-05-05 (rev 196) - documentation updated
     - 2010-05-20 (rev 205) - enumeration value for WaypointObjects added
     - 2010-05-21 (rev 206) - documentation updated

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSKOBJECT_H
#define DUSKOBJECT_H

#include <string>
#include <fstream>
#include <OgreUserDefinedObject.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreVector3.h>

namespace Dusk{

enum ObjectTypes {otUndefined, otStatic, otItem, otAnimated, otWaypoint, otLight, otContainer, otNPC};

unsigned int GenerateUniqueObjectID();

class DuskObject: public Ogre::UserDefinedObject
{
    public:
        /* default constructor */
        DuskObject();

        /* constructor with parameter list */
        DuskObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

        /* destructor */
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

        /* returns true, if the player can pick up the object

           remarks:
               This function will always return false for DuskObject, because
               these objects are NOT supposed to be picked up. Derived classes
               have to override this function, if they want to change that.
               This function is only present to provide an interface for all
               classes derived from DuskObject.
        */
        virtual bool canPickUp() const;

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
        /* Helper function which saves all data in a DuskObject to the given
           stream. Returns true on success.

           remarks:
               Derived classes will (most likely) call this function as part of
               their implementation of SaveToStream().
        */
        bool SaveDuskObjectPart(std::ofstream& output) const;
        /* Helper function which loads all data in a DuskObject from the given
           stream. Returns true on success.

           remarks:
               Derived classes will (most likely) call this function as part of
               their implementation of LoadFromStream().
        */
        bool LoadDuskObjectPart(std::ifstream& InStream);
        std::string ID;
        Ogre::Entity *entity;
        Ogre::Vector3 position, rotation;
        float m_Scale;
};

}//namespace

#endif // DUSKOBJECT_H
