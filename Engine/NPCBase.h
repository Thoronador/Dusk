/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-01-14
 Purpose: NPCBase singleton class
          holds information about all distinct NPCs in the game

 History:
     - 2009-12-22 (rev 143) - initial version (by thoronador)
     - 2010-01-14 (rev 153) - load/ save functions

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef NPCBASE_H
#define NPCBASE_H

#include <fstream>
#include <map>
#include <string>
#include "DuskTypes.h"
#include "Inventory.h"

namespace Dusk
{

  /* struct to summarize all attributes */
  struct NPCAttributes
  {
    uint8 Str, Agi, Vit, Int, Will, Cha, Luck;
    static NPCAttributes GetNullAttributes();
  };
  struct NPCRecord
  {
    std::string Name, Mesh;
    uint8 Level;
    NPCAttributes Attributes;
    Inventory InventoryAtStart;
  }; //struct


/* class NPCBase
         The purpose of this class is to hold the IDs of all distinct NPCs in
         the game, as well as their names, meshes, attributes, level, etc.

         Think of NPCBase as a sort of look-up table, e.g.:

             ID  |  Name    |  Mesh         | Level | Str | Agi | ...
           ------+----------+---------------+-------+-----+-----+ ...
           good  | Good Guy | cowboy.mesh   |   3   |  12 |  9  | ...
           bad   | Bad Guy  | bad.mesh      |   4   |  15 |  10 | ...
           ugly  | Ugly Guy | ugly_man.mesh |   5   |  17 |  11 | ...
            ...  | ...      | ...           | ...   | ... | ... | ...

         However, the attributes, the levels, and so on are only valid
         at the point of the NPC's creation in the game engine. Every
         'instance' of an NPC gets its own NPC class, which keeps track
         of changing attributes, inventory and so forth.
*/

  class NPCBase
  {
    public:
      /* static Singleton retrieval */
      static NPCBase& GetSingleton();
      virtual ~NPCBase();

      /* Adds a new NPC record which uses the given ID, name, mesh, etc. to the
         'table'.

         remarks:
           Always succeeds (except if out of memory). If there already is a
           record for a NPC with the given ID, this record is overwritten.
           However, emtpy strings as ID, name or mesh are not allowed. In this
           case nothing will be added to NPCBase.
      */
      void addNPC(const std::string& ID, const std::string& Name,
                  const std::string& Mesh, const uint8 Level, const NPCAttributes& Attr,
                  const Inventory& StartingInventory);

      /* Tries to delete the NPC record with the given ID. Returns true, if such
         a record was present, or false otherwise. */
      bool deleteNPC(const std::string& NPC_ID);

      /* Returns true, if a record for NPC 'NPC_ID' is present. */
      bool hasNPC(const std::string& NPC_ID) const;

      /* Deletes ALL entries.

         remarks:
           Only use that before realoading from a file, or if you really know
           what you are doing.
      */
      void ClearAllNPCs();

      /* Returns the current number of NPC entries (for statistics only) */
      unsigned int NumberOfNPCs() const;

      /* Returns the name of the NPC with ID 'NPC_ID', or an empty string if no
         such record is present.
      */
      std::string getNPCName(const std::string& NPC_ID) const;

      /* Returns the mesh name of the NPC with the given ID.

         params:
           UseMarkerOnError - if set to true (default), the function will return
                              a special error mesh name should no record for
                              'NPC_ID' exist. Otherwise it returns an empty
                              string in that case.
      */
      std::string getNPCMesh(const std::string& NPC_ID, const bool UseMarkerOnError=true) const;

      /* Returns the level of the requested NPC, or zero if no such record is
         present.
      */
      uint8 getLevel(const std::string& NPC_ID) const;

      /* Returns the attributes of the given NPC. If no record for that ID
         exists, a record with all attribute values set to zero is returned. */
      NPCAttributes getAttributes(const std::string& NPC_ID) const;

      /* Returns the inventory of the NPC, if present. */
      const Inventory& getNPCInventory(const std::string& NPC_ID) const;

      /* Tries to save all data to the given stream and returns true on success,
         or false if an error occured.
      */
      bool SaveToStream(std::ofstream& output) const;

      /* Tries to load next NPCRecord from stream and returns true on success,
         or false on failure.
      */
      bool LoadNextRecordFromStream(std::ifstream& input);
    private:
      NPCBase();
      NPCBase(const NPCBase& op){}
      std::map<std::string,NPCRecord> m_NPCList;
  };//class

} //namespace

#endif // NPCBASE_H
