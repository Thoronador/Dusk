/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012  thoronador

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

#include "NPC.h"
#include <sstream>
#include "NPCBase.h"
#include "ItemBase.h"
#include "WeaponBase.h"
#include "Settings.h"
#include "DuskConstants.h"
#include "DuskFunctions.h"
#include "ObjectManager.h"
#include "InjectionManager.h"
#include "API.h"
#include "DiceBox.h"
#include "Landscape.h"
#include "Vehicle.h"
#include "Messages.h"

namespace Dusk
{

const float NPC::cMaximumPickUpDistance=100; //it's a guess; adjust it, if needed
const unsigned char NPC::Flag_DoesAttack = 1;
const unsigned char NPC::Flag_CanRightAttack = 2;
const unsigned char NPC::Flag_CanLeftAttack = 4;

NPC::NPC()
  : AnimatedObject("", Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, 1.0f)
{
  //assume some random data
  //all attributes to one
  m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
  m_Level = 1; //assume level 1
  m_Inventory.makeEmpty(); //empty inventory
  m_Health = getMaxHealth();
  m_EquippedLeft = NULL;
  m_EquippedRight = NULL;
  m_TimeToNextAttackLeft = m_TimeToNextAttackRight = 0.0f;
  m_AttackFlags = 0;
  m_JumpVelocity = 0.0f;
  m_Jump = false;
  m_Vehicle = NULL;
}

NPC::NPC(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
 : AnimatedObject(_ID, pos, rot, Scale)
{
  if (NPCBase::getSingleton().hasNPC(ID))
  {
    //feed the data from NPCBase
    //-- attributes
    const NPCAttributes temp_attr = NPCBase::getSingleton().getAttributes(ID);
    m_Strength = temp_attr.Str;
    m_Agility = temp_attr.Agi;
    m_Vitality = temp_attr.Vit;
    m_Intelligence = temp_attr.Int;
    m_Willpower = temp_attr.Will;
    m_Charisma = temp_attr.Cha;
    m_Luck = temp_attr.Luck;
    //-- level
    m_Level = NPCBase::getSingleton().getLevel(ID);
    m_Inventory = NPCBase::getSingleton().getNPCInventory(ID);
  }
  else
  { //assume some random data
    //all attributes to one
    m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
    m_Level = 1; //assume level 1
    m_Inventory.makeEmpty(); //empty inventory
  }
  m_Health = getMaxHealth();
  m_EquippedLeft = NULL;
  m_EquippedRight = NULL;
  m_TimeToNextAttackLeft = m_TimeToNextAttackRight = 0.0f;
  m_AttackFlags = 0;
  m_JumpVelocity = 0.0f;
  m_Jump = false;
  m_Vehicle = NULL;
}

NPC::~NPC()
{
  //empty
  this->disable();
}

ObjectTypes NPC::getDuskType() const
{
  return otNPC;
}

bool NPC::canCollide() const
{
  return true;
}

bool NPC::isHitByRay(const Ogre::Ray& ray, Ogre::Vector3& impact) const
{
  //just make sure we use the right one here
  return AnimatedObject::isHitByRay(ray, impact);
}

void NPC::jump(void)
{
  //only set values if we don't jump yet
  if (!m_Jump)
  {
    m_Jump = true;
    m_JumpVelocity = 30.0f; //only a guess; maybe we should adjust that
                            //  value later
    startJumpAnimation();
  }//if
}

Vehicle* NPC::getVehicle() const
{
  return m_Vehicle;
}

void NPC::setVehicle(Vehicle* new_vehicle)
{
  m_Vehicle = new_vehicle;
}

void NPC::move(const float SecondsPassed)
{
  /*basically we use the movement function of WaypointObject (i.e.
    WaypointObject::injectTime()) here, with some modifications for jumoing */
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  WaypointObject::injectTime(SecondsPassed);
  //now check for height
  const float land_height = Landscape::getSingleton().getHeightAtPosition(position.x, position.z)
                                    /*+cAboveGroundLevel*/;
  //check for static objects below entity and above landscape
  Ogre::SceneManager* scm = entity->getParentSceneNode()->getCreator();
  /*Add 15% of NPC's height to current position for ray scene query to allow NPC
    to step onto smaller, not too high objects. */
  const Ogre::Ray ray = Ogre::Ray(position+Ogre::Vector3(0.0,
          entity->getBoundingBox().getSize().y*0.15, 0.0),
          Ogre::Vector3(0.0, -1.0, 0.0)); //straight down
  Ogre::RaySceneQuery* rsq = scm->createRayQuery(ray);
  rsq->setSortByDistance(true);
  Ogre::RaySceneQueryResult& result = rsq->execute();
  Ogre::Real hit_level = land_height;
  const Ogre::Real max_distance = ray.getOrigin().y-land_height;
  unsigned int i;
  for (i=0; i<result.size(); ++i)
  {
    //If distance is greater the distance to the ground (landscape),
    // then quit here.
    if (result.at(i).distance>max_distance) break;
    if (result.at(i).movable!=NULL and result.at(i).movable!=entity)
    {
      //No need to check for landscape here, that has been handled by Landscape's
      // getHeightAtPosition() already.
      DuskObject* obj = static_cast<DuskObject*>(result.at(i).movable->getUserObject());
      if (obj!=NULL and obj!=this and obj->canCollide()
         and ((obj->getDuskType()!=otWeapon and obj->getDuskType()!=otItem)
              or !static_cast<Item*>(obj)->isEquipped())
         )
      {
        Ogre::Vector3 vec_i(0.0, 0.0, 0.0);
        //Is object really hit by this ray?
        if (obj->isHitByRay(ray, vec_i))
        {
          //Is it the highest value so far?
          if (vec_i.y>hit_level)
          {
            hit_level = vec_i.y; //set new highest y-value
          }//if highest
        }//if object is hit by ray
      }//if object not NULL and eligible for collision
    }//if movable not NULL
  }//for i

  //now delete the scene query
  scm->destroyQuery(rsq);
  rsq = NULL;

  //adjust position
  if (m_Jump)
  {
    const float jump_height = position.y+m_JumpVelocity*SecondsPassed;
    if (jump_height>=hit_level)
    {
      const float gravity = -9.81*2.25; //maybe we need to adjust this later
      position = Ogre::Vector3(position.x, jump_height, position.z);
      m_JumpVelocity = m_JumpVelocity + gravity*SecondsPassed;
    }
    else
    {
      m_Jump = false;
      position = Ogre::Vector3(position.x, hit_level, position.z);
      stopJumpAnimation();
    }
  }//if jumping
  else
  {
    position = Ogre::Vector3(position.x, hit_level, position.z);
  }
  //adjust position of scene node/ entity in Ogre
  if (isEnabled())
  {
    setPosition(position);
  }
}

void NPC::injectTime(const float SecondsPassed)
{
  AnimatedObject::injectTime(SecondsPassed);
  //WaypointObject::injectTime(SecondsPassed);
  NPC::move(SecondsPassed);
  if (doesAttack())
  {
    if (canAttackLeft())
    {
      m_TimeToNextAttackLeft = m_TimeToNextAttackLeft - SecondsPassed;
      if (m_TimeToNextAttackLeft<=0.0f)
      {
        //time for next attack has come
        if (m_EquippedLeft!=NULL)
        {
          if (m_EquippedLeft->getDuskType()==otWeapon)
          {
            performAttack(stLeftHand);
            //update time
            m_TimeToNextAttackLeft = m_TimeToNextAttackLeft
                +WeaponBase::getSingleton().getWeaponData(m_EquippedLeft->getID()).TimeBetweenAttacks;
          }
        }//if
      }//time less/equal zero
    }// can attack left
    if (canAttackRight())
    {
      m_TimeToNextAttackRight = m_TimeToNextAttackRight - SecondsPassed;
      if (m_TimeToNextAttackRight<=0.0f)
      {
        //time for next attack has come
        if (m_EquippedRight!=NULL)
        {
          if (m_EquippedRight->getDuskType()==otWeapon)
          {
            performAttack(stRightHand);
            //update time
            m_TimeToNextAttackRight = m_TimeToNextAttackRight
                +WeaponBase::getSingleton().getWeaponData(m_EquippedRight->getID()).TimeBetweenAttacks;
          }
        }//if
      }//time less/equal zero
    }// can attack right hand
  }//if NPC does attack
}

void NPC::setSpeed(const float v)
{
  UniformMotionObject::setSpeed(v);
  if (isMoving())
  {
    stopIdleAnimation();
    startWalkAnimation();
  }
  else
  {
    stopWalkAnimation();
    startIdleAnimation();
  }
}

float NPC::getHealth() const
{
  return m_Health;
}

void NPC::setHealth(const float new_health)
{
  if (new_health<0.0f)
  {
    if (m_Health>0.0f)
    { //if we are alive (yet), play the animation for death
      stopAttack();
      playDeathAnimation();
    }
    m_Health = 0.0f;
  }
  else
  {
    m_Health = new_health;
  }
}

void NPC::inflictDamage(const float damage_amount)
{
  /*after amour has been added, this function should also consider the equipped
    armour to lower the inflicted damage */

  /* We only need to inflict (further) damage, if the NPC is still alive. */
  if (m_Health>0.0f)
  {
    m_Health = m_Health - damage_amount;
    if (m_Health<=0.0f)
    {
      m_Health = 0.0f;
      stopAttack();
      playDeathAnimation();
    }
  }
}

bool NPC::isAlive() const
{
  return (m_Health>0.0f);
}

uint8 NPC::getLevel() const
{
  return m_Level;
}

uint8 NPC::getStrength() const
{
  return m_Strength;
}

uint8 NPC::getAgility() const
{
  return m_Strength;
}

uint8 NPC::getVitality() const
{
  return m_Vitality;
}

uint8 NPC::getIntelligence() const
{
  return m_Intelligence;
}

uint8 NPC::getCharisma() const
{
  return m_Charisma;
}

uint8 NPC::getWillpower() const
{
  return m_Willpower;
}

uint8 NPC::getLuck() const
{
  return m_Luck;
}

void NPC::setLevel(const uint8 new_level)
{
  m_Level = new_level;
}


void NPC::setStrength(const uint8 str)
{
  m_Strength = str;
}

void NPC::setAgility(const uint8 agi)
{
  m_Agility = agi;
}

void NPC::setVitality(const uint8 vit)
{
  m_Vitality = vit;
}

void NPC::setIntelligence(const uint8 intelligence)
{
  m_Intelligence = intelligence;
}

void NPC::setWillpower(const uint8 wil)
{
  m_Willpower = wil;
}

void NPC::setCharisma(const uint8 cha)
{
  m_Charisma = cha;
}

void NPC::setLuck(const uint8 luck)
{
  m_Luck = luck;
}

bool NPC::isFemale() const
{
  return NPCBase::getSingleton().isNPCFemale(ID);
}

Inventory& NPC::getInventory()
{
  return m_Inventory;
}

const Inventory& NPC::getConstInventory() const
{
  return m_Inventory;
}

unsigned int NPC::getCurrentEncumbrance() const
{
  const float w = m_Inventory.getTotalWeight();
  if (w<=0.0f)
  {
    return 0;
  }
  return static_cast<unsigned int>(w);
}

unsigned int NPC::getMaxEncumbrance() const
{
  const Settings& set = Settings::getSingleton();
  return set.getSetting_uint("BaseEncumbrance") +
         set.getSetting_uint("EncumbranceStrengthCoefficient")*m_Strength;
  //cBaseEncumbrance + cEncumbranceStrengthCoefficient*m_Strength
}

bool NPC::isEncumbered() const
{
  return (getCurrentEncumbrance()>getMaxEncumbrance());
}

unsigned int NPC::getMaxHealth() const
{
  const Settings& set = Settings::getSingleton();
  return set.getSetting_uint("HealthBase")
         + set.getSetting_uint("HealthVitalityFactor")*m_Vitality
         + set.getSetting_uint("HealthLevelFactor")*m_Level;
  //return cHealthBase + cHealthVitalityFactor*m_Vitality + cHealthLevelFactor*m_Level;
}

bool NPC::pickUp(Item* target)
{
  if (target==NULL) return false;
  if (!(target->canPickUp()))
  {
    return false;
  }
  if (position.squaredDistance(target->getPosition())
       > cMaximumPickUpDistance * cMaximumPickUpDistance)
  {
    return false;
  }
  //we have to save item's ID now, because the pointer might be invalid after
  // the call of removeItemReference()
  const std::string ItemID = target->getID();
  if (ObjectManager::getSingleton().removeItemReference(target))
  {
    m_Inventory.addItem(ItemID, 1);
    return true;
  }
  return false;
}

bool NPC::equip(const std::string& ItemID)
{
  if (m_Inventory.getItemCount(ItemID)==0)
  {
    DuskLog() << "NPC::equip: ERROR: NPC has no item with ID \""<<ItemID
              << "\" in inventory.\n";
    return false; //NPC has no such item
  }
  if (entity==NULL)
  {
    DuskLog() << "NPC::equip: ERROR: NPC is not enabled!\n";
    return false; //cannot equip on an unenabled NPC
  }
  if (m_EquippedRight==NULL)
  {
    return equip(ItemID, stRightHand);
  }
  else if (m_EquippedLeft==NULL)
  {
    return equip(ItemID, stLeftHand);
  }
  //if we're here, no hand is free
  return false;
}

bool NPC::equip(const std::string& ItemID, const SlotType slot)
{
  //protected version of equip()
  Item* pItem = NULL;
  if (ItemBase::getSingleton().hasItem(ItemID))
  {
    pItem = new Item(ItemID, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, 1.0f);
  }
  else if (WeaponBase::getSingleton().hasWeapon(ItemID))
  {
    pItem = new Weapon(ItemID, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, 1.0f);
  }
  else
  {
    DuskLog() << "NPC::equip: ERROR: there is no Item or weapon with the given "
              << "ID \""<<ItemID<<"\".\n";
    return false;
  }
  //now to the tag points
  std::string bone_name;
  switch (slot)
  {
    case stRightHand:
         bone_name = NPCBase::getSingleton().getNPCTagPoints(ID).HandRight;
         break;
    case stLeftHand:
         bone_name = NPCBase::getSingleton().getNPCTagPoints(ID).HandLeft;
         break;
    default:
         DuskLog() << "NPC::equip: ERROR: unknown slot type!\n";
         delete pItem;
         return false;
  } //swi
  if (bone_name=="")
  {
    DuskLog() << "NPC::equip: ERROR: received empty tag point identifier for "
              << "NPC \""<<ID<<"\".\n";
    delete pItem;
    return false; //no bone name, no equip()
  }
  pItem->enableWithoutSceneNode(entity->getParentSceneNode()->getCreator());
  entity->attachObjectToBone(bone_name, pItem->exposeEntity());
  pItem->setEquipped(true);
  if (slot==stLeftHand)
  {
    m_EquippedLeft = pItem;
    if (pItem->getDuskType()==otWeapon)
    {
      //set attack flag for left hand
      m_AttackFlags |= Flag_CanLeftAttack;
      m_TimeToNextAttackLeft = 0.0f;
    }
  }
  else
  {
    m_EquippedRight = pItem;
    if (pItem->getDuskType()==otWeapon)
    {
      //set attack flag for right hand
      m_AttackFlags |= Flag_CanRightAttack;
      m_TimeToNextAttackRight = 0.0f;
    }
  }
  return true;
}

bool NPC::hasEquipped(const std::string& ItemID) const
{
  if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->getID()==ItemID) return true;
    if (m_EquippedLeft!=NULL)
    {
      if (m_EquippedLeft->getID()==ItemID) return true;
    }
  }
  return false;
}

