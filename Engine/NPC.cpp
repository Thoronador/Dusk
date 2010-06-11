#include "NPC.h"
#include <sstream>
#include "NPCBase.h"
#include "ItemBase.h"
#include "WeaponBase.h"
#include "Settings.h"
#include "DuskConstants.h"
#include "ObjectData.h"
#include "AnimationData.h"
#include "API.h"
#include "DiceBox.h"

namespace Dusk
{

const float NPC::cMaximumPickUpDistance=100; //it's a guess; adjust it, if needed
const unsigned char NPC::Flag_DoesAttack = 1;
const unsigned char NPC::Flag_CanRightAttack = 2;
const unsigned char NPC::Flag_CanLeftAttack = 4;

NPC::NPC()
  : AnimatedObject("", Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f)
{
  //assume some random data
  //all attributes to one
  m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
  m_Level = 1; //assume level 1
  m_Inventory.MakeEmpty(); //empty inventory
  m_Health = getMaxHealth();
  m_EquippedLeft = NULL;
  m_EquippedRight = NULL;
  m_TimeToNextAttackLeft = m_TimeToNextAttackRight = 0.0f;
  m_AttackFlags = 0;
}

NPC::NPC(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
 : AnimatedObject(_ID, pos, rot, Scale)
{
  if (NPCBase::GetSingleton().hasNPC(ID))
  {
    //feed the data from NPCBase
    //-- attributes
    const NPCAttributes temp_attr = NPCBase::GetSingleton().getAttributes(ID);
    m_Strength = temp_attr.Str;
    m_Agility = temp_attr.Agi;
    m_Vitality = temp_attr.Vit;
    m_Intelligence = temp_attr.Int;
    m_Willpower = temp_attr.Will;
    m_Charisma = temp_attr.Cha;
    m_Luck = temp_attr.Luck;
    //-- level
    m_Level = NPCBase::GetSingleton().getLevel(ID);
    m_Inventory = NPCBase::GetSingleton().getNPCInventory(ID);
  }
  else
  { //assume some random data
    //all attributes to one
    m_Strength=m_Agility=m_Vitality=m_Intelligence=m_Willpower=m_Charisma=m_Luck=1;
    m_Level = 1; //assume level 1
    m_Inventory.MakeEmpty(); //empty inventory
  }
  m_Health = getMaxHealth();
  m_EquippedLeft = NULL;
  m_EquippedRight = NULL;
  m_TimeToNextAttackLeft = m_TimeToNextAttackRight = 0.0f;
  m_AttackFlags = 0;
}

NPC::~NPC()
{
  //empty
  this->Disable();
}

ObjectTypes NPC::GetType() const
{
  return otNPC;
}

void NPC::injectTime(const float SecondsPassed)
{
  AnimatedObject::injectTime(SecondsPassed);
  WaypointObject::injectTime(SecondsPassed);
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
          if (m_EquippedLeft->GetType()==otWeapon)
          {
            performAttack(stLeftHand);
            //update time
            m_TimeToNextAttackLeft = m_TimeToNextAttackLeft
                +WeaponBase::GetSingleton().getWeaponData(m_EquippedLeft->GetID()).TimeBetweenAttacks;
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
          if (m_EquippedRight->GetType()==otWeapon)
          {
            performAttack(stRightHand);
            //update time
            m_TimeToNextAttackRight = m_TimeToNextAttackRight
                +WeaponBase::GetSingleton().getWeaponData(m_EquippedRight->GetID()).TimeBetweenAttacks;
          }
        }//if
      }//time less/equal zero
    }// can attack right hand
  }//if NPC does attack
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
  return NPCBase::GetSingleton().isNPCFemale(ID);
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
  const float w = m_Inventory.GetTotalWeight();
  if (w<=0.0f)
  {
    return 0;
  }
  return static_cast<unsigned int>(w);
}

unsigned int NPC::getMaxEncumbrance() const
{
  const Settings& set = Settings::GetSingleton();
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
  const Settings& set = Settings::GetSingleton();
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
  if (position.squaredDistance(target->GetPosition())
       > cMaximumPickUpDistance * cMaximumPickUpDistance)
  {
    return false;
  }
  //we have to save item's ID now, because the pointer might be invalid after
  // the call of removeItemReference()
  const std::string ItemID = target->GetID();
  if (ObjectData::GetSingleton().removeItemReference(target))
  {
    m_Inventory.AddItem(ItemID, 1);
    return true;
  }
  return false;
}

