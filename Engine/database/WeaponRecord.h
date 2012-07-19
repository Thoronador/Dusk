/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012 thoronador

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
 Date:    2010-05-31
 Purpose: WeaponBase Singleton class
          Represents a look-up table for all distinct Weapon objects
          within the game.

 History:
     - 2010-05-31 (rev 212) - initial version (by thoronador)
     - 2010-06-02 (rev 213) - getWeaponValue() and getWeaponWeight() added
     - 2010-06-06 (rev 215) - time between attacks/ weapon attack speed added
     - 2010-06-10 (rev 217) - tiny adjustment in getWeaponData()
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 287) - getFirst() and getEnd() added
     - 2011-05-17 (rev 290) - updateProjectilesAfterIDChange() added
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions
     - 2012-07-07 (rev 316) - update of WeaponRecord to be a descendant of
                              DataRecord
                            - WeaponBase removed (Database will handle that)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSK_WEAPONRECORD_H
#define DUSK_WEAPONRECORD_H

#include <string>
#include <fstream>
#include "DataRecord.h"

namespace Dusk
{

enum WeaponType {wtMelee, wtGun};

struct WeaponRecord: public DataRecord
{
    //path to mesh
    std::string Mesh;
    //item-related stuff
    std::string Name;
    int32_t value;
    float weight;
    //weapon type
    WeaponType Type;
    //weapon range (for melee weapons/ wtMelee)
    float Range;
    //time between two subsequent attacks
    float TimeBetweenAttacks;
    //projectile ID (for guns/ wtGun)
    std::string ProjectileID;
    /*damage (for melee weapons; guns get damage from projectiles)
      the damage is calculated as DamageTimes d DamageDice, e.g.
      1d6 for DamageTimes=1 and DamageDice=6
    */
    uint8_t DamageTimes;
    uint8_t DamageDice;

    /* returns an integer value that uniquely identifies this record's type */
    virtual uint32_t getRecordType() const;

    //record type identifier (usually the value returned by the above function)
    static const uint32_t RecordType;

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

} //namespace

#endif // DUSK_WEAPONRECORD_H
