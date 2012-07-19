/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2012  thoronador

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
 Author:  thoronador
 Date:    2010-05-20
 Purpose: WaypointObject class
          abstract class/ interface for objects that can move between waypoints

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)
     - 2010-06-02 (rev 213) - changed header in load/save functions
     - 2010-08-01 (rev 220) - GetObjectMesh() added,
                            - Enable() removed (inherited method is used instead)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-11-26 (rev 260) - canCollide() added (always returns true)
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition
     - 2012-07-02 (rev 310) - update of getObjectMesh() to use Database instead
                              of ObjectBase

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
    WaypointObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~WaypointObject();

    /* Adds a new destination point to the list of points to travel to and
       returns the number of waypoints present after the waypoint is added.

       parameters:
           waypoint - the new waypoint that will be added to the end of the list
    */
    unsigned int addWaypoint(const Ogre::Vector3& waypoint);

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

    /* returns object's type as enumeration */
    virtual ObjectTypes getDuskType() const;

    /* returns true, if the object shall be considered during collision
       detection

       remarks:
           Will always return true for waypoint objects.
    */
    virtual bool canCollide() const;

    /* function to inject time for movement and perform movement

       parameters:
           SecondsPassed - the time (in seconds) that passed since last frame
    */
    virtual void injectTime(const float SecondsPassed);

    /* Saves the object to the given stream. Returns true on success, false
       otherwise.

       parameters:
           OutStream - the output stream that is used to save that object

       remarks:
           Every derived class has to have its own implementation of this
           function to ensure the object is saved properly.
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Tries to load an object from the given stream. Returns true on
       success, false otherwise.

       parameters:
           InStream - the input stream that is used to load the object

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
       WaypointObject to the given stream. Returns true on success.

       parameters:
           OutStream - the output stream that is used to save that object

       remarks:
         Derived classes will (most likely) call this function as part of
         their implementation of SaveToStream().
    */
    bool saveWaypointObjectPart(std::ofstream& OutStream) const;

    /* Utility function which loads all data that is specific to an
       WaypointObject from the given stream. Returns true on success.

       parameters:
           InStream - the input stream that is used to load the object from

       remarks:
         Derived classes will (most likely) call this function as part of
         their implementation of loadFromStream().
    */
    bool loadWaypointObjectPart(std::ifstream& InStream);

    bool m_WaypointTravel;
    bool m_Patrol;
    std::vector<Ogre::Vector3> m_Waypoints;
    unsigned int m_currentWaypoint;
}; //class

} //namespace

#endif // WAYPOINTOBJECT_H
