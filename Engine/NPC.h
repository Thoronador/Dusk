/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-21
 Purpose: NPC class
          represents a single NPC within the game

 History:
     - 2009-12-09 (rev 141) - initial version (by thoronador)
     - 2009-12-18 (rev 142) - attributes and inventory added
     - 2009-12-22 (rev 143) - Enable() implemented, ancestor class will handle
                              Disable() and IsEnabled()
                            - maximum health calculation added
                            - constructor tries to get initial attributes
                              from NPCBase
     - 2009-12-23 (rev 145) - use of Settings class to get the balancing settings
     - 2010-01-16 (rev 154) - LoadFromStream() and SaveToStream() implemented
     - 2010-01-17 (rev 156) - LoadFromStream() and SaveToStream() updated
     - 2010-01-26 (rev 159) - getConstInventory() added for getting a constant
                              Inventory reference
     - 2010-03-27 (rev 188) - pickUp() added for picking up items
     - 2010-04-21 (rev 190) - isFemale() added
     - 2010-05-20 (rev 205) - adjustments for new object hierarchy
     - 2010-05-21 (rev 206) - small improvement
     - 2010-06-02 (rev 213) - getCurrentEncumbrance() and isEncumbered() added
                            - inflictDamage() added
                            - equip(), unequip(), hasEquipped() added; allows to
                              equip/unequip items in NPC's hands
     - 2010-06-06 (rev 215) - some of the attack-related stuff implemented
     - 2010-06-10 (rev 217) - adjustments in load/save functions to load/save
                              equipped items and attacking state, too
                            - attack-related code can now handle two-weapon
                              attacks better
     - 2010-06-11 (rev 218) - fixed a bug with animations

 ToDo list:
     - add possibility to equip weapons, clothes, armour, etc.
     - update load/save functions for equipped items and attack status
     - adjust position of created projectile in performAttack()
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef NPC_H
#define NPC_H

#include <string>
#include <OgreVector3.h>
#include <OgreSceneManager.h>
#include "AnimatedObject.h"
#include "WaypointObject.h"
#include "Inventory.h"
#include "DuskTypes.h"
#include "Item.h"

namespace Dusk
{

  class NPC: public AnimatedObject, public WaypointObject
  {
    public:
      /* default constructor */
      NPC();

