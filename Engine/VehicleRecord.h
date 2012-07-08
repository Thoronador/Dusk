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
     - 2012-07-07 (rev 316) - update of VehicleRecord to be a descendant of
                              DataRecord
                            - VehicleBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_VEHICLERECORD_H
#define DUSK_VEHICLERECORD_H

#include <vector>
#include <fstream>
#include <OgreVector3.h>
#include "DataRecord.h"

namespace Dusk
{

  struct MountpointData
  {
    Ogre::Vector3 offset;
    Ogre::Quaternion rotation;
  };//struct

  struct VehicleRecord: public DataRecord
  {
    std::string Mesh;
    std::string Name;
    float MaxSpeed;
    std::vector<MountpointData> Mountpoints;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const;

    //record type identifier (usually the value returned by the above function)
    static const uint32_t RecordType;

    /* Tries to save the data record to stream outStream and returns true on
       success, false otherwise.

       parameters:
           outStream - the output stream to which the data will be saved
    */
    virtual bool saveToStream(std::ofstream& outStream) const;

    /* Tries to load the data record from stream inStream and returns true on
       success, false otherwise.

       parameters:
           inStream - the input stream from which the data will be read

       remarks:
           The record may have inconsistent data, if the function fails.
    */
    virtual bool loadFromStream(std::ifstream& inStream);
}; //struct

} //namespace

#endif // DUSK_VEHICLERECORD_H
