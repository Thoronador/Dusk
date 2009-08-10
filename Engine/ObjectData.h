#ifndef OBJECTDATA_H
#define OBJECTDATA_H

#include "ObjectBase.h"
#include "DuskObject.h"
#include <vector>
#include <fstream>

namespace Dusk
{

  class ObjectData
  {
    public:
      virtual ~ObjectData();
      static ObjectData& GetSingleton();
      unsigned int NumberOfReferences() const;
      DuskObject* addReference(const std::string ID, const Ogre::Vector3 position,
                               const Ogre::Vector3 rotation, const float scale);
      bool LoadFromFile(const std::string FileName);
      bool SaveToFile(const std::string FileName);
      bool SaveToStream(std::ofstream* Stream);
      bool LoadFromStream(std::ifstream* Stream);
      void EnableAllObjects(Ogre::SceneManager * scm);
      void DisableAllObjects();
      unsigned int deleteReferencesOfObject(const std::string ID);
      unsigned int reenableReferencesOfObject(const std::string ID, Ogre::SceneManager * scm);
      unsigned int updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm);
      void ClearData();
    private:
      ObjectData();
      ObjectData(const ObjectData& op){}
      std::vector<DuskObject*> m_ReferenceList;
  };//class

}//namespace

#endif // OBJECTDATA_H
