/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012  thoronador

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
 Date:    2010-05-05
 Purpose: Inventory class
          This class represents the collection of items a player or NPC can
          carry.

 History:
     - 2009-07-17 (rev 102) - initial version (by thoronador)
     - 2009-09-24 (rev 131) - IsEmpty() and MakeEmpty() added
                            - function to add all items of 'this' to another
                              inventory added, AddAllTo()
                            - GetTotalValue() added
                            - utility function GetEmptyInventory() added
                            - GetFirst(), GetEnd() and equality operator
     - 2009-09-27 (rev 132) - LoadFromStream() and SaveToStream() added
     - 2009-12-18 (rev 142) - } small improvements in declarations
     - 2010-02-08 (rev 169) - } small improvements in declarations
     - 2010-05-05 (rev 196) - documentation update
     - 2010-06-02 (rev 213) - update to work with Weapons, too
     - 2010-08-03 (rev 221) - minor improvement
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-21 (rev 257) - minor optimization
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2012-07-02 (rev 310) - update to use Database instead of LightBase

 ToDo list:
     - Probably we should change the inventory class from a map of strings and
       integers to a map of strings an Item pointers, combined with integers.
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <map>
#include <fstream>

namespace Dusk
{

//iterator for items in inventory
typedef std::map<std::string, unsigned int>::const_iterator ConstInventoryIterator;

/* class Inventory
         Holds all the items (as defined by class ItemBase) of a NPC (or the
         player, in one case).
*/

class Inventory
{
  public:
    /* constructor */
    Inventory();

    /* destructor */
    virtual ~Inventory();

    /* addItem: adds count items of ID ItemID to inventory; always succeeds.

       parameters:
           ItemID - ID of the item(s) that will be added
           count  - number of items that will be added
    */
    void addItem(const std::string& ItemID, const unsigned int count);

    /* RemoveItem: tries to remove count items of ID ItemID from inventory and
       returns number of items actually removed. If Inventory has count or more
       items of ID ItemID, then it always removes count items. If it has less
       than count items of ID ItemID, then it removes the amount of items it
       has when the function was called.

       parameters:
           ItemID - ID of the item(s) that will be removed
           count  - number of items that shall be removed
    */
    unsigned int removeItem(const std::string& ItemID, const unsigned int count);

    /* GetItemCount: returns number of items with ID ItemID in inventory

       parameters:
           ItemID - ID of the item whose amout shall be returned
    */
    unsigned int getItemCount(const std::string& ItemID) const;

    /* Checks whether inventory is empty (true) or not (false) */
    bool isEmpty() const;

    /* Clears all items in inventory */
    void makeEmpty();

    /* Adds all items from this Inventory to the target Inventory.

       parameters:
           target - the inventory that will get the same items
    */
    void addAllItemsTo(Inventory& target) const;

    /* GetTotalWeight: returns the sum of the weights of all items currently in
       the inventory. */
    float getTotalWeight() const;

    /* GetTotalValue: returns the sum of the values of all items currently in
       the inventory. */
    int getTotalValue() const;

    /* utility function to easily get access to an empty Inventory */
    static const Inventory& getEmptyInventory();

    /* saves contents of inventory to given stream and returns true on success,
       false otherwise

       parameters:
           OutStream - the output stream that will be used to save the inventory
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Loads contents of inventory from stream and returns true on success,
       false otherwise. The Inventory content is probably inconsistent after
       that function failed, so don't rely on its contents in that case.

       parameters:
           InStream - the input stream that will be used to load the inventory
    */
    virtual bool loadFromStream(std::ifstream& InStream);

    /* utility function to get iterator to the beginning of the internal map */
    ConstInventoryIterator getFirst() const;

    /* utility function to get iterator to the end of the internal map */
    ConstInventoryIterator getEnd() const;

    /* comparison operator for Inventory - returns true, if two Inventorys
       contain the same items (ID) and same amount of them

       parameters:
           other - the other inventory that will be compared to this one
    */
    bool operator==(const Inventory& other) const;
  private:
    std::map<std::string, unsigned int> m_Items;
    float m_TotalWeight;
};//class Inventory

}//namespace

#endif // INVENTORY_H
