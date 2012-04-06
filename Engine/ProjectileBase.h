/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012 thoronador

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
 Date:    2010-05-30
 Purpose: ProjectileBase Singleton class
          Represents a look-up table for all distinct Projectile objects
          within the game.

 History:
     - 2010-05-30 (rev 210) - initial version (by thoronador)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging
     - 2011-05-11 (rev 286) - getFirst() and getEnd() added
     - 2012-04-06 (rev 304) - non-existent IDs in get-function will now throw
                              exceptions

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef PROJECTILEBASE_H
#define PROJECTILEBASE_H

#include <string>
#include <map>
#include <fstream>
#include "DuskTypes.h"

namespace Dusk
{

  struct ProjectileRecord
  {
    std::string Mesh;
    float DefaultTTL;
    float DefaultVelocity;
    //damage
    uint8 times, dice;
  }; //struct

  #ifdef DUSK_EDITOR
  //iterator type for iterating through the projectiles
  typedef std::map<std::string, ProjectileRecord>::const_iterator ProjectileBaseIterator;
  #endif

  /* class ProjectileBase
         The purpose of this class is to hold the IDs of all distinct
         projectiles in the game, as well as their meshes and attributes.

         Think of ProjectileBase as a sort of look-up table, e.g.:

             ID   |  Mesh           | TTL | Velocity | times | dice
           -------+-----------------+-----+----------+-------+------
           arrow  | wooden.mesh     | 5.0 |  123.45  |   4   |   6
           bullet | bullet.mesh     | 8.9 |  678.9   |  10   |  12
           sonic  | sonic_gun.mesh  | 0.1 |  330.0   |  20   |  20
            ...   | ...             | ... | ...      | ...   | ...

  */

  class ProjectileBase
  {
    private:
      std::map<std::string, ProjectileRecord> m_Projectiles;

      /* constructor */
      ProjectileBase();

      /* copy constructor - empty */
      ProjectileBase(const ProjectileBase& op) {}
    public:
      /* destructor */
      ~ProjectileBase();

      /* Singleton access */
      static ProjectileBase& getSingleton();

      /* adds a new projectile with the given data

         parameters:
             ID       - the ID of the new projectile
             mesh     - path to the projectile's mesh
             TTL      - time to live of the projectile
             velocity - speed of movement
             n, dice  - the number and type of dice that are used to calculate
                        the amount of damage the prjectile inflicts

         remarks:
             This function always succeeds, except for empty ID or mesh path.
             In that case, nothing is added.
             If a projectile record with the same ID already exists, it is
             overwritten by the new data.
      */
      void addProjectile(const std::string& ID, const std::string& mesh,
                         const float TTL, const float velocity, const uint8 n,
                         const uint8 dice);

      /* same as above, but with struct for data

         parameters:
             ID   - the ID of the new projectile
             data - record holding the projectile's data
      */
      void addProjectile(const std::string& ID, const ProjectileRecord& data);

      /* returns true, if a projectile with the given ID is present

         parameter(s):
             ID - ID of the projectile
      */
      bool hasProjectile(const std::string& ID) const;

      /* Returns the mesh of projectile with given ID, if present. If no record
         for the requested ID is present, the function throws an exception.
         However, if UseMarkerOnError is set to true, the function will return
         the path of a predefined error marker mesh instead.

         parameters:
             ID               - the ID of the projectile whose mesh path is
                                requested
             UseMarkerOnError - if set to true, requests for non-existent IDs
                                will return the path of an error marker. If set
                                to false, the function will throw instead.
      */
      const std::string& getProjectileMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

      /* Returns the data of projectile with given ID, if present. If no record
         for the requested ID is present, the function will throw an exception.

         parameter(s):
             ID - ID of the projectile
      */
      const ProjectileRecord& getProjectileData(const std::string& ID) const;

      /* deletes a projectile by ID and returns true, if a projectile was deleted

         parameters:
             ID - ID of the projectile that should be deleted
      */
      bool deleteProjectile(const std::string& ID);

      /* deletes all present projectiles */
      void clearAll();

      /* returns the number of present projectiles in the list */
      unsigned int getNumberOfProjectiles() const;

      /* Saves all Projectiles to stream and returns true on success

         parameters:
             OutStream - the output stream that is used to save the projectile
                         data
      */
      bool saveAllToStream(std::ofstream& OutStream) const;

      /* Loads one(!) single projectile from stream; returns true on success,
         false otherwise. The data of the last loaded projectile is probably
         inconsistent after that function failed, so don't rely on it in that
         case.

         parameters:
             InStream - the input stream that is used to load the next
                        projectile record from
      */
      bool loadNextProjectileFromStream(std::ifstream& InStream);

      #ifdef DUSK_EDITOR
      /* returns constant iterator to first element in projectile list*/
      ProjectileBaseIterator getFirst() const;

      /* returns constant iterator to end of projectile list*/
      ProjectileBaseIterator getEnd() const;
      #endif
  }; //class

} //namespace

#endif // PROJECTILEBASE_H
