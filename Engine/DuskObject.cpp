#include "DuskObject.h"
#include "API.h"
#include "ObjectBase.h"
#include <sstream>

namespace Dusk{

unsigned int GenerateUniqueObjectID()
{
  static unsigned int m_genUID = 0;
  return m_genUID++;
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

DuskObject::DuskObject(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale)
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
    //dtor
}

Ogre::Vector3 DuskObject::GetPosition() const
{
  return position;
}

Ogre::Vector3 DuskObject::GetRotation() const
{
  return rotation;
}

void DuskObject::SetPosition(const Ogre::Vector3 pos)
{
  position = pos;
}

void DuskObject::SetRotation(const Ogre::Vector3 rot)
{
  rotation = rot;
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
  m_Scale = newScale;
  return true;
}

std::string DuskObject::GetID() const
{
  return ID;
}

bool DuskObject::ChangeID(const std::string newID)
{
  if (newID!="" and entity==NULL)
  {
    ID = newID;
    return true;
  }
  return false;
}

bool DuskObject::Enable()
{
  if (entity!=NULL)
  {
    return true;
  }

  Ogre::SceneManager * scm;
  scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "DuskObject::Enable: ERROR: no scene manager present.\n";
    return false;
  }

  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ObjectBase::GetSingleton().GetMeshName(ID));
  Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  return (entity!=NULL);
}

bool DuskObject::Disable()
{
  if (entity == NULL)
  {
    return true;
  }

  Ogre::SceneManager * scm;
  scm = getAPI().getOgreSceneManager();
  if (scm==NULL)
  {
    std::cout << "DuskObject::Disable: ERROR: no scene manager present.\n";
    return false;
  }

  Ogre::SceneNode* ent_node = entity->getParentSceneNode();
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


}
