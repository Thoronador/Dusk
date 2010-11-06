/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "Item.h"
#include "DuskConstants.h"
#include "ItemBase.h"
#include "API.h"

namespace Dusk
{

Item::Item()
  : DuskObject()
{

}

Item::Item(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : DuskObject(_ID, pos, rot, Scale)
{

}

Item::~Item()
{
  disable();
}

std::string Item::getObjectMesh() const
{
  return ItemBase::getSingleton().getMeshName(ID);
}

bool Item::enableWithoutSceneNode(Ogre::SceneManager* scm)
{
  //We basically use the exact code from the DuskObject Enable() function here,
  // just with the difference of not creating a scene node for it.
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Item::EnableWithoutNode: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), getObjectMesh());
  /*Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));*/
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

bool Item::disable()
{
  if (entity == NULL)
  {
    return true;
  }
  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm;
  if (ent_node!=NULL)
  {
    scm = ent_node->getCreator();
  }
  else
  {
    scm = getAPI().getOgreSceneManager();
  }
  if (scm==NULL)
  {
    std::cout << "DuskObject::Disable: ERROR: got NULL for scene manager.\n";
    return false;
  }
  if (ent_node!=NULL)
  {
    ent_node->detachObject(entity);
    scm->getRootSceneNode()->removeAndDestroyChild(ent_node->getName());
  }
  scm->destroyEntity(entity);
  entity = NULL;
  return true;
}

ObjectTypes Item::getDuskType() const
{
  return otItem;
}

bool Item::canPickUp() const
{
  return !m_Equipped;
}

bool Item::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Item::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefI" (reference of Item)
  OutStream.write((char*) &cHeaderRefI, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    std::cout << "Item::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data from Item
  if (!saveItemPart(OutStream))
  {
    std::cout << "Item::SaveToStream: ERROR while writing item data!\n";
    return false;
  }
  return true;
}

bool Item::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Item::LoadFromStream: ERROR: Cannot load from stream while "
              << "item is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Item::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefI"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefI)
  {
    std::cout << "Item::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    std::cout << "Item::LoadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff from Item
  if (!loadItemPart(InStream))
  {
    std::cout << "Item::LoadFromStream: ERROR while reading item data.\n";
    return false;
  }//if
  return true;
}

bool Item::saveItemPart(std::ofstream& output) const
{
  // -- equipped
  output.write((char*) &m_Equipped, sizeof(bool));
  return output.good();
}

bool Item::loadItemPart(std::ifstream& input)
{
  // -- equipped
  input.read((char*) &m_Equipped, sizeof(bool));
  return input.good();
}

bool Item::isEquipped() const
{
  return m_Equipped;
}

void Item::setEquipped(const bool value)
{
  m_Equipped = value;
}

Ogre::Entity* Item::exposeEntity() const
{
  return entity;
}

} //namespace
