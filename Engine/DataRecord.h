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
 Date:    2012-07-01
 Purpose: DataRecord class
          Abstract base class for data records.

 History:
     - 2012-07-01 (rev 309) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_DATARECORD_H
#define DUSK_DATARECORD_H

#include <stdint.h>
#include <string>

namespace Dusk
{

struct DataRecord
{
  public:
    std::string ID;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const = 0;

    /* Tries to save the data record to stream outStream and returns true on
       success, false otherwise.

       parameters:
           outStream - the output stream to which the data will be saved
    */
    virtual bool saveToStream(std::ofstream& outStream) const = 0;

    /* Tries to load the data record from stream inStream and returns true on
       success, false otherwise.

       parameters:
           inStream - the input stream from which the data will be read

       remarks:
           The record may have inconsistent data, if the function fails.
    */
    virtual bool loadFromStream(std::ifstream& inStream) = 0;
};//struct

} //namespace

#endif // DUSK_DATARECORD_H
