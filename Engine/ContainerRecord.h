/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012 thoronador

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
 Date:    2009-09-27
 Purpose: ContainerBase Singleton class
          Represents a look-up table for all distinct container objects
          within the game.

 History:
     - 2009-09-24 (rev 131) - initial version (by thoronador)
     - 2009-09-27 (rev 132) - NumberOfCointainers() added
                            - methods to load and save containers added
                              (LoadNextContainerFromStream(), SaveAllToStream())
     - 2010-01-01 (rev 148) - documentation update
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-21 (rev 257) - minor optimization
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2012-04-05 (rev 303) - non-existent IDs in query functions now get logged
                              to DuskLog and might throw exceptions
     - 2012-06-30 (rev 308) - update of getContainerMesh() definition
     - 2012-07-05 (rev 313) - update of ContainerRecord to be a descendant of
                              DataRecord
                            - ContainerBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_CONTAINERRECORD_H
#define DUSK_CONTAINERRECORD_H

#include <fstream>
#include "DataRecord.h"
#include "Inventory.h"

namespace Dusk
{
  struct ContainerRecord: public DataRecord
  {
    std::string Mesh;
    Inventory ContainerInventory;

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
  };

}//namespace

#endif // DUSK_CONTAINERRECORD_H