bool NPC::unequip(const std::string& ItemID)
{
  if (entity==NULL)
  {
    return false; //cannot unequip when NPC's not enabled
  }
  if (m_EquippedLeft!=NULL)
  {
    if (m_EquippedLeft->getID()==ItemID)
      return unequip(stLeftHand);
  }
  if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->getID()==ItemID)
      return unequip(stRightHand);
  }
  return false;
}

bool NPC::unequip(const SlotType slot)
{
  if (entity==NULL)
  {
    return false; //cannot unequip when NPC's not enabled
  }
  switch(slot)
  {
    case stLeftHand:
         if (m_EquippedLeft==NULL) return true;
         entity->detachObjectFromBone(m_EquippedLeft->exposeEntity());
         delete m_EquippedLeft;
         m_EquippedLeft = NULL;
         //reset left attack bit
         m_AttackFlags = m_AttackFlags & ~Flag_CanLeftAttack;
         if (!canAttackRight()) stopAttack();
         return true;
         break;
    case stRightHand:
         if (m_EquippedRight==NULL) return true;
         entity->detachObjectFromBone(m_EquippedRight->exposeEntity());
         delete m_EquippedRight;
         m_EquippedRight = NULL;
         //reset right attack bit
         m_AttackFlags = m_AttackFlags & ~Flag_CanRightAttack;
         if (!canAttackLeft()) stopAttack();
         return true;
         break;
    default:
         DuskLog() << "NPC::unequip: ERROR: unknown slot type!\n";
         return false;
  } //swi
}

