/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

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
 Date:    2012-06-24
 Purpose: ResourceBase Singleton class
          Represents a look-up table for all distinct resource objects
          within the game.

 History:
     - 2012-06-24 (rev 306) - initial version (by thoronador)
     - 2012-07-02 (rev 310) - fixed iterator type
     - 2012-07-07 (rev 316) - update of ResourceRecord to be a descendant of
                              DataRecord
                            - ResourceBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_RESOURCERECORD_H
#define DUSK_RESOURCERECORD_H

#include <string>
#include "DataRecord.h"

namespace Dusk
{

  struct ResourceRecord: public DataRecord
  {
    std::string meshSpawned;
    std::string meshHarvested;
    std::string harvestItem; //ID of the item the NPC will get from that resource
    float respawnInterval; //respawn interval in seconds
    std::string harvestSound; //ID of sound that is played while harvesting the resource

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
  };//struct

  /* class ResourceBase
         The purpose of this class is to hold the IDs of all distinct
         resources in the game, as well as their meshes and attributes.

         Think of ResourceBase as a sort of look-up table, e.g.:

             ID   |  mesh spawned   | mesh harv.      | item      | respawn | ...
           -------+-----------------+-----------------+-----------+---------+----
           coal   | coal.mesh       | coal_taken.mesh | coal_lump | inf     | ...
            ...   | ...             | ...             | ...       | ...     | ...

  */

}//namespace

#endif // DUSK_RESOURCERECORD_H
