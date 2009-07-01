#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <map>
#include <string>
#include <iostream>

namespace Dusk
{
  /* class ObjectBase
           This class' purpose is to hold the IDs of all distinct static objects
           in the game and their corresponding mesh name.
           Think of it as a sort of look-up table, e.g.:

             ID  |  Mesh
           ------+------------------
           Robot | robot.mesh
           Chair | wooden_chair.mesh
            ...  | ...
  */
  class ObjectBase
  {
    public:
      virtual ~ObjectBase();
      static ObjectBase& GetSingleton();
      bool hasObject(const std::string NameOfObject) const;
      void addObject(const std::string ID, const std::string Mesh);
      void ClearAllObjects();
      unsigned int NumberOfObjects() const;
      std::string GetMeshName(const std::string ID) const;
      bool SaveToFile(const std::string FileName);
      bool LoadFromFile(const std::string FileName);
    private:
      ObjectBase();
      ObjectBase(const ObjectBase& op){}
      std::map<std::string, std::string> m_ObjectList;
  };//class

}//namespace

#endif
