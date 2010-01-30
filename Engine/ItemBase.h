/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-30
 Purpose: ItemBase Singleton class
          holds information about all distinct items in the game

 History:
     - 2009-07-02 (rev 102) - initial version (by thoronador)
     - 2009-07-06 (rev 103) - possibility to work with error mesh added
     - 2009-07-13 (rev 104) - SaveToStream() added, integration into DataLoader
     - 2009-07-15 (rev 105) - LoadFromStream() added
     - 2009-07-24 (rev 111) - GetFirst() and GetEnd() added
     - 2009-07-31 (rev 113) - deleteItem() added
     - 2009-09-13 (rev 128) - }
     - 2009-09-24 (rev 131) - } small fixes
     - 2009-09-27 (rev 132) - }
     - 2010-01-30 (rev 161) - obsolete load/save functions removed
                            - documentation updated

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

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
      /* destructor */
      virtual ~ItemBase();

      /* singleton access */
      static ItemBase& GetSingleton();

      /* returns true, if information about item with given ID is present */
      bool hasItem(const std::string& ID_of_item) const;

      /* adds a new item with given ID, name, value, weight and mesh path

         remarks:
             If one of ID, name and Mesh is an empty string, or if value or
             weight is below zero, nothing will be added.
             If an item with the given ID already exists, then it will be
             replaced by the new one.
      */
      void addItem(const std::string& ID, const std::string& name, const int value,
                   const float weight, const std::string& Mesh);

      /* utility variant of previous function; see remarks of addItem() for more
         information
      */
      void addItem(const std::string& ID, const ItemRecord& record);

      /* tries to delete the item with the given ID and returns true, if such
         an item existed before deleting it. If no such item was present, the
         funtion will return false.
      */
      bool deleteItem(const std::string& ID_of_item);

      /* deletes information of ALL items - use with caution (or not at all) */
      void ClearAllItems();

      /* returns the number of distinct items which are currently present */
      unsigned int NumberOfItems() const;

      /* returns the name of the given item, or an empty string if no such item
         is present
      */
      std::string GetItemName(const std::string& itemID) const;

      /* returns the value of the item with ID itemID, or -1 if no item with
         that ID is present.
      */
      int GetItemValue(const std::string& itemID) const;

      /* returns the weight of the item with ID itemID, or zero if no item with
         that ID is present.

         remarks:
             Since zero is also a valid weight value for items, a return value
             of zero is NOT a safe way to detect absence of a certain item.
      */
      float GetItemWeight(const std::string& itemID) const;

      /* returns the mesh path of the given item, or an empty string, if no
          item with that ID is present. However, if UseMarkerOnError is set to
          true (default), this function will return the path to a predefined
          error mesh instead.
      */
      std::string GetMeshName(const std::string& itemID, const bool UseMarkerOnError=true) const;

      /* tries to save all items to the stream and returns true on success */
      bool SaveToStream(std::ofstream& Stream) const;

      /* tries to read the next item from stream and returns true, if successful */
      bool LoadFromStream(std::ifstream& Stream);

      /* helper functions to access internal map iterators - not used in-game,
         only used by Editor application.
      */
      std::map<std::string, ItemRecord>::const_iterator GetFirst() const;
      std::map<std::string, ItemRecord>::const_iterator GetEnd() const;
    private:
      /* constructor */
      ItemBase();

      /* empty copy constructor - we follow the singleton passtern */
      ItemBase(const ItemBase& op){}
      std::map<std::string, ItemRecord> m_ItemList;
  };//class

}//namespace

#endif // ITEMBASE_H
