#ifndef ANIMATEDOBJECT_H
#define ANIMATEDOBJECT_H
#include <Ogre.h>
#include <OgreVector3.h>
#include <vector>
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
    protected:
        Ogre::Vector3 m_Direction, m_Destination;
        float m_Speed;

        std::string m_Anim;
        bool m_DoPlayAnim;
        bool m_LoopAnim;
        bool m_Travel;

        bool m_WaypointTravel;
        std::vector<Ogre::Vector3> m_Waypoints;
        unsigned int m_currentWaypoint;
    };

    static const Ogre::Vector3 Gravitation(0.0, -9.81, 0.0);
}

#endif // ANIMATEDOBJECT_H
