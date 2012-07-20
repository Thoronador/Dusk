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

#include "Inventory.h"
#include "database/Database.h"
#include "database/ItemRecord.h"
#include "database/WeaponRecord.h"
#include "DuskConstants.h"
#include "Messages.h"

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

const Inventory& Inventory::getEmptyInventory()
{
  static Inventory EmptyThing;
  EmptyThing.makeEmpty();
  return EmptyThing;
}

void Inventory::addItem(const std::string& ItemID, const unsigned int count)
{
  if (ItemID.empty() or count==0)
  {
    DuskLog() << "Inventory::addItem: ERROR: ItemID is emtpy string or amount "
              << "is zero!\n";
    return;
  }
  std::map<std::string, unsigned int>::iterator iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    //there are already items of given ID
    iter->second = iter->second + count;
  }
  else
  {
    m_Items[ItemID] = count;
  }
  //adjust weight of inventory according to added items
  if (Database::getSingleton().hasTypedRecord<ItemRecord>(ItemID))
  {
    m_TotalWeight = m_TotalWeight +count*Database::getSingleton().getTypedRecord<ItemRecord>(ItemID).weight;
  }
  else
  {
    //if there's no such item in item base, it must have been a weapon
    m_TotalWeight = m_TotalWeight +count*Database::getSingleton().getTypedRecord<WeaponRecord>(ItemID).weight;
  }
}

unsigned int Inventory::removeItem(const std::string& ItemID, const unsigned int count)
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
    //adjust weight of inventory according to removed items
    if (Database::getSingleton().hasTypedRecord<ItemRecord>(ItemID))
    {
      m_TotalWeight = m_TotalWeight - removed*Database::getSingleton().getTypedRecord<ItemRecord>(ItemID).weight;
    }
    else
    {
      //must have been a weapon instead
      m_TotalWeight = m_TotalWeight - removed*Database::getSingleton().getTypedRecord<WeaponRecord>(ItemID).weight;
    }
    return removed;
  }
  return 0;
}

unsigned int Inventory::getItemCount(const std::string& ItemID) const
{
  std::map<std::string, unsigned int>::const_iterator iter = m_Items.find(ItemID);
  if (iter!=m_Items.end())
  {
    return iter->second;
  }
  return 0;
}

bool Inventory::isEmpty() const
{
  return m_Items.empty();
}

void Inventory::makeEmpty()
{
  m_Items.clear();
  m_TotalWeight = 0.0f;
}

void Inventory::addAllItemsTo(Inventory& target) const
{
  std::map<std::string, unsigned int>::const_iterator iter = m_Items.begin();
  while (iter!=m_Items.end())
  {
    target.addItem(iter->first, iter->second);
    ++iter;
  }//while
}

float Inventory::getTotalWeight() const
{
  return m_TotalWeight;
}

int Inventory::getTotalValue() const
{
  int sum = 0;
  std::map<std::string, unsigned int>::const_iterator iter;
  iter = m_Items.begin();
  while (iter!=m_Items.end())
  {
    if (Database::getSingleton().hasTypedRecord<ItemRecord>(iter->first))
    { //it's an item
      sum = sum + iter->second * Database::getSingleton().getTypedRecord<ItemRecord>(iter->first).value;
    }
    else
    { //it's a weapon
      sum = sum + iter->second * Database::getSingleton().getTypedRecord<WeaponRecord>(iter->first).value;
    }
    ++iter;
  }//while
  return sum;
}

bool Inventory::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Inventory::saveToStream: ERROR: stream contains errors!\n";
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
    ++traverse;
  }//while
  return OutStream.good();
}

bool Inventory::loadFromStream(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "Inventory::loadFromStream: ERROR: stream contains errors!\n";
    return false;
  }
  unsigned int len=0, i, count=0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len!=cHeaderInve)
  {
    DuskLog() << "Inventory::loadFromStream: ERROR: stream contains unexpected header!\n";
    return false;
  }
  //read number of items
  InStream.read((char*) &count, sizeof(unsigned int));
  makeEmpty();
  char ID_Buffer[256];
  ID_Buffer[0] = ID_Buffer[255] = '\0';
  for (i=0; i<count; i++)
  { //read loop
    //ID
    len=0;
    InStream.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      DuskLog() << "Inventory::loadFromStream: ERROR: ID is longer than 255 characters!\n";
      return false;
    }
    InStream.read(ID_Buffer, len);
    if (!InStream.good())
    {
      DuskLog() << "Inventory::loadFromStream: ERROR while reading item ID!\n";
      return false;
    }
    ID_Buffer[len] = '\0';
    //amount
    InStream.read((char*) &len, sizeof(unsigned int));
    if (!InStream.good())
    {
      DuskLog() << "Inventory::loadFromStream: ERROR while reading item amount!\n";
      return false;
    }
    addItem(std::string(ID_Buffer), len);
  }//for
  return InStream.good();
}

ConstInventoryIterator Inventory::getFirst() const
{
  return m_Items.begin();
}

ConstInventoryIterator Inventory::getEnd() const
{
  return m_Items.end();
}

bool Inventory::operator==(const Inventory& other) const
{
  if (isEmpty() and other.isEmpty())
  { //both are empty, so they are the same
    return true;
  }
  if (isEmpty() or other.isEmpty())
  { //only one is empty, so they are not the same
    return false;
  }
  ConstInventoryIterator other_one = other.getFirst();
  const ConstInventoryIterator other_end = other.getEnd();
  ConstInventoryIterator self_one = m_Items.begin();
  const ConstInventoryIterator self_end = m_Items.end();

  while (self_one!=self_end and other_one!=other_end)
  {
    if (self_one->first!=other_one->first or self_one->second!=other_one->second)
    {
      return false;
    }
    ++self_one;
    ++other_one;
  }//while
  if ((self_one!=self_end) xor (other_one!=other_end))
  {
    return false;
  }
  return true;
}

}//namespace
