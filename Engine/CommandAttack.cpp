#include "CommandAttack.h"
#include "Player.h"

namespace Dusk
{

//constructor
CommandAttack::CommandAttack(const bool start)
{
  m_DoStart = start;
}
// destructor
CommandAttack::~CommandAttack()
{
  //empty
}

bool CommandAttack::execute(Dusk::Scene* scene, int count)
{
  if (m_DoStart)
  {
    return Player::getSingleton().startAttack();
  }
  else
  {
    return Player::getSingleton().stopAttack();
  }
}

} //namespace
