/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012  thoronador

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
 Date:    2010-01-17
 Purpose: Container class
          represents a single container object within the game

 History:
     - 2009-09-24 (rev 131) - initial version (by thoronador)
     - 2009-09-27 (rev 132) - SaveToStream(), LoadFromStream() added
     - 2009-12-05 (rev 139) - update of GetType()
     - 2009-12-08 (rev 140) - TransferAllContentTo() renamed
     - 2010-01-01 (rev 148) - documentation update
     - 2010-01-17 (rev 156) - update of SaveToStream() and LoadFromStream()
     - 2010-07-31 (rev 220) - GetObjectMesh() added
                            - Enable() removed (uses inherited method instead)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as quaternion
     - 2010-11-26 (rev 260) - canCollide() added (always returns true)
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition
     - 2012-07-05 (rev 313) - update to use Database instead of ContainerBase

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
      Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);
      virtual ~Container();

      /* returns true, if the container is empty */
      bool isEmpty() const;

      /* adds all items in the container to Inventory target and empties
         container afterwards

         parameters:
             target - the inventory that will receive this container's items

         remarks:
             will not work as intended, if target is the calling container object
      */
      void transferAllItemsTo(Inventory& target);

      /* Adds count items of ID ItemID to the container

         parameters:
             ItemID - the ID of the item that will be added to the container
             count  - amount of items that shall be added
      */
      void addItem(const std::string& ItemID, const unsigned int count);

      /* Tries to remove count items of ID ItemID from the container. If the
         container does not have that many items of ItemID, all available items
         with that ID are removed. Returns the number of items that were
         removed.

         parameters:
             ItemID - the ID of the item that will be removed from the container
             count  - amount of items that shall be removed

         returns:
             the number of items that were actually removed from this container.
             This number can be smaller than count, if there are not count items
             of the given ID.
      */
      unsigned int removeItem(const std::string& ItemID, const unsigned int count);

      /* Returns the number of items with ID ItemID within the container */
      unsigned int getItemCount(const std::string& ItemID) const;

      /* returns true, if the object shall be considered during collision
         detection

         remarks:
             Will always return true for containers.
      */
      virtual bool canCollide() const;

      /* returns object type (in this case otContainer) */
      virtual ObjectTypes getDuskType() const;

      /* Tries to save the container data to OutStream and returns true on
         success, false otherwise.

         parameters:
               OutStream - the output stream to which the object will be saved
      */
      virtual bool saveToStream(std::ofstream& OutStream) const;

      /* Tries to load the container data from stream InStream and returns true
         on success, false otherwise.

         parameters:
             InStream - the input stream from which the data will be read

         remarks:
             The container may have inconsistent data, if the function fails.
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

      //holds the contents of this container
      Inventory m_Contents;
      //flag that indicates whether this container's content was changed
      bool m_Changed;
  };//class

}//namespace

#endif // CONTAINER_H
