#include "Player.h"
#include "API.h"
//#include <OgreSceneQuery.h>

namespace Dusk
{

Player& Player::GetSingleton()
{
  static Player Instance;
  return Instance;
}

Player::Player()
  : NPC("player", Ogre::Vector3(0.0, 0.0, 300), Ogre::Vector3::ZERO, 1.0f)
{
  //not much done yet
}

Player::~Player()
{
  Disable();
}

bool Player::pickUpNearest()
{
  Ogre::SceneManager* scm = getAPI().getOgreSceneManager();
  if (scm==NULL) return false;
  Ogre::SphereSceneQuery* sp_query =
      scm->createSphereQuery(Ogre::Sphere(position, cMaximumPickUpDistance));
  Ogre::SceneQueryResult& result = sp_query->execute();
  float distance = -1.0f;
  Item* item_ptr = NULL;
  Ogre::SceneQueryResultMovableList::iterator iter = result.movables.begin();
  while (iter!=result.movables.end())
  {
    Ogre::UserDefinedObject* udo = (*iter)->getUserObject();
    if (udo!=NULL)
    {
      DuskObject* obj_ptr = static_cast<DuskObject*>(udo);
      if (obj_ptr->canPickUp())
      {
        const float sd = position.squaredDistance(obj_ptr->GetPosition());
        if ((sd<distance || distance<0.0f) and (obj_ptr->GetType()==otItem))
        {
          distance = sd;
          item_ptr = static_cast<Item*>(obj_ptr);
        }
      }
    }//if
    ++iter;
  }//while
  //clean up
  scm->destroyQuery(sp_query);
  sp_query = NULL;
  //check whether we found something
  if (item_ptr!=NULL)
  {
    return this->pickUp(item_ptr);
  }
  return false;
}

bool Player::Enable(Ogre::SceneManager* scm)
{
  //do nothing yet, because we don't have a player mesh
  return false;
}

} //namespace
