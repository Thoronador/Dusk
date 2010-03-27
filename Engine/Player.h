/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-03-27
 Purpose: Player Singleton class
          represents the player character within the game
          (Basically, it's the NPC class transformed into a Singleton.)

 History:
     - 2010-03-27 (rev 188) - initial version

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
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
      bool pickUpNearest();

      /* override NPC's Enable() method */
      virtual bool Enable(Ogre::SceneManager* scm);
    private:
      Player();
      Player(const Player& op) {}
  }; //class Player

} //namespace

#endif // PLAYER_H
