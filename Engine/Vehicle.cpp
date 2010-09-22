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
  /*****
   ***** To Do:
   +++++ ======
   +++++
   +++++   -adjust position and rotation of passengers according to their
   +++++    mountpoint (e.g. seat) on the vehicle
  */
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
  unsigned int i;
  for (i=0; i<m_Passengers.size(); ++i)
  {
    if (m_Passengers[i]!=NULL)
    {
      if (!m_Passengers[i]->disable()) return false;
    }//if
  }//for
  return true;
}

bool Vehicle::enablePassengers(Ogre::SceneManager* scm)
{
  unsigned int i;
  for (i=0; i<m_Passengers.size(); ++i)
  {
    if (m_Passengers[i]!=NULL)
    {
      if (!m_Passengers[i]->enable(scm)) return false;
      /*****
       ***** To Do:
       +++++ ======
       +++++
       +++++   -adjust position and rotation of passengers according to their
       +++++    mountpoint (e.g. seat) on the vehicle
      */
    }//if
  }//for
  //all passengers could be enabled, if we get to this point
  return true;
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
  unsigned int i;
  for (i=0; i<m_Passengers.size(); ++i)
  {
    m_Passengers[i] = NULL;
  }//for
}

std::string Vehicle::getObjectMesh() const
{
  return VehicleBase::getSingleton().getVehicleMesh(ID);
}

} //namespace
