/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-09-22
 Purpose: Vehicle class
          Represents a Vehicle within the game.

 History:
     - 2010-09-22 (rev 243) - initial version (by thoronador)

 ToDo list:
     - position and orientation of passengers has to be adjusted during
       enabling; same goes for movement
     - methods to save, load and query (const) passengers somehow
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef VEHICLE_H
#define VEHICLE_H

#include "AnimatedObject.h"
#include "WaypointObject.h"

namespace Dusk
{

//forward declaration of NPC
class NPC;

class Vehicle: public AnimatedObject, public WaypointObject
{
  public:
    /* constructor */
    Vehicle();

    /* constructor with parameters */
    Vehicle(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~Vehicle();

    /* returns the enumeration type indicating that this is a vehicle */
    virtual ObjectTypes getDuskType() const;

    /* animate and move the vehicle according to the passed time

       parameters:
           SecondsPassed - the amount of seconds that passed since the last
                           call of this function/ since the last frame

       remarks:
           This function is intended to be called regularly, i.e. every frame,
           to accomplish the desired animation of the object. If you don't call
           this function in such a manner, the animation will be processed
           improperly and/or will not be fluent.
    */
    virtual void injectTime(const float SecondsPassed);

    /* Enables the vehicle, i.e. tells the SceneManager to display it.
       Returns true on success, false on error.

       parameters:
           scm - the scene manager that is used to show the vehicle
    */
    virtual bool enable(Ogre::SceneManager* scm);

    /* Disables the object, i.e. tells the SceneManager not to display it.
       Returns true on success, false on error. */
    virtual bool disable();

    /* returns the overall number of available mountpoints */
    virtual unsigned int getTotalMountpoints() const;

    /* returns the number of unoccupied mountpoints */
    virtual unsigned int getFreeMountpoints() const;

    /* returns the current number of passengers */
    virtual unsigned int getPassengerCount() const;

    /* immediately drops all passengers from the vehicle*/
    virtual void dropAllPassengers();

    /* saves vehicle to given stream and returns true on success, false otherwise

       parameters:
           OutStream - the output stream that is used to save the vehicle
    */
    virtual bool saveToStream(std::ofstream& OutStream) const;

    /* Loads vehicle from stream and returns true on success, false otherwise.
       The vehicle's data is probably inconsistent after that function failed,
       so don't rely on its data in that case.

       parameters:
           InStream - the input stream that is used to load the vehicle from
    */
    virtual bool loadFromStream(std::ifstream& InStream);
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
    */
    virtual std::string getObjectMesh() const;

    /* Enables the vehicle's passengers, i.e. tells the SceneManager to display
       them. Returns true on success, false on error.

       parameters:
           scm - the scene manager that is used to show the passengers
    */
    bool enablePassengers(Ogre::SceneManager* scm);

    //list of passengers
    std::vector<NPC*> m_Passengers;
};//class


} //namespace

#endif // VEHICLE_H
