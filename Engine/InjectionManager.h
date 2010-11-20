/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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
 Date:    2010-05-06
 Purpose: InjectionManager Singleton class
          Holds all animated objects within the game and makes sure they all
          get their animations played properly by injecting the frame time.

 History:
     - 2009-09-10 (rev 127) - initial version (by thoronador), contains
                              addReference(), InjectAnimationTime() and
                              ClearData() functions
     - 2009-09-13 (rev 128) - private copy constructor added
     - 2009-12-31 (rev 147) - documentation update
     - 2010-01-16 (rev 154) - update for NPC (descendant of AnimatedObject)
                            - SaveAllToStream(), LoadNextFromStream() added
     - 2010-02-10 (rev 171) - moved from vector of references to map of vectors
                              of references to allow faster search for a certain
                              reference
                            - GetAnimatedObjectReference() added
     - 2010-04-28 (rev 191) - deleteReferencesOfAnimatedObject() added
     - 2010-05-06 (rev 198) - small improvements
     - 2010-05-20 (rev 205) - adjustments for new object hierarchy
     - 2010-05-21 (rev 206) - GetInjectionObjectReference() added
     - 2010-05-31 (rev 211) - requestDeletion() and performRequestedDeletions(),
                              addProjectileReference() added
                            - adjustments for Projectiles
     - 2010-06-02 (rev 213) - adjustments for weapons
     - 2010-08-18 (rev 230) - renamed AnimationData to InjectionManager
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-09-13 (rev 242) - reenableReferencesOfObject() and
                              updateReferencesAfterIDChange() added (Editor only)
     - 2010-09-22 (rev 243) - addVehicleReference() added; adjustments for new
                              Vehicle class
     - 2010-11-12 (rev 252) - getBegin() and getEnd() added
     - 2010-11-20 (rev 255) - rotation is now stored as quaternion

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef INJECTIONMANAGER_H
#define INJECTIONMANAGER_H

#include "Projectile.h"
#include "AnimatedObject.h"
#include "NPC.h"
#include <fstream>
#include <vector>
#include <map>

namespace Dusk
{
  class Vehicle; //forward declaration for Vehicle

  class InjectionManager
  {
    public:
      //iterator type for use in other classes
      typedef std::map<std::string, std::vector<InjectionObject*> >::const_iterator ConstMapIterator;

      /* destructor */
      virtual ~InjectionManager();

      /* singleton access function */
      static InjectionManager& getSingleton();

      /* returns the number of object references the class holds */
      unsigned int getNumberOfReferences() const;

      /* adds a new AnimatedObject with given ID at given position with rotation
         and scale, and returns a pointer to the created AnimatedObject.

         parameters:
             ID       - ID of the new animated object
             position - position of the object
             rotation - rotation of the object
             scale    - scaling factor of the object
      */
      AnimatedObject* addAnimatedReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Quaternion& rotation, const float scale);

      /* adds a new NPC with given ID at given position with rotation and scale.
         Returns a pointer to the created NPC.

      parameters:
             ID       - ID of the new NPC
             position - position of the NPC
             rotation - rotation of the NPC
             scale    - scaling factor of the NPC

      */
      NPC* addNPCReference(const std::string& ID, const Ogre::Vector3& position,
                           const Ogre::Quaternion& rot, const float Scale);

      /* adds a new Vehicle with given ID at given position with rotation and
         scale, and returns a pointer to the created Vehicle.

         parameters:
             ID       - ID of the new Vehicle
             position - position of the Vehicle
             rotation - rotation of the Vehicle
             scale    - scaling factor of the Vehicle
      */
      Vehicle* addVehicleReference(const std::string& ID, const Ogre::Vector3& position,
                                   const Ogre::Quaternion& rotation, const float scale);

