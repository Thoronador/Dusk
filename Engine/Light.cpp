#include "Light.h"
#include "LightBase.h"
#include <sstream>

namespace Dusk
{

Light::Light()
  : DuskObject()
{
  //constructor
  objectType = otLight;
  m_Direction = Ogre::Vector3::ZERO;
}

Light::Light(const std::string& ID, const Ogre::Vector3& pos, const Ogre::Vector3& dir)
  : DuskObject(ID, pos, Ogre::Vector3::ZERO, 1.0f)
{
  //constructor
  objectType = otLight;
  m_Direction = dir;
}

bool Light::Enable(Ogre::SceneManager* scm)
{
  if (entity != NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Light::Enable: ERROR: no scene manager present.\n";
    return false;
  }

  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();

  entity = scm->createLight(entity_name.str());
  LightRecord lr = LightBase::GetSingleton().getLightData(ID);
  entity->setType(lr.type);
  entity->setPosition(Ogre::Vector3::ZERO);
  if (lr.type!=Ogre::Light::LT_POINT)
  { //only directional lights and spotlights need direction
    entity->setDirection(m_Direction);
  }
  entity->setDiffuseColour(lr.red, lr.green, lr.blue);
  entity->setSpecularColour(lr.red, lr.green, lr.blue);

  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

bool Light::Disable()
{
  if (entity == NULL)
  {
    return true;
  }
  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm = ent_node->getCreator();
  if (scm==NULL)
  {
    std::cout << "Light::Disable: ERROR: got NULL for scene manager.\n";
    return false;
  }
  ent_node->detachObject(entity);
  scm->getRootSceneNode()->removeAndDestroyChild(ent_node->getName());
  scm->destroyLight(entity);
  entity = NULL;
  return true;
}

void Light::SetDirection(const Ogre::Vector3& dir)
{
  if (entity != NULL)
  {
    entity->setDirection(dir);
  }
  m_Direction = dir;
}

Ogre::Vector3 Light::GetDirection() const
{
  return m_Direction;
}

}