bool NPC::startAttack()
{
  if (doesAttack())
  {
    //NPC is already attacking, so return true
    return true;
  }
  if (!isAlive())
  {
    //dead NPC cannot attack any more
    return false;
  }
  if (canAttackLeft() or canAttackRight())
  {
    //we have at least one weapon -> attack
    m_AttackFlags = m_AttackFlags | Flag_DoesAttack;
    startAttackAnimation();
    return true;
  }
  //NPC has not equipped a weapon, we can't start an attack
  return false;
}

bool NPC::stopAttack()
{
  if (doesAttack())
  {
    stopAttackAnimation();
    m_AttackFlags &= compl Flag_DoesAttack;
    //m_TimeToNextAttack = 0.0f;
  }
  return true;
}

const std::string& NPC::getObjectMesh() const
{
  return NPCBase::getSingleton().getNPCMesh(ID);
}

bool NPC::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  return AnimatedObject::enable(scm);
}

bool NPC::saveNPCPart(std::ofstream& output) const
{
  if (!output.good())
  {
    DuskLog() << "NPC::saveNPCPart: ERROR: bad stream given!\n";
    return false;
  }
  //health
  output.write((char*) &m_Health, sizeof(float));
  //level
  output.write((char*) &m_Level, 1);
  //attributes
  output.write((char*) &m_Strength, 1);
  output.write((char*) &m_Agility, 1);
  output.write((char*) &m_Vitality, 1);
  output.write((char*) &m_Intelligence, 1);
  output.write((char*) &m_Willpower, 1);
  output.write((char*) &m_Charisma, 1);
  output.write((char*) &m_Luck, 1);

  if (!output.good())
  {
    DuskLog() << "NPC::saveNPCPart: ERROR while writing character data.\n";
    return false;
  }
  if (!(m_Inventory.saveToStream(output)))
  {
    DuskLog() << "NPC::saveNPCPart: ERROR while writing inventory data.\n";
    return false;
  }

  //attack flags and times
  output.write((char*) &m_AttackFlags, sizeof(unsigned char));
  output.write((char*) &m_TimeToNextAttackRight, sizeof(float));
  output.write((char*) &m_TimeToNextAttackLeft, sizeof(float));
  if (!output.good())
  {
    DuskLog() << "NPC::saveNPCPart: ERROR while writing attack data.\n";
    return false;
  }
  //equipped items
  unsigned int equipCount = 0;
  if (m_EquippedRight!=NULL) equipCount = 1;
  if (m_EquippedLeft!=NULL) ++equipCount;
  output.write((char*) &equipCount, sizeof(unsigned int));
  if (m_EquippedRight!=NULL)
  {
    //write ID of right hand item
    equipCount = m_EquippedRight->getID().length();
    output.write((char*) &equipCount, sizeof(unsigned int));
    output.write(m_EquippedRight->getID().c_str(), equipCount);
  }
  if (m_EquippedLeft!=NULL)
  {
    //write ID of left hand item
    equipCount = m_EquippedLeft->getID().length();
    output.write((char*) &equipCount, sizeof(unsigned int));
    output.write(m_EquippedLeft->getID().c_str(), equipCount);
  }
  //jumping info
  output.write((char*) &m_Jump, sizeof(bool));
  output.write((char*) &m_JumpVelocity, sizeof(float));
  if (!output.good())
  {
    DuskLog() << "NPC::saveNPCPart: ERROR while writing equipped items or "
              << "jump data.\n";
    return false;
  }
  return output.good();
}

