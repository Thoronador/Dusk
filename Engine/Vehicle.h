/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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
 Date:    2010-09-22
 Purpose: Vehicle class
          Represents a Vehicle within the game.

 History:
     - 2010-09-22 (rev 243) - initial version (by thoronador)
     - 2010-09-24 (rev 244) - method for adjusting passenger's positions added
                            - methods to mount/unmount passengers and query
                              current number of passengers or get passengers
     - 2010-09-27 (rev 245) - rotation of passengers adjusted
     - 2010-10-17 (rev 246) - error in calculation of position of passengers
                              fixed
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion

 ToDo list:
     - position and orientation of passengers has to be adjusted during
       enabling; same goes for movement
     - methods to save, load and query (const) passengers somehow
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef VEHICLE_H
#define VEHICLE_H

#include "AnimatedObject.h"
#include "WaypointObject.h"

namespace Dusk
{

//forward declaration of NPC
class NPC;

class Vehicle: public AnimatedObject, public WaypointObject
{
  public:
    /* constructor */
    Vehicle();

    /* constructor with parameters */
    Vehicle(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~Vehicle();

    /* returns the enumeration type indicating that this is a vehicle */
    virtual ObjectTypes getDuskType() const;

    /* animate and move the vehicle according to the passed time

       parameters:
           SecondsPassed - the amount of seconds that passed since the last
                           call of this function/ since the last frame

       remarks:
           This function is intended to be called regularly, i.e. every frame,
           to accomplish the desired animation of the object. If you don't call
           this function in such a manner, the animation will be processed
           improperly and/or will not be fluent.
    */
    virtual void injectTime(const float SecondsPassed);

    /* Enables the vehicle, i.e. tells the SceneManager to display it.
       Returns true on success, false on error.

       parameters:
           scm - the scene manager that is used to show the vehicle
    */
    virtual bool enable(Ogre::SceneManager* scm);

    /* Disables the object, i.e. tells the SceneManager not to display it.
       Returns true on success, false on error. */
    virtual bool disable();

    /* returns the overall number of available mountpoints */
    virtual unsigned int getTotalMountpoints() const;

    /* returns the number of unoccupied mountpoints */
    virtual unsigned int getFreeMountpoints() const;

    /* tries to set a passenger onto the vehicla at position with given index,
       and returns true on success, false on failure.

       parameters:
           idx - zero-based index of the passenger's mountpoint (e.g. seat or
                 something similar)
           who - pointer to the NPC that shall be placed on/in the vehicle

       remarks:
           If there already is a NPC at the given index, the passenger will not
           be changed and the function returns false.
           The value of idx must be less than the return value of the function
           getTotalMountpoints(), i.e. it must be less than the number of
           available mountpoints.
    */
    bool mountPassengerAtIndex(const unsigned int idx, NPC* who);

    /* tries to remove the passenger at position with given index from the vehicle,
       and returns true on success, false on failure.

       parameters:
           idx - zero-based index of the passenger's mountpoint (e.g. seat or
                 something similar)

       remarks:
           If there already is no NPC at the given index, the function will
           return true(!), because the place is then free for another NPC.
           The value of idx must be less than the return value of the function
           getTotalMountpoints(), i.e. it must be less than the number of
           available mountpoints.
    */
    bool unmountPassengerAtIndex(const unsigned int idx);

    /* tries to remove the passenger from the vehicle, and returns true on
       success (i.e. an NPC was removed), or false on failure.

       parameters:
           who - pointer to the NPC that should be removed

       remarks:
           If the NPC the pointer points to is not on the vehicle, or if the
           pointer is NULL, the function returns false.
    */
    bool unmountPassenger(NPC* who);

    /* returns a pointer to the passenger at position with given index, or NULL
       if there is no passenger

       parameters:
           idx - zero-based index of the passenger's mountpoint (e.g. seat or
                 something similar)
    */
    NPC* getPassengerAtIndex(const unsigned int idx) const;

    /* returns the current number of passengers */
    virtual unsigned int getPassengerCount() const;

    /* immediately drops all passengers from the vehicle*/
    virtual void dropAllPassengers();

    /* saves vehicle to given stream and returns true on success, false otherwise

       parameters:
           OutStream - the output stream that is used to save the vehicle
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Loads vehicle from stream and returns true on success, false otherwise.
       The vehicle's data is probably inconsistent after that function failed,
       so don't rely on its data in that case.

       parameters:
           InStream - the input stream that is used to load the vehicle from
    */
    virtual bool loadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string getObjectMesh() const;

    /* Enables the vehicle's passengers, i.e. tells the SceneManager to display
       them. Returns true on success, false on error.

       parameters:
           scm - the scene manager that is used to show the passengers
    */
    bool enablePassengers(Ogre::SceneManager* scm);

    /* re-adjusts the position of all passengers */
    void adjustPassengerPosition();

    struct PassengerRecord
    {
      NPC* who;
      Ogre::Vector3 position_offset;
      Ogre::Quaternion rotation_offset;
    };//struct

    //list of passengers
    std::map<unsigned int, PassengerRecord> m_Passengers;
};//class


} //namespace

#endif // VEHICLE_H
