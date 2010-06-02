/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-06
 Purpose: Weapon class
          represents a single weapon within the game

 History:
     - 2010-06-02 (rev 213) - initial version (by thoronador)

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

    /* Enables the object, i.e. tells the SceneManager to display it.
       Returns true on success, false on error.

       remarks:
           Derived classes potentially implement their individual versions of
           this function.
    */
    virtual bool Enable(Ogre::SceneManager* scm);

    /* Creates an entity of the object, but does not attach it to anny scene
       node. Returns true on success, false on error.

       remarks:
           This is only used in NPC::equip(), because only objects without
           parent nodes can be attached to a bone.
           Derived classes potentially implement their individual versions of
           this function.
    */
    virtual bool EnableWithoutSceneNode(Ogre::SceneManager* scm);

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
}; //class

} //namespace

#endif // WEAPON_H
