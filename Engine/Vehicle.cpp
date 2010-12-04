/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/
#include "Vehicle.h"
#include "VehicleBase.h"
#include "NPC.h"
#include "DuskConstants.h"
#include "Messages.h"

namespace Dusk
{

Vehicle::Vehicle()
 : AnimatedObject("", Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, 1.0f)

{
  WaypointObject::WaypointObject("", Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, 1.0f);
  ID = "";
  m_Passengers.clear();
}

Vehicle::Vehicle(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
: AnimatedObject(_ID, pos, rot, Scale)
{
  WaypointObject::WaypointObject(_ID, pos, rot, Scale);
  m_Passengers.clear();
}

Vehicle::~Vehicle()
{
  dropAllPassengers();
  disable();
}

ObjectTypes Vehicle::getDuskType() const
{
  return otVehicle;
}

bool Vehicle::canCollide() const
{
  return true;
}

void Vehicle::injectTime(const float SecondsPassed)
{
  //animate vehicle
  AnimatedObject::injectTime(SecondsPassed);
  //move vehicle
  WaypointObject::injectTime(SecondsPassed);
  //adjust position and rotation of passengers according to their
  // mountpoint (e.g. seat) on the vehicle
  if (isEnabled())
  {
    adjustPassengerPosition();
  }//if
}

bool Vehicle::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (!AnimatedObject::enable(scm))
  {
    DuskLog() << "Vehicle::enable: ERROR: could not enable animation object.\n";
    return false;
  }
  //enable passengers, too
  return enablePassengers(scm);
}

bool Vehicle::disable()
{
  if (!AnimatedObject::disable()) return false;
  //disable all passengers
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.begin();
  while (iter!=m_Passengers.end())
  {
    if (iter->second.who!=NULL)
    {
      if (!iter->second.who->disable()) return false;
    }//if
    ++iter;
  }//while
  return true;
}

bool Vehicle::enablePassengers(Ogre::SceneManager* scm)
{
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.begin();
  const Ogre::Quaternion& rot_self = entity->getParentSceneNode()->getOrientation();
  while (iter!=m_Passengers.end())
  {
    if (iter->second.who!=NULL)
    {
      if (!iter->second.who->enable(scm)) return false;
      /*****
       ***** To Do:
       +++++ ======
       +++++
       +++++   -adjust position and rotation of passengers according to their
       +++++    mountpoint (e.g. seat) on the vehicle
      */
      //sets position, which calculates as vehicle's position plus relative
      // offset position of passenger
      iter->second.who->setPosition(getPosition()+ rot_self*iter->second.position_offset);
      //sets rotation which calculates as vehicle's rotation plus something else
      // I'm not so sure about yet.
      iter->second.who->setRotation(getRotation()*iter->second.rotation_offset);
    }//if
    ++iter;
  }//while
  //all passengers could be enabled, if we get to this point
  return true;
}

void Vehicle::adjustPassengerPosition()
{
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.begin();
  while (iter!=m_Passengers.end())
  {
    if (iter->second.who!=NULL)
    {
      //sets passenger's position, which calculates as vehicle's position plus
      // relative offset position of passenger
      iter->second.who->setPosition(getPosition()+m_Rotation*iter->second.position_offset);
      //sets rotation which calculates as vehicle's rotation multiplied with
      // rotational offset.
      // However, I'm not so sure about that yet.
      iter->second.who->setRotation(m_Rotation*iter->second.rotation_offset);
    }//who
    ++iter;
  }//while
}

unsigned int Vehicle::getTotalMountpoints() const
{
  return VehicleBase::getSingleton().getVehicleMountpoints(ID);
}

unsigned int Vehicle::getFreeMountpoints() const
{
  const unsigned int total = getTotalMountpoints();
  const unsigned int occupied = getPassengerCount();
  if (total<=occupied) return 0;
  return total-occupied;
}

bool Vehicle::mountPassengerAtIndex(const unsigned int idx, NPC* who)
{
  //check for valid index and pointer
  if (idx>=getTotalMountpoints() or (NULL==who)) return false;
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.find(idx);
  if (iter!=m_Passengers.end())
  {
    //place is already occupied by another NPC
    return false;
  }
  //Is the given NPC already on/in a vehicle?
  if (who->getVehicle()!=NULL) return false;
  //now finally mount the NPC
  PassengerRecord temp;
  temp.who = who;
  temp.position_offset = VehicleBase::getSingleton().getMountpointOffset(ID, idx);
  temp.rotation_offset = VehicleBase::getSingleton().getMountpointRotation(ID, idx);
  m_Passengers[idx] = temp;
  who->setVehicle(this);
  return true;
}

bool Vehicle::unmountPassengerAtIndex(const unsigned int idx)
{
  //check for valid index
  if (idx>=getTotalMountpoints()) return false;
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.find(idx);
  if (iter!=m_Passengers.end())
  {
    //There is an NPC, so remove it.
    iter->second.who->setVehicle(NULL);
    iter->second.who = NULL;
    m_Passengers.erase(iter);
  }
  return true;
}

bool Vehicle::unmountPassenger(NPC* who)
{
  if (who==NULL) return false;
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.begin();
  while (iter!=m_Passengers.end())
  {
    if (iter->second.who==who)
    {
      //found wanted NPC, so remove it
      iter->second.who->setVehicle(NULL);
      iter->second.who = NULL;
      m_Passengers.erase(iter);
      return true;
    }
    ++iter;
  } //while
  return false;
}

NPC* Vehicle::getPassengerAtIndex(const unsigned int idx) const
{
  std::map<unsigned int, PassengerRecord>::const_iterator cIter;
  cIter = m_Passengers.find(idx);
  if (cIter==m_Passengers.end()) return NULL;
  return cIter->second.who;
}

unsigned int Vehicle::getPassengerCount() const
{
  return m_Passengers.size();
}

bool Vehicle::saveToStream(std::ofstream& OutStream) const
{
  if (!OutStream.good())
  {
    DuskLog() << "Vehicle::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefV" (reference of Vehicle)
  OutStream.write((char*) &cHeaderRefV, sizeof(unsigned int));
  //save stuff inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    DuskLog() << "Vehicle::saveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!saveAnimatedObjectPart(OutStream))
  {
    DuskLog() << "Vehicle::saveToStream: ERROR while saving animation data!\n";
    return false;
  }
  // go on with new data members from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    DuskLog() << "Vehicle::saveToStream: ERROR while saving basic motion data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  if (!saveWaypointObjectPart(OutStream))
  {
    DuskLog() << "Vehicle::saveToStream: ERROR while saving waypoint data!\n";
    return false;
  }
  //done with inherited data members; go on with Vehicle stuff
  /*****
   ***** To Do:
   +++++ ======
   +++++
   +++++   -save current passengers somehow
  */
  if (!OutStream.good())
  {
    DuskLog() << "Vehicle::saveToStream: ERROR while writing vehicle data.\n";
    return false;
  }
  return OutStream.good();
}

bool Vehicle::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR: Cannot load from stream while"
              << " object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefV"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefV)
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!loadAnimatedObjectPart(InStream))
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  // go on with data members from WaypointObject
  if (!loadWaypointObjectPart(InStream))
  {
    DuskLog() << "vehicle::loadFromStream: ERROR while loading waypoint data.\n";
    return false;
  }
  //done with inherited data, go on with Vehicle data
  /*****
   ***** To Do:
   +++++ ======
   +++++
   +++++   -load current passengers
  */
  if (!InStream.good())
  {
    DuskLog() << "Vehicle::loadFromStream: ERROR while reading vehicle data.\n";
    return false;
  }
  return InStream.good();
}

void Vehicle::dropAllPassengers()
{
  std::map<unsigned int, PassengerRecord>::iterator iter;
  iter = m_Passengers.begin();
  while (iter!=m_Passengers.end())
  {
    if (iter->second.who!=NULL)
    {
      iter->second.who->setVehicle(NULL);
      iter->second.who= NULL;
    }
    //erase it
    m_Passengers.erase(iter);
    iter = m_Passengers.begin();
  }//while
}

std::string Vehicle::getObjectMesh() const
{
  return VehicleBase::getSingleton().getVehicleMesh(ID);
}

} //namespace
