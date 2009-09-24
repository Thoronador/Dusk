#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <fstream>
#include <string>
#ifndef NO_OGRE_IN_LANDSCAPE
  #include <OgreManualObject.h>
  #include <OgreSceneManager.h>
#endif

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
      LandscapeRecord();
      virtual ~LandscapeRecord();
       //shift of land via x and z axis
      float OffsetX() const;
      float OffsetY() const;
      float Stride; //probably used in case the distances are not fixed
      float Height[65][65]; //height of the land
      unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
      //Highest + lowest altitude in record (calculated during loading process)
      float Highest() const;
      float Lowest() const;
      //load and save functions
      bool LoadFromStream(std::ifstream& AStream);
      bool SaveToStream(std::ofstream& AStream) const;
      //function for determining whether data is loaded or not
      bool IsLoaded() const;
      //mainpulate loading state directly, use with care, or better: not at all
      void SetLoadedState(const bool value);
      //manipulating landscape data
      //   shifts land delta units up/ down
      bool Shift(const float delta);
      //   scales land by factor (factor==10 -> all land is ten times as high)
      bool Scale(const float factor);
      //   make it plain at level value (i.e. all is as high as value indicates)
      bool MakePlain(const float value);
      //   is it plain?
      bool IsPlain() const;
      //   create landscape data by generator function
      //   (function has to be able to produce values for x and z in [0;1])
      bool GenerateByFunction( float (*func) (const float x, const float z));
      //move to new position
      void MoveTo(const float Offset_X, const float Offset_Y);
      #ifndef NO_OGRE_IN_LANDSCAPE
      //send loaded data to scene manager
      bool SendDataToEngine(Ogre::SceneManager * scm);
      //remove data from scene manager
      bool RemoveDataFromEngine();
      #endif
      //"identifier"
      unsigned int GetID() const;
      static const float cDefaultStride;
      static const float cMinScale;
    private:
      static unsigned int m_genID;
      //not part of actual data, but calculated during loading process
      float m_Highest, m_Lowest;
      float m_OffsetX, m_OffsetY; //shift of land via x and z axis
      bool m_Loaded;
      unsigned int m_RecordID;
      #ifndef NO_OGRE_IN_LANDSCAPE
      Ogre::ManualObject * m_OgreObject;
      Ogre::SceneManager * m_scm;
      #endif
  };

  class Landscape
  {
    public:
      virtual ~Landscape();
      static Landscape& GetSingleton();

      //load and save (all) records from/to file
      bool LoadFromFile(const std::string& FileName);
      bool SaveToFile(const std::string& FileName);
      LandscapeRecord* CreateRecord();
      void DestroyRecord(const LandscapeRecord* recPtr);
      unsigned int RecordsAvailable();
      LandscapeRecord* GetRecordByPosition(const unsigned int record);
      LandscapeRecord* GetRecordByID(const unsigned int recordID);
      void ClearAllRecords();
      #ifndef NO_OGRE_IN_LANDSCAPE
      //send loaded data to scene manager
      bool SendToEngine(Ogre::SceneManager * scm);
      //remove loaded data from scene manager
      bool RemoveFromEngine(Ogre::SceneManager * scm);
      #endif
      //for collision detection
      float GetHeightAtPosition(const float x, const float y) const;
      static const std::string cLandNodeName;
      static const unsigned int cMaxLandRecords;
    private:
      Landscape();
      Landscape(const Landscape& op){}
      void ChangeListSize(const unsigned int new_size);

      LandscapeRecord ** m_RecordList;
      unsigned int m_numRec, m_Capacity;
  };
}

#endif // LANDSCAPE_H
