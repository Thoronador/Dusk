/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-21
 Purpose: NPC class
          represents a single NPC within the game

 History:
     - 2009-12-09 (rev 141) - initial version
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

 ToDo list:
     - add possibility to equip weapons, clothes, armour, etc.
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
      NPC();
      NPC(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);
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

      /* returns true, if the NPC is considered to be alive */
      bool isAlive() const;

      /*attribute set/get methods */
      uint8 getLevel() const;
      uint8 getStrength() const;
      uint8 getAgility() const;
      uint8 getVitality() const;
      uint8 getIntelligence() const;
      uint8 getWillpower() const;
      uint8 getCharisma() const;
      uint8 getLuck() const;
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

      /* maximum encumbrance */
      unsigned int getMaxEncumbrance() const;

      /* maximum health */
      unsigned int getMaxHealth() const;

      /* tries to pick up the Item target and adds it to the NPC's inventory.
         Returns true on success, false otherwise.
      */
      bool pickUp(Item* target);

      /* Enables the NPC, i.e. tells the SceneManager to display it.
         Returns true on success, false on error.
      */
      virtual bool Enable(Ogre::SceneManager* scm);

      /* saves NPC to given stream and returns true on success, false otherwise
      */
      virtual bool SaveToStream(std::ofstream& OutStream) const;

      /* Loads contents of inventory from stream and returns true on success,
         false otherwise. The Inventory content is probably inconsistent after
         that function failed, so don't rely on its contents in that case. */
      virtual bool LoadFromStream(std::ifstream& InStream);

      /* the maximum distance an item can be away from the NPC while being
         picked up
      */
      static const float cMaximumPickUpDistance;
    protected:
      Inventory m_Inventory;
      float m_Health;
      //level
      uint8 m_Level;
      //attributes
      uint8 m_Strength, m_Agility, m_Vitality, m_Intelligence, m_Willpower, m_Charisma, m_Luck;
  }; //class

} //namespace

#endif // NPC_H