bool NPC::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "NPC::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefN" (reference of NPC)
  OutStream.write((char*) &cHeaderRefN, sizeof(unsigned int));
  //save stuff inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "NPC::saveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!saveAnimatedObjectPart(OutStream))
  {
    DuskLog() << "NPC::saveToStream: ERROR while saving animation data!\n";
    return false;
  }
  // go on with new data members from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    DuskLog() << "NPC::saveToStream: ERROR while saving basic motion data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  if (!saveWaypointObjectPart(OutStream))
  {
    DuskLog() << "NPC::saveToStream: ERROR while saving waypoint data!\n";
    return false;
  }
  //done with inherited data members; go on with NPC stuff
  if (!saveNPCPart(OutStream))
  {
    DuskLog() << "NPC::saveToStream: ERROR while saving NPC data!\n";
    return false;
  }
  return OutStream.good();
}

bool NPC::loadNPCPart(std::ifstream& InStream)
{
  if (!InStream.good())
  {
    DuskLog() << "NPC::loadNPCPart: ERROR: bad stream given!\n";
    return false;
  }
  //health
  InStream.read((char*) &m_Health, sizeof(float));
  //level
  InStream.read((char*) &m_Level, 1);
  //attributes
  InStream.read((char*) &m_Strength, 1);
  InStream.read((char*) &m_Agility, 1);
  InStream.read((char*) &m_Vitality, 1);
  InStream.read((char*) &m_Intelligence, 1);
  InStream.read((char*) &m_Willpower, 1);
  InStream.read((char*) &m_Charisma, 1);
  InStream.read((char*) &m_Luck, 1);

  if (!InStream.good())
  {
    DuskLog() << "NPC::loadNPCPart: ERROR while reading character data.\n";
    return false;
  }
  if (!(m_Inventory.loadFromStream(InStream)))
  {
    DuskLog() << "NPC::loadNPCPart: ERROR while reading inventory data.\n";
    return false;
  }

  //attack flags and times
  InStream.read((char*) &m_AttackFlags, sizeof(unsigned char));
  InStream.read((char*) &m_TimeToNextAttackRight, sizeof(float));
  InStream.read((char*) &m_TimeToNextAttackLeft, sizeof(float));
  if (!InStream.good())
  {
    DuskLog() << "NPC::loadNPCPart: ERROR while reading attack data.\n";
    return false;
  }
  //equipped items
  unsigned int equipCount = 0;
  InStream.read((char*) &equipCount, sizeof(unsigned int));
  if (equipCount>0)
  {
    //read ID of right hand item
    unsigned int len = 0;
    InStream.read((char*) &len, sizeof(unsigned int));
    if (len>255)
    {
      DuskLog() << "NPC::loadNPCPart: ERROR: ID of right hand item seems "
                << "to be longer than 255 characters.\n";
      return false;
    }
    char Buffer[256];
    Buffer[0] = Buffer[255] = '\0';
    InStream.read(Buffer, len);
    Buffer[len] = '\0';
    if (!InStream.good())
    {
      DuskLog() << "NPC::loadNPCPart: ERROR while reading ID of right hand "
                << "item.\n";
      return false;
    }
    if (!equip(std::string(Buffer)))
    {
      DuskLog() << "NPC::loadNPCPart: ERROR while equipping right hand item.\n";
      return false;
    }
    //check for presence of another item
    if (equipCount>1)
    {
      //read ID of left hand item
      len = 0;
      InStream.read((char*) &len, sizeof(unsigned int));
      if (len>255)
      {
        DuskLog() << "NPC::loadNPCPart: ERROR: ID of left hand item seems "
                  << "to be longer than 255 characters.\n";
        return false;
      }
      Buffer[0] = Buffer[255] = '\0';
      InStream.read(Buffer, len);
      Buffer[len] = '\0';
      if (!InStream.good())
      {
        DuskLog() << "NPC::loadNPCPart: ERROR while reading ID of left hand "
                  << "item.\n";
        return false;
      }
      if (!equip(std::string(Buffer)))
      {
        DuskLog() << "NPC::loadNPCPart: ERROR while equipping left hand item.\n";
        return false;
      }
    }//second item
  }//have something equipped
  //jumping info
  InStream.read((char*) &m_Jump, sizeof(bool));
  InStream.read((char*) &m_JumpVelocity, sizeof(float));
  return InStream.good();
}

