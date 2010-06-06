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

class WeaponBase
{
  public:
    /* destructor */
    ~WeaponBase();

    /* singleton access method */
    static WeaponBase& GetSingleton();

    /* adds a weapon with the given ID and data.

       remarks:
           This function always succeeds, except for empty ID or mesh path, or
           some other invalid data. In that case, nothing is added.
           If a weapon record with the same ID already exists, it is
           overwritten by the new data.
    */
    void addWeapon(const std::string& ID, const WeaponRecord& data);

    /* returns true, if a weapon with the given ID is present */
    bool hasWeapon(const std::string& ID) const;

    /* Returns the mesh of weapon with given ID, if present. */
    std::string getWeaponMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

    /* returns the name of the given weapon, or an empty string if no weapon
       with that ID is present
    */
    std::string getWeaponName(const std::string& ID) const;

    /* returns the value of the weapon with ID weaponID, or -1 if no weapon with
       that ID is present.
    */
    int getWeaponValue(const std::string& weaponID) const;

    /* returns the weight of the weapon with ID weaponID, or zero if no weapon
       with that ID is present.

       remarks:
           Since zero is also a valid weight value for weaponss, a return value
           of zero is NOT a safe way to detect absence of a certain weapon.
    */
    float getWeaponWeight(const std::string& weaponID) const;

    /* returns all data of the weapon with the given ID. If no weapon with that
       ID is present, the return value is undefined
    */
    WeaponRecord getWeaponData(const std::string& ID) const;

    /* deletes a weapon by ID and returns true, if a weapon was deleted */
    bool deleteWeapon(const std::string& ID);

    /* deletes all present weapons */
    void ClearAll();

    /* returns the number of present weapons in the list */
    unsigned int NumberOfWeapons() const;

    /* tries to save all weapons to the stream and returns true on success */
    bool SaveAllToStream(std::ofstream& OutStream) const;

    /* Loads one(!) single weapon from stream; returns true on success, false
       otherwise. The data of the last loaded projectile is probably
       inconsistent after that function failed, so don't rely on it in that
       case.
    */
    bool LoadNextWeaponFromStream(std::ifstream& InStream);
  private:
    /* constructor - singleton */
    WeaponBase();

    /* private, empty copy constructor - singleton pattern */
    WeaponBase(const WeaponBase& op) {}

    std::map<std::string, WeaponRecord> m_Weapons;
}; //class

} //namespace

#endif // WEAPONBASE_H
