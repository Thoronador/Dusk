#include "Inventory.h"
#include "ItemBase.h"

namespace Dusk
{

Inventory::Inventory()
{
  //initial value
  m_TotalWeight = 0.0;
  m_Items.clear();
}

Inventory::~Inventory()
{
  m_Items.clear();
  m_TotalWeight = 0.0;
}

void Inventory::AddItem(const std::string ItemID, const unsigned int count)
{
  std::map<std::string, unsigned int>::iterator iter;
  iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    //there are already items of given ID
    iter->second = iter->second + count;
  }
  else
  {
    m_Items[ItemID] = count;
  }
  m_TotalWeight = m_TotalWeight +count*ItemBase::GetSingleton().GetItemWeight(ItemID);
}

unsigned int Inventory::RemoveItem(const std::string ItemID, const unsigned int count)
{
  std::map<std::string, unsigned int>::iterator iter;
  iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    //there are items of given ID
    unsigned int removed;
    if (count < iter->second)
    {
      iter->second = iter->second - count;
      removed = count;
    }
    else
    {
      removed = iter->second;
      iter->second = 0;
      m_Items.erase(iter);
    }
    m_TotalWeight = m_TotalWeight - removed*ItemBase::GetSingleton().GetItemWeight(ItemID);
    return removed;
  }
  return 0;
}

unsigned int Inventory::GetItemCount(const std::string ItemID)
{
  std::map<std::string, unsigned int>::iterator iter;
  iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    return iter->second;
  }
  return 0;
}

float Inventory::GetTotalWeight()
{
  return m_TotalWeight;
}

}//namespace
