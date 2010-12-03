/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
 Purpose: LightBase Singleton class
          holds information about all distinct lights in the game
          Think of it as a sort of look-up table for lights.

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

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please. I'll try
       to fix it as soon as possible.
 --------------------------------------------------------------------------*/

#ifndef LIGHTBASE_H
#define LIGHTBASE_H

#include <string>
#include <map>
#include <fstream>
#include <OgreLight.h>

namespace Dusk
{

  /* structure that holds the data of a light */
  struct LightRecord
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
             d - range of light
      */
      static LightRecord getBlack(const float d=0.0f);

      /* static function to get a red point light record

         parameters:
             d - range of light
      */
      static LightRecord getRed(const float d=0.0f);

      /* static function to get a green point light record

         parameters:
             d - range of light
      */
      static LightRecord getGreen(const float d=0.0f);

      /* static function to get a blue point light record

         parameters:
             d - range of light
      */
      static LightRecord getBlue(const float d=0.0f);
  };

  /* overloaded equality operator for light records */
  bool operator==(const LightRecord& l, const LightRecord& r);


  /* the LightBase Singleton class */
  class LightBase
  {
    public:
      /* destructor */
      virtual ~LightBase();

      /* singleton access method */
      static LightBase& getSingleton();

      /* adds a new light record to the database

         parameters:
             ID     - ID of the new light record
             r      - red colour component of light (should be in [0;1])
             g      - green colour component of light (should be in [0;1])
             b      - blue colour component of light (should be in [0;1])
             radius - range of light (should be greater than zero)
             _type  - type of light (point, spotlight,...)

         remarks:
             This function will always succeed, except if the application runs
             out of memory or the ID is an empty string. If a light record with
             the given (not empty) ID already exists, this record will be over-
             written.
      */
      void addLight(const std::string& ID, const float r, const float g, const float b, const float radius,
                    const Ogre::Light::LightTypes _type = Ogre::Light::LT_POINT);

      /* adds a new light record to the database

         parameters:
             ID     - ID of the new light record
             record - LightRecord containing the light data

         remarks:
             This function variant is provided for convenience.
             See prevoius version of addLight() for remarks.
      */
      void addLight(const std::string& ID, const LightRecord& record);

      /* returns true, if a light record for the given ID exists */
      bool hasLight(const std::string& ID) const;

      /* returns the light record of the Light with ID 'ID'

         remarks:
             If no record with that given ID exists, a black point light with
             range zero is returned. You can use LightRecord's IsBlack() method
             to check for such a return value.
      */
      LightRecord getLightData(const std::string& ID) const;

      /* tries to delete the light record with the given ID and returns true,
         if a record with that ID was deleted
      */
      bool deleteLight(const std::string& ID);

      /* deletes all light records

         remarks:
             Currently only used be Editor and DataLoader.
      */
      void clearAllData();

      /* returns the number of distinct light records */
      unsigned int numberOfLights() const;

      /* tries to save all light records to the given stream. Returns true, if
         the operation was successful, and false if it failed.

         parameters:
             out_stream - the output stream that is used to save the data
      */
      bool saveAllToStream(std::ofstream& out_stream) const;

      /* tries to load one single light record from the given stream and returns
         true, if it was successful

         parameters:
             in_stream - the input stream that is used to load the light data
      */
      bool loadRecordFromStream(std::ifstream& in_stream);

      /* utility functions to access internal map iterators - not used in-game,
         only used by Editor application.
      */
      std::map<std::string, LightRecord>::const_iterator getFirst() const;
      std::map<std::string, LightRecord>::const_iterator getEnd() const;
    private:
      /* private constructor (singleton pattern) */
      LightBase();
      /* empty copy constructor due to singleton pattern */
      LightBase(const LightBase& op){}
      //list of lights
      std::map<std::string, LightRecord> m_LightList;
  };//class

}//namespace

#endif // LIGHTBASE_H
