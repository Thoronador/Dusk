#include "AnimatedObject.h"
#include "ObjectBase.h" //should replace this one later
#include <OgreAnimationState.h>
#include <OgreMath.h>
#include "DuskConstants.h"

namespace Dusk
{

AnimatedObject::AnimatedObject()
{
  //ctor
  ID = "";
  position = Ogre::Vector3::ZERO;
  rotation = Ogre::Vector3::ZERO;
  m_Scale = 1.0f;
  entity = NULL;
  m_Direction = Ogre::Vector3::ZERO;
  m_Destination = Ogre::Vector3::ZERO;
  m_Speed = 0.0f;
  m_Travel = false;

  m_Anim = "";
  m_DoPlayAnim = false;
  m_LoopAnim = false;

  m_WaypointTravel = false;
  m_Waypoints.clear();
  m_currentWaypoint = 0;
}

AnimatedObject::AnimatedObject(const std::string _ID, const Ogre::Vector3 pos, const Ogre::Vector3 rot, const float Scale)
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
  m_Direction = Ogre::Vector3::ZERO;
  m_Destination = Ogre::Vector3::ZERO;
  m_Speed = 0.0f;
  m_Travel = false;

  m_Anim = "";
  m_DoPlayAnim = false;
  m_LoopAnim = false;

  m_WaypointTravel = false;
  m_Waypoints.clear();
  m_currentWaypoint = 0;
}

AnimatedObject::~AnimatedObject()
{
  //dtor
  Disable();
}

Ogre::Vector3 AnimatedObject::GetDirection() const
{
  return m_Direction;
}

void AnimatedObject::SetDirection(const Ogre::Vector3& direc)
{
  m_Direction = direc;
  m_Direction.normalise();
}

float AnimatedObject::GetSpeed() const
{
  return m_Speed;
}

void AnimatedObject::SetSpeed(const float v)
{
  m_Speed = v;
}

void AnimatedObject::TravelToDestination(const Ogre::Vector3& dest)
{
  m_Destination = dest;
  m_Travel = true;
  SetDirection(dest-GetPosition());
}

Ogre::Vector3 AnimatedObject::GetDestination() const
{
  return m_Destination;
}

bool AnimatedObject::IsOnTravel() const
{
  return m_Travel;
}

bool AnimatedObject::Enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (scm==NULL)
  {
    std::cout << "AnimatedObject::Enable: ERROR: no scene manager present.\n";
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
  if (m_Anim != "")
  {
    Ogre::AnimationStateSet* anim_set = entity->getAllAnimationStates();
    if (anim_set->hasAnimationState(m_Anim))
    {
      Ogre::AnimationState* state = anim_set->getAnimationState(m_Anim);
      state->setTimePosition(0.0f);
      state->setLoop(m_LoopAnim);
      state->setEnabled(true);
    }
  }
  return (entity!=NULL);
}

ObjectTypes AnimatedObject::GetType() const
{
  return otAnimated;
}

void AnimatedObject::PlayAnimation(const std::string& AnimName, const bool DoLoop)
{
  if (AnimName == m_Anim and DoLoop==m_LoopAnim)
  { //no work to do here
    return;
  }
  if (entity!=NULL)
  {
    Ogre::AnimationState* state = NULL;
    Ogre::AnimationStateSet * anim_set = NULL;
    anim_set = entity->getAllAnimationStates();
    if (m_Anim != "")
    {
      if (anim_set == NULL)
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animations!\n";
        return;
      }
      //stop old animation
      state = entity->getAnimationState(m_Anim);
      state->setEnabled(false);
    }
    //new animation
    if (AnimName!="")
    {
      if (anim_set == NULL)
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animations!\n";
        return;
      }
      if (!anim_set->hasAnimationState(AnimName))
      {
        std::cout << "AnimatedObject::PlayAnimation: Error: mesh has no animation named \""
                  << AnimName<<"\"!\n";
        return;
      }
      //new anim
      state = entity->getAnimationState(AnimName);
      state->setTimePosition(0.0f);
      state->setLoop(DoLoop);
      state->setEnabled(true);
    }
  }//if
  m_Anim = AnimName;
  m_LoopAnim = DoLoop;
  m_DoPlayAnim = (AnimName != "");
}

