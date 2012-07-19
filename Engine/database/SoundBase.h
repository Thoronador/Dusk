/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012 thoronador

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
 Date:    2012-04-07
 Purpose: SoundRecord class
          Holds a single entry within a look-up table for all distinct sound
          "objects" within the game.

          Think of SoundRecord as a row in the following table:

             ID      |  path
           ----------+----------------------
           gunShot   | bang.wav
           stepSound | moves/feet.ogg
           windy     | blowingInTheWind.ogg
            ...      | ...

 History:
     - 2012-04-07 (rev 305) - initial version (by thoronador)
     - 2012-07-19 (rev 321) - update of SoundRecord to be a descendant of
                              DataRecord
                            - removed SoundBase (Database will handle it)

 ToDo list:
     - ???

 Known bugs:
     - None. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_SOUNDRECORD_H
#define DUSK_SOUNDRECORD_H

#include <string>
#include "DataRecord.h"

namespace Dusk
{

  //SoundRecord class
  struct SoundRecord: public DataRecord
  {
    std::string filePath;

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

#endif // DUSK_SOUNDRECORD_H
