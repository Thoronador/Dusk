/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

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
 Date:    2010-01-30
 Purpose: ObjectRecord structure
          holds information about one distinct object type in the game.
          Think of it as a database entry.

 History:
     - 2009-07-01 (rev 101) - initial version (by thoronador)
     - 2009-07-02 (rev 102) - checks for empty ID and mesh in addObject()
     - 2009-07-06 (rev 103) - possibility to work with error mesh added
     - 2009-07-13 (rev 104) - SaveToStream() added
     - 2009-07-15 (rev 105) - LoadFromStream() added
     - 2009-07-24 (rev 111) - GetFirst() and GetEnd() added
     - 2009-07-31 (rev 113) - deleteObject() added
     - 2009-09-24 (rev 131) - } small fixes
     - 2009-09-27 (rev 132) - }
     - 2010-01-30 (rev 161) - obsolete load/save functions removed
                            - documentation updated
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-26 (rev 260) - value for collision detection added
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - renamed numberOfObjects() to getNumberOfObjects()
                            - iterator type for getFirst() and getEnd()
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions
     - 2012-07-02 (rev 310) - update of ObjectRecord to be a descendant of
                              DataRecord
                            - ObjectBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef DUSK_OBJECTRECORD_H
#define DUSK_OBJECTRECORD_H

#include <fstream>
#include "DataRecord.h"

namespace Dusk
{
  /* struct ObjectRecord
            This struct's purpose is to hold all data of one distinct static
            object type in the game, including its corresponding mesh name.
            "Static objects" in the above context are all non-moving objects
            which cannot be picked up by the player or a NPC. Examples of
            static objects would be walls, trees, furniture (non-movable) and
            so on. Things like an apple or a bottle are NOT "static objects",
            they are "items", because they can be picked up and/or used by the
            player. See the classes ItemRecord and Item for items.


            Think of ObjectRecord as a sort of table entry, e.g.:

              ID  |  Mesh             | collision
            ------+-------------------+-----------
            tree  | tree_oak.mesh     | no
            Chair | wooden_chair.mesh | yes
             ...  | ...               | ...
  */

  struct ObjectRecord: public DataRecord
  {
    std::string Mesh;
    bool collide;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const;

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

}//namespace

#endif // DUSK_OBJECTRECORD_H
