#include "DuskObject.h"
#include "ObjectBase.h"
#include "DuskConstants.h"
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
  m_Scale = 1.0f;
  entity = NULL;
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
    if (entity->getParentSceneNode()!=NULL)
    {
      entity->getParentSceneNode()->setPosition(pos);
    }
  }
  position = pos;
}

void DuskObject::SetRotation(const Ogre::Vector3& rot)
{
  if (entity!=NULL)
  {
    if (entity->getParentSceneNode()!=NULL)
    {
      entity->getParentSceneNode()->resetOrientation();
      //not sure whether this is the best one...
      // maybe we still need to set a different transform space
      entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_X, Ogre::Degree(rot.x));
      entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Y, Ogre::Degree(rot.y));
      entity->getParentSceneNode()->rotate(Ogre::Vector3::UNIT_Z, Ogre::Degree(rot.z));
    }
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

std::string DuskObject::GetObjectMesh() const
{
  return ObjectBase::GetSingleton().GetMeshName(ID);
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
  entity = scm->createEntity(entity_name.str(), GetObjectMesh());
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

bool DuskObject::IsEnabled() const
{
  return (entity!=NULL);
}

ObjectTypes DuskObject::GetType() const
{
  return otStatic;
}

bool DuskObject::canPickUp() const
{
  return false;
}

bool DuskObject::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "DuskObject::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefO" (reference of Object)
  OutStream.write((char*) &cHeaderRefO, sizeof(unsigned int)); //header
  //write all data members, i.e. ID, position and rotation, and scale
  return SaveDuskObjectPart(OutStream);
}

bool DuskObject::SaveDuskObjectPart(std::ofstream& output) const
{
  //write ID
  const unsigned int len = ID.length();
  output.write((char*) &len, sizeof(unsigned int));
  output.write(ID.c_str(), len);

  //write position and rotation, and scale
  // -- position
  float xyz = position.x;
  output.write((char*) &xyz, sizeof(float));
  xyz = position.y;
  output.write((char*) &xyz, sizeof(float));
  xyz = position.z;
  output.write((char*) &xyz, sizeof(float));
  // -- rotation
  xyz = rotation.x;
  output.write((char*) &xyz, sizeof(float));
  xyz = rotation.y;
  output.write((char*) &xyz, sizeof(float));
  xyz = rotation.z;
  output.write((char*) &xyz, sizeof(float));
  // -- scale
  output.write((char*) &m_Scale, sizeof(float));
  return output.good();
}

bool DuskObject::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "DuskObject::LoadFromStream: ERROR: Cannot load from stream "
              << "while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "DuskObject::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  unsigned int Header;

  //read header "RefO"
  Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefO)
  {
    std::cout << "DuskObject::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //load all data members of DuskObject, i.e. ID, position, rotation, scale
  return LoadDuskObjectPart(InStream);
}

bool DuskObject::LoadDuskObjectPart(std::ifstream& InStream)
{
  //read ID
  unsigned int len;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "DuskObject::LoadFromStream: ERROR: ID cannot be longer than "
              << "255 characters.\n";
    return false;
  }
  char ID_Buffer[256];
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    std::cout << "DuskObject::LoadFromStream: ERROR while reading data (ID).\n";
    return false;
  }
  ID = std::string(ID_Buffer);

  float f_temp;
  //position
  InStream.read((char*) &f_temp, sizeof(float));
  position.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  position.z = f_temp;
  //rotation
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  rotation.z = f_temp;
  //scale
  InStream.read((char*) &f_temp, sizeof(float));
  m_Scale = f_temp;
  return InStream.good();
}

}
