/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-04-28
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

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef ANIMATIONDATA_H
#define ANIMATIONDATA_H

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
      virtual ~AnimationData();

      /* singleton access function */
      static AnimationData& GetSingleton();

      /* returns the number of object references the class holds */
      unsigned int NumberOfReferences() const;

      /* adds a new AnimatedObject with given ID at given position with rotation
         and scale, and returns a pointer to the created AnimatedObject. */
      AnimatedObject* addAnimatedReference(const std::string ID, const Ogre::Vector3 position,
                                           const Ogre::Vector3 rotation, const float scale);

      /* adds a new NPC with given ID at given position with rotation and scale.
         Returns a pointer to the created NPC. */
      NPC* addNPCReference(const std::string ID, const Ogre::Vector3 position,
                           const Ogre::Vector3 rot, const float Scale);

      /* returns a pointer to the first object reference with the given ID, or
         NULL of no such object is present
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
    private:
      AnimationData();
      AnimationData(const AnimationData& op) {}

      std::map<std::string, std::vector<AnimatedObject*> > m_ReferenceMap;
      unsigned int m_RefCount;
  };//class
}//namespace

#endif // ANIMATIONDATA_H