bool NPC::equip(const std::string& ItemID)
{
  if (m_Inventory.GetItemCount(ItemID)==0)
  {
    std::cout << "NPC::equip: ERROR: NPC has no item with ID \""<<ItemID
              << "\" in inventory.\n";
    return false; //NPC has no such item
  }
  if (entity==NULL)
  {
    std::cout << "NPC::equip: ERROR: NPC is not enabled!\n";
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
  if (ItemBase::GetSingleton().hasItem(ItemID))
  {
    pItem = new Item(ItemID, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f);
  }
  else if (WeaponBase::GetSingleton().hasWeapon(ItemID))
  {
    pItem = new Weapon(ItemID, Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f);
  }
  else
  {
    std::cout << "NPC::equip: ERROR: there is no Item or weapon with the given "
              << "ID \""<<ItemID<<"\".\n";
    return false;
  }
  //now to the tag points
  std::string bone_name;
  switch (slot)
  {
    case stRightHand:
         bone_name = NPCBase::GetSingleton().getNPCTagPoints(ID).HandRight;
         break;
    case stLeftHand:
         bone_name = NPCBase::GetSingleton().getNPCTagPoints(ID).HandLeft;
         break;
    default:
         std::cout << "NPC::equip: ERROR: unknown slot type!\n";
         delete pItem;
         return false;
  } //swi
  if (bone_name=="")
  {
    std::cout << "NPC::equip: ERROR: received empty tag point identifier for "
              << "NPC \""<<ID<<"\".\n";
    delete pItem;
    return false; //no bone name, no equip()
  }
  pItem->EnableWithoutSceneNode(entity->getParentSceneNode()->getCreator());
  entity->attachObjectToBone(bone_name, pItem->exposeEntity());
  pItem->setEquipped(true);
  if (slot==stLeftHand)
  {
    m_EquippedLeft = pItem;
    if (pItem->GetType()==otWeapon)
    {
      //set attack flag for left hand
      m_AttackFlags |= Flag_CanLeftAttack;
      m_TimeToNextAttackLeft = 0.0f;
    }
  }
  else
  {
    m_EquippedRight = pItem;
    if (pItem->GetType()==otWeapon)
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
    if (m_EquippedRight->GetID()==ItemID) return true;
    if (m_EquippedLeft!=NULL)
    {
      if (m_EquippedLeft->GetID()==ItemID) return true;
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
    if (m_EquippedLeft->GetID()==ItemID)
      return unequip(stLeftHand);
  }
  if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->GetID()==ItemID)
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
         std::cout << "NPC::unequip: ERROR: unknown slot type!\n";
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
  /*if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->GetType()==otWeapon)
    {
      //we got a weapon here, so use it
      m_AttackFlags |= Flag_DoesAttack;
      startAttackAnimation();
      return true;
    }//if weapon
  }
  if (m_EquippedLeft!=NULL)
  {
    if (m_EquippedLeft->GetType()==otWeapon)
    {
      //we got a weapon here, so use it
      m_AttackFlags |= Flag_DoesAttack;
      startAttackAnimation();
      return true;
    }//if weapon
  }*/
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

bool NPC::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "NPC::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), NPCBase::GetSingleton().getNPCMesh(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this NPC as reverse link
  entity->setUserObject(this);
  if (m_Anim != "")
  {
    const Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    if (anim_set->hasAnimationState(m_Anim))
    {
      Ogre::AnimationState* state = anim_set->getAnimationState(m_Anim);
      state->setTimePosition(0.0f);
      state->setLoop(m_LoopAnim);
      state->setEnabled(true);
    }
    else
    {
      m_Anim = "";
    }
  }
  return (entity!=NULL);
}

bool NPC::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefN" (reference of NPC)
  OutStream.write((char*) &cHeaderRefN, sizeof(unsigned int));
  //save stuff inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!SaveAnimatedObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving animation data!\n";
    return false;
  }
  // go on with new data members from UniformMotionObject
  if (!SaveUniformMotionObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving basic motion data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  if (!SaveWaypointObjectPart(OutStream))
  {
    std::cout << "NPC::SaveToStream: ERROR while saving waypoint data!\n";
    return false;
  }
  //done with inherited data members; go on with NPC stuff
  //health
  OutStream.write((char*) &m_Health, sizeof(float));
  //level
  OutStream.write((char*) &m_Level, 1);
  //attributes
  OutStream.write((char*) &m_Strength, 1);
  OutStream.write((char*) &m_Agility, 1);
  OutStream.write((char*) &m_Vitality, 1);
  OutStream.write((char*) &m_Intelligence, 1);
  OutStream.write((char*) &m_Willpower, 1);
  OutStream.write((char*) &m_Charisma, 1);
  OutStream.write((char*) &m_Luck, 1);

  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR while writing character data.\n";
    return false;
  }
  if (!(m_Inventory.SaveToStream(OutStream)))
  {
    std::cout << "NPC::SaveToStream: ERROR while writing inventory data.\n";
    return false;
  }

  //attack flags and times
  OutStream.write((char*) &m_AttackFlags, sizeof(unsigned char));
  OutStream.write((char*) &m_TimeToNextAttackRight, sizeof(float));
  OutStream.write((char*) &m_TimeToNextAttackLeft, sizeof(float));
  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR while writing attack data.\n";
    return false;
  }
  //equipped items
  unsigned int equipCount = 0;
  if (m_EquippedRight!=NULL) equipCount = 1;
  if (m_EquippedLeft!=NULL) ++equipCount;
  OutStream.write((char*) &equipCount, sizeof(unsigned int));
  if (m_EquippedRight!=NULL)
  {
    //write ID of right hand item
    equipCount = m_EquippedRight->GetID().length();
    OutStream.write((char*) &equipCount, sizeof(unsigned int));
    OutStream.write(m_EquippedRight->GetID().c_str(), equipCount);
  }
  if (m_EquippedLeft!=NULL)
  {
    //write ID of left hand item
    equipCount = m_EquippedLeft->GetID().length();
    OutStream.write((char*) &equipCount, sizeof(unsigned int));
    OutStream.write(m_EquippedLeft->GetID().c_str(), equipCount);
  }
  if (!OutStream.good())
  {
    std::cout << "NPC::SaveToStream: ERROR while writing equipped items.\n";
    return false;
  }
  return OutStream.good();
}

