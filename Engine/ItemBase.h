#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <map>
#include <string>
#include <fstream>

namespace Dusk
{
  struct ItemRecord
  {
    std::string Name;
    int value;
    float weight;
    std::string Mesh;
  };

  const ItemRecord cEmptyItemRecord = { "", 0, 0.0, ""};


  /* class ItemBase
           This class' purpose is to hold the IDs of all distinct items in the
           game and their corresponding data like mesh name, cost/value, weight.
           "Items" in the above context are all non-moving objects which can be
           picked up by the player or a NPC and put in the inventory. Examples
           of items would be apples, bottles and so on, maybe even weapons.
           Things like a chair are not "items", they are "static objects",
           because they cannot be picked up and/or used by the player. See the
           class ObjectBase for "objects".

           Think of ItemBase as a sort of look-up table, e.g.:

             ID   |  name          |  value | weight | mesh
           -------+----------------+--------+--------+-------------------
           apple  | Fresh Apple    |   5    |  0.2   | tree_oak.mesh
           bottle | Bottle of Milk |   8    |  0.75  | filled_bottle.mesh
             ...  |  ...           |  ...   |  ...   | ...

  */

  class ItemBase
  {
    public:
      virtual ~ItemBase();
      static ItemBase& GetSingleton();
      bool hasItem(const std::string& ID_of_item) const;
      void addItem(const std::string& ID, const std::string& name, const int value,
                   const float weight, const std::string& Mesh);
      void addItem(const std::string& ID, const ItemRecord& record);
      bool deleteItem(const std::string& ID_of_item);
      void ClearAllItems();
      unsigned int NumberOfItems() const;
      std::string GetItemName(const std::string& itemID) const;
      int GetItemValue(const std::string& itemID) const;
      float GetItemWeight(const std::string& itemID) const;
      std::string GetMeshName(const std::string& itemID, const bool UseMarkerOnError=true) const;
      bool SaveToFile(const std::string& FileName) const;
      bool LoadFromFile(const std::string& FileName);
      bool SaveToStream(std::ofstream& Stream) const;
      bool LoadFromStream(std::ifstream& Stream);
      std::map<std::string, ItemRecord>::const_iterator GetFirst() const;
      std::map<std::string, ItemRecord>::const_iterator GetEnd() const;
    private:
      ItemBase();
      ItemBase(const ItemBase& op){}
      std::map<std::string, ItemRecord> m_ItemList;
  };//class

}//namespace

#endif // ITEMBASE_H
