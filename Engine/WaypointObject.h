/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-20
 Purpose: WaypointObject class
          abstract class/ interface for objects that can move between waypoints

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)

 ToDo list:
     - implement possibility to make object "look" into the direction it is
       moving when traveling
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef WAYPOINTOBJECT_H
#define WAYPOINTOBJECT_H

#include "UniformMotionObject.h"
#include <vector>

namespace Dusk
{

class WaypointObject: public UniformMotionObject
{
  public:
    /* default constructor */
    WaypointObject();

    /* constructor with parameter list*/
    WaypointObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~WaypointObject();

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
       already moving to a waypoint while the list was cleared, it still
       continues to move to the waypoint it had been travelling to and stops
       there.
    */
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

    /* returns object's type as enumeration */
    virtual ObjectTypes GetType() const;

    /* function to inject time for movement and perform movement */
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
       WaypointObject to the given stream. Returns true on success.

       remarks:
         Derived classes will (most likely) call this function as part of
         their implementation of SaveToStream().
    */
    bool SaveWaypointObjectPart(std::ofstream& OutStream) const;

    /* Utility function which loads all data that is specific to an
       WaypointObject from the given stream. Returns true on success.

       remarks:
         Derived classes will (most likely) call this function as part of
         their implementation of LoadFromStream().
    */
    bool LoadWaypointObjectPart(std::ifstream& InStream);

    bool m_WaypointTravel;
    bool m_Patrol;
    std::vector<Ogre::Vector3> m_Waypoints;
    unsigned int m_currentWaypoint;
}; //class

} //namespace

#endif // WAYPOINTOBJECT_H