std::string AnimatedObject::GetAnimation() const
{
  return m_Anim;
}

void AnimatedObject::Move(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  if (m_Travel)
  {
    const float Distance = Ogre::Vector3(m_Destination-GetPosition()).squaredLength();
    //are we moving to fast?
    if (Ogre::Math::Sqr(m_Speed*SecondsPassed)>Distance)
    { //finished travelling
      SetPosition(m_Destination);
      if (!m_WaypointTravel)
      {
        m_Travel = false;
        m_Direction = Ogre::Vector3::ZERO;
        m_Speed = 0.0f;
      }
      else
      { //we have waypoints, select the next one
        if (m_currentWaypoint+1>=m_Waypoints.size())
        { //we have run out of waypoints, so let's stop
          m_Travel = false;
          m_WaypointTravel = false;
          m_Direction = Ogre::Vector3::ZERO;
          m_Speed = 0.0f;
        }
        else
        { //there are more waypoints, so select next
          m_currentWaypoint++;
          TravelToDestination(m_Waypoints.at(m_currentWaypoint));
        }
      }
    }
    else
    {
      position = position + SecondsPassed*m_Speed*m_Direction;
    }
  }
  else
  {
    position = position + SecondsPassed*m_Speed*m_Direction;
  }
  //adjust position of scene node/ entity in Ogre
  if (IsEnabled())
  {
    SetPosition(position);
    if (m_Anim!="")
    {
      entity->getAnimationState(m_Anim)->addTime(SecondsPassed);
    }
  }
}

unsigned int AnimatedObject::AddWaypoint(const Ogre::Vector3& waypoint)
{
  if (m_Waypoints.empty())
  {
    m_Waypoints.push_back(waypoint);
    return 1;
  }
  //avoid having the same waypoint twice in a row
  if (m_Waypoints.back()!=waypoint)
  {
    m_Waypoints.push_back(waypoint);
  }
  return m_Waypoints.size();
}

/* tells the object whether to use waypoints or not */
void AnimatedObject::setUseWaypoints(const bool doUse)
{
  m_WaypointTravel = doUse;
  if (doUse && m_currentWaypoint<m_Waypoints.size())
  { //we have points and want movement, so start it
    TravelToDestination(m_Waypoints.at(m_currentWaypoint));
  }
}

void AnimatedObject::clearWaypoints()
{
  m_Waypoints.clear();
  m_WaypointTravel = false;
  m_currentWaypoint = 0;
}

bool AnimatedObject::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "AnimatedObject::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  unsigned int len;

  //write header "RefA" (reference of AnimatedObject)
  OutStream.write((char*) &cHeaderRefA, sizeof(unsigned int));
  //write ID
  len = ID.length();
  OutStream.write((char*) &len, sizeof(unsigned int));
  OutStream.write(ID.c_str(), len);

  //write position and rotation, and scale
  // -- position
  float xyz;
  xyz = position.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = position.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = position.z;
  OutStream.write((char*) &xyz, sizeof(float));
  // -- rotation
  xyz = rotation.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = rotation.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = rotation.z;
  OutStream.write((char*) &xyz, sizeof(float));
  // -- scale
  OutStream.write((char*) &m_Scale, sizeof(float));
  if (!OutStream.good())
  {
    std::cout << "AnimatedObject::SaveToStream: ERROR while writing basic "
              << "data.\n";
    return false;
  }
  //now all data inherited from DuskObject is written
  // go on with new data members from AnimatedObject

  //direction
  xyz = m_Direction.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = m_Direction.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = m_Direction.z;
  OutStream.write((char*) &xyz, sizeof(float));
  //destination
  xyz = m_Destination.x;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = m_Destination.y;
  OutStream.write((char*) &xyz, sizeof(float));
  xyz = m_Destination.z;
  OutStream.write((char*) &xyz, sizeof(float));
  //speed
  OutStream.write((char*) &m_Speed, sizeof(float));
  //travel?
  OutStream.write((char*) &m_Travel, sizeof(bool));
  //animation data
  // -- anim name
  len = m_Anim.length();
  OutStream.write((char*) &len, sizeof(len));
  // -- loop mode?
  OutStream.write((char*) &m_LoopAnim, sizeof(bool));
  // -- playing?
  OutStream.write((char*) &m_DoPlayAnim, sizeof(bool));
  //waypoint data
  // -- waypoint travel enabled?
  OutStream.write((char*) &m_WaypointTravel, sizeof(bool));
  // -- current waypoint
  OutStream.write((char*) &m_currentWaypoint, sizeof(m_currentWaypoint));
  // -- waypoints themselves
  // ---- number of WPs
  len = m_Waypoints.size();
  OutStream.write((char*) &len, sizeof(unsigned int));
  // ---- waypoint data
  unsigned int i;
  for (i=0; i<len; i=i+1)
  {
    xyz = m_Waypoints.at(i).x;
    OutStream.write((char*) &xyz, sizeof(float));
    xyz = m_Waypoints.at(i).y;
    OutStream.write((char*) &xyz, sizeof(float));
    xyz = m_Waypoints.at(i).z;
    OutStream.write((char*) &xyz, sizeof(float));
  } //for
  return (OutStream.good());
}

