#include "AnimatedObject.h"
#include "ObjectBase.h" //should replace this one later
#include <OgreMath.h>

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
    float Distance = Ogre::Vector3(m_Destination-GetPosition()).squaredLength();
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

}
