#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <fstream>
#include <iostream>
#include <string>
#include <OgreManualObject.h>

namespace Dusk
{
  const float cDefaultStride = 5.00;
  const unsigned int cMaxLandRecords = 2500; //50 x 50 squares should do
  const float cMinScale = 0.01;

  //maybe this should go somewhere else
  #if defined(_WIN32)
    const std::string path_sep = "\\";
  #else
    const std::string path_sep = "/";
  #endif

  /*typedef struct land_rec
  {
    char Land[4]; //"Land"
    float OffsetX, OffsetY; //shift of land via x and z axis
    float Stride; //probably used in case the distances are not fixed
    float Height[65][65]; //height of the land
    unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
    //not part of actual data, but calculated during loading process
    float Highest, Lowest;
  };
  typedef land_rec TLandscapeRecord;*/

  class LandscapeRecord
  {
    public:
      LandscapeRecord();
      virtual ~LandscapeRecord();
      float OffsetX, OffsetY; //shift of land via x and z axis
      float Stride; //probably used in case the distances are not fixed
      float Height[65][65]; //height of the land
      unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
      //not part of actual data, but calculated during loading process
      float Highest, Lowest;
      bool LoadFromStream(std::ifstream *AStream);
      bool SaveToStream(std::ofstream *AStream);
      bool IsLoaded();
      //manipulating landscape data
      bool Shift(const float delta);
      bool Scale(const float factor);
      bool MakePlain(const float value);
      bool IsPlain();
      //send loaded data to scene manager
      bool SendDataToEngine();
      bool RemoveDataFromEngine();
    private:
      bool m_Loaded;
      Ogre::ManualObject * m_OgreObject;
      unsigned int m_ObjectIndex;
  };

  //don't touch this
  static unsigned int m_ObjectCount=0;

  class Landscape
  {
    public:
      virtual ~Landscape();
      static Landscape& GetSingleton();

      //load and save (all) records from/to file
      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      unsigned int RecordsAvailable();
      LandscapeRecord* GetRecord(const unsigned int record);
      //send loaded data to scene manager
      bool SendToEngine();
      //for collision detection
      float GetHeigtAtPosition(const float x, const float y) const;
    private:
      Landscape();
      Landscape(const Landscape& op){}

      LandscapeRecord * m_RecordList;
      unsigned int m_numRec;
  };
}

#endif // LANDSCAPE_H
