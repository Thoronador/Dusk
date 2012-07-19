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
 Author:  ssj5000, thoronador
 Date:    2009-12-31
 Purpose: AnimatedObject class
          Represents an animated object within the game

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2009-01-23 (rev 73)  - Object class removed and replaced by DuskObject,
                              position, rotation, velocity and acceleration
                              added, gravitational constant (by thoronador)
                            - set-/get-functions for velocity and acceleration
                            - move() function
     - 2009-01-23 (rev 74)  - getAccelleration() and getVelocity() made const
                              member functions
     - 2009-07-16 (rev 107) - position of object's entity is now adjusted to
                              match set position
     - 2009-09-10 (rev 127) - velocity, acceleration, gravitation and related
                              get-/set-functions removed
                            - animation, direction, destination, travel, speed
                              added; functions PlayAnimation(), GetAnimation(),
                              Enable(), GetDirection(), SetDirection(),
                              GetSpeed(), SetSpeed(), TravelToDestionation(),
                              IsOnTravel(), GetDestionation() functions added
                            - new constructor version with parameters
     - 2009-12-05 (rev 139) - ability to travel to waypoints added, functions
                              AddWaypoint(), setUseWaypoints(), clearWaypoints()
                            - function descriptions added
                            - GetType() changed (as in all classes which are
                              derived from DuskObject)
     - 2009-12-31 (rev 147) - documentation update
     - 2010-01-16 (rev 154) - LoadFromStream() and SaveToStream() added
     - 2010-01-17 (rev 156) - LoadFromStream() and SaveToStream() updated
                            - patrol mode introduced
     - 2010-05-20 (rev 205) - reduction to pure animation
     - 2010-05-21 (rev 206) - GetPossibleAnimationStates() added
     - 2010-05-27 (rev 208) - StartAnimation(), StopAnimation(),
                              StopAllAnimations() & IsAnimationActive() added
     - 2010-06-02 (rev 213) - fixed an issue in StopAllAnimations()
     - 2010-07-31 (rev 219) - PlayAnimation() removed
                            - adjustments to class to work with multiple
                              parallel animations
     - 2010-08-01 (rev 220) - GetObjectMesh() added
     - 2010-08-15 (rev 224) - isHitByRay() added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as quaternion
     - 2010-11-26 (rev 260) - canCollide() added
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition
     - 2012-07-02 (rev 310) - update of getObjectMesh() and canCollide() to use
                              Database instead of ObjectBase

 ToDo list:
     - review implementation of canCollide() at a later stage of development
       when there actually are pure animated objects
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H
#include <OgreSceneManager.h>
#include <vector>
#include <map>
#include <fstream>
#include "InjectionObject.h"

namespace Dusk
{

    struct AnimRecord
    {
      float position;
      bool DoLoop;
      //default constuctor
      AnimRecord();
      //constuctor with params
      AnimRecord(const float pos, const bool loop);
    };//struct

    class AnimatedObject : virtual public InjectionObject
    {
    public:
        /* default constructor */
        AnimatedObject();

        /* constructor with parameter list */
        AnimatedObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

        /* destructor */
        virtual ~AnimatedObject();

        /* displays the object

           parameters:
               scm - the Scene Manager that is used to display the object
        */
        virtual bool enable(Ogre::SceneManager* scm);

        /* Disables the object, i.e. tells the SceneManager not to display it.
           Returns true on success, false on error.
        */
        virtual bool disable();

        /* returns the object type as enumeration */
        virtual ObjectTypes getDuskType() const;

        /* returns true, if the object shall be considered during collision
           detection

           remarks:
               Currently this function uses the ObjectRecord-type data from
               Database. However, animated objects might have their own type of
               data source in the future, so we need to adjust the function
               implementation in that case.
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

        /* causes an animation to be played and returns true on success

           parameters:
               AnimName - name of the animation
               DoLoop   - if false, the animation will stop after it has been
                          played once. If true, the animation will loop for an
                          infinite time (or until is stopped manually).

           remarks:
               This function only works, if the object is enabled (see Enabled()
               or IsEnabled()). As long as the object isn't enabled, this will
               always return false / fail.
        */
        bool startAnimation(const std::string& AnimName, const bool DoLoop);