      /* constructor with parameter list */
      NPC(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

      /* destructor */
      virtual ~NPC();

      /* returns the enumeration type indicating that this is an NPC */
      virtual ObjectTypes GetType() const;

      /* animated and move the NPC according to the passed time

         remarks:
             This function is intended to be called regularly, i.e. every
             frame, to accomplish the desired animation of the object. If you
             don't call this function in such a manner, the animation will be
             processed improperly and/or will not be fluent.
      */
      virtual void injectTime(const float SecondsPassed);

      /* returns the current amount of health/ hitpoints of that NPC */
      float getHealth() const;

      /* sets the current health of the NPC */
      void setHealth(const float new_health);

      /* inflicts damage on that NPC */
      void inflictDamage(const float damage_amount);

      /* returns true, if the NPC is considered to be alive */
      bool isAlive() const;

      /* ---- attribute set/get methods ---- */
      // returns the NPC's current level
      uint8 getLevel() const;
      uint8 getStrength() const;
      uint8 getAgility() const;
      uint8 getVitality() const;
      uint8 getIntelligence() const;
      uint8 getWillpower() const;
      uint8 getCharisma() const;
      uint8 getLuck() const;
      // set the NPC's current level to new_level
      void setLevel(const uint8 new_level);
      void setStrength(const uint8 str);
      void setAgility(const uint8 agi);
      void setVitality(const uint8 vit);
      void setIntelligence(const uint8 intelligence);
      void setCharisma(const uint8 cha);
      void setWillpower(const uint8 wil);
      void setLuck(const uint8 luck);

      /* returns true, if the NPC is female */
      bool isFemale() const;

      /* inventory access method */
      Inventory& getInventory();

      const Inventory& getConstInventory() const;

      /* current encumbrance */
      unsigned int getCurrentEncumbrance() const;

      /* maximum encumbrance */
      unsigned int getMaxEncumbrance() const;

      /* returns true, if the NPC is carrying more weight than it is able to */
      bool isEncumbered() const;

      /* maximum health */
      unsigned int getMaxHealth() const;

      /* tries to pick up the Item target and adds it to the NPC's inventory.
         Returns true on success, false otherwise.
      */
      bool pickUp(Item* target);

      /* tries to equip the Item with the given ID in one hand. Usually, items
         are equipped in the right hand first, or, if the right hand is not
         free, in the left hand. If no hand is free, nothing will be equipped.
         Returns true on success (i.e. item could be equipped), false otherwise.

         remarks:
           An item with the given ID must be in the NPC's inventory in order to
           be equipped.
      */
      bool equip(const std::string& ItemID);

      /* returns true, if the NPC has equipped an item with the given ID */
      bool hasEquipped(const std::string& ItemID) const;

      /* tries to unequip the Item in the given ID..
         Returns true on success (i.e. item could be unequipped), false otherwise.
      */
      bool unequip(const std::string& ItemID);

      /* tries to start an attack and returns true if an attack was started

         remarks:
             In order to attack the NPC must have a weapon equipped.
      */
      bool startAttack();

      /* tries to stop an attack and returns true, if the NPC is not attacking
         any more
      */
      bool stopAttack();

      /* returns true, if the NPC does/did a critical hit

         remarks:
            Call this only once(!) during attack to determine whether to do a
            critical hit or not, because the return value might change after
            every call.
      */
      bool criticalHit() const;

      /* Enables the NPC, i.e. tells the SceneManager to display it.
         Returns true on success, false on error.
      */
      virtual bool Enable(Ogre::SceneManager* scm);

      /* saves NPC to given stream and returns true on success, false otherwise
      */
      virtual bool SaveToStream(std::ofstream& OutStream) const;

      /* Loads NPC from stream and returns true on success, false otherwise.
         The NPC's data is probably inconsistent after that function failed, so
         don't rely on its data in that case.
      */
      virtual bool LoadFromStream(std::ifstream& InStream);

      /* the maximum distance an item can be away from the NPC while being
         picked up
      */
      static const float cMaximumPickUpDistance;
    protected:
      //enumeration type for equipment slots
      enum SlotType { stRightHand, stLeftHand };

      /* plays animation to signal NPC's death */
      void playDeathAnimation();

      /* starts animation to signal NPC's attacking */
      void startAttackAnimation();

      /* stops NPC's attack animation */
      void stopAttackAnimation();

      /* function to attack surrounding NPCs */
      void performAttack(const SlotType attackSlot);

      /* equips an item in the given slot

         parameters:
             ItemID - ID of the item/weapon that shall be equipped
             slot   - slot where the item/weapon has to be equipped
      */
      bool equip(const std::string& ItemID, const SlotType slot);

      /* tries to unequip the Item in the given slot..
         Returns true on success (i.e. slot could be freed), false otherwise.

         parameters:
           slot - slot where the item/weapon has to be removed
      */
      bool unequip(const SlotType slot);

      //the NPC's inventory
      Inventory m_Inventory;
      //current health
      float m_Health;
      //level
      uint8 m_Level;
      //attributes
      uint8 m_Strength, m_Agility, m_Vitality, m_Intelligence, m_Willpower, m_Charisma, m_Luck;
      //equipped items in left and right hand
      Item* m_EquippedLeft;
      Item* m_EquippedRight;
      //time until next attack/ hit is triggered
      float m_TimeToNextAttackLeft;
      float m_TimeToNextAttackRight;
      //holds attack-related flags
      unsigned char m_AttackFlags;
      /*flag to indicate that NPC is attacking */
      static const unsigned char Flag_DoesAttack;
      /*flag to indicate that NPC is holding a weapon in the right hand */
      static const unsigned char Flag_CanRightAttack;
      /*flag to indicate that NPC is holding a weapon in the left hand */
      static const unsigned char Flag_CanLeftAttack;

      /* utility function to check for attack flag */
      bool doesAttack() const;
      /* utility function to check for attack flag - left hand */
      bool canAttackLeft() const;
      /* utility function to check for attack flag - right hand */
      bool canAttackRight() const;
  }; //class

} //namespace

#endif // NPC_H
