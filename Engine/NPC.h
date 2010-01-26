/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-26
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

 ToDo list:
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
#include "Inventory.h"
#include "DuskTypes.h"

namespace Dusk
{
  class NPC: public AnimatedObject
  {
    public:
      NPC();
      NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
      virtual ~NPC();

      /* returns the enumeration type indicating that this is an NPC */
      virtual ObjectTypes GetType() const;

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

      /* inventory access method */
      Inventory& getInventory();

      const Inventory& getConstInventory() const;

      /* maximum encumbrance */
      unsigned int getMaxEncumbrance() const;

      /* maximum health */
      unsigned int getMaxHealth() const;

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
