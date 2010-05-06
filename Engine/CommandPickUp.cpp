#include "CommandPickUp.h"
#include "Player.h"

namespace Dusk
{

CommandPickUp::CommandPickUp()
{
  //empty
}

CommandPickUp::~CommandPickUp()
{
  //empty
}

bool CommandPickUp::execute(Dusk::Scene* scene, int count)
{
  //get player object, perform scene query around it, get nearest object and pick it up
  //that's the plan, which is done by Player's pickUpNearest()
  return Player::GetSingleton().pickUpNearest();
}

} //namespace
