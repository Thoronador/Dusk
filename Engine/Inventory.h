#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <map>

namespace Dusk
{

/* class Inventory
         Holds all the items (as defined by class ItemBase) of a NPC (or the
         player, in one case).
*/

class Inventory
{
  public:
    Inventory();
    ~Inventory();

    /* AddItem: adds count items of ID ItemID to inventory; always succeeds. */
    void AddItem(const std::string ItemID, const unsigned int count);

    /* RemoveItem: tries to remove count items of ID ItemID from inventory and
       returns number of items actually removed. */
    unsigned int RemoveItem(const std::string ItemID, const unsigned int count);

    /* GetItemCount: returns number of items with ID ItemID in inventory*/
    unsigned int GetItemCount(const std::string ItemID);

    /* GetTotalWeight: returns the sum of the weights of all items currently in
       the inventory. */
    float GetTotalWeight();
  private:
    std::map<std::string, unsigned int> m_Items;
    float m_TotalWeight;
};//class

}//namespace

#endif // INVENTORY_H
