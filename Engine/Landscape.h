#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <fstream>
#include <string>
#ifndef NO_OGRE_IN_LANDSCAPE
  #include <OgreManualObject.h>
#endif

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
      //load and save functions
      bool LoadFromStream(std::ifstream *AStream);
      bool SaveToStream(std::ofstream *AStream);
      //function for determining whether data is loaded or not
      bool IsLoaded();
      //mainpulate loading state directly, use with care, or better: not at all
      void SetLoadedState(const bool value);
      //manipulating landscape data
      bool Shift(const float delta);
      bool Scale(const float factor);
      bool MakePlain(const float value);
      bool IsPlain();
      #ifndef NO_OGRE_IN_LANDSCAPE
      //send loaded data to scene manager
      bool SendDataToEngine();
      bool RemoveDataFromEngine();
      #endif
    private:
      bool m_Loaded;
      #ifndef NO_OGRE_IN_LANDSCAPE
      Ogre::ManualObject * m_OgreObject;
      unsigned int m_ObjectIndex;
      #endif
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
      LandscapeRecord* CreateRecord();
      void DestroyRecord(const LandscapeRecord* recPtr);
      unsigned int RecordsAvailable();
      LandscapeRecord* GetRecord(const unsigned int record);
      #ifndef NO_OGRE_IN_LANDSCAPE
      //send loaded data to scene manager
      bool SendToEngine();
      #endif
      //for collision detection
      float GetHeightAtPosition(const float x, const float y) const;
    private:
      Landscape();
      Landscape(const Landscape& op){}
      void Landscape::ChangeListSize(const unsigned int new_size);

      LandscapeRecord ** m_RecordList;
      unsigned int m_numRec, m_Capacity;
  };
}

#endif // LANDSCAPE_H
