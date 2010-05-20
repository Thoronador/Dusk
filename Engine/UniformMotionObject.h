/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-20
 Purpose: UniformMotionObject class
          abstract class/ interface for objects that moving

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)

 ToDo list:
     - implement possibility to make object "look" into the direction it is
       moving when traveling
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef UNIFORMMOTIONOBJECT_H
#define UNIFORMMOTIONOBJECT_H

#include "InjectionObject.h"
#include <OgreVector3.h>

namespace Dusk
{

class UniformMotionObject: virtual public InjectionObject
{
  public:
    /* default constructor */
    UniformMotionObject();

    /* constructor with parameter list */
    UniformMotionObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Vector3& rot, const float Scale);

    /* destructor */
    virtual ~UniformMotionObject();

    /* gets the direction the object is moving to. If it returns the zero
       vector, it's not moving.*/
    Ogre::Vector3 GetDirection() const;

    /* Sets the direction in which the object shall move.

       remarks:
           The given direction will be normalised automatically.
           The direction just indicates, where the object will move to.
           However, you also need to set the movement speed to a value
           greater than zero to actually make the object move.
    */
    void SetDirection(const Ogre::Vector3& direc);

    /* returns the movement speed*/
    float GetSpeed() const;

    /* sets the movement speed*/
    void SetSpeed(const float v);

    /* Sets a point in space to which the object will start moving, as long
       as the movement speed was previously set to something greater than 0.
       The object will automatically stop moving after it has reached this
       place.
    */
    void TravelToDestination(const Ogre::Vector3& dest);

    /* Returns the destination the object will is travelling to.

       remarks:
           Since this vector could be anything (in particular it can also
           be (0,0,0)), use the return value of IsOnTravel() to determine
           whether the object is currently moving or not.
    */
    Ogre::Vector3 GetDestination() const;

    /* Returns true, if the object is currently moving/ travelling to a certain
       destination.

       remarks:
           If this function returns true, this does NOT neccessarily mean that
           this object is not moving at all. It just means that it does not
           move to a certain location. The object could still be moving into a
           given direction endlessly.
    */
    bool IsOnTravel() const;

    /* function to inject time for movement and perform movement */
    virtual void injectTime(const float SecondsPassed);
  protected:
    Ogre::Vector3 m_Direction, m_Destination;
    float m_Speed;
    bool m_Travel;

    /* Utility function which saves all data that is specific to an
       UniformMotionObject to the given stream. Returns true on success.

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of SaveToStream().
    */
    bool SaveUniformMotionObjectPart(std::ofstream& output) const;

    /* Utility function which loads all data that is specific to an
       UniformMotionObject from the given stream. Returns true on success.

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of LoadFromStream().
    */
    bool LoadUniformMotionObjectPart(std::ifstream& InStream);
}; //class

} //namespace

#endif // UNIFORMMOTIONOBJECT_H
