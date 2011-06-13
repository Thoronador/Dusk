/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011 thoronador

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
 Date:    2010-05-13
 Purpose: LandscapeRecord class and Landscape Singleton class
          both classes together manage the landscape data of the game

 History:
     - 2009-01-16 (rev 70)  - initial version (by thoronador)
     - 2009-01-25 (rev 75)  - landscape now sends data to engine/ Ogre
     - 2009-01-27 (rev 77)  - SaveToFile() implemented
                            - indexed geometry for landscape
     - 2009-02-11 (rev 79)  - Shift(), Scale(), MakePlain(), IsPlain() added,
                              i.e. functions for data manipulation
                            - destructor now frees Ogre::ManualObjects
                            - RecordsAvailable() added
     - 2009-02-13 (rev 80)  - lower bound check for Scale()
     - 2009-02-19 (rev 81)  - Landscape class splitted into LandscapeRecord and
                              Landscape (manager class) to ease handling of
                              landscape data
     - 2009-02-23 (rev 83)  - unused variable removed
     - ...
     - ... There's still something missing here!
     - ...
     - 2010-05-08 (rev 200) - documentation updated
                            - saveAllToStream() added
     - 2010-05-14 (rev 203) - possibility to render Landscape as wire frame
     - 2010-08-16 (rev 229) - error in changeListSize() fixed
     - 2010-11-13 (rev 254) - naming guidelines enforced
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2011-01-19 (rev 275) - setColourRadial() added
     - 2011-06-13 (rev 293) - improvements to get rid of some compiler warnings

 ToDo list:
     - implement LoadRecordFromStream() for Landscape class
     - try to get rid of getRecordByPosition(), if it's not used anymore by any
       other files in Engine and Editor
     - complete history (see above)
     - allow landscape textures, multiple ones per record
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <fstream>
#include <string>
#ifndef NO_OGRE_IN_LANDSCAPE
  #include <OgreRay.h>
  #include <OgreVector3.h>
  #include <OgreManualObject.h>
  #include <OgreSceneManager.h>
  #include <vector>
#endif
#include "DuskTypes.h"

namespace Dusk
{
  unsigned int GenerateUniqueID();

  /*struct land_rec
  {
    char Land[4]; //"Land"
    float OffsetX, OffsetY; //shift of land via x and z axis
    float Stride; //probably used in case the distances are not fixed
    float Height[65][65]; //height of the land
    unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
    //not part of actual data, but calculated during loading process
    float Highest, Lowest;
  };
  */

  class LandscapeRecord
  {
    public:
      /* constructor */
      LandscapeRecord();

      /* destructor */
      virtual ~LandscapeRecord();

      //shift of land via x and z axis
      /* returns the offset of record at the X-axis */
      float getOffsetX() const;

      /* returns the offset of record at the Z-axis

         remarks:
           Yes, it's Z and not Y, despite the name.
      */
      float getOffsetY() const;

      /* returns the distance between two adjacent points */
      float getStride() const;

      float Height[65][65]; //height of the land
      unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values

      /*returns highest altitude in record (calculated during loading process)*/
      float getHighest() const;

      /*returns lowest altitude in record (calculated during loading process)*/
      float getLowest() const;

      //load and save functions
      /* tries to load the LandscapeRecord from the given stream and returns
         true on success, or false if an error occured.

         remarks:
             If the functions fails, the record may possibly contain incon-
             sistent data and therefore should not be used any more.
      */
      bool loadFromStream(std::ifstream& AStream);

      /* tries to save landscape data to stream and returns true on success */
      bool saveToStream(std::ofstream& AStream) const;

      /*function for determining whether data is loaded or not - returns true,
        if the record contains valid landscape data. However, only the height
        values/data are considered, the colour data may still be crap.
      */
      bool isLoaded() const;

      /* manipulates loading state (i.e. return value of IsLoaded() directly

         remarks:
             Use with care, or better: not at all.
      */
      void setLoadedState(const bool value);

      // **** manipulating landscape data ****

      /* shifts land delta units up/ down - positive values produce a shift
         upwards, negative values downwards. Returns true on success.

         remarks:
            Will always return true.
      */
      bool shift(const float delta);

      /* scales the land data by factor, e.g. factor==10 -> all land is ten
         times as high as before

         return value:
             True in case of success, false in case of failure.

         remarks:
            The value of factor has to be larger than cMinScale.
            If no data is loaded (yet), the function will fail.
      */
      bool scale(const float factor);
      /* make it plain at level value (i.e. all is as high as value indicates)

         return value:
             True on success (currently, that function cannot fail)
      */
      bool makePlain(const float value);

