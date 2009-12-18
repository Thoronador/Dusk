#ifndef NPC_H
#define NPC_H

#include <string>
#include <OgreVector3.h>
#include "AnimatedObject.h"
#include "Inventory.h"
#include "DuskTypes.h"

namespace Dusk
{
  class NPC: public AnimatedObject
  {
    public:
      NPC(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale);
      virtual ~NPC();
      virtual ObjectTypes GetType() const;

      float getHealth() const;
      void setHealth(const float new_health);
      bool isAlive() const;
      /*attribute set/get methods */
      uint8 getLevel() const;
      uint8 getStrength() const;
      uint8 getAgility() const;
      uint8 getVitality() const;
      uint8 getIntelligence() const;
      uint8 getCharisma() const;
      uint8 getLuck() const;
      void setLevel(const uint8 new_level);
      void setStrength(const uint8 str);
      void setAgility(const uint8 agi);
      void setVitality(const uint8 vit);
      void setIntelligence(const uint8 intelligence);
      void setCharisma(const uint8 cha);
      void setLuck(const uint8 luck);

      /* inventory access method */
      Inventory& getInventory();

      /* maximum encumbrance */
      unsigned int getMaxEncumbrance() const;

      /* saves NPC to given stream and returns true on success,
         false otherwise */
      virtual bool SaveToStream(std::ofstream& OutStream) const;

      /* Loads contents of inventory from stream and returns true on success,
         false otherwise. The Inventory content is probably inconsistent after
         that function failed, so don't rely on its contents in that case. */
      virtual bool LoadFromStream(std::ifstream& InStream);

      /* still to do:
         -Enable, and possibly Disable and isEnabled
         -implement LoadFromStream, SaveToStream
      */
    protected:
      Inventory m_Inventory;
      float m_Health;
      //level
      uint8 m_Level;
      //attributes
      uint8 m_Strength, m_Agility, m_Vitality, m_Intelligence, m_Charisma, m_Luck;
  }; //class

} //namespace

#endif // NPC_H