bool AnimatedObject::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Cannot load from "
              << "stream while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }

  char ID_Buffer[256];
  float f_temp;
  unsigned int Header, len;

  //read header "RefA"
  Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefA)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: Stream contains "
              << "invalid reference header.\n";
    return false;
  }
  //read ID
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: ID cannot be longer "
              << "than 255 characters.\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR while reading data "
              << "(ID).\n";
    return false;
  }
  ID = std::string(ID_Buffer);

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

  //done with basic DuskObject stuff
  // go on with new data members from AnimatedObject

  //direction
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Direction.z = f_temp;
  //destination
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.x = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.y = f_temp;
  InStream.read((char*) &f_temp, sizeof(float));
  m_Destination.z = f_temp;
  //speed
  InStream.read((char*) &m_Speed, sizeof(float));
  //travel
  InStream.read((char*) &m_Travel, sizeof(bool));
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR while reading "
              << "direction, destination and speed.\n";
    return false;
  }
  //animation data
  // -- anim name
  len = 0;
  InStream.read((char*) &len, sizeof(unsigned int));
  if (len>255)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: animation name cannot"
              << " be longer than 255 characters.\n";
    return false;
  }
  InStream.read(ID_Buffer, len);
  ID_Buffer[len] = '\0';
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR while reading animation"
              << " name.\n";
    return false;
  }
  m_Anim = std::string(ID_Buffer);
  // -- loop mode?
  InStream.read((char*) &m_LoopAnim, sizeof(bool));
  // -- playing?
  InStream.read((char*) &m_DoPlayAnim, sizeof(bool));
  //waypoint data
  // -- waypoint travel enabled?
  InStream.read((char*) &m_WaypointTravel, sizeof(bool));
  if (!InStream.good())
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR while reading flags.\n";
    return false;
  }
  // -- current waypoint
  InStream.read((char*) &m_currentWaypoint, sizeof(m_currentWaypoint));
  // -- waypoints themselves
  // ---- number of WPs
  Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header>100)
  {
    std::cout << "AnimatedObject::LoadFromStream: ERROR: There are more than "
              << "100 waypoints for one object. Aborting to avoid to much "
              << "data in vector.\n";
    return false;
  }
  m_Waypoints.clear();
  Ogre::Vector3 temp_vec;
  for (len=0; len<Header; len=len+1)
  {
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.x = f_temp;
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.y = f_temp;
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.z = f_temp;
    if (!(InStream.good()))
    {
      std::cout << "AnimatedObject::LoadFromStream: ERROR while reading "
                << "waypoints data.\n";
      return false;
    }
    m_Waypoints.push_back(temp_vec);
  } //for
  return (InStream.good());
}

}
