#ifndef LANDSCAPE_H
#define LANDSCAPE_H

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

  typedef struct land_rec
  {
    char Land[4]; //"Land"
    float OffsetX, OffsetY; //shift of land via x and z axis
    float Stride; //probably used in case the distances are not fixed
    float Height[65][65]; //height of the land
    unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
    //not part of actual data, but calculated during loading process
    float Highest, Lowest;
  };
  typedef land_rec TLandscapeRecord;

  class Landscape
  {
    public:
      virtual ~Landscape();
      static Landscape& GetSingleton();

      //load and save (all) records from/to file
      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      unsigned int RecordsAvailable();
      //send loaded data to scene manager
      bool SendToEngine();
      //manipulating landscape data
      bool Shift(const unsigned int record, const float delta);
      bool Scale(const unsigned int record, const float factor);
      bool MakePlain(const unsigned int record, const float value);
      bool IsPlain(const unsigned int record);

      //for collision detection
      float GetHeigtAtPosition(const float x, const float y) const;
    private:
      Landscape();
      Landscape(const Landscape& op){}

      void InitObjects(const unsigned int num);

      TLandscapeRecord * m_RecordList;
      unsigned int m_numRec;

      Ogre::ManualObject ** m_ObjectList;
      unsigned int m_numObj;

  };
}

#endif // LANDSCAPE_H
