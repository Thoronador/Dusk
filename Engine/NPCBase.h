/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2011, 2012 thoronador

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
 Purpose: NPCRecord class
          holds information about one distinct NPC type in the game

 History:
     - 2009-12-22 (rev 143) - initial version (by thoronador)
     - 2010-01-14 (rev 153) - load/ save functions
     - 2010-04-21 (rev 190) - female flag added
                            - GetFirst() and GetEnd() added
     - 2010-06-02 (rev 213) - information for animations and tag points added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-04 (rev 267) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 286) - iterator type for getFirst() and getEnd() added
     - 2011-05-11 (rev 287) - renamed numberOfNPCs() to getNumberOfNPCs()
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions
     - 2012-07-05 (rev 313) - update of NPCRecord to be a descendant of
                              DataRecord
                            - NPCBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_NPCRECORD_H
#define DUSK_NPCRECORD_H

#include <fstream>
#include "DataRecord.h"
#include "Inventory.h"

namespace Dusk
{

  /* struct to summarize all attributes */
  struct NPCAttributes
  {
    uint8_t Str, Agi, Vit, Int, Will, Cha, Luck;
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
  struct NPCRecord: public DataRecord
  {
    std::string Name, Mesh;
    uint8_t Level;
    NPCAttributes Attributes;
    bool Female;
    Inventory InventoryAtStart;
    NPCAnimations Animations;
    NPCTagPoints TagPoints;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const;

    /* Tries to save the data record to stream outStream and returns true on
       success, false otherwise.

       parameters:
           outStream - the output stream to which the data will be saved
    */
    virtual bool saveToStream(std::ofstream& outStream) const;

    /* Tries to load the data record from stream inStream and returns true on
       success, false otherwise.

       parameters:
           inStream - the input stream from which the data will be read

       remarks:
           The record may have inconsistent data, if the function fails.
    */
    virtual bool loadFromStream(std::ifstream& inStream);
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

} //namespace

#endif // DUSK_NPCRECORD_H
