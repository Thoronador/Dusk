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
    Weapon(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~Weapon();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes GetType() const;

    /* returns true, if the player (or another NPC) can pick up the weapon */
    virtual bool canPickUp() const;

    /* Saves the weapon to the given stream. Returns true on success, false
       otherwise.
    */
    virtual bool SaveToStream(std::ofstream& OutStream) const;

    /* Tries to load a weapon from the given stream. Returns true on
       success, false otherwise.

       remarks:
           If the function returns false, the data within the weapon may be
           corrupted. It's advised not to use the weapon in this case.
    */
    virtual bool LoadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string GetObjectMesh() const;
}; //class

} //namespace

#endif // WEAPON_H