      /* returns true, if landscape data of this record is a plane */
      bool isPlain() const;

      /* creates landscape data via generator function; returns true on success

         parameters:
             func - the function that returns the height of the landscape at a
                    given point

         remarks:
             The function has to be able to produce values for both x and z in [0;1].
      */
      bool generateByFunction( float (*func) (const float x, const float z));

      /* colour landscape by generator function; returns true on success

          remarks:
              Function func has to be able to produce values for x and z in [0;1]
      */
      bool colourByFunction(ColourData (*func) (const float x, const float z));

      /* move record to new position

         parameters:
             Offset_X - x-coordinate of new position
             Offset_Y - z(!)-coordinate of new position
      */
      void moveTo(const float Offset_X, const float Offset_Y);

      /* moves the landscape above/below point (x, 0, z) delta units up/down

         parameters:
             x     - x-coordinate of the point that shall be moved
             z     - z-coordinate of the point that shall be moved
             delta - amount the point should be moved up/down
                     (positive value -> up; negative value -> down)

         return value:
             Returns true on success.
      */
      bool terraform(const float x, const float z, const float delta);

      /* funtion to set colour at certain point. Returns true on success.

         parameters:
             x     - x-coordinate of the point that shall be moved
             z     - z-coordinate of the point that shall be moved
             r,g,b - new RGB-colour value
      */
      bool setColour(const float x, const float z, const unsigned char r,const unsigned char g, const unsigned char b);

      /* funtion to set colour at certain point and its surrounding area.
         Returns true on success.

         parameters:
             x      - x-coordinate of the point that shall be moved
             z      - z-coordinate of the point that shall be moved
             r,g,b  - new RGB-colour value
             radius - radius of the effect
      */
      bool setColourRadial(const float x, const float z, const unsigned char r,
                           const unsigned char g, const unsigned char b,
                           const float radius);

      /* sets the stride value (i.e. distance between adjacent points) to
         new_stride and returns true on success, false otherwise.

         remarks:
             The function will fail, if the new stride value is less or equal to
             zero. In this case, the value won't be changed.
      */
      bool setStride(const float new_stride);

      #ifndef NO_OGRE_IN_LANDSCAPE
      /* send loaded data to scene manager and return true on success

         parameters:
             scm       - SceneManager that shall be used to display landscape
             WireFrame - if true, landscape will be drawn as wireframe
      */
      bool enable(Ogre::SceneManager * scm, const bool WireFrame);

      /*remove data from scene manager; returns true on success */
      bool disable();

      /* updates record, i.e. disable and re-enable it to get new data shown
         Returns true on success, false on failure.
         If the record is currently not enabled, it returns true and does
         NOT enable it.

         parameters:
             WireFrame - if true, landscape will be drawn as wireframe
      */
      bool update(const bool WireFrame);

      /* determines, whether record is currently shown */
      bool isEnabled() const;

      /* checks a string for a valid Landscape record name and returns true, if
         the string val contains a valid name
      */
      static bool isLandscapeRecordName(const std::string& val);

      /* checks whether a ray hits this record or not

         remarks:
             This can be very slow, so only use it, when you know that this
             LandscapeRecord might possibly be hit by the ray, e.g. from a
             previous scene query.

         parameters:
             ray      - the ray to check for hit on this record
             HitPoint - a 3D vector, where the hit location will be stored
      */
      bool isHitByRay(const Ogre::Ray& ray, Ogre::Vector3& HitPoint) const;
      #endif
      // returns unique "identifier" of landscape record
      unsigned int getID() const;

      // contains the dafault value for stride
      static const float cDefaultStride;
      // minimum value allowed in Scale()
      static const float cMinScale;
      // prefix for names of all ManualObjects created during landscape enabling
      static const std::string cLandscapeNamePrefix;
    private:
      static unsigned int m_genID;
      //not part of actual data, but calculated during loading process
      float m_Highest, m_Lowest;
      float m_OffsetX, m_OffsetY; //shift of land via x and z axis
      float m_Stride; //distance between points
      bool m_Loaded;
      unsigned int m_RecordID;
      #ifndef NO_OGRE_IN_LANDSCAPE
      Ogre::ManualObject * m_OgreObject;

