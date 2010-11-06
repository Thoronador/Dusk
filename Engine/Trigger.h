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
 Date:    2010-08-30
 Purpose: Trigger interface class
          provides basic interface for a trigger

          AABoxTrigger abstract class
          Provides a class for a trigger with an axis aligned box as trigger
          area

          SphereTrigger abstract class
          provides a class for a trigger with spheric trigger area

 History:
     - 2010-08-30 (rev 238) - initial version (by thoronador)

 ToDo list:
     - ???
 Bugs:
     - No known bugs. If you find one (or more), then tell us please.
 --------------------------------------------------------------------------*/

#ifndef TRIGGER_H
#define TRIGGER_H

#include <OgreAxisAlignedBox.h>
#include <OgreSphere.h>
#include "NPC.h"

namespace Dusk
{

//set TriggerObject as alias for NPC - we only want NPCs to fire triggers
typedef NPC TriggerObject;

/* Trigger interface class

   To trigger events derive a class from Trigger and implement the methods
   below. Active triggers should be registered at a (yet to code) TriggerManager
   class via a special method.
*/
class Trigger
{
  public:
    /* virtual destructor */
    virtual ~Trigger();

    /* is called whenever an object enters the trigger area

       parameters:
           obj - the object that entered the trigger
    */
    virtual void onEnter(TriggerObject* obj) = 0;

    /* is called whenever an object leaves the trigger area

       parameters:
           obj - the object that entered the trigger
    */
    virtual void onExit(TriggerObject* obj) = 0;

    /* is called once every frame for each object within the trigger area

       parameters:
           obj - the object that is within the trigger area
    */
    virtual void onWithin(TriggerObject* obj) = 0;

    /* function to determine whether or not an object is within the trigger
       area - should return true, if object obj is within the trigger, false
       otherwise

       parameters:
           obj - the object that needs to be tested for being within the trigger
    */
    virtual bool isWithin(const TriggerObject* obj) const = 0;
}; //class Trigger


/* AABoxTrigger abstract class

   Provides a class for a trigger with an axis aligned box as trigger area.
   It implements methods to get and set the trigger's box, and implements
   isWithin() for a box. However, it does not implement any trigger behaviour.
*/
class AABoxTrigger
{
  public:
    /* default constructor */
    AABoxTrigger();

    /* constructor with box initialisation

       parameters:
           box - the trigger's box
    */
    AABoxTrigger(const Ogre::AxisAlignedBox& box);

    /* destructor */
    ~AABoxTrigger();

    /* method to manually set the trigger's box

       parameters:
           box - the box that describes the trigger area
    */
    virtual void setBox(const Ogre::AxisAlignedBox& box);

    /* returns the axis aligned box that represents the trigger area */
    virtual const Ogre::AxisAlignedBox& getBox() const;

    /* function to determine whether or not an object is within the trigger
       area - should return true, if object obj is within the trigger, false
       otherwise

       parameters:
           obj - the object that needs to be tested for being within the trigger
    */
    virtual bool isWithin(const TriggerObject* obj) const;
  protected:
    //the box that represents the trigger area
    Ogre::AxisAlignedBox m_Box;
}; //class AABoxTrigger

/* SphereTrigger abstract class

   Provides a class for a trigger with spheric trigger area. It implements
   methods to get and set the trigger's sphere, and implements isWithin() for a
   sphere. However, it does not implement any trigger behaviour.
*/
class SphereTrigger: public virtual Trigger
{
  public:
    /* constructor */
    SphereTrigger();

    /* constructor with sphere initialisation

       parameters:
           sphere - the sphere that describes the trigger area
    */
    SphereTrigger(const Ogre::Sphere& sphere);

    /* destructor */
    ~SphereTrigger();

    /* method to manually set the trigger's sphere

       parameters:
           sphere - the sphere that describes the trigger area
    */
    virtual void setSphere(const Ogre::Sphere& sphere);

    /* function to determine whether or not an object is within the trigger
       area - should return true, if object obj is within the trigger, false
       otherwise

       parameters:
           obj - the object that needs to be tested for being within the trigger
    */
    virtual bool isWithin(const TriggerObject* obj) const;

    /* returns the sphere that represents the trigger area */
    virtual const Ogre::Sphere& getSphere() const;
  protected:
    //the sphere that represents the trigger area
    Ogre::Sphere m_Sphere;
}; //class SphereTrigger

} //namespace

#endif // TRIGGER_H