bool NPC::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "NPC::LoadFromStream: ERROR: Cannot load from stream while "
              << "object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "NPC::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefN"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefN)
  {
    std::cout << "NPC::LoadFromStream: ERROR: Stream contains invalid reference"
              << "header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!LoadAnimatedObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!LoadUniformMotionObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  // go on with data members from WaypointObject
  if (!LoadWaypointObjectPart(InStream))
  {
    std::cout << "NPC::LoadFromStream: ERROR while loading waypoint data.\n";
    return false;
  }
  //done with inherited data, go on with NPC data

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
    std::cout << "NPC::LoadFromStream: ERROR while reading character data.\n";
    return false;
  }
  if (!(m_Inventory.LoadFromStream(InStream)))
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading inventory data.\n";
    return false;
  }

  //attack flags and times
  InStream.read((char*) &m_AttackFlags, sizeof(unsigned char));
  InStream.read((char*) &m_TimeToNextAttackRight, sizeof(float));
  InStream.read((char*) &m_TimeToNextAttackLeft, sizeof(float));
  if (!InStream.good())
  {
    std::cout << "NPC::LoadFromStream: ERROR while reading attack data.\n";
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
      std::cout << "NPC::LoadFromStream: ERROR: ID of right hand item seems "
                << "to be longer than 255 characters.\n";
      return false;
    }
    char Buffer[256];
    Buffer[0] = Buffer[255] = '\0';
    InStream.read(Buffer, len);
    Buffer[len] = '\0';
    if (!InStream.good())
    {
      std::cout << "NPC::LoadFromStream: ERROR while reading ID of right hand "
                << "item.\n";
      return false;
    }
    if (!equip(std::string(Buffer)))
    {
      std::cout << "NPC::LoadFromStream: ERROR while equipping right hand item.\n";
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
        std::cout << "NPC::LoadFromStream: ERROR: ID of left hand item seems "
                  << "to be longer than 255 characters.\n";
        return false;
      }
      Buffer[0] = Buffer[255] = '\0';
      InStream.read(Buffer, len);
      Buffer[len] = '\0';
      if (!InStream.good())
      {
        std::cout << "NPC::LoadFromStream: ERROR while reading ID of left hand "
                  << "item.\n";
        return false;
      }
      if (!equip(std::string(Buffer)))
      {
        std::cout << "NPC::LoadFromStream: ERROR while equipping left hand item.\n";
        return false;
      }
    }//second item
  }//have something equipped
  return InStream.good();
}

