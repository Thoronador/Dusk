/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-27
 Purpose: Player Singleton class
          represents the player character within the game
          (Basically, it's the NPC class transformed into a Singleton.)

 History:
     - 2010-03-27 (rev 188) - initial version
     - 2010-05-06 (rev 198) - documentation update

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
      static Player& GetSingleton();

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

      /* override NPC's Enable() method */
      virtual bool Enable(Ogre::SceneManager* scm);
    private:
      /* constructor - private due to singleton pattern */
      Player();
      /* private, empty copy constructor (singleton pattern) */
      Player(const Player& op) {}
  }; //class Player

} //namespace

#endif // PLAYER_H
