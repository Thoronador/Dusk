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
 Date:    2010-05-30
 Purpose: ProjectileRecord class
          Represents an entry in a look-up table for all distinct Projectiles
          within the game.

 History:
     - 2010-05-30 (rev 210) - initial version (by thoronador)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 286) - getFirst() and getEnd() added
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions
     - 2012-07-07 (rev 316) - update of ProjectileRecord to be a descendant of
                              DataRecord
                            - ProjectileBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_PROJECTILERECORD_H
#define DUSK_PROJECTILERECORD_H

#include <string>
#include <fstream>
#include "DataRecord.h"

namespace Dusk
{

  struct ProjectileRecord: public DataRecord
  {
    std::string Mesh;
    float DefaultTTL;
    float DefaultVelocity;
    //damage
    uint8_t times, dice;

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


  /* class ProjectileBase
         The purpose of this class is to hold the IDs of all distinct
         projectiles in the game, as well as their meshes and attributes.

         Think of ProjectileBase as a sort of look-up table, e.g.:

             ID   |  Mesh           | TTL | Velocity | times | dice
           -------+-----------------+-----+----------+-------+------
           arrow  | wooden.mesh     | 5.0 |  123.45  |   4   |   6
           bullet | bullet.mesh     | 8.9 |  678.9   |  10   |  12
           sonic  | sonic_gun.mesh  | 0.1 |  330.0   |  20   |  20
            ...   | ...             | ... | ...      | ...   | ...

  */

} //namespace

#endif // DUSK_PROJECTILERECORD_H
