#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "ObjectBase.h"
#include "DuskObject.h"
#include <vector>

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
      ObjectData& GetSingleton();
      unsigned int NumberOfReferences() const;
      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      void EnableAllObjects();
      void DisableAllObjects();
    private:
      ObjectData();
      ObjectData(const ObjectData& op){}
      std::vector<DuskObject*> m_ReferenceList;
  };//class

}//namespace

#endif // OBJECTDATA_H
