/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011 thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-21
 Purpose: NPCBase singleton class
          holds information about all distinct NPCs in the game

 History:
     - 2009-12-22 (rev 143) - initial version (by thoronador)
     - 2010-01-14 (rev 153) - load/ save functions
     - 2010-04-21 (rev 190) - female flag added
                            - GetFirst() and GetEnd() added
     - 2010-06-02 (rev 213) - information for animations and tag points added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 286) - iterator type for getFirst() and getEnd() added

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
    static NPCAttributes getNullAttributes();
  };
  /* struct to summarize all animations of NPC */
  struct NPCAnimations
  {
    //idle animation, i.e. when NPC is doing nothing
    std::string Idle;
    //walking animation
    std::string Walk;
    //animation for melee attack
    std::string MeleeAttack;
    //animation for ranged/ projectile attack
    std::string ProjectileAttack;
    //animation for jumping
    std::string Jump;
    //animation for dying
    std::string Death;
    static const NPCAnimations& getNullAnimations();
  }; //struct

  struct NPCTagPoints
  {
    //tag point for item in left hand
    std::string HandLeft;
    //tag point for item in right hand
    std::string HandRight;
    //tag point for sheathed weapon on the left side of the NPC's body
    std::string SheathLeft;
    //tag point for sheathed weapon on the right side of the NPC's body
    std::string SheathRight;
    static const NPCTagPoints& getNullTagPoints();
  }; //struct

  /* struct to summarize all properties of an NPC*/
  struct NPCRecord
  {
    std::string Name, Mesh;
    uint8 Level;
    NPCAttributes Attributes;
    bool Female;
    Inventory InventoryAtStart;
    NPCAnimations Animations;
    NPCTagPoints TagPoints;
  }; //struct

  #ifdef DUSK_EDITOR
  //iterator type for iterating through the NPCs
  typedef std::map<std::string, NPCRecord>::const_iterator NPCBaseIterator;
  #endif


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
      static NPCBase& getSingleton();

      /* destructor */
      virtual ~NPCBase();

      /* Adds a new NPC record which uses the given ID, name, mesh, etc. to the
         'table'.

         parameters:
             ID                - ID of the NPC
             Name              - name of the NPC (shown in game)
             Mesh              - mesh path of the NPC's mesh
             Level             - level of the NPC
             Attr              - record that contains the NPC's initial attributes
             female            - set this to true, if NPC is female
             StartingInventory - the initial inventory contents of the NPC
             Anims             - animations for that NPC
             TagPoints         - tag point names for that NPC

         remarks:
           Always succeeds (except if out of memory). If there already is a
           record for a NPC with the given ID, this record is overwritten.
           However, emtpy strings as ID, name or mesh are not allowed. In this
           case nothing will be added to NPCBase.
      */
      void addNPC(const std::string& ID, const std::string& Name,
                  const std::string& Mesh, const uint8 Level, const NPCAttributes& Attr,
                  const bool female, const Inventory& StartingInventory,
                  const NPCAnimations& Anims, const NPCTagPoints& TagPoints);

      /* Tries to delete the NPC record with the given ID. Returns true, if such
         a record was present, or false otherwise.

         parameters:
             NPC_ID - ID of the NPC data that has to be deleted
      */
      bool deleteNPC(const std::string& NPC_ID);

      /* Returns true, if a record for NPC 'NPC_ID' is present. */
      bool hasNPC(const std::string& NPC_ID) const;

      /* Deletes ALL entries.

         remarks:
           Only use that before realoading from a file, or if you really know
           what you are doing.
      */
      void clearAllNPCs();

      /* Returns the current number of NPC entries (for statistics only) */
      unsigned int numberOfNPCs() const;

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

      /* returns true, if the given NPC has is female/ has the female flag set.
         If the NPC with the given ID is not present, false is returned.
      */
      bool isNPCFemale(const std::string& NPC_ID) const;

      /* Returns the inventory of the NPC, if present. */
      const Inventory& getNPCInventory(const std::string& NPC_ID) const;

      /* returns the animations of the given NPC */
      const NPCAnimations& getNPCAnimations(const std::string& NPC_ID) const;

      /* returns the names of the tag points of the NPC with the given ID */
      const NPCTagPoints& getNPCTagPoints(const std::string& NPC_ID) const;

      /* Tries to save all data to the given stream and returns true on success,
         or false if an error occured.

         parameters:
             output - the output stream that is used to save the NPC data
      */
      bool saveToStream(std::ofstream& output) const;

      /* Tries to load next NPCRecord from stream and returns true on success,
         or false on failure.

         parameters:
             input - the input stream that is used to load the next NPC record
      */
      bool loadNextRecordFromStream(std::ifstream& input);

      #ifdef DUSK_EDITOR
      /* returns constant iterator to first element in NPC list*/
      NPCBaseIterator getFirst() const;

      /* returns constant iterator to end of NPC list*/
      NPCBaseIterator getEnd() const;
      #endif
    private:
      /* private constructor due to singleton pattern */
      NPCBase();

      /* empty, private copy constructor (singleton pattern) */
      NPCBase(const NPCBase& op){}
      std::map<std::string,NPCRecord> m_NPCList;
  };//class

} //namespace

#endif // NPCBASE_H
