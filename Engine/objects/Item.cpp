/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2012, 2013  thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Item.h"
#include "../database/Database.h"
#include "../DuskConstants.h"
#include "../database/ItemRecord.h"
#include "../API.h"
#include "../Messages.h"

namespace Dusk
{

Item::Item()
  : DuskObject(),
    m_Equipped(false)
{
}

Item::Item(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
  : DuskObject(_ID, pos, rot, Scale),
    m_Equipped(false)
{
}

Item::~Item()
{
  disable();
}

const std::string& Item::getObjectMesh() const
{
  return Database::getSingleton().getTypedRecord<ItemRecord>(ID).Mesh;
}

bool Item::enableWithoutSceneNode(Ogre::SceneManager* scm)
{
  //We basically use the exact code from the DuskObject enable() function here,
  // just with the difference of not creating a scene node for it.
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    DuskLog() << "Item::enableWithoutNode: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), getObjectMesh());
  /*Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale); */
  //set user defined object to this object as reverse link
  entity->setUserAny(Ogre::Any(this));
  return true;
}

bool Item::disable()
{
  if (entity == NULL)
  {
    return true;
  }
  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm = NULL;
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
    DuskLog() << "DuskObject::disable: ERROR: got NULL for scene manager.\n";
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

bool Item::canCollide() const
{
  return true;
}

bool Item::canPickUp() const
{
  return !m_Equipped;
}

bool Item::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Item::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefI" (reference of Item)
  OutStream.write((char*) &cHeaderRefI, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Item::saveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data from Item
  if (!saveItemPart(OutStream))
  {
    DuskLog() << "Item::saveToStream: ERROR while writing item data!\n";
    return false;
  }
  return true;
}

bool Item::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Item::loadFromStream: ERROR: Cannot load from stream while "
              << "item is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Item::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefI"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefI)
  {
    DuskLog() << "Item::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Item::loadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff from Item
  if (!loadItemPart(InStream))
  {
    DuskLog() << "Item::loadFromStream: ERROR while reading item data.\n";
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
