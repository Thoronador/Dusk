/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012 thoronador

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
 Purpose: VehicleBase Singleton class
          Represents a look-up table for all distinct Vehicle objects
          within the game.

 History:
     - 2010-09-22 (rev 243) - initial version (by thoronador)
     - 2010-09-24 (rev 244) - mountpoint data and functions for retrieval added
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2011-05-18 (rev 291) - getFirst() and getEnd() added
     - 2011-09-29 (rev 301) - removed unnecessary data member
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef VEHICLEBASE_H
#define VEHICLEBASE_H

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <OgreVector3.h>

namespace Dusk
{

struct MountpointData
{
  Ogre::Vector3 offset;
  Ogre::Quaternion rotation;
};//struct

struct VehicleRecord
{
  std::string Mesh;
  std::string Name;
  float MaxSpeed;
  std::vector<MountpointData> Mountpoints;
}; //struct

class VehicleBase
{
  public:
    /* singleton access method */
    static VehicleBase& getSingleton();

    /* destructor */
    ~VehicleBase();

    /* adds a vehicle with the given ID and data.

       remarks:
           This function always succeeds, except for empty ID or mesh path, or
           some other invalid data. In that case, nothing is added.
           If a vehicle record with the same ID already exists, it is
           overwritten by the new data.

       parameters:
           ID   - ID of the vehicle
           data - data for that vehicle
    */
    void addVehicle(const std::string& ID, const VehicleRecord& data);

    /* returns true, if a vehicle with the given ID is present

       parameters:
           ID - the ID of the vehicle
    */
    bool hasVehicle(const std::string& ID) const;

    /* Returns the mesh of vehicle with given ID. If not vehicle with the given
       ID is present, the function will throw an excpetion. */
    const std::string& getVehicleMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

    /* returns the name of the given vehicle, or throws an exception if no
       vehicle with that ID is present

       parameters:
           ID - the ID of the vehicle
    */
    const std::string& getVehicleName(const std::string& ID) const;

    /* returns the maximum speed of the given vehicle, or throws an excpetion
       if no vehicle with that ID is present

       parameters:
           ID - the ID of the vehicle
    */
    float getVehicleSpeed(const std::string& ID) const;

    /* returns the number of mountpoints for the given vehicle, or throws an
       exception if no vehicle with that ID is present.

       parameters:
           ID - the ID of the vehicle
    */
    unsigned int getVehicleMountpoints(const std::string& ID) const;

    /* returns the offset of a specified mountpoint for the given vehicle.
       If no vehicle of with that ID is present, the function throws an exception.

       parameters:
           ID  - the ID of the vehicle
           idx - zero-based index of the mountpoint

       remarks:
           The value of the parameter idx has to be less than the value
           returned by getVehicleMountpoints().
    */
    const Ogre::Vector3& getMountpointOffset(const std::string& ID, const unsigned int idx) const;

    /* returns the rotation of a specified mountpoint for the given vehicle.
       If no vehicle of with that ID is present, the function throws an exception.

       parameters:
           ID  - the ID of the vehicle
           idx - zero-based index of the mountpoint

       remarks:
           The value of the parameter idx has to be less than the value
           returned by getVehicleMountpoints().
    */
    const Ogre::Quaternion& getMountpointRotation(const std::string& ID, const unsigned int idx) const;

    /* returns the data of a specified mountpoint for the given vehicle.
       If no vehicle of with that ID is present, or mountpoint is not within the
       acceptable range, the return value is undefined and the function may
       throw an exception.

       parameters:
           ID  - the ID of the vehicle
           idx - zero-based index of the mountpoint

       remarks:
           The value of the parameter idx has to be less than the value
           returned by getVehicleMountpoints().
    */
    const MountpointData& getMountpointData(const std::string& ID, const unsigned int idx) const;

    /* deletes a vehicle by ID and returns true, if a vehicle was deleted */
    bool deleteVehicle(const std::string& ID);

    /* deletes all present vehicles */
    void clearAll();

    /* returns the number of distinct vehicles */
    unsigned int getVehicleNumber() const;

    /* tries to save all vehicles to the stream and returns true on success

       parameters:
           OutStream - the output stream that is used to save the vehicle data
    */
    bool saveAllToStream(std::ofstream& OutStream) const;

    /* Loads one(!) single vehicle record from stream; returns true on success,
       false otherwise. The data of the last loaded vehicle is probably
       inconsistent after that function failed, so don't rely on it in that
       case.

       parameters:
           InStream - the input stream that is used to load the weapon data
    */
    bool loadNextVehicleFromStream(std::ifstream& InStream);


    #ifdef DUSK_EDITOR
    //constant iterator type for iterating through the vehicles
    typedef std::map<std::string, VehicleRecord>::const_iterator Iterator;

    /* returns constant iterator to first element in vehicle list*/
    Iterator getFirst() const;

    /* returns constant iterator to end of vehicle list*/
    Iterator getEnd() const;
    #endif
  private:
    /* constructor - private due to singleton pattern */
    VehicleBase();

    /* private, empty copy constructor - there shall be only one */
    VehicleBase(const VehicleBase& op) {}

    //list of vehicles
    std::map<std::string, VehicleRecord> m_Vehicles;

    MountpointData mp_null;
}; //class

} //namespace

#endif // VEHICLEBASE_H
