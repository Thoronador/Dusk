#include "Projectile.h"
#include "ProjectileBase.h"
#include "AnimationData.h"
#include "DuskConstants.h"

namespace Dusk
{

const float Projectile::InfiniteTTL = -1.0f;

Projectile::Projectile()
  : UniformMotionObject()
{
  m_TTL = InfiniteTTL;
}

Projectile::Projectile(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
  : UniformMotionObject(_ID, pos, rot, Scale)
{
  if (ProjectileBase::GetSingleton().hasProjectile(_ID))
  {
    const ProjectileRecord rec = ProjectileBase::GetSingleton().getProjectileData(_ID);
    m_TTL = rec.DefaultTTL;
    m_Speed = rec.DefaultVelocity;
  }
  else
  {
    m_TTL = InfiniteTTL;
  }
}

Projectile::~Projectile()
{
  Disable();
}

ObjectTypes Projectile::GetType() const
{
  return otProjectile;
}

void Projectile::SetTTL(const float newTTL)
{
  m_TTL = newTTL;
}

float Projectile::GetTTL() const
{
  return m_TTL;
}

void Projectile::injectTime(const float SecondsPassed)
{
  UniformMotionObject::injectTime(SecondsPassed);
  if (m_TTL>0.0f)
  {
    m_TTL = m_TTL - SecondsPassed;
    if (m_TTL<=0.0f)
    {
      //we are done here, so delete this object
      AnimationData::GetSingleton().requestDeletion(this);
      return;
    }
  }//if
}

bool Projectile::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "Projectile::Enable: ERROR: no scene manager present.\n";
    return false;
  }
  //generate unique entity name
  std::stringstream entity_name;
  entity_name << ID << GenerateUniqueObjectID();
  //create entity + node and attach entity to node
  entity = scm->createEntity(entity_name.str(), ProjectileBase::GetSingleton().getProjectileMesh(ID));
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

void Projectile::TravelToDestination(const Ogre::Vector3& dest)
{
  m_Destination = Ogre::Vector3::ZERO;
  m_Travel = false;
  SetDirection(dest-GetPosition());
}

bool Projectile::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "Projectile::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefP" (reference of Projectile)
  OutStream.write((char*) &cHeaderRefP, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "Projectile::SaveToStream: ERROR while writing basic data!\n";
    return false;
  }
  //write all data inherited from UniformMotionObject
  if (!SaveUniformMotionObjectPart(OutStream))
  {
    std::cout << "Projectile::SaveToStream: ERROR while writing motion data!\n";
    return false;
  }
  //write TTL
  OutStream.write((char*) &m_TTL, sizeof(float));
  return OutStream.good();
}

bool Projectile::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Projectile::LoadFromStream: ERROR: Cannot load from stream "
              << "while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Projectile::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefP"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefP)
  {
    std::cout << "Projectile::LoadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "Projectile::LoadFromStream: ERROR while reading basic "
              << "data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!LoadUniformMotionObjectPart(InStream))
  {
    std::cout << "Projectile::LoadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  //TTL
  InStream.read((char*) &m_TTL, sizeof(m_TTL));
  return InStream.good();
}

} //namespace
