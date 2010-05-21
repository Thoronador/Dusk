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
  /* Until we have a proper player mesh, we use the Ogre mascot mesh from
     Zi Ye / omniter. */
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Player::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), "Sinbad.mesh");
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this NPC as reverse link
  entity->setUserObject(this);
  if (m_Anim != "")
  {
    const Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    if (anim_set->hasAnimationState(m_Anim))
    {
      Ogre::AnimationState* state = anim_set->getAnimationState(m_Anim);
      state->setTimePosition(0.0f);
      state->setLoop(m_LoopAnim);
      state->setEnabled(true);
    }
    else
    {
      m_Anim = "";
    }
  }
  unsigned int i;
  const std::vector<std::string> as = GetPossibleAnimationStates();
  for (i=0; i<as.size(); ++i)
  {
    std::cout << "Player animation state available: "<<as[i]<<"\n";
  }//for
  return (entity!=NULL);
}

bool Player::SaveToStream(std::ofstream& OutStream) const
{
  //not implemented yet
  return false;
}

bool Player::LoadFromStream(std::ifstream& InStream)
{
  //not implemented yet
  return false;
}

} //namespace
