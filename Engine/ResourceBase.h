/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012  thoronador

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
 Date:    2012-06-24
 Purpose: ResourceBase Singleton class
          Represents a look-up table for all distinct resource objects
          within the game.

 History:
     - 2012-06-24 (rev 306) - initial version (by thoronador)
     - 2012-07-02 (rev 310) - fixed iterator type

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef RESOURCEBASE_H
#define RESOURCEBASE_H

#include <string>
#include <map>

namespace Dusk
{

  struct ResourceRecord
  {
    std::string meshSpawned;
    std::string meshHarvested;
    std::string harvestItem; //ID of the item the NPC will get from that resource
    float respawnInterval; //respawn interval in seconds
    std::string harvestSound; //ID of sound that is played while harvesting the resource
  };//struct

  /* class ResourceBase
         The purpose of this class is to hold the IDs of all distinct
         resources in the game, as well as their meshes and attributes.

         Think of ResourceBase as a sort of look-up table, e.g.:

             ID   |  mesh spawned   | mesh harv.      | item      | respawn | ...
           -------+-----------------+-----------------+-----------+---------+----
           coal   | coal.mesh       | coal_taken.mesh | coal_lump | inf     | ...
            ...   | ...             | ...             | ...       | ...     | ...

  */
  class ResourceBase
  {
    private:
      std::map<std::string, ResourceRecord> m_Resources;

      /* constructor */
      ResourceBase();

      /* copy constructor - empty */
      ResourceBase(const ResourceBase& op) {}
    public:
      /* destructor */
      ~ResourceBase();

      /* Singleton access */
      static ResourceBase& getSingleton();

      /* adds a new resource with the given data

         parameters:
             ID   - the ID of the new resource type
             data - its data

         remarks:
             This function always succeeds, except for empty ID or mesh path.
             In that case, nothing is added.
             If a resource record with the same ID already exists, it is
             overwritten by the new data.
      */
      void addResource(const std::string& ID, const ResourceRecord& data);

      /* returns true, if a resource with the given ID is present

         parameter(s):
             ID - ID of the resource
      */
      bool hasResource(const std::string& ID) const;

      /* Returns the data of resource with given ID, if present. If no record
         for the requested ID is present, the function will throw an exception.

         parameter(s):
             ID - ID of the resource
      */
      const ResourceRecord& getResourceData(const std::string& ID) const;

      /* deletes a resource by ID and returns true, if a resource was deleted

         parameters:
             ID - ID of the resource that should be deleted
      */
      bool deleteResource(const std::string& ID);

      /* deletes all present resources */
      void clearAll();

      /* returns the number of present resources in the list */
      unsigned int getNumberOfResources() const;

      /* saves all resources to stream and returns true on success

         parameters:
             outStream - the output stream that is used to save the resource
                         data
      */
      bool saveAllToStream(std::ofstream& outStream) const;

      /* Loads one(!) single resource from stream; returns true on success,
         false otherwise. The data of the last loaded resource is probably
         inconsistent after that function failed, so don't rely on it in that
         case.

         parameters:
             InStream - the input stream that is used to load the next
                        resource record from
      */
      bool loadNextResourceFromStream(std::ifstream& InStream);

      #ifdef DUSK_EDITOR
      //iterator type for iterating through the projectiles
      typedef std::map<std::string, ResourceRecord>::const_iterator Iterator;

      /* returns constant iterator to first element in resource list*/
      Iterator getFirst() const;

      /* returns constant iterator to end of resource list*/
      Iterator getEnd() const;
      #endif
  };//class

}//namespace

#endif // RESOURCEBASE_H
