#include "Weapon.h"
#include "WeaponBase.h"
#include "DuskConstants.h"

namespace Dusk
{

Weapon::Weapon()
  : Item()
{
  m_Equipped = false;
}

Weapon::Weapon(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : Item(_ID, pos, rot, Scale)
{
  m_Equipped = false;
}

Weapon::~Weapon()
{
  Disable();
}

bool Weapon::Enable(Ogre::SceneManager* scm)
{
  //We basically use the exact code from Item's Enable() function here,
  // just with the difference of using WeaponBase instead of ItemBase for mesh
  // retrieval.
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Weapon::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), WeaponBase::GetSingleton().getWeaponMesh(ID));
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

bool Weapon::EnableWithoutSceneNode(Ogre::SceneManager* scm)
{
  //We basically use the exact code from the above Enable() function here,
  // just with the difference of not creating a scene node for it.
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Weapon::EnableWithoutNode: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity
  entity = scm->createEntity(entity_name.str(), WeaponBase::GetSingleton().getWeaponMesh(ID));
  /*Ogre::SceneNode* ent_node = scm->getRootSceneNode()->createChildSceneNode(entity_name.str(), position);
  ent_node->attachObject(entity);
  ent_node->scale(m_Scale, m_Scale, m_Scale);
  //not sure whether this is the best one for rotation
  ent_node->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rotation.x));
  ent_node->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rotation.y));
  ent_node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rotation.z));*/
  //set user defined object to this object as reverse link
  entity->setUserObject(this);
  return (entity!=NULL);
}

ObjectTypes Weapon::GetType() const
{
  return otWeapon;
}

bool Weapon::canPickUp() const
{
  //we don't want to pick up equipped weapons
  return !m_Equipped;
}

bool Weapon::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Weapon::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RfWe" (reference of Weapon)
  OutStream.write((char*) &cHeaderRfWe, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "Weapon::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data inherited from Item
  if (!SaveItemPart(OutStream))
  {
    std::cout << "Weapon::SaveToStream: ERROR while writing item data!\n";
    return false;
  }
  //Weapon has no new data members, so return here
  return OutStream.good();
}

bool Weapon::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Cannot load from stream while"
              << " weapon is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RfWe"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRfWe)
  {
    std::cout << "Weapon::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "Weapon::LoadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //read all stuff inherited from item
  if (!LoadItemPart(InStream))
  {
    std::cout << "Weapon::LoadFromStream: ERROR while reading item data.\n";
    return false;
  }//if
  //class Weapon has no new data members, so return here
  return InStream.good();
}

} //namespace