bool NPC::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "NPC::loadFromStream: ERROR: Cannot load from stream while "
              << "object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "NPC::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefN"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefN)
  {
    DuskLog() << "NPC::loadFromStream: ERROR: Stream contains invalid reference"
              << " header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "NPC::loadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!loadAnimatedObjectPart(InStream))
  {
    DuskLog() << "NPC::loadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    DuskLog() << "NPC::loadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  // go on with data members from WaypointObject
  if (!loadWaypointObjectPart(InStream))
  {
    DuskLog() << "NPC::loadFromStream: ERROR while loading waypoint data.\n";
    return false;
  }
  //done with inherited data, go on with NPC data
  if (!loadNPCPart(InStream))
  {
    DuskLog() << "NPC::loadFromStream: ERROR while loading NPC data.\n";
    return false;
  }
  return InStream.good();
}

void NPC::playDeathAnimation()
{
  stopAllAnimations();
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Death;
  if (anim!="")
  {
    startAnimation(anim, false);
  }
}

void NPC::startWalkAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Walk;
  if (anim!="")
  {
    const std::vector<std::string> walk_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<walk_list.size(); ++i)
    {
      startAnimation(walk_list[i], true);
    }//for
  }//if
}

void NPC::stopWalkAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Walk;
  if (anim!="")
  {
    const std::vector<std::string> walk_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<walk_list.size(); ++i)
    {
      stopAnimation(walk_list[i]);
    }//for
  }//if
}

