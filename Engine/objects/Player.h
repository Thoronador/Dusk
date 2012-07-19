/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2012  thoronador

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
 Date:    2010-03-27
 Purpose: Player Singleton class
          represents the player character within the game
          (Basically, it's the NPC class transformed into a Singleton.)

 History:
     - 2010-03-27 (rev 188) - initial version
     - 2010-05-06 (rev 198) - documentation update
     - 2010-05-21 (rev 206) - Enable() now actually does something
     - 2010-05-27 (rev 208) - player will now wield swords, too
     - 2010-06-06 (rev 215) - changes in Enable()
     - 2010-07-31 (rev 219) - update to reflect changes of AnimatedObject
     - 2010-08-01 (rev 220) - GetObjectMesh() added, Enable() simplified
     - 2010-08-16 (rev 229) - fix to prevent segfault in destructor
     - 2010-08-27 (rev 236) - translate() added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-09-03 (rev 240) - adjustments for Editor
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2011-09-12 (rev 299) - loadFromStream() and saveToStream() implemented
     - 2012-06-30 (rev 308) - update of getObjectMesh() definition

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please. I'll try
       to fix it as soon as possible.
 --------------------------------------------------------------------------*/

#ifndef PLAYER_H
#define PLAYER_H

#include "NPC.h"

namespace Dusk
{

  class Player : public NPC
  {
    public:
      /* singleton access */
      static Player& getSingleton();

      /* destructor */
      virtual ~Player();

      /* player tries to pick up the nearest item it can find and returns true,
         if an item was picked up, false otherwise.

         remarks:
             The items must not be more than cMaximumPickUpDistance world units
             away from the player. See NPC.h for the exact value of that
             constant.

         dev. note:
             Should possibly be put into NPC class, so that Player only inherits
             this function, because pickUp method is already a member of NPC.
      */
      bool pickUpNearest();

      /* adds to the direction/speed of player */
      void translate(const Ogre::Vector3& translationVector);

      /* animates and move the player according to the passed time

         parameters:
             SecondsPassed - the amount of seconds that passed since the last
                             call of this function/ since the last frame

         remarks:
             This function is intended to be called regularly, i.e. every
             frame, to accomplish the desired animation of the object. If you
             don't call this function in such a manner, the animation will be
             processed improperly and/or will not be fluent.
      */
      virtual void injectTime(const float SecondsPassed);

      /* displays NPC mesh

         parameters:
             scm - the SceneManager that is used to show the player
      */
      virtual bool enable(Ogre::SceneManager* scm);

      /* saves NPC to given stream and returns true on success, false otherwise

         parameters:
             OutStream - the output stream that is used to save the player

         remarks:
            In its current state, this function does nothing but return false.
      */
      virtual bool saveToStream(std::ofstream& OutStream) const;

      /* Loads NPC from stream and returns true on success, false otherwise.
         The NPC's data is probably inconsistent after that function failed, so
         don't rely on its data in that case.

         parameters:
             InStream - the input stream that is used to save the player

         remarks:
            In its current state, this function does nothing but return false.
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
    private:
      /* constructor - private due to singleton pattern */
      Player();
      /* private, empty copy constructor (singleton pattern) */
      Player(const Player& op) {}
  }; //class Player

} //namespace

#endif // PLAYER_H
