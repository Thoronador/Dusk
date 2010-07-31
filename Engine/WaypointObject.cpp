#include "WaypointObject.h"
#include "ObjectBase.h"
#include "DuskConstants.h"

namespace Dusk
{

WaypointObject::WaypointObject()
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

  m_WaypointTravel = false;
  m_Waypoints.clear();
  m_currentWaypoint = 0;
  m_Patrol = false;
}

WaypointObject::WaypointObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
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

  m_WaypointTravel = false;
  m_Waypoints.clear();
  m_currentWaypoint = 0;
  m_Patrol = false;
}

WaypointObject::~WaypointObject()
{
  //dtor
  Disable();
}

unsigned int WaypointObject::AddWaypoint(const Ogre::Vector3& waypoint)
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

void WaypointObject::setUseWaypoints(const bool doUse)
{
  m_WaypointTravel = doUse;
  if (doUse && m_currentWaypoint<m_Waypoints.size())
  { //we have points and want movement, so start it
    TravelToDestination(m_Waypoints.at(m_currentWaypoint));
  }
}

void WaypointObject::clearWaypoints()
{
  m_Waypoints.clear();
  m_WaypointTravel = false;
  m_Patrol = false;
  m_currentWaypoint = 0;
}

void WaypointObject::setPatrolMode(const bool doPatrol)
{
  m_Patrol = doPatrol;
  if (doPatrol and m_Waypoints.size()<2)
  { //patrol mode not useful, if there is only one waypoint or none at all
    m_Patrol = false;
  }
}

bool WaypointObject::getPatrolMode() const
{
  return m_Patrol;
}

std::string WaypointObject::GetObjectMesh() const
{
  return ObjectBase::GetSingleton().GetMeshName(ID);
}

ObjectTypes WaypointObject::GetType() const
{
  return otWaypoint;
}

void WaypointObject::injectTime(const float SecondsPassed)
{
  if (SecondsPassed<=0.0f)
  {
    return;
  }
  if (m_Travel)
  {
    const float Distance = Ogre::Vector3(m_Destination-GetPosition()).squaredLength();
    //are we moving to fast?
    if (Ogre::Math::Sqr(m_Speed*SecondsPassed)>=Distance)
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
        { //we have run out of waypoints,...
          if (m_Patrol)
          { //...but we are in patrol mode, so start again
            m_currentWaypoint = 0;
            TravelToDestination(m_Waypoints.at(0));
          }
          else
          { //...so let's stop, since we do not patrol
            m_Travel = false;
            m_WaypointTravel = false;
            m_Direction = Ogre::Vector3::ZERO;
            m_Speed = 0.0f;
          }
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
  }
}

bool WaypointObject::SaveWaypointObjectPart(std::ofstream& OutStream) const
{
  // save data members from WaypointObject
  //waypoint data
  // -- waypoint travel enabled?
  OutStream.write((char*) &m_WaypointTravel, sizeof(bool));
  // -- patrol mode?
  OutStream.write((char*) &m_Patrol, sizeof(bool));
  // -- current waypoint
  OutStream.write((char*) &m_currentWaypoint, sizeof(m_currentWaypoint));
  // -- waypoints themselves
  // ---- number of WPs
  unsigned int len = m_Waypoints.size();
  OutStream.write((char*) &len, sizeof(unsigned int));
  // ---- waypoint data
  float xyz;
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
  return OutStream.good();
}

bool WaypointObject::LoadWaypointObjectPart(std::ifstream& InStream)
{
  //load data members from WaypointObject
  //waypoint data
  // -- waypoint travel enabled?
  InStream.read((char*) &m_WaypointTravel, sizeof(bool));
  // -- patrol mode?
  InStream.read((char*) &m_Patrol, sizeof(bool));
  if (!InStream.good())
  {
    std::cout << "WaypointObject::LoadWaypointObjectPart: ERROR while reading flags.\n";
    return false;
  }
  // -- current waypoint
  InStream.read((char*) &m_currentWaypoint, sizeof(m_currentWaypoint));
  // -- waypoints themselves
  // ---- number of WPs
  unsigned int wp_count = 0;
  InStream.read((char*) &wp_count, sizeof(unsigned int));
  if (wp_count>100)
  {
    std::cout << "WaypointObject::LoadWaypointObjectPart: ERROR: There are "
              << "more than 100 waypoints for one object. Aborting to avoid too"
              << " much data in vector.\n";
    return false;
  }
  m_Waypoints.clear();
  Ogre::Vector3 temp_vec;
  float f_temp;
  unsigned int len;
  for (len=0; len<wp_count; len=len+1)
  {
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.x = f_temp;
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.y = f_temp;
    InStream.read((char*) &f_temp, sizeof(float));
    temp_vec.z = f_temp;
    if (!(InStream.good()))
    {
      std::cout << "WaypointObject::LoadWaypointObjectPart: ERROR while reading"
                << " waypoints' data.\n";
      return false;
    }
    m_Waypoints.push_back(temp_vec);
  } //for
  return (InStream.good());
}

bool WaypointObject::SaveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    std::cout << "WaypointObject::SaveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RfWP" (reference of WaypointObject)
  OutStream.write((char*) &cHeaderRfWP, sizeof(unsigned int));
  //write all data inherited from DuskObject
  if (!SaveDuskObjectPart(OutStream))
  {
    std::cout << "WaypointObject::SaveToStream: ERROR while writing basic "
              << "data!\n";
    return false;
  }
  //write all data inherited from UniformMotionObject
  if (!SaveUniformMotionObjectPart(OutStream))
  {
    std::cout << "WaypointObject::SaveToStream: ERROR while writing motion "
              << "data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  return SaveWaypointObjectPart(OutStream);
}

bool WaypointObject::LoadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "WaypointObject::LoadFromStream: ERROR: Cannot load from "
              << "stream while object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "WaypointObject::LoadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RfWP"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRfWP)
  {
    std::cout << "WaypointObject::LoadFromStream: ERROR: Stream contains "
              << "invalid reference header.\n";
    return false;
  }
  //read all stuff inherited from DuskObject
  if (!LoadDuskObjectPart(InStream))
  {
    std::cout << "WaypointObject::LoadFromStream: ERROR while reading basic "
              << "object data.\n";
    return false;
  }//if
  //read all stuff inherited from UniformMotionObject
  if (!LoadUniformMotionObjectPart(InStream))
  {
    std::cout << "WaypointObject::LoadFromStream: ERROR while reading motion "
              << "object data.\n";
    return false;
  }//if
  // go on with new data members from WaypointObject
  return LoadWaypointObjectPart(InStream);
}

} //namespace
