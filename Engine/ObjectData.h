#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "ObjectBase.h"
#include "DuskObject.h"
#include <vector>
#include <fstream>

namespace Dusk
{

  /* basic data should contain:
     ID of Object
     position
     rotation/ orientation
     scaling factor
  */

  class ObjectData
  {
    public:
      virtual ~ObjectData();
      static ObjectData& GetSingleton();
      unsigned int NumberOfReferences() const;
      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      bool SaveToStream(std::ofstream* Stream);
      void EnableAllObjects(Ogre::SceneManager * scm);
      void DisableAllObjects();
    private:
      ObjectData();
      ObjectData(const ObjectData& op){}
      std::vector<DuskObject*> m_ReferenceList;
  };//class

}//namespace

#endif // OBJECTDATA_H