void NPC::startJumpAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Jump;
  if (anim!="")
  {
    const std::vector<std::string> jump_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<jump_list.size(); ++i)
    {
      startAnimation(jump_list[i], true);
    }//for
  }//if
}

void NPC::stopJumpAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Jump;
  if (anim!="")
  {
    const std::vector<std::string> jump_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<jump_list.size(); ++i)
    {
      stopAnimation(jump_list[i]);
    }//for
  }//if
}

void NPC::startIdleAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Idle;
  if (anim!="")
  {
    const std::vector<std::string> idle_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<idle_list.size(); ++i)
    {
      startAnimation(idle_list[i], true);
    }//for
  }//if
}

void NPC::stopIdleAnimation()
{
  const std::string anim = NPCBase::getSingleton().getNPCAnimations(ID).Idle;
  if (anim!="")
  {
    const std::vector<std::string> idle_list = CSVToVector(anim);
    unsigned int i;
    for (i=0; i<idle_list.size(); ++i)
    {
      stopAnimation(idle_list[i]);
    }//for
  }//if
}

void NPC::startAttackAnimation()
{
  const NPCAnimations& anim = NPCBase::getSingleton().getNPCAnimations(ID);
  bool melee = false;
  if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->getDuskType()==otWeapon)
    {
      melee = WeaponBase::getSingleton().getWeaponData(m_EquippedRight->getID()).Type==wtMelee;
    }
  }
  else if (m_EquippedLeft!=NULL)
  {
    if (m_EquippedLeft->getDuskType()==otWeapon)
    {
      melee = WeaponBase::getSingleton().getWeaponData(m_EquippedLeft->getID()).Type==wtMelee;
    }
  }
  if (melee and (anim.MeleeAttack!=""))
  {
    startAnimation(anim.MeleeAttack, true);
  }
  if (!melee and (anim.ProjectileAttack!=""))
  {
    startAnimation(anim.ProjectileAttack, true);
  }
}

