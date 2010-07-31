/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-30
 Purpose: ProjectileBase Singleton class
          Represents a look-up table for all distinct Projectile objects
          within the game.

 History:
     - 2010-05-30 (rev 210) - initial version (by thoronador)

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
      static ProjectileBase& GetSingleton();

      /* adds a new projectile with the given data

         remarks:
           This function always succeeds, except for empty ID or mesh path. In
           that case, nothing is added.
           If a projectile record with the same ID already exists, it is
           overwritten by the new data.
      */
      void addProjectile(const std::string& ID, const std::string& mesh,
                         const float TTL, const float velocity, const uint8 n,
                         const uint8 dice);

      /* same as above, but with struct for data */
      void addProjectile(const std::string& ID, const ProjectileRecord& data);

      /* returns true, if a projectile with the given ID is present */
      bool hasProjectile(const std::string& ID) const;

      /* Returns the mesh of projectile with given ID, if present. */
      std::string getProjectileMesh(const std::string& ID, const bool UseMarkerOnError=true) const;

      /* Returns the data of projectile with given ID, if present. */
      ProjectileRecord getProjectileData(const std::string& ID) const;

      /* deletes a projectile by ID and returns true, if a projectile was deleted */
      bool deleteProjectile(const std::string& ID);

      /* deletes all present projectiles */
      void ClearAll();

      /* returns the number of present projectiles in the list */
      unsigned int GetNumberOfProjectiles() const;

      /* Saves all Projectiles to stream and returns true on success */
      bool SaveAllToStream(std::ofstream& OutStream) const;

      /* Loads one(!) single projectile from stream; returns true on success,
         false otherwise. The data of the last loaded projectile is probably
         inconsistent after that function failed, so don't rely on it in that
         case.
      */
      bool LoadNextProjectileFromStream(std::ifstream& InStream);
  }; //class

} //namespace

#endif // PROJECTILEBASE_H