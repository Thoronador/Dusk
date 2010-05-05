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

 ToDo list:
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

    /* AddItem: adds count items of ID ItemID to inventory; always succeeds. */
    void AddItem(const std::string& ItemID, const unsigned int count);

    /* RemoveItem: tries to remove count items of ID ItemID from inventory and
       returns number of items actually removed. If Inventory has count or more
       items of ID ItemID, then it always removes count items. If it has less
       than count items of ID ItemID, then it removes the amount of items it
       has when the function was called. */
    unsigned int RemoveItem(const std::string& ItemID, const unsigned int count);

    /* GetItemCount: returns number of items with ID ItemID in inventory*/
    unsigned int GetItemCount(const std::string& ItemID) const;

    /* Checks whether inventory is empty (true) or not (false) */
    bool IsEmpty() const;

    /* Clears all items in inventory */
    void MakeEmpty();

    /* Adds all items from this Inventory to the target Inventory. */
    void AddAllItemsTo(Inventory& target) const;

    /* GetTotalWeight: returns the sum of the weights of all items currently in
       the inventory. */
    float GetTotalWeight() const;

    /* GetTotalValue: returns the sum of the values of all items currently in
       the inventory. */
    int GetTotalValue() const;

    /* utility function to easily get access to an empty Inventory */
    static const Inventory& GetEmptyInventory();

    /* saves contents of inventory to given stream and returns true on success,
       false otherwise */
    virtual bool SaveToStream(std::ofstream& OutStream) const;

    /* Loads contents of inventory from stream and returns true on success,
       false otherwise. The Inventory content is probably inconsistent after
       that function failed, so don't rely on its contents in that case. */
    virtual bool LoadFromStream(std::ifstream& InStream);

    std::map<std::string, unsigned int>::const_iterator GetFirst() const;
    std::map<std::string, unsigned int>::const_iterator GetEnd() const;

    /* comparison operator for Inventory - returns true, if two Inventorys
       contain the same items (ID) and same amount of them  */
    bool operator==(const Inventory& other) const;
  private:
    std::map<std::string, unsigned int> m_Items;
    float m_TotalWeight;
};//class

}//namespace

#endif // INVENTORY_H
