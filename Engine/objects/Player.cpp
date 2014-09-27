/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012, 2013  thoronador

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

#include "Player.h"
#include "../API.h"
#include "../Messages.h"
#include "../DuskConstants.h"
#ifndef DUSK_EDITOR
  #include "../Camera.h"
#endif
#include <OgreSkeleton.h>
#include "AnyConversion.h"

namespace Dusk
{

void listBoneChildren(const Ogre::Node* b, const unsigned int indents)
{
  unsigned int i;
  for (i=0; i<b->numChildren(); ++i)
  {
    Ogre::Node* cb = b->getChild(i);
    DuskLog() << std::string(indents, ' ') << cb->getName();
    DuskLog() << " (Children: " << cb->numChildren() << ")\n";
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
    if (!((*iter)->getUserAny().isEmpty()))
    {
      DuskObject* obj_ptr = AnyToObjectPtr((*iter)->getUserAny());
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

const std::string& Player::getObjectMesh() const
{
  /* Until we have a proper player mesh, we use the Ogre mascot mesh from
     Zi Ye / omniter. */
  static const std::string playerMesh = "Sinbad.mesh";
  return playerMesh;
}

bool Player::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    DuskLog() << "Player::enable: ERROR: no scene manager present.\n";
    return false;
  }
  if (!NPC::enable(scm)) return false;
  //just for information
  // -- animation states
  unsigned int i;
  const std::vector<std::string> as = getPossibleAnimationStates();
  for (i=0; i<as.size(); ++i)
  {
    DuskLog() << "Info: Player animation state available: "<<as[i]<<"\n";
  }//for
  // -- bones of skeleton
  if (entity->hasSkeleton())
  {
    Ogre::SkeletonInstance* skelInst = entity->getSkeleton();
    DuskLog() << "Info: Player animation bones available: "<< skelInst->getNumBones()<< "\n";
    Ogre::Skeleton::BoneIterator rbIter = skelInst->getRootBoneIterator();
    while (rbIter.hasMoreElements())
    {
      Ogre::Bone* b = rbIter.getNext();
      DuskLog() << "  Bone name: " << b->getName() << "\n";
      DuskLog() << "    Children: " << b->numChildren() << "\n";
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
  return true;
}

bool Player::saveToStream(std::ofstream& OutStream) const
{
  /*This function is basically copied from NPC::saveToStream(). The only
    difference is the different header ("Play" instead of "RefN"). */
  if (!OutStream.good())
  {
    DuskLog() << "Player::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "Play" (Player)
  OutStream.write((const char*) &cHeaderPlay, sizeof(uint32_t));
  //save stuff inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Player::saveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!saveAnimatedObjectPart(OutStream))
  {
    DuskLog() << "Player::saveToStream: ERROR while saving animation data!\n";
    return false;
  }
  // go on with new data members from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    DuskLog() << "Player::saveToStream: ERROR while saving basic motion data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  if (!saveWaypointObjectPart(OutStream))
  {
    DuskLog() << "Player::saveToStream: ERROR while saving waypoint data!\n";
    return false;
  }
  //done with inherited data members; go on with NPC stuff
  if (!saveNPCPart(OutStream))
  {
    DuskLog() << "Player::saveToStream: ERROR while saving NPC data!\n";
    return false;
  }
  return OutStream.good();
}

bool Player::loadFromStream(std::ifstream& InStream)
{
  /*This function is basically copied from NPC::loadFromStream(). The only
    difference is the different header ("Play" instead of "RefN"). */
  if (!InStream.good())
  {
    DuskLog() << "Player::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "Play"
  uint32_t Header = 0;
  InStream.read((char*) &Header, sizeof(uint32_t));
  if (Header!=cHeaderPlay)
  {
    DuskLog() << "Player::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Player::loadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!loadAnimatedObjectPart(InStream))
  {
    DuskLog() << "Player::loadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    DuskLog() << "Player::loadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  // go on with data members from WaypointObject
  if (!loadWaypointObjectPart(InStream))
  {
    DuskLog() << "Player::loadFromStream: ERROR while loading waypoint data.\n";
    return false;
  }
  //done with inherited data, go on with NPC data
  if (!loadNPCPart(InStream))
  {
    DuskLog() << "Player::loadFromStream: ERROR while loading NPC data.\n";
    return false;
  }
  //if player was enabled during loading (which he is most likely), we will have
  // to re-enable the object for the changes to take effect
  if (entity!=NULL)
  {
    //get scene manager for later use
    Ogre::SceneManager* scm = entity->getParentSceneNode()->getCreator();
    if (scm==NULL)
    {
      DuskLog() << "Player::loadFromStream: ERROR: No scene manager found.\n";
      return false;
    }
    disable();
    if (!enable(scm))
    {
      DuskLog() << "Player::loadFromStream: ERROR: Could not re-enable object.\n";
      return false;
    }
  }
  return InStream.good();
}

} //namespace