void NPC::stopAttackAnimation()
{
  const NPCAnimations& anim_rec = NPCBase::getSingleton().getNPCAnimations(ID);
  stopAnimation(anim_rec.MeleeAttack);
  stopAnimation(anim_rec.ProjectileAttack);
}

void NPC::performAttack(const SlotType attackSlot)
{
  if (attackSlot!=stRightHand and attackSlot!=stLeftHand)
  {
    DuskLog() << "NPC::performAttack: ERROR: invalid slot.\n";
    return;
  }
  //only enabled NPC can perform an attack
  if (!isEnabled()) return;
  WeaponRecord wRec;
  if (attackSlot==stRightHand)
  {
    wRec = WeaponBase::getSingleton().getWeaponData(m_EquippedRight->getID());
  }
  else
  {
    wRec = WeaponBase::getSingleton().getWeaponData(m_EquippedLeft->getID());
  }
  if (wRec.Range<0.0f)
  {
    DuskLog() << "NPC::performAttack: ERROR: weapon range is below zero.\n";
    return;
  }
  //check for projectile-based attack
  if (wRec.Type==wtGun)
  {
    //create a new projectile for that weapon
    /* TODO:
         - adjust position of projectile that way that it "spawns" a bit away
           from NPC in order to avoid hitting the NPC itself
    */
    Projectile* projPtr = InjectionManager::getSingleton().addProjectileReference(
      wRec.ProjectileID, //ID
      position,
      m_Rotation,
      1.0f);
    //not sure whether this is the best choice
    projPtr->setDirection(entity->getParentSceneNode()->getOrientation()*Ogre::Vector3(0.0, 0.0, 1.0));
    projPtr->setEmitter(this);
    //enable it
    projPtr->enable(getAPI().getOgreSceneManager());
    DuskLog() << "NPC::performAttack: projectile \""<< wRec.ProjectileID << "\" emitted.\n";
    //we are done here
    return;
  }//projectile based

  /* We perform a melee attack, thus perform scene query to get all possible
     targets.
  */
  DuskLog() << "NPC::performAttack: performing melee attack on ";
  if (attackSlot==stRightHand) DuskLog() << "right";
  if (attackSlot==stLeftHand) DuskLog() << "left";
  DuskLog() << " hand.\n";
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL) return;
  Ogre::SphereSceneQuery* sp_query =
      scm->createSphereQuery(Ogre::Sphere(position, wRec.Range));
  Ogre::SceneQueryResult& result = sp_query->execute();
  //check results for NPCs
  DuskObject* obj_ptr;
  Ogre::SceneQueryResultMovableList::iterator iter = result.movables.begin();
  while (iter!=result.movables.end())
  {
    if ((*iter)->getUserObject()!=NULL)
    {
      obj_ptr = static_cast<DuskObject*>((*iter)->getUserObject());
      if (obj_ptr->getDuskType()==otNPC)
      {
        //We've found an NPC!
        NPC* NPC_Ptr = dynamic_cast<NPC*>(obj_ptr);
        if (NPC_Ptr!=this)
        {
          //calculate damage
          unsigned int times;
          if (criticalHit())
          {
            times = wRec.DamageTimes
                    * Settings::getSingleton().getSetting_uint("CriticalDamageFactor");
          }
          else
          {
            times = wRec.DamageTimes;
          }
          switch (wRec.DamageDice)
          {
            case 0:
                 //no damage at all
                 break;
            case 4:
                 NPC_Ptr->inflictDamage(DiceBox::getSingleton().d4(times));
                 break;
            case 6:
                 NPC_Ptr->inflictDamage(DiceBox::getSingleton().d6(times));
                 break;
            case 8:
                 NPC_Ptr->inflictDamage(DiceBox::getSingleton().d8(times));
                 break;
            case 10:
                 NPC_Ptr->inflictDamage(DiceBox::getSingleton().d10(times));
                 break;
            case 20:
                 NPC_Ptr->inflictDamage(DiceBox::getSingleton().d20(times));
                 break;
            default:
                 DuskLog() << "NPC::performAttack: ERROR: weapon has unsupported"
                           << " hit die (d="<<(int) wRec.DamageDice << ".\n";
                 break;
          }//swi
        }//NPC is not self
      }//NPC found
    }//object found
    ++iter;
  }//while
  //clean up scene query
  scm->destroyQuery(sp_query);
  sp_query = NULL;
  return;
}

bool NPC::criticalHit() const
{
  /* NPC does a critical hit, if his/her luck is greater or equal to a d20. */
  return getLuck()>=DiceBox::getSingleton().d20();
}

bool NPC::doesAttack() const
{
  return ((m_AttackFlags & Flag_DoesAttack)!=0);
}

bool NPC::canAttackLeft() const
{
  return ((m_AttackFlags & Flag_CanLeftAttack)!=0);
}

bool NPC::canAttackRight() const
{
  return ((m_AttackFlags & Flag_CanRightAttack)!=0);
}

} //namespace