      /* adds a new WaypointObject with given ID at given position with rotation
         and scale, and returns a pointer to the created WaypointObject.

         parameters:
             ID       - ID of the new waypoint object
             position - position of the waypoint object
             rotation - rotation of the waypoint object
             scale    - scaling factor of the waypoint object
      */
      WaypointObject* addWaypointReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Quaternion& rotation, const float scale);

      /* adds a new Projectile with given ID at given position with rotation
         and scale, and returns a pointer to the created WaypointObject.

         parameters:
             ID       - ID of the new projectile
             position - position of the object
             rotation - rotation of the object
             scale    - scaling factor of the object
      */
      Projectile* addProjectileReference(const std::string& ID, const Ogre::Vector3& position,
                                         const Ogre::Quaternion& rotation, const float scale);

      /* returns a pointer to the first object reference with the given ID, or
         NULL if no such object is present
      */
      InjectionObject* getInjectionObjectReference(const std::string& ID) const;

      /* returns a pointer to the first animated object reference (including NPC)
         with the given ID, or NULL if no such object is present
      */
      AnimatedObject* getAnimatedObjectReference(const std::string& ID) const;

      /* same as GetAnimatedObjectReference, but with check for NPC -> if the
         referenced object is not a NPC, it returns NULL
      */
      NPC* getNPCReference(const std::string& ID) const;

      #ifdef DUSK_EDITOR
      /* Deletes all objects with the given ID and returns the number of deleted objects.

         remarks:
             This function should not be called in-game. It is only used by the Editor.
      */
      unsigned int deleteReferencesOfAnimatedObject(const std::string& del_ID);

      /* Disables and re-enables all currently enabled objects of given ID.
         Returns the number of objects that were re-enabled.

         remarks:
             This is the method to update all enabled objects of one ID after
             the mesh path has changed. This will not happen in-game and is
             only used by the Editor.
      */
      unsigned int reenableReferencesOfObject(const std::string& ID, Ogre::SceneManager * scm);

      /* Changes the ID of all objects with ID oldID to newID and returns the
         number of objects that were updated

         remarks:
             Do not call this manually or in-game.
             Method is used to update all references of an object after the ID
             was changed (by Editor application).
      */
      unsigned int updateReferencesAfterIDChange(const std::string& oldID, const std::string& newID, Ogre::SceneManager* scm);
      #endif

      /* makes all objects move according to the amount of time passed

         parameters:
             TimePassed - the amount of time that has passed since last frame,
                          measured in seconds
      */
      void injectAnimationTime(const float TimePassed);

      /* Tries to save all references to the stream. Returns true on success.

         parameters:
             output - the output stream that is used to save the data
      */
      bool saveAllToStream(std::ofstream& output) const;

      /* Tries to load the next object from stream and returns true on success

         parameters:
             Stream           - the input stream that is used to load the data
             PrefetchedHeader - the first four bytes of the record to come
      */
      bool loadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader);

      /* deletes all referenced objects */
      void clearData();

      /* notify InjectionManager about an object that wants to be deleted

         parameters:
             objPtr - pointer to the InjectionObject that wants to be deleted

         remarks:
             An object that requested to be deleted will be deleted during the
             next frame after that call. The pointer objPtr should not be used
             after that!
             The same object MUST NOT request deletion more than one time,
             because doing so might cause a memory access violation.
      */
      void requestDeletion(InjectionObject* objPtr);

      /* returns a constant iterator that points to the begin of the internal
         map of objects
      */
      ConstMapIterator getBegin() const;

      /* returns a constant iterator that points to the end of the internal
         map of objects
      */
      ConstMapIterator getEnd() const;
    private:
      /* private constructor (singleton pattern) */
      InjectionManager();
      /* empty, private copy constructor due to singleton pattern*/
      InjectionManager(const InjectionManager& op) {}

      std::map<std::string, std::vector<InjectionObject*> > m_ReferenceMap;
      unsigned int m_RefCount;

      /* vector to hold the objects which requested to be deleted */
      std::vector<InjectionObject*> m_DeletionObjects;

      /* deletes all objects that previously requested to be deleted and have
         not been deleted yet
      */
      void performRequestedDeletions();
  };//class
}//namespace

#endif // INJECTIONMANAGER_H
