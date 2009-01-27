#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <string>
#include <OgreManualObject.h>

namespace Dusk
{
  const float cDefaultStride = 5.00;
  const unsigned int cMaxLandRecords = 2500; //50 x 50 squares should do

  typedef struct land_rec
  {
    char Land[4]; //"Land"
    float OffsetX, OffsetY; //shift of land via x and z axis
    float Stride; //probably used in case the distances are not fixed
    float Height[65][65]; //height of the land
    unsigned char Colour[65][65][3]; //colour of the land in RGB-byte values
  };
  typedef land_rec TLandscapeRecord;

  class Landscape
  {
    public:
      virtual ~Landscape();
      static Landscape& GetSingleton();

      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      bool SendToEngine();

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
