/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2009, 2010, 2012  ssj5000, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
     - 2010-05-31 (rev 211) - enumeration value for Projectiles added
     - 2010-06-02 (rev 213) - enumeration value for Weapon added
     - 2010-07-31 (rev 220) - GetObjectMesh() added
     - 2010-08-04 (rev 221) - isHitByRay() added
     - 2010-08-09 (rev 223) - fixed a bug that prevented compiling of DuskObject
                              under Ogre "Shoggoth" 1.6.0 and later versions
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-09-22 (rev 243) - enumeration value for Vehicle added
     - 2010-09-27 (rev 245) - minor optimization
     - 2010-11-20 (rev 255) - rotation is now stored as quaternion
     - 2010-11-26 (rev 260) - canCollide() added to determine, whether object is
                              considered for collision during collision
                              detections
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2012-06-30 (rev 307) - enumeration value for Resource added
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition
     - 2012-07-02 (rev 310) - update of getObjectMesh() and canCollide() to use
                              Database instead of ObjectBase

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
#include <OgreQuaternion.h>
#include <OgreVector3.h>
#include <OgreRay.h>

namespace Dusk{

enum ObjectTypes {otUndefined, otStatic, otItem, otAnimated, otWaypoint,
                  otLight, otContainer, otNPC, otProjectile, otResource,
                  otVehicle, otWeapon};

unsigned int GenerateUniqueObjectID();

class DuskObject: public Ogre::UserDefinedObject
{
    public:
        /* default constructor */
        DuskObject();

        /* constructor with parameter list

           parameters:
               _ID   - the object's ID
               pos   - position of the object
               rot   - rotation of the object
               Scale - scaling factor of the object
        */
        DuskObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

        /* destructor */
        virtual ~DuskObject();

        /* Retrieves object's position */
        const Ogre::Vector3& getPosition() const;

        /* Retrieves object's rotation */
        const Ogre::Quaternion& getRotation() const;

        /* Sets the position of the object.

           parameters:
               pos - the new position of the object
        */
        void setPosition(const Ogre::Vector3& pos);

        /* Sets rotation of the object.

           parameters:
               rot - the object's new rotation
        */
        void setRotation(const Ogre::Quaternion& rot);

        /* Retrieves scaling factor of the object. */
        float getScale() const;

        /* Sets scaling factor of the object. Returns true on success.

           parameters:
               newScale - the new, desired scaling factor

           remarks:
               The scaling factor of an already enabled object cannot be changed.
               In such a case you have to call Disable() first, then set the new
               scaling factor and then re-enable it by calling Enable() again.
        */
        bool setScale(const float newScale);

        /* Retrieves ID of the object. */
        const std::string& getID() const;

        /* Changes ID of the object and returns true on success, false otherwise.

           parameters:
               newID - the new object ID

           remarks:
               Trying to change the ID to an empty string or changing the ID of
               a currently enabled object will always fail.
        */
        bool changeID(const std::string& newID);

        /* Enables the object, i.e. tells the SceneManager to display it.
           Returns true on success, false on error.

           parameters:
               scm - the SceneManager that will be used to display the object

           remarks:
               Derived classes potentially implement their individual versions
               of this function. The same applies for Disable() and IsEnabled().
        */
        virtual bool enable(Ogre::SceneManager* scm);

        /* Disables the object, i.e. tells the SceneManager not to display it.
           Returns true on success, false on error. */
        virtual bool disable();

        /* Returns true, if the object is currently enabled, or false otherwise. */
        virtual bool isEnabled() const;

        /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
        virtual ObjectTypes getDuskType() const;

        /* returns true, if the player can pick up the object

           remarks:
               This function will always return false for DuskObject, because
               these objects are NOT supposed to be picked up. Derived classes
               have to override this function, if they want to change that.
               This function is only present to provide an interface for all
               classes derived from DuskObject.
        */
        virtual bool canPickUp() const;

        /* returns true, if the object shall be considered during collision
           detection
        */
        virtual bool canCollide() const;

        /* checks if a static object is hit by a ray. If the ray hits the
           object, the function will return true and impact will be set to the
           point where the ray hits the object.

           parameters:
               ray    - the ray which should be examined
               impact - vector that will hold the location of the impact, if
                        the function returned true

           remarks:
               Not tested!
        */
        virtual bool isHitByRay(const Ogre::Ray& ray, Ogre::Vector3& impact) const;

        /* Saves the object to the given stream. Returns true on success, false
           otherwise.

           parameters:
               OutStream - the output stream that will be used to save the object

           remarks:
               Every derived class has to have its own implementation of this
               function to ensure the object is saved properly.
        */
        virtual bool saveToStream(std::ofstream& OutStream) const;

        /* Tries to load an object from the given stream. Returns true on
           success, false otherwise.

           parameters:
               InStream - the input stream that will be used to load the data

           remarks:
               Every derived class has to have its own implementation of this
               function to ensure the object is saved properly.
               If the function returns false, the data within the object may be
               corrupted. It's advised not to use the object in this case.
        */
        virtual bool loadFromStream(std::ifstream& InStream);
    protected:
        /* returns the name/path of the mesh that is used during enabling this
           object

           remarks:
               Every(!) derived, non-abstract class has to implement their own
               version of that function to ensure the use of the right meshes.
        */
        virtual const std::string& getObjectMesh() const;

        /* Helper function which saves all data in a DuskObject to the given
           stream. Returns true on success.

           parameters:
               output - the output stream that will be used to save the object

           remarks:
               Derived classes will (most likely) call this function as part of
               their implementation of SaveToStream().
        */
        bool saveDuskObjectPart(std::ofstream& output) const;

        /* Helper function which loads all data in a DuskObject from the given
           stream. Returns true on success.

           parameters:
               InStream - the input stream that will be used to read the data

           remarks:
               Derived classes will (most likely) call this function as part of
               their implementation of LoadFromStream().
        */
        bool loadDuskObjectPart(std::ifstream& InStream);

        std::string ID;
        Ogre::Entity *entity;
        Ogre::Vector3 position;
        Ogre::Quaternion m_Rotation;
        float m_Scale;
};

}//namespace

#endif // DUSKOBJECT_H
