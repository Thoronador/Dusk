#include "Container.h"
#include "ContainerBase.h"

namespace Dusk
{

Container::Container()
    : DuskObject()
{
  objectType = otContainer;
  m_Changed = false;
  m_Contents.MakeEmpty();
}

Container::Container(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
    : DuskObject(_ID, pos, rot, Scale)
{
  objectType = otContainer;
  m_Changed = false;
  m_Contents.MakeEmpty();
  ContainerBase::GetSingleton().GetContainerInventory(_ID).AddAllTo(m_Contents);
}

Container::~Container()
{
  m_Contents.MakeEmpty();
  Disable();
}

bool Container::IsEmpty() const
{
  return m_Contents.IsEmpty();
}

void Container::TransferAllContentTo(Inventory& target)
{
  m_Contents.AddAllTo(target);
  m_Contents.MakeEmpty();
  m_Changed = true;
}

void Container::AddItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return;
  }
  m_Contents.AddItem(ItemID, count);
  m_Changed = true;
}

unsigned int Container::RemoveItem(const std::string& ItemID, const unsigned int count)
{
  if (count==0 or ItemID=="")
  {
    return 0;
  }
  m_Changed = true;
  return m_Contents.RemoveItem(ItemID, count);
}

unsigned int Container::GetItemCount(const std::string& ItemID) const
{
  return m_Contents.GetItemCount(ItemID);
}

bool Container::Enable(Ogre::SceneManager* scm)
{
 if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Container::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ContainerBase::GetSingleton().GetContainerMesh(ID));
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

}//namespace
