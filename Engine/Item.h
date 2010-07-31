/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-02-06
 Purpose: Item class
          represents a single item within the game

 History:
     - 2010-02-06 (rev 165) - initial version (by thoronador)
     - 2010-06-02 (rev 213) - isEquipped() and setEquipped() added
                            - possibility to enable object without a scene node
                              added
     - 2010-07-31 (rev 220) - GetObjectMesh() added
                            - Enable() removed (uses inherited method instead)

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

    /* Creates an entity of the object, but does not attach it to anny scene
       node. Returns true on success, false on error.

       remarks:
           This is only used in NPC::equip(), because only objects without
           parent nodes can be attached to a bone.
           Derived classes potentially implement their individual versions of
           this function.
    */
    virtual bool EnableWithoutSceneNode(Ogre::SceneManager* scm);

    /* Disables the object, i.e. tells the SceneManager not to display it.
       Returns true on success, false on error. */
    virtual bool Disable();

    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes GetType() const;

    /* returns true, if the player can pick up the object

       remarks:
           This function will always return true for Item (and possibly for its
           derived classes, too, unless these classes provide their own
           implementation of this function) as long as the item is not equipped.
    */
    virtual bool canPickUp() const;

    /* returns true, if the item is currently equipped by an NPC */
    bool isEquipped() const;

    /* change the equipped status */
    void setEquipped(const bool value);

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

    /* returns the Ogre entity which is used to display that object */
    Ogre::Entity* exposeEntity() const;
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string GetObjectMesh() const;

    /* Helper function which saves all data in an Item to the given stream.
       Returns true on success.

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of SaveToStream().
    */
    bool SaveItemPart(std::ofstream& output) const;

    /* Helper function which loads all data in an Item from the given stream.
       Returns true on success.

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of LoadFromStream().
    */
    bool LoadItemPart(std::ifstream& input);

    //indicates whether a NPC has this item equipped
    bool m_Equipped;
}; //class

} //namespace

#endif // ITEM_H
