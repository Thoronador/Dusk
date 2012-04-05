/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2012 thoronador

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
 Date:    2009-09-27
 Purpose: ContainerBase Singleton class
          Represents a look-up table for all distinct container objects
          within the game.

 History:
     - 2009-09-24 (rev 131) - initial version (by thoronador)
     - 2009-09-27 (rev 132) - NumberOfCointainers() added
                            - methods to load and save containers added
                              (LoadNextContainerFromStream(), SaveAllToStream())
     - 2010-01-01 (rev 148) - documentation update
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-21 (rev 257) - minor optimization
     - 2010-12-01 (rev 265) - use DuskLog/Messages class for logging
     - 2012-04-05 (rev 303) - non-existent IDs in query functions now get logged
                              to DuskLog and might throw exceptions

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef CONTAINERBASE_H
#define CONTAINERBASE_H

#include <string>
#include <map>
#include <fstream>
#include "Inventory.h"

namespace Dusk
{
  struct ContainerRecord
  {
    std::string Mesh;
    Inventory ContainerInventory;
  };

  class ContainerBase
  {
    public:
      /* destructor */
      virtual ~ContainerBase();
      ///singleton access method
      static ContainerBase& getSingleton();

      /* Adds Container prototype with given ID, the mesh _mesh and the contents of ''contents''.

         parameters:
             ID       - the ID of the new container
             _mesh    - the mesh path for the container
             contents - initial content of the container

         remarks:
             If a container with the same ID already exists, the old data will
             be replaced by the new data.
      */
      void addContainer(const std::string& ID, const std::string& _mesh, const Inventory& contents);

      #ifdef DUSK_EDITOR
      /* Deletes Container with given ID. Returns true, if such a Container
         was present, false otherwise.

         parameter:
             ID - ID of the container that should be deleted
      */
      bool deleteContainer(const std::string& ID);
      #endif

      /* Returns true, if a Container with ID ID is present, false otherwise.

         parameters:
             ID - ID of the container which will be checked
      */
      bool hasContainer(const std::string& ID) const;

      /* Returns the mesh of container with given ID, if present. If no
         container with the given ID is present, the function will return the
         error marker mesh's path, if UseMarkerOnError is true. Otherwise it
         will throw an exception.

         parameters:
             ID               - ID of the container whose mesh shall be returned
             UseMarkerOnError - If the requested container does not exist and
                                this parameter is set to true, the function will
                                return the path of the error marker mesh instead
                                of throwing an eexception.
      */
      std::string getContainerMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

      /* Returns the inventory of the container, if present.

         parameters:
             ID - ID of the container whose initial inventory shall be returned
      */
      const Inventory& getContainerInventory(const std::string& ID) const;

      /* Removes all containers from list. */
      void deleteAllContainers();

      /* Returns number of currently available containers */
      unsigned int numberOfContainers() const;

      /* Saves all Containers to stream; returns true on success

         parameters:
             OutStream - the output stream to which the object will be saved
      */
      bool saveAllToStream(std::ofstream& OutStream) const;

      /* Loads one(!) single container from stream; returns true on success,
         false otherwise. The data of the last loaded container is probably
         inconsistent after that function failed, so don't rely on it in that
         case.

         parameters:
             InStream - the input stream from which the container will be read
      */
      bool loadNextContainerFromStream(std::ifstream& InStream);

    private:
      /* constructor - private due to singleton pattern */
      ContainerBase();
      // copy constructor
      ContainerBase(const ContainerBase& op) {}
      std::map<std::string, ContainerRecord> m_ContainerList;
  };//class

}//namespace

#endif // CONTAINERBASE_H
