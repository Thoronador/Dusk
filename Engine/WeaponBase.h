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

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef WEAPONBASE_H
#define WEAPONBASE_H

#include <string>
#include <map>
#include <fstream>
#include "DuskTypes.h"

namespace Dusk
{

enum WeaponType {wtMelee, wtGun};

struct WeaponRecord
{
  //path to mesh
  std::string Mesh;
  //item-related stuff
  std::string Name;
  int value;
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
  uint8 DamageTimes;
  uint8 DamageDice;
}; //struct

#ifdef DUSK_EDITOR
//iterator type for iterating through the weapons
typedef std::map<std::string, WeaponRecord>::const_iterator WeaponBaseIterator;
#endif

class WeaponBase
{
  public:
    /* destructor */
    ~WeaponBase();

    /* singleton access method */
    static WeaponBase& getSingleton();

    /* adds a weapon with the given ID and data.

       remarks:
           This function always succeeds, except for empty ID or mesh path, or
           some other invalid data. In that case, nothing is added.
           If a weapon record with the same ID already exists, it is
           overwritten by the new data.
    */
    void addWeapon(const std::string& ID, const WeaponRecord& data);

    /* returns true, if a weapon with the given ID is present

       parameters:
           ID - the ID of the weapon
    */
    bool hasWeapon(const std::string& ID) const;

    /* Returns the mesh of weapon with given ID. If no weapon with the given ID
       is present, the function will throw an exception. However, if the
       UseMarkerOnError parameter is true, it will return an error marker mesh
       instead.

       parameters:
           ID               - ID of the weapon
           UseMarkerOnError - If set to true, the function will return an error
                              marker's mesh for non-existent weapons. Otherwise
                              it will throw for non-existent weapons.
    */
    const std::string& getWeaponMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

    /* returns the name of the given weapon, or an throws an exception if no
       weapon with that ID is present.

       parameters:
           ID - the ID of the weapon
    */
    const std::string& getWeaponName(const std::string& ID) const;

    /* returns the value of the weapon with ID weaponID, or throws an exception
       if no weapon with that ID is present.

       parameters:
           weaponID - the ID of the weapon whose value is requested
    */
    int getWeaponValue(const std::string& weaponID) const;

    /* returns the weight of the weapon with ID weaponID, or throws an exception
       if no weapon with that ID is present.

       parameters:
           weaponID - the ID of the weapons whose weight is requested
    */
    float getWeaponWeight(const std::string& weaponID) const;

    /* returns all data of the weapon with the given ID. If no weapon with that
       ID is present, the function will throw an exception.

       parameters:
           ID - the ID of the weapon
    */
    const WeaponRecord& getWeaponData(const std::string& ID) const;

    /* deletes a weapon by ID and returns true, if a weapon was deleted */
    bool deleteWeapon(const std::string& ID);

    /* deletes all present weapons */
    void clearAll();

    /* returns the number of present weapons in the list */
    unsigned int getNumberOfWeapons() const;

    /* tries to save all weapons to the stream and returns true on success

       parameters:
           OutStream - the output stream that is used to save the weapon data
    */
    bool saveAllToStream(std::ofstream& OutStream) const;

    /* Loads one(!) single weapon from stream; returns true on success, false
       otherwise. The data of the last loaded projectile is probably
       inconsistent after that function failed, so don't rely on it in that
       case.

       parameters:
           InStream - the input stream that is used to load the weapon data
    */
    bool loadNextWeaponFromStream(std::ifstream& InStream);


    #ifdef DUSK_EDITOR
    /* returns constant iterator to first element in weapon list*/
    WeaponBaseIterator getFirst() const;

    /* returns constant iterator to end of weapon list*/
    WeaponBaseIterator getEnd() const;

    /* updates the IDs of projectile-based guns after the ID of a projectile
       was changed in the editor

       parameters:
           oldProjID - the old ID of the projectile
           newProjID - the new ID of the projectile
    */
    void updateProjectilesAfterIDChange(const std::string& oldProjID, const std::string& newProjID);
    #endif
  private:
    /* constructor - singleton */
    WeaponBase();

    /* private, empty copy constructor - singleton pattern */
    WeaponBase(const WeaponBase& op) {}

    //internal list of weapons
    std::map<std::string, WeaponRecord> m_Weapons;
}; //class

} //namespace

#endif // WEAPONBASE_H
