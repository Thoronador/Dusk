/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-06
 Purpose: AnimationData Singleton class
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

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

#include "Projectile.h"
#include "AnimatedObject.h"
#include "NPC.h"
#include <fstream>
#include <vector>
#include <map>

namespace Dusk
{
  class AnimationData
  {
    public:
      /* destructor */
      virtual ~AnimationData();

      /* singleton access function */
      static AnimationData& GetSingleton();

      /* returns the number of object references the class holds */
      unsigned int NumberOfReferences() const;

      /* adds a new AnimatedObject with given ID at given position with rotation
         and scale, and returns a pointer to the created AnimatedObject. */
      AnimatedObject* addAnimatedReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Vector3& rotation, const float scale);

      /* adds a new NPC with given ID at given position with rotation and scale.
         Returns a pointer to the created NPC. */
      NPC* addNPCReference(const std::string& ID, const Ogre::Vector3& position,
                           const Ogre::Vector3& rot, const float Scale);

      /* adds a new WaypointObject with given ID at given position with rotation
         and scale, and returns a pointer to the created WaypointObject. */
      WaypointObject* addWaypointReference(const std::string& ID, const Ogre::Vector3& position,
                                           const Ogre::Vector3& rotation, const float scale);

      /* adds a new Projectile with given ID at given position with rotation
         and scale, and returns a pointer to the created WaypointObject.
      */
      Projectile* addProjectileReference(const std::string& ID, const Ogre::Vector3& position,
                                         const Ogre::Vector3& rotation, const float scale);

      /* returns a pointer to the first object reference with the given ID, or
         NULL if no such object is present
      */
      InjectionObject* GetInjectionObjectReference(const std::string& ID) const;

      /* returns a pointer to the first animated object reference (including NPC)
         with the given ID, or NULL if no such object is present
      */
      AnimatedObject* GetAnimatedObjectReference(const std::string& ID) const;

      /* same as GetAnimatedObjectReference, but with check for NPC -> if the
         referenced object is not a NPC, it returns NULL
      */
      NPC* GetNPCReference(const std::string& ID) const;

      /* Deletes all objects with the given ID and returns the number of deleted objects.

         remarks:
             This function should not be called in-game. It is only used by the Editor.
      */
      unsigned int deleteReferencesOfAnimatedObject(const std::string& del_ID);


      /* makes all objects move according to the amount of time passed

         parameters:
             TimePassed - the amount of time that has passed since last frame,
                          measured in seconds
      */
      void InjectAnimationTime(const float TimePassed);

      /* Tries to save all references to the stream. Returns true on success. */
      bool SaveAllToStream(std::ofstream& output) const;

      /* Tries to load the next object from stream and returns true on success

         parameters:
             PrefetchedHeader - the first four bytes of the record to come
      */
      bool LoadNextFromStream(std::ifstream& Stream, const unsigned int PrefetchedHeader);

      /* deletes all referenced objects */
      void ClearData();

      /* notify AnimationData about an object that wants to be deleted

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
    private:
      /* private constructor (singleton pattern) */
      AnimationData();
      /* empty, private copy constructor due to singleton pattern*/
      AnimationData(const AnimationData& op) {}

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

#endif // ANIMATIONDATA_H
