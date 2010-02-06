/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-06
 Purpose: Item class
          represents a single item within the game

 History:
     - 2010-02-06 (rev 165) - initial version (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef ITEM_H
#define ITEM_H

#include "DuskObject.h"

namespace Dusk
{

class Item: public DuskObject
{
  public:
    /* simple constructor */
    Item();

    /* constructor with initial values */
    Item(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~Item();

    /* Enables the object, i.e. tells the SceneManager to display it.
       Returns true on success, false on error.

       remarks:
           Derived classes potentially implement their individual versions of
           this function.
    */
    virtual bool Enable(Ogre::SceneManager* scm);

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes GetType() const;

    /* returns true, if the player can pick up the object

       remarks:
           This function will always return true for Item (and possibly for its
           derived classes, too, unless these classes provide their own
           implementation of this function.
    */
    virtual bool canPickUp() const;

    /* Saves the item to the given stream. Returns true on success, false
       otherwise.

       remark:
           Every derived class has to have its own implementation of this
           function to ensure the derived object is saved properly.
    */
    virtual bool SaveToStream(std::ofstream& OutStream) const;

    /* Tries to load an item from the given stream. Returns true on
       success, false otherwise.

       remarks:
           Every derived class has to have its own implementation of this
           function to ensure the derived object is saved properly.
           If the function returns false, the data within the item may be
           corrupted. It's advised not to use the item in this case.
    */
    virtual bool LoadFromStream(std::ifstream& InStream);
}; //class

} //namespace

#endif // ITEM_H
