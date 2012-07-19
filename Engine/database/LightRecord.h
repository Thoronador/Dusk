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
 Date:    2010-03-27
 Purpose: LightRecord structure
          holds information about one distinct light in the game
          Think of it as a sort of record in a look-up table for lights.

 History:
     - 2009-09-13 (rev 128) - initial version (by thoronador)
     - 2009-09-18 (rev 129) - light type added to LightRecord
                            - GetFirst() and GetEnd() added
     - 2009-09-24 (rev 131) - small improvements
     - 2009-09-27 (rev 132) - small improvements
     - 2009-09-27 (rev 133) - equality operator for LightRecords
     - 2010-05-06 (rev 198) - documentation update
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-21 (rev 257) - minor optimization
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - iterator type for getFirst() and getEnd() added
     - 2012-04-05 (rev 303) - non-existent IDs in getLightData() will now throw
                              exceptions
     - 2012-07-02 (rev 310) - update of LightRecord to be a descendant of
                              DataRecord
                            - LightBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please. I'll try
       to fix it as soon as possible.
 --------------------------------------------------------------------------*/

#ifndef DUSK_LIGHTRECORD_H
#define DUSK_LIGHTRECORD_H

#include <fstream>
#include <OgreLight.h>
#include "DataRecord.h"

namespace Dusk
{

  /* structure that holds the data of a light */
  struct LightRecord: public DataRecord
  {
    public:
      /* red, green and blue colour values of light */
      float red,green,blue;
      /* radius/range of light */
      float radius;
      /* type of light */
      Ogre::Light::LightTypes type;

      /* returns true, if the colour of the light is black */
      bool isBlack() const;

      /* "normalises" colour components of the light, i.e. it makes sure the
         values are within [0;1], and sets negative radius to zero
      */
      void normalise();

      /* static function to get a black point light record

         parameters:
             _ID - ID of the record
             d   - range of light
      */
      static LightRecord getBlack(const std::string& _ID, const float d=0.0f);

      /* static function to get a red point light record

         parameters:
             _ID - ID of the record
             d   - range of light
      */
      static LightRecord getRed(const std::string& _ID, const float d=0.0f);

      /* static function to get a green point light record

         parameters:
             _ID - ID of the record
             d   - range of light
      */
      static LightRecord getGreen(const std::string& _ID, const float d=0.0f);

      /* static function to get a blue point light record

         parameters:
             _ID - ID of the record
             d   - range of light
      */
      static LightRecord getBlue(const std::string& _ID, const float d=0.0f);

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

  /* overloaded equality operator for light records */
  bool operator==(const LightRecord& l, const LightRecord& r);

}//namespace

#endif // DUSK_LIGHTRECORD_H
