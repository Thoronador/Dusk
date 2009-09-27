#ifndef OBJECTBASE_H
#define OBJECTBASE_H

#include <map>
#include <string>
#include <fstream>

namespace Dusk
{
  /* class ObjectBase
           This class' purpose is to hold the IDs of all distinct static objects
           in the game and their corresponding mesh name. "Static objects" in
           the above context are all non-moving objects which cannot be picked
           up by the player or a NPC. Examples of static objects would be walls,
           trees, furniture (non-movable) and so on.
           Things like an apple or a bottle are NOT "static objects", they are
           "items", because they can be picked up and/or used by the player. See
           the class ItemBase for items.


           Think of ObjectBase as a sort of look-up table, e.g.:

             ID  |  Mesh
           ------+------------------
           tree  | tree_oak.mesh
           Chair | wooden_chair.mesh
            ...  | ...
  */
  class ObjectBase
  {
    public:
      virtual ~ObjectBase();
      static ObjectBase& GetSingleton();
      bool hasObject(const std::string& NameOfObject) const;
      void addObject(const std::string& ID, const std::string& Mesh);
      bool deleteObject(const std::string& ID_of_Object);
      void ClearAllObjects();
      unsigned int NumberOfObjects() const;
      std::string GetMeshName(const std::string& ID, const bool UseMarkerOnError=true) const;
      bool SaveToFile(const std::string& FileName) const;
      bool LoadFromFile(const std::string& FileName);
      bool SaveToStream(std::ofstream& Stream) const;
      bool LoadFromStream(std::ifstream& Stream);
      std::map<std::string, std::string>::const_iterator GetFirst() const;
      std::map<std::string, std::string>::const_iterator GetEnd() const;
    private:
      ObjectBase();
      ObjectBase(const ObjectBase& op){}
      std::map<std::string, std::string> m_ObjectList;
  };//class

}//namespace

#endif
