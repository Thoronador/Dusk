#include "Item.h"
#include "DuskConstants.h"
#include "ItemBase.h"

namespace Dusk
{

Item::Item()
  : DuskObject()
{

}

Item::Item(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : DuskObject(_ID, pos, rot, Scale)
{

}

Item::~Item()
{
  Disable();
}

bool Item::Enable(Ogre::SceneManager* scm)
{
  //We basically use the exact code from DuskObject's Enable() function here,
  // just with the difference of using ItemBase instead of ObjectBase for mesh
  // retrieval.
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
  entity = scm->createEntity(entity_name.str(), ItemBase::GetSingleton().GetMeshName(ID));
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

ObjectTypes Item::GetType() const
{
  return otItem;
}

bool Item::canPickUp() const
{
  return true;
}

bool Item::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Item::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefI" (reference of Item)
  OutStream.write((char*) &cHeaderRefI, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "Item::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //Item has no own data members yet, so return true here
  return true;
}

bool Item::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Item::LoadFromStream: ERROR: Cannot load from stream while "
              << "item is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Item::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefI"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefI)
  {
    std::cout << "Item::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "Item::LoadFromStream: ERROR while reading basic object "
              << "data.\n";
    return false;
  }//if
  //class Item has no new data members yet, so return true here
  return true;
}

} //namespace
