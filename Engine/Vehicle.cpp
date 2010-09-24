#include "Vehicle.h"
#include "VehicleBase.h"
#include "NPC.h"
#include "DuskConstants.h"

namespace Dusk
{

Vehicle::Vehicle()
 : AnimatedObject("", Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f)

{
  WaypointObject::WaypointObject("", Ogre::Vector3::ZERO, Ogre::Vector3::ZERO, 1.0f);
  ID = "";
  m_Passengers.clear();
}

Vehicle::Vehicle(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale)
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

void Vehicle::injectTime(const float SecondsPassed)
{
  //animate vehicle
  AnimatedObject::injectTime(SecondsPassed);
  //move vehicle
  WaypointObject::injectTime(SecondsPassed);
  //adjust position and rotation of passengers according to their
  // mountpoint (e.g. seat) on the vehicle
  adjustPassengerPosition();
}

bool Vehicle::enable(Ogre::SceneManager* scm)
{
  if (entity!=NULL)
  {
    return true;
  }
  if (!AnimatedObject::enable(scm))
  {
    std::cout << "Vehicle::enable: ERROR: could not enable animation object.\n";
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
      iter->second.who->setPosition(getPosition()+iter->second.position_offset);
      //sets rotation which calculates as vehicle's rotation plus something else
      // I'm not so sure about yet.
      iter->second.who->setRotation(getRotation()+iter->second.rotation_offset);
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
      //sets position, which calculates as vehicle's position plus relative
      // offset position of passenger
      iter->second.who->setPosition(getPosition()+iter->second.position_offset);
      //sets rotation which calculates as vehicle's rotation plus something else
      // I'm not so sure about yet.
      iter->second.who->setRotation(getRotation()+iter->second.rotation_offset);
      /*****
       ***** To Do:
       +++++ ======
       +++++
       +++++   -get correct rotation values
      */
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
    std::cout << "Vehicle::saveToStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //write header "RefV" (reference of Vehicle)
  OutStream.write((char*) &cHeaderRefV, sizeof(unsigned int));
  //save stuff inherited from DuskObject
  if (!saveDuskObjectPart(OutStream))
  {
    std::cout << "Vehicle::saveToStream: ERROR while saving basic data!\n";
    return false;
  }
  // go on with new data members from AnimatedObject
  if (!saveAnimatedObjectPart(OutStream))
  {
    std::cout << "Vehicle::saveToStream: ERROR while saving animation data!\n";
    return false;
  }
  // go on with new data members from UniformMotionObject
  if (!saveUniformMotionObjectPart(OutStream))
  {
    std::cout << "Vehicle::saveToStream: ERROR while saving basic motion data!\n";
    return false;
  }
  // go on with new data members from WaypointObject
  if (!saveWaypointObjectPart(OutStream))
  {
    std::cout << "Vehicle::saveToStream: ERROR while saving waypoint data!\n";
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
    std::cout << "Vehicle::saveToStream: ERROR while writing vehicle data.\n";
    return false;
  }
  return OutStream.good();
}

bool Vehicle::loadFromStream(std::ifstream& InStream)
{
  if (entity!=NULL)
  {
    std::cout << "Vehicle::loadFromStream: ERROR: Cannot load from stream while"
              << " object is enabled.\n";
    return false;
  }
  if (!InStream.good())
  {
    std::cout << "Vehicle::loadFromStream: ERROR: Stream contains errors!\n";
    return false;
  }
  //read header "RefV"
  unsigned int Header = 0;
  InStream.read((char*) &Header, sizeof(unsigned int));
  if (Header!=cHeaderRefV)
  {
    std::cout << "Vehicle::loadFromStream: ERROR: Stream contains invalid "
              << "reference header.\n";
    return false;
  }
  //read DuskObject stuff
  if (!loadDuskObjectPart(InStream))
  {
    std::cout << "Vehicle::loadFromStream: ERROR while reading basic data.\n";
    return false;
  }
  // go on with data members from AnimatedObject
  if (!loadAnimatedObjectPart(InStream))
  {
    std::cout << "Vehicle::loadFromStream: ERROR while loading animation data.\n";
    return false;
  }
  // go on with data members from UniformMotionObject
  if (!loadUniformMotionObjectPart(InStream))
  {
    std::cout << "Vehicle::loadFromStream: ERROR while loading motion data.\n";
    return false;
  }
  // go on with data members from WaypointObject
  if (!loadWaypointObjectPart(InStream))
  {
    std::cout << "vehicle::loadFromStream: ERROR while loading waypoint data.\n";
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
    std::cout << "Vehicle::loadFromStream: ERROR while reading vehicle data.\n";
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
