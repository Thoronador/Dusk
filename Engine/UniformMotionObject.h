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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-20
 Purpose: UniformMotionObject class
          abstract class/ interface for objects that moving

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)
     - 2010-05-21 (rev 207) - small improvement in SetSpeed()
     - 2010-05-31 (rev 211) - small fix
     - 2010-08-26 (rev 235) - isMoving() added
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-20 (rev 255) - rotation is now stored as Quaternion
     - 2010-12-03 (rev 266) - use DuskLog/Messages class for logging

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
    UniformMotionObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale);

    /* destructor */
    virtual ~UniformMotionObject();

    /* gets the direction the object is moving to. If it returns the zero
       vector, it's not moving.*/
    Ogre::Vector3 getDirection() const;

    /* Sets the direction in which the object shall move.

       parameters:
           direc - the new direction

       remarks:
           The given direction will be normalised automatically.
           The direction just indicates, where the object will move to.
           However, you also need to set the movement speed to a value
           greater than zero to actually make the object move.
    */
    void setDirection(const Ogre::Vector3& direc);

    /* returns the movement speed*/
    float getSpeed() const;

    /* sets the movement speed

       parameters:
           v - the new speed

       remarks:
           Smallest possible value is zero. Values less than zero will be set
           to zero instead.
           If you want the object to move backwards, use SetDirection() to
           change its direction to the opposite.
    */
    virtual void setSpeed(const float v);

    /* returns true, if the object is currently moving somehow */
    virtual bool isMoving() const;

    /* Sets a point in space to which the object will start moving, as long
       as the movement speed was previously set to something greater than 0.
       The object will automatically stop moving after it has reached this
       place.

       parameters:
           dest - the new destination
    */
    virtual void travelToDestination(const Ogre::Vector3& dest);

    /* Returns the destination the object will is travelling to.

       remarks:
           Since this vector could be anything (in particular it can also
           be (0,0,0)), use the return value of IsOnTravel() to determine
           whether the object is currently moving or not.
    */
    Ogre::Vector3 getDestination() const;

    /* Returns true, if the object is currently moving/ travelling to a certain
       destination.

       remarks:
           If this function returns true, this does NOT neccessarily mean that
           this object is not moving at all. It just means that it does not
           move to a certain location. The object could still be moving into a
           given direction endlessly.
    */
    bool isOnTravel() const;

    /* function to inject time for movement and perform movement

       parameters:
           SecondsPassed - the time (in seconds) that passed since the last
                           frame/ the last call of this function
    */
    virtual void injectTime(const float SecondsPassed);
  protected:
    Ogre::Vector3 m_Direction, m_Destination;
    float m_Speed;
    bool m_Travel;

    /* Utility function which saves all data that is specific to an
       UniformMotionObject to the given stream. Returns true on success.

       parameters:
           output - the output stream that is used to save the object

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of SaveToStream().
    */
    bool saveUniformMotionObjectPart(std::ofstream& output) const;

    /* Utility function which loads all data that is specific to an
       UniformMotionObject from the given stream. Returns true on success.

       parameters:
           InStream - the input stream that is used to load the object

       remarks:
           Derived classes will (most likely) call this function as part of
           their implementation of LoadFromStream().
    */
    bool loadUniformMotionObjectPart(std::ifstream& InStream);
}; //class

} //namespace

#endif // UNIFORMMOTIONOBJECT_H
