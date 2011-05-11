/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

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
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - iterator type for getFirst() and getEnd() added

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
  //structure that can hold the data of one item
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
      #ifdef DUSK_EDITOR
      //iterator type for iterating through the items
      typedef std::map<std::string, ItemRecord>::const_iterator Iterator;
      #endif

      /* destructor */
      virtual ~ItemBase();

      /* singleton access */
      static ItemBase& getSingleton();

      /* returns true, if information about item with given ID is present */
      bool hasItem(const std::string& ID_of_item) const;

      /* adds a new item with given ID, name, value, weight and mesh path

         parameters:
             ID     - ID of the new item
             name   - name of the item (shown in game)
             value  - value of the item
             weight - weight of the item
             Mesh   - mesh path of the item

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

         parameters:
             ID     - ID of the new item
             record - structure that holds all data for the item
      */
      void addItem(const std::string& ID, const ItemRecord& record);

      /* tries to delete the item with the given ID and returns true, if such
         an item existed before deleting it. If no such item was present, the
         funtion will return false.

         parameters:
             ID_of_item - ID of the item that shall be deleted
      */
      bool deleteItem(const std::string& ID_of_item);

      /* deletes information of ALL items - use with caution (or not at all) */
      void clearAllItems();

      /* returns the number of distinct items which are currently present */
      unsigned int getNumberOfItems() const;

      /* returns the name of the given item, or an empty string if no such item
         is present

         parameters:
             itemID - ID of the item whose (ingame) name is requested
      */
      std::string getItemName(const std::string& itemID) const;

      /* returns the value of the item with ID itemID, or -1 if no item with
         that ID is present.

         parameters:
             itemID - ID of the item whose value is requested
      */
      int getItemValue(const std::string& itemID) const;

      /* returns the weight of the item with ID itemID, or zero if no item with
         that ID is present.

         parameters:
             itemID - ID of the item whose weight is requested

         remarks:
             Since zero is also a valid weight value for items, a return value
             of zero is NOT a safe way to detect absence of a certain item.
      */
      float getItemWeight(const std::string& itemID) const;

      /* returns the mesh path of the given item, or an empty string, if no
          item with that ID is present. However, if UseMarkerOnError is set to
          true (default), this function will return the path to a predefined
          error mesh instead.

          parameters:
             itemID - ID of the item whose mesh path is requested
      */
      std::string getMeshName(const std::string& itemID, const bool UseMarkerOnError=true) const;

      /* tries to save all items to the stream and returns true on success

         parameters:
             Stream - output stream that is used to save all items
      */
      bool saveToStream(std::ofstream& Stream) const;

      /* tries to read the next item from stream and returns true, if successful

         parameters:
             Stream - input stream that is used to load the next item
      */
      bool loadFromStream(std::ifstream& Stream);

      #ifdef DUSK_EDITOR
      /* helper functions to access internal map iterators - not used in-game,
         only used by Editor application.
      */
      Iterator getFirst() const;
      Iterator getEnd() const;
      #endif
    private:
      /* constructor */
      ItemBase();

      /* empty copy constructor - we follow the singleton passtern */
      ItemBase(const ItemBase& op){}

      //item list
      std::map<std::string, ItemRecord> m_ItemList;
  };//class

}//namespace

#endif // ITEMBASE_H