        /* stops a named animation and returns true on success

           parameters:
               AnimName - name of the animation to be stopped

           remarks:
               This function only works, if the object is enabled (see Enabled()
               or IsEnabled()). However, as long as the object isn't enabled,
               this will always return true, because a disabled object cannot
               play any animations, thus every animation can be considered to be
               stopped. ;D.
        */
        bool stopAnimation(const std::string& AnimName);

        /* stops all animations and returns the number of stopped animations

           remarks:
               This function only works, if the object is enabled (see Enabled()
               or IsEnabled()). However, as long as the object isn't enabled,
               this will always return zero, because a disabled object cannot
               play any animations, thus every animation can be considered to be
               stopped already. ;D.
        */
        unsigned int stopAllAnimations();

        /* returns true, if the names animation is playing

           parameters:
               AnimName - name of the animation to be checked

           remarks:
               This function only works, if the object is enabled (see Enabled()
               or IsEnabled()). However, as long as the object isn't enabled,
               this will always return false, because a disabled object cannot
               play any animations, thus every animation can be considered to be
               stopped. ;D.
        */
        bool isAnimationActive(const std::string& AnimName) const;

        /* Returns the names of all currently playing animations. If no
            animation is playing, it returns an empty vector.
        */
        std::vector<std::string> getCurrentAnimations() const;

        /* Returns the list of possible animations for an enabled object. This
           works only for enabled objects. If the object is not enabled, the
           function will return an empty vector.

           remarks/ bugs:
              This function is not thread-safe, because the internally used
              iterator isn't thread-safe.
        */
        std::vector<std::string> getPossibleAnimationStates() const;

        /* returns true, if the given animation is /will be looped

           parameters:
               AnimName - name of the animation
        */
        bool getLoopState(const std::string& AnimName) const;

        /* animated the object according to the passed time

           remarks:
               This function is intended to be called regularly, i.e. every
               frame, to accomplish the desired animation of the object. If you
               don't call this function in such a manner, the animation will be
               processed improperly and/or will not be fluent.
        */
        virtual void injectTime(const float SecondsPassed);

        /* Saves the object to the given stream. Returns true on success, false
           otherwise.

           parameters:
               OutStream - the output stream to which the object will be saved

           remark:
               Every derived class has to have its own implementation of this
               function to ensure the object is saved properly.
        */
        virtual bool saveToStream(std::ofstream& OutStream) const;

        /* Tries to load an object from the given stream. Returns true on
           success, false otherwise.

           parameters:
               InStream - the input stream from which the data will be read

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

        /* Utility function which saves all data that is specific to an
           AnimatedObject to the given stream. Returns true on success.

           parameters:
               OutStream - the output stream to which the object will be saved

           remarks:
             Derived classes will (most likely) call this function as part of
             their implementation of SaveToStream().
        */
        bool saveAnimatedObjectPart(std::ofstream& OutStream) const;

        /* Utility function which loads all data that is specific to an
           AnimatedObject from the given stream. Returns true on success.

           parameters:
               InStream - the input stream from which the data will be read

           remarks:
             Derived classes will (most likely) call this function as part of
             their implementation of LoadFromStream().
        */
        bool loadAnimatedObjectPart(std::ifstream& InStream);

        /* updates the information about anims listed in m_Anims with currently
           playing animations
        */
        void synchronizeAnimationList();

        //map that holds the animations while object is disabled
        std::map<std::string, AnimRecord> m_Anims;
    }; //class AnimatedObject

} //namespace

#endif // ANIMATEDOBJECT_H
