#include "Inventory.h"
#include "ItemBase.h"
#include "DuskConstants.h"
#include <iostream>

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

const Inventory& Inventory::GetEmptyInventory()
{
  static Inventory EmptyThing;
  EmptyThing.MakeEmpty();
  return EmptyThing;
}

void Inventory::AddItem(const std::string& ItemID, const unsigned int count)
{
  if (ItemID=="" or count==0)
  {
    std::cout << "Inventory::AddItem: ERROR: ItemID is emtpy string or amount "
              << "is zero!\n";
    return;
  }
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

unsigned int Inventory::RemoveItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0)
  {
    return 0;
  }
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

unsigned int Inventory::GetItemCount(const std::string& ItemID) const
{
  std::map<std::string, unsigned int>::const_iterator iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    return iter->second;
  }
  return 0;
}

bool Inventory::IsEmpty() const
{
  return m_Items.empty();
}

void Inventory::MakeEmpty()
{
  m_Items.clear();
  m_TotalWeight = 0.0f;
}

void Inventory::AddAllItemsTo(Inventory& target) const
{
  std::map<std::string, unsigned int>::const_iterator iter = m_Items.begin();
  while (iter!=m_Items.end())
  {
    target.AddItem(iter->first, iter->second);
    iter++;
  }//while
}

float Inventory::GetTotalWeight() const
{
  return m_TotalWeight;
}

int Inventory::GetTotalValue() const
{
  int sum = 0;
  std::map<std::string, unsigned int>::const_iterator iter;
  iter = m_Items.begin();
  while (iter!=m_Items.end())
  {
    sum = sum + iter->second * ItemBase::GetSingleton().GetItemValue(iter->first);
    iter++;
  }//while
  return sum;
}

bool Inventory::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Inventory::SaveToStream: ERROR: stream contains errors!\n";
    return false;
  }
  OutStream.write((char*) &cHeaderInve, sizeof(unsigned int));
  //write number of items
  unsigned int len = m_Items.size();
  OutStream.write((char*) &len, sizeof(unsigned int));
  //write items (ID and amount)
  std::map<std::string, unsigned int>::const_iterator traverse;
  traverse = m_Items.begin();
  while (traverse != m_Items.end())
  {
    //ID
    len = traverse->first.length();
    OutStream.write((char*) &len, sizeof(unsigned int));
    OutStream.write(traverse->first.c_str(), len);
    //amount
    OutStream.write((char*) &(traverse->second), sizeof(unsigned int));
    traverse++;
  }//while
  return OutStream.good();
}

bool Inventory::LoadFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    std::cout << "Inventory::LoadFromStream: ERROR: stream contains errors!\n";
    return false;
  }
  unsigned int len=0, i, count=0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len!=cHeaderInve)
  {
    std::cout << "Inventory::LoadFromStream: ERROR: stream contains unexpected header!\n";
    return false;
  }
  //read number of items
  InStream.read((char*) &count, sizeof(unsigned int));
  MakeEmpty();
  char ID_Buffer[256];
  ID_Buffer[0] = ID_Buffer[255] = '\0';
  for (i=0; i<count; i++)
  { //read loop
    //ID
    len=0;
    InStream.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      std::cout << "Inventory::LoadFromStream: ERROR: ID is longer than 255 characters!\n";
      return false;
    }
    InStream.read(ID_Buffer, len);
    if (!InStream.good())
    {
      std::cout << "Inventory::LoadFromStream: ERROR while reading item ID!\n";
      return false;
    }
    ID_Buffer[len] = '\0';
    //amount
    InStream.read((char*) &len, sizeof(unsigned int));
    if (!InStream.good())
    {
      std::cout << "Inventory::LoadFromStream: ERROR while reading item amount!\n";
      return false;
    }
    AddItem(std::string(ID_Buffer), len);
  }//for
  return InStream.good();
}

std::map<std::string, unsigned int>::const_iterator Inventory::GetFirst() const
{
  return m_Items.begin();
}

std::map<std::string, unsigned int>::const_iterator Inventory::GetEnd() const
{
  return m_Items.end();
}

bool Inventory::operator==(const Inventory& other) const
{
  if (IsEmpty() and other.IsEmpty())
  { //both are empty, so they are the same
    return true;
  }
  if (IsEmpty() or other.IsEmpty())
  { //only one is empty, so they are not the same
    return false;
  }
  std::map<std::string, unsigned int>::const_iterator other_one = other.GetFirst();
  std::map<std::string, unsigned int>::const_iterator other_end = other.GetEnd();
  std::map<std::string, unsigned int>::const_iterator self_one = m_Items.begin();
  std::map<std::string, unsigned int>::const_iterator self_end = m_Items.end();

  while (self_one!=self_end and other_one!=other_end)
  {
    if (self_one->first!=other_one->first or self_one->second!=other_one->second)
    {
      return false;
    }
    self_one++;
    other_one++;
  }//while
  if ((self_one!=self_end) xor (other_one!=other_end))
  {
    return false;
  }
  return true;
}

}//namespace
