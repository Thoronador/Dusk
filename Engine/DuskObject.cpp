#include "DuskObject.h"
#include "ObjectBase.h"
#include <sstream>
#include <OgreSceneNode.h>

namespace Dusk{

const float cMinimumScaleBound = 0.01;

unsigned int GenerateUniqueObjectID()
{
  static unsigned int m_genUID = 0;
  return m_genUID++;
}

void IntervalVector360(Ogre::Vector3& vec)
{
  if (vec.x>360.0f)
  {
    vec.x -= 360.0f;
  }
  else if (vec.x < 0.0f)
  {
    vec.x += 360.0f;
  }
  if (vec.y>360.0f)
  {
    vec.y -= 360.0f;
  }
  else if (vec.y < 0.0f)
  {
    vec.y += 360.0f;
  }
  if (vec.z>360.0f)
  {
    vec.z -= 360.0f;
  }
  else if (vec.z < 0.0f)
  {
    vec.z += 360.0f;
  }
}

DuskObject::DuskObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
  entity = NULL;
  objectType = otUndefined;
  m_Scale = 1.0f;
}

DuskObject::DuskObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
{
  ID = _ID;
  position = pos;
  rotation = rot;
  if (m_Scale>0.0f)
  {
    m_Scale = Scale;
  } else {
    m_Scale = 1.0f;
  }
  objectType = otUndefined;
  entity = NULL;
}

DuskObject::~DuskObject()
{
  //deletes related Ogre entity and scene node, if present
  Disable();
}

Ogre::Vector3 DuskObject::GetPosition() const
{
  return position;
}

Ogre::Vector3 DuskObject::GetRotation() const
{
  return rotation;
}

void DuskObject::SetPosition(const Ogre::Vector3& pos)
{
  if(entity!=NULL)
  {
    entity->getParentSceneNode()->setPosition(pos);
  }
  position = pos;
}

void DuskObject::SetRotation(const Ogre::Vector3& rot)
{
  if (entity != NULL)
  {
    entity->getParentSceneNode()->resetOrientation();
    //not sure whether this is the best one...
    // maybe we still need to set a different transform space
    entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rot.x));
    entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rot.y));
    entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rot.z));
  }
  rotation = rot;
  IntervalVector360(rotation);
}

float DuskObject::GetScale() const
{
  return m_Scale;
}

bool DuskObject::SetScale(const float newScale)
{
  if (entity!=NULL)
  {
    return false; //we don't change existing objects
  }
  //we don't want zero or negative values as scaling factor
  if (newScale>= cMinimumScaleBound)
  {
    m_Scale = newScale;
    return true;
  }
  std::cout << "DuskObject::Scale: Error: new scaling factor ("<<newScale
            << ") is less than minimum scale boundary ("<<cMinimumScaleBound
            << "). New scaling factor will not be applied.\n";
  return false;
}

std::string DuskObject::GetID() const
{
  return ID;
}

bool DuskObject::ChangeID(const std::string& newID)
{
  if (newID!="" and entity==NULL)
  {
    ID = newID;
    return true;
  }
  return false;
}

bool DuskObject::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }

  if (scm==NULL)
  {
    std::cout << "DuskObject::Enable: ERROR: no scene manager present.\n";
    return false;
  }

  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ObjectBase::GetSingleton().GetMeshName(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

bool DuskObject::Disable()
{
  if (entity == NULL)
  {
    return true;
  }

  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
  Ogre::SceneManager * scm;
  scm = ent_node->getCreator();
  if (scm==NULL)
  {
    std::cout << "DuskObject::Disable: ERROR: got NULL for scene manager.\n";
    return false;
  }
  ent_node->detachObject(entity);
  scm->getRootSceneNode()->removeAndDestroyChild(ent_node->getName());
  scm->destroyEntity(entity);
  entity = NULL;
  return true;
}

bool DuskObject::IsEnabled()
{
  return (entity!=NULL);
}

ObjectTypes DuskObject::GetType() const
{
  return objectType;
}

}
