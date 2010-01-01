/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2009-12-05
 Purpose: Container class
          represents a single container object within the game

 History:
     - 2009-09-24 (rev 131) - initial version (by thoronador)
     - 2009-09-27 (rev 132) - SaveToStream(), LoadFromStream() added
     - 2009-12-05 (rev 139) - update of GetType()
     - 2009-12-08 (rev 140) - TransferAllContentTo() renamed
     - 2010-01-01 (rev 148) - documentation update

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef CONTAINER_H
#define CONTAINER_H
#include "DuskObject.h"
#include "Inventory.h"
#include <string>
#include <OgreVector3.h>

namespace Dusk
{
  class Container: public DuskObject
  {
    public:
      Container();
      Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);
      virtual ~Container();

      /* returns true, if the container is empty */
      bool IsEmpty() const;

      /* adds all items in the container to Inventory target and empties
         container afterwards

         remarks:
             will not work as intended, if target is the calling container object
      */
      void TransferAllItemsTo(Inventory& target);

      /* Adds count items of ID ItemID to the container */
      void AddItem(const std::string& ItemID, const unsigned int count);

      /* Tries to remove count items of ID ItemID from the container. If the
         container does not have that many items of ItemID, all available items
         with that ID are removed. Returns the number of items that were
         removed.
      */
      unsigned int RemoveItem(const std::string& ItemID, const unsigned int count);

      /* Returns the number of items with ID ItemID within the container */
      unsigned int GetItemCount(const std::string& ItemID) const;

      /* enables container object, i.e. makes it visible in-game.
         Returns true on success, false otherwise.
      */
      virtual bool Enable(Ogre::SceneManager* scm);

      /* returns object type (in this case otContainer) */
      virtual ObjectTypes GetType() const;

      /* Tries to save the container data to OutStream and returns true on
         success, false otherwise.
      */
      virtual bool SaveToStream(std::ofstream& OutStream) const;

      /* Tries to load the container data from stream InStream and returns true
         on success, false otherwise.

         remarks:
             The container may have inconsistent data, if the function fails.
      */
      virtual bool LoadFromStream(std::ifstream& InStream);
    private:
      Inventory m_Contents;
      bool m_Changed;
  };//class

}//namespace

#endif // CONTAINER_H
