/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2012 thoronador

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
 Date:    2010-02-06
 Purpose: Weapon class
          represents a single weapon within the game

 History:
     - 2010-06-02 (rev 213) - initial version (by thoronador)
     - 2010-08-01 (rev 220) - GetObjectMesh() added
                            - Enable() and EnableWithoutSceneNode() removed
                              (inherited method is used instead)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-11-26 (rev 260) - canCollide() added (always returns true)
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition

 ToDo list:
     - add possibility to actually attack and inflict damage with a weapon
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef WEAPON_H
#define WEAPON_H

#include "Item.h"

namespace Dusk
{

class Weapon: public Item
{
  public:
    /* simple constructor */
    Weapon();

    /* constructor with initial values */
    Weapon(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~Weapon();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes getDuskType() const;

    /* returns true, if the object shall be considered during collision
       detection

       remarks:
           Will always return true for weapons.
    */
    virtual bool canCollide() const;

    /* returns true, if the player (or another NPC) can pick up the weapon */
    virtual bool canPickUp() const;

    /* Saves the weapon to the given stream. Returns true on success, false
       otherwise.

       parameters:
           OutStream - the output stream that is used to save the weapon
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Tries to load a weapon from the given stream. Returns true on
       success, false otherwise.

       parameters:
           InStream - the input stream that is used to load the weapon from

       remarks:
           If the function returns false, the data within the weapon may be
           corrupted. It's advised not to use the weapon in this case.
    */
    virtual bool loadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual const std::string& getObjectMesh() const;
}; //class

} //namespace

#endif // WEAPON_H
