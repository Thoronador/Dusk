/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    31.12.2009
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
      std::vector<AnimatedObject*> m_ReferenceList;
  };//class
}//namespace

#endif // ANIMATIONDATA_H
