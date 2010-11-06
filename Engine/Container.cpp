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

#include "Container.h"
#include "ContainerBase.h"
#include "DuskConstants.h"

namespace Dusk
{

Container::Container()
    : DuskObject()
{
  m_Changed = false;
  m_Contents.makeEmpty();
}

Container::Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
    : DuskObject(_ID, pos, rot, Scale)
{
  m_Changed = false;
  m_Contents.makeEmpty();
  ContainerBase::getSingleton().getContainerInventory(_ID).addAllItemsTo(m_Contents);
}

Container::~Container()
{
  m_Contents.makeEmpty();
  disable();
}

bool Container::isEmpty() const
{
  return m_Contents.isEmpty();
}

void Container::transferAllItemsTo(Inventory& target)
{
  m_Contents.addAllItemsTo(target);
  m_Contents.makeEmpty();
  m_Changed = true;
}

void Container::addItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return;
  }
  m_Contents.addItem(ItemID, count);
  m_Changed = true;
}

unsigned int Container::removeItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return 0;
  }
  m_Changed = true;
  return m_Contents.removeItem(ItemID, count);
}

unsigned int Container::getItemCount(const std::string& ItemID) const
{
  return m_Contents.getItemCount(ItemID);
}

std::string Container::getObjectMesh() const
{
  return ContainerBase::getSingleton().getContainerMesh(ID);
}

ObjectTypes Container::getDuskType() const
{
  return otContainer;
}

bool Container::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Container::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefC" (reference of Container)
  OutStream.write((char*) &cHeaderRefC, sizeof(unsigned int)); //header
  //write data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    std::cout << "Container::SaveToStream: ERROR while writing basic data!\n";
    return false;
  }
  //write inventory
  // -- flags
  OutStream.write((char*) &m_Changed, sizeof(bool));
  // -- inventory (only if neccessary)
  if (m_Changed)
  {
    if (!m_Contents.saveToStream(OutStream))
    {
      std::cout << "Container::SaveToStream: ERROR while writing inventory data!\n";
      return false;
    }//if
  }
  return (OutStream.good());
}

bool Container::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Container::LoadFromStream: ERROR: Cannot load from stream "
              << "while container is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Container::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  //read header "RefC"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefC)
  {
    std::cout << "Container::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //load data members inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    std::cout << "Container::LoadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  //Container's own stuff
  //load inventory
  // -- flags
  InStream.read((char*) &m_Changed, sizeof(bool));
  if (m_Changed)
  { //load it from stream, contents were changed
    if (!m_Contents.loadFromStream(InStream))
    {
      std::cout << "Container::LoadFromStream: ERROR while reading container "
                << "inventory from stream.\n";
      return false;
    }
  }
  else
  { //inventory was not changed, so get it from ContainerBase
    m_Contents.makeEmpty();
    ContainerBase::getSingleton().getContainerInventory(ID).addAllItemsTo(m_Contents);
  }
  return (InStream.good());
}

}//namespace
