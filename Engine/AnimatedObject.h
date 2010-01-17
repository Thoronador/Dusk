/*---------------------------------------------------------------------------
 Author:  ssj5000, thoronador
 Date:    2009-12-31
 Purpose: AnimatedObject class
          Represents an animated, movable object within the game

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

 ToDo list:
     - implement possibility to make object "look" into the direction it is
       moving when traveling
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
#include "DuskObject.h"

namespace Dusk
{
    class AnimatedObject : public DuskObject
    {
    public:
        AnimatedObject();
        AnimatedObject(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
        virtual ~AnimatedObject();

        /* gets the direction the object is moving to. If it returns the zero
           vector, it's not moving.*/
        Ogre::Vector3 GetDirection() const;

        /* Sets the direction in which the object shall move.

           remarks:
               The given direction will be normalised automatically.
               The direction just indicates, where the object will move to.
               However, you also need to set the movement speed to a value
               greater than zero to actually make the object move.
        */
        void SetDirection(const Ogre::Vector3& direc);

        /* returns the movement speed*/
        float GetSpeed() const;

        /* sets the movement speed*/
        void SetSpeed(const float v);

        /* Sets a point in space to which the object will start moving, as long
           as the movement speed was previously set to something greater than 0.
           The object will automatically stop moving after it has reached this
           place.
        */
        void TravelToDestination(const Ogre::Vector3& dest);

        /* Returns the destination the object will is travelling to.

           remarks:
               Since this vector could be anything (in particular it can also
               be (0,0,0)), use the return value of IsOnTravel() to determine
               whether the object is currently moving or not.
        */
        Ogre::Vector3 GetDestination() const;

        /* Returns true, if the object is currently moving/ travelling.*/
        bool IsOnTravel() const;

        /* Adds a new destination point to the list of points to travel to and
           returns the number of waypoints present after the waypoint is added.
        */
        unsigned int AddWaypoint(const Ogre::Vector3& waypoint);

        /* Tells the object whether to use waypoints or not.

           remarks:
               This function should be called after one ore more waypoints were
               added, not before the waypoints were added, because if you call
               setUseWaypoints(true) before any waypoints are listed, it might
               have no effect.
        */
        void setUseWaypoints(const bool doUse);

        /* Clears all previously added waypoints. However, if the object was
           already moving to a waypoint whil the list was cleared, it still
           continues to move to the waypoint it had been travelling to and stops
           there. */
        void clearWaypoints();

        /* Tells the object whether to re-use waypoints or not when the last
           waypoint in the list is reached. If set to true, the AnimatedObject
           will return to the first waypoint and start to travel through all
           waypoints from there again.

           remarks:
               Passing true as argument if there are less than two waypoints
               will actually set patrol mode to false to prevent strange or
               unwanted behaviour.
        */
        void setPatrolMode(const bool doPatrol);

        /* returns whether the patrol mode is set or not*/
        bool getPatrolMode() const;

        /* displays the object */
        virtual bool Enable(Ogre::SceneManager* scm);
        virtual ObjectTypes GetType() const;

        /* Sets the name of the animation which shall be played.

           parameters:
               AnimName - name of the animation
               DoLoop   - if false, the animation will stop after it has been
                          played once. If true, the animation will loop for an
                          infinite time (or until another animation is set).
        */
        void PlayAnimation(const std::string& AnimName, const bool DoLoop);

        /* Returns the name of the currently playing animation. If no animation
           is playing, it returns an empty string. */
        std::string GetAnimation() const;

        /* moves the object according to the passed time

           remarks:
               This function is intended to be called regularly, i.e. every
               frame, to accomplish the desired movement and animation of the
               object. If you don't call this function in such a manner, neither
               movement nor animation will be processed.
        */
        void Move(const float SecondsPassed);

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
        bool SaveAnimatedObjectPart(std::ofstream& OutStream) const;
        bool LoadAnimatedObjectPart(std::ifstream& InStream);
        Ogre::Vector3 m_Direction, m_Destination;
        float m_Speed;

        std::string m_Anim;

        bool m_DoPlayAnim;
        bool m_LoopAnim;
        bool m_Travel;

        bool m_WaypointTravel;
        std::vector<Ogre::Vector3> m_Waypoints;
        unsigned int m_currentWaypoint;
        bool m_Patrol;
    };

    static const Ogre::Vector3 Gravitation(0.0, -9.81, 0.0);
}

#endif // ANIMATEDOBJECT_H