      /* returns the position of the point represented by Height[i][j] as a 3D
         Ogre Vector (utility function)
      */
      const Ogre::Vector3 getPositionOfIndex(const unsigned int i, const unsigned int j) const;
      #endif
  };

  class Landscape
  {
    public:
      /* destructor */
      virtual ~Landscape();

      /* singleton access method */
      static Landscape& getSingleton();

      //load and save (all) records from/to file
      /* tries to load LandscapeRecords from the file specified by FileName.
         Returns true on success, false on failure.
      */
      bool loadFromFile(const std::string& FileName);

      /* tries to save all present LandscapeRecords to the file specified by
         FileName. Returns true on success.
      */
      bool saveToFile(const std::string& FileName) const;

      /* tries to save all records to the given stream and returns true on
         success
      */
      bool saveAllToStream(std::ofstream& AStream) const;

      /* creates a new landscape record and returns a pointer to it */
      LandscapeRecord* createRecord();

      /* deletes the landscape record pointed to be recPtr

         remarks:
             After the call to that function, recPtr must NOT be referenced,
             because it will point to then deleted memory.
      */
      void destroyRecord(const LandscapeRecord* recPtr);

      /* returns the number of present LandscapeRecords */
      unsigned int getNumberOfRecordsAvailable();

      /* returns the LandscapeRecord at internal position record, or NULL if no
         record exists at this position

         parameters:
             record - position of the record in internal array

         remarks:
             This function will NOT always return the same record for the same
             value of parameter record, because randomly deleting a record will
             change the internal position of records.
      */
      LandscapeRecord* getRecordByPosition(const unsigned int record);

      /* returns the record with ID recordID, or NULL if no record with that ID
         is available

         remarks:
             This function is safer than getRecordByPosition(), because you will
             always get the same record for the same ID until the record is
             deleted. (After deletion, it will return a NULL pointer.)
      */
      LandscapeRecord* getRecordByID(const unsigned int recordID);

      /* tries to find the LandscapeRecord which covers the point (x,0.0,y) and
         returns a pointer to it. If no record is at that point, NULL will be
         returned.
      */
      LandscapeRecord* getRecordAtXZ(const float x, const float y) const;

      /* deletes all LandscapeRecords */
      void clearAllRecords();

      #ifndef NO_OGRE_IN_LANDSCAPE
      /* sends all loaded data to scene manager and returns true on success

         parameters:
             scm       - SceneManager that shall be used to display landscape
             WireFrame - if true, landscape will be drawn as wireframe
      */
      bool sendToEngine(Ogre::SceneManager * scm, const bool WireFrame=false);

      /* removes all loaded landscape data from scene manager and returns true
         in case of success

         parameters:
             scm - SceneManager that displays the landscape

          remarks:
             scm should be the same SceneManager that was previously used in the
             call to SendToEngine().
      */
      bool removeFromEngine(Ogre::SceneManager * scm);

      /* notify Landscape about a record that needs to be updated

         parameters:
             who - pointer to the LandscapeRecord that needs to be updated

         remarks:
             A LandscapeRecord that requests an update will be disabled and
             re-enabled during the next call of UpdateRecords().
      */
      void requestUpdate(LandscapeRecord* who);

      /*returns true, if there are any records who need an update */
      bool needsUpdate() const;

      /* performs update of records who requested it since last call,
         and returns number of updated records

         parameters:
             WireFrame - if true, landscape will be drawn as wireframe
      */
      unsigned int updateRecords(const bool WireFrame);
      #endif
      /* returns the height of the landscape at a given point, i.e. the y-value
         of the point (x,y,z), where (x,y,z) is directly at the landscape's
         surface. If no landscape data for the given point is present, 0.0 will
         be returned.

         remarks:
             This function is used for "collision detection" with landscape
             within the game.
      */
      float getHeightAtPosition(const float x, const float z) const;
      static const std::string cLandNodeName;
      static const unsigned int cMaxLandRecords;
    private:
      /* private constructor (singleton) */
      Landscape();
      /* private, empty constructor (singleton pattern) */
      Landscape(const Landscape& op){}

      /* internal function to change the size/length of m_RecordList */
      void changeListSize(const unsigned int new_size);

      LandscapeRecord ** m_RecordList;
      unsigned int m_numRec, m_Capacity;
      #ifndef NO_OGRE_IN_LANDSCAPE
      //list of LandscapeRecords that want to be updated
      std::vector<LandscapeRecord*> m_RecordsForUpdate;
      #endif
  };
}

#endif // LANDSCAPE_H
