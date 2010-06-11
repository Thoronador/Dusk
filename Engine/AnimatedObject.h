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

 ToDo list:
     - adjust GetAnimation() and GetLoopState() so that they can be used to get
       information about all currently playing animation states
     - remove PlayAnimation(), because StartAnimation() can replace it
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H
#include <OgreSceneManager.h>
#include <OgreVector3.h>
#include <vector>
#include <fstream>
#include "InjectionObject.h"

namespace Dusk
{
    class AnimatedObject : virtual public InjectionObject
    {
    public:
        /* default constructor */
        AnimatedObject();

        /* constructor with parameter list */
        AnimatedObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

        /* destructor */
        virtual ~AnimatedObject();

        /* displays the object */
        virtual bool Enable(Ogre::SceneManager* scm);

        /* returns the object type as enumeration */
        virtual ObjectTypes GetType() const;

        /* Sets the name of the animation which shall be played.

           parameters:
               AnimName - name of the animation
               DoLoop   - if false, the animation will stop after it has been
                          played once. If true, the animation will loop for an
                          infinite time (or until another animation is set).
        */
        void PlayAnimation(const std::string& AnimName, const bool DoLoop);

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
        bool StartAnimation(const std::string& AnimName, const bool DoLoop);

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
        bool StopAnimation(const std::string& AnimName);

        /* stops all animations and returns the number of stopped animations

           remarks:
               This function only works, if the object is enabled (see Enabled()
               or IsEnabled()). However, as long as the object isn't enabled,
               this will always return zero, because a disabled object cannot
               play any animations, thus every animation can be considered to be
               stopped already. ;D.
        */
        unsigned int StopAllAnimations();

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
        bool IsAnimationActive(const std::string& AnimName) const;

        /* Returns the name of the currently playing animation. If no animation
           is playing, it returns an empty string. */
        std::string GetAnimation() const;

        /* Returns the list of possible animations for an enabled object. This
           works only for enabled objects. If the object is not enabled, the
           function will return an empty vector.

           remarks/ bugs:
              This function is not thread-safe, because the internally used
              iterator isn't thread-safe.
        */
        std::vector<std::string> GetPossibleAnimationStates() const;

        /* returns true, if the animation (if present) shall be looped */
        bool GetLoopState() const;

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
        /* Utility function which saves all data that is specific to an
           AnimatedObject to the given stream. Returns true on success.

           remarks:
             Derived classes will (most likely) call this function as part of
             their implementation of SaveToStream().
        */
        bool SaveAnimatedObjectPart(std::ofstream& OutStream) const;

        /* Utility function which loads all data that is specific to an
           AnimatedObject from the given stream. Returns true on success.

           remarks:
             Derived classes will (most likely) call this function as part of
             their implementation of LoadFromStream().
        */
        bool LoadAnimatedObjectPart(std::ifstream& InStream);

        std::string m_Anim;
        bool m_DoPlayAnim;
        bool m_LoopAnim;
    };

} //namespace

#endif // ANIMATEDOBJECT_H
