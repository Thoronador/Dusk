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

#include "Player.h"
#include "API.h"
#ifndef DUSK_EDITOR
  #include "Camera.h"
#endif
#include <OgreSkeleton.h>

namespace Dusk
{

void listBoneChildren(const Ogre::Node* b, const unsigned int indents)
{
  unsigned int i;
  for (i=0; i<b->numChildren(); ++i)
  {
    Ogre::Node* cb = b->getChild(i);
    std::cout << std::string(indents, ' ') << cb->getName();
    std::cout << " (Children: " << cb->numChildren() << ")\n";
    if (cb->numChildren()>0)
    {
      listBoneChildren(cb, indents+2);
    }//if
  }//for
}

Player& Player::getSingleton()
{
  static Player Instance;
  return Instance;
}

Player::Player()
  : NPC("player", Ogre::Vector3(0.0, 0.0, 300), Ogre::Quaternion::IDENTITY, 1.0f)
{
  //not much done yet
  ID = "player";
}

Player::~Player()
{
  if (Ogre::Root::getSingletonPtr()!=NULL)
  {
    disable();
  }//if
  entity = NULL;
}

bool Player::pickUpNearest()
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL) return false;
  Ogre::SphereSceneQuery* sp_query =
      scm->createSphereQuery(Ogre::Sphere(position, cMaximumPickUpDistance));
  Ogre::SceneQueryResult& result = sp_query->execute();
  float distance = -1.0f;
  Item* item_ptr = NULL;
  Ogre::SceneQueryResultMovableList::iterator iter = result.movables.begin();
  while (iter!=result.movables.end())
  {
    Ogre::UserDefinedObject* udo = (*iter)->getUserObject();
    if (udo!=NULL)
    {
      DuskObject* obj_ptr = static_cast<DuskObject*>(udo);
      if (obj_ptr->canPickUp())
      {
        const float sd = position.squaredDistance(obj_ptr->getPosition());
        if ((sd<distance || distance<0.0f) and (obj_ptr->getDuskType()==otItem))
        {
          distance = sd;
          item_ptr = static_cast<Item*>(obj_ptr);
        }
      }
    }//if
    ++iter;
  }//while
  //clean up
  scm->destroyQuery(sp_query);
  sp_query = NULL;
  //check whether we found something
  if (item_ptr!=NULL)
  {
    return this->pickUp(item_ptr);
  }
  return false;
}

void Player::translate(const Ogre::Vector3& translationVector)
{
  m_Direction = m_Speed*m_Direction+translationVector;
  NPC::setSpeed(m_Direction.normalise());
}

void Player::injectTime(const float SecondsPassed)
{
  //call inherited method
  NPC::injectTime(SecondsPassed);
  #ifndef DUSK_EDITOR
  //adjust camera position, because camera follows player
  Camera::getSingleton().setPosition(Ogre::Vector3(position.x,
                            position.y+Camera::cAboveGroundLevel,
                            position.z));
  #endif
}

std::string Player::getObjectMesh() const
{
  /* Until we have a proper player mesh, we use the Ogre mascot mesh from
     Zi Ye / omniter. */
  return "Sinbad.mesh";
}

bool Player::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Player::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  if (!NPC::enable(scm)) return false;
  //just for information
  // -- animation states
  unsigned int i;
  const std::vector<std::string> as = getPossibleAnimationStates();
  for (i=0; i<as.size(); ++i)
  {
    std::cout << "Player animation state available: "<<as[i]<<"\n";
  }//for
  // -- bones of skeleton
  if ( entity->hasSkeleton())
  {
    Ogre::SkeletonInstance* skelInst = entity->getSkeleton();
    std::cout << "Player animation bones available: "<< skelInst->getNumBones()<< "\n";
    Ogre::Skeleton::BoneIterator rbIter = skelInst->getRootBoneIterator();
    while (rbIter.hasMoreElements())
    {
      Ogre::Bone* b = rbIter.getNext();
      std::cout << "  Bone name: " << b->getName() << "\n";
      std::cout << "    Children: " << b->numChildren() << "\n";
      listBoneChildren(b, 4);
    } //while
  }//if

  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //sheathed weapons
  Ogre::Entity* ent_sword = scm->createEntity(entity_name.str()+"_sword.Right", "Sword.mesh");
  entity->attachObjectToBone("Sheath.R", ent_sword);
  ent_sword = scm->createEntity(entity_name.str()+"_sword.Left", "Sword.mesh");
  entity->attachObjectToBone("Sheath.L", ent_sword);
  return (entity!=NULL);
}

bool Player::saveToStream(std::ofstream& OutStream) const
{
  //not implemented yet
  return false;
}

bool Player::loadFromStream(std::ifstream& InStream)
{
  //not implemented yet
  return false;
}

} //namespace