void NPC::playDeathAnimation()
{
  StopAllAnimations();
  const std::string anim = NPCBase::GetSingleton().getNPCAnimations(ID).Death;
  if (anim!="")
  {
    StartAnimation(anim, false);
  }
}

void NPC::startAttackAnimation()
{
  const NPCAnimations& anim = NPCBase::GetSingleton().getNPCAnimations(ID);
  bool melee = false;
  if (m_EquippedRight!=NULL)
  {
    if (m_EquippedRight->GetType()==otWeapon)
    {
      melee = WeaponBase::GetSingleton().getWeaponData(m_EquippedRight->GetID()).Type==wtMelee;
    }
  }
  else if (m_EquippedLeft!=NULL)
  {
    if (m_EquippedLeft->GetType()==otWeapon)
    {
      melee = WeaponBase::GetSingleton().getWeaponData(m_EquippedLeft->GetID()).Type==wtMelee;
    }
  }
  if (melee and (anim.MeleeAttack!=""))
  {
    StartAnimation(anim.MeleeAttack, true);
  }
  if (!melee and (anim.ProjectileAttack!=""))
  {
    StartAnimation(anim.ProjectileAttack, true);
  }
}

void NPC::stopAttackAnimation()
{
  const NPCAnimations& anim_rec = NPCBase::GetSingleton().getNPCAnimations(ID);
  StopAnimation(anim_rec.MeleeAttack);
  StopAnimation(anim_rec.ProjectileAttack);
}

void NPC::performAttack(const SlotType attackSlot)
{
  if (attackSlot!=stRightHand and attackSlot!=stLeftHand)
  {
    std::cout << "NPC::performAttack(): ERROR: invalid slot.\n";
    return;
  }
  WeaponRecord wRec;
  if (attackSlot==stRightHand)
  {
    wRec = WeaponBase::GetSingleton().getWeaponData(m_EquippedRight->GetID());
  }
  else
  {
    wRec = WeaponBase::GetSingleton().getWeaponData(m_EquippedLeft->GetID());
  }
  if (wRec.Range<0.0f)
  {
    std::cout << "NPC::performAttack: ERROR: weapon range is below zero.\n";
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
    Projectile* projPtr = AnimationData::GetSingleton().addProjectileReference(
      wRec.ProjectileID, //ID
      position,
      rotation,
      1.0f);
    projPtr->Enable(getAPI().getOgreSceneManager());
    //we are done here
    return;
  }//projectile based

  /* We perform a melee attack, thus perform scene query to get all possible
     targets.
  */
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
      if (obj_ptr->GetType()==otNPC)
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
                    * Settings::GetSingleton().getSetting_uint("CriticalDamageFactor");
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
                 NPC_Ptr->inflictDamage(DiceBox::GetSingleton().d4(times));
                 break;
            case 6:
                 NPC_Ptr->inflictDamage(DiceBox::GetSingleton().d6(times));
                 break;
            case 8:
                 NPC_Ptr->inflictDamage(DiceBox::GetSingleton().d8(times));
                 break;
            case 10:
                 NPC_Ptr->inflictDamage(DiceBox::GetSingleton().d10(times));
                 break;
            case 20:
                 NPC_Ptr->inflictDamage(DiceBox::GetSingleton().d20(times));
                 break;
            default:
                 std::cout << "NPC::performAttack: ERROR: weapon has unsupported"
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
  return getLuck()>=DiceBox::GetSingleton().d20();
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
