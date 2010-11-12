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

          ScriptedTrigger abstract class
          provides a class for a trigger with scripts

 History:
     - 2010-08-30 (rev 238) - initial version (by thoronador)
     - 2010-11-10 (rev 251) - small fix; ScriptedTrigger added
                            - methods for handling objects within trigger added
                              (pointers to objects that are affected by trigger
                               can now be stored in trigger)
     - 2010-11-12 (rev 252) - checkForRemoval() added

 ToDo list:
     - provide a way for scripts of ScriptedTrigger to access the related
       TriggerObject
     - store objects that are currently within a trigger in the Trigger for
       faster access
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell us please.
 --------------------------------------------------------------------------*/

#ifndef TRIGGER_H
#define TRIGGER_H

#include <OgreAxisAlignedBox.h>
#include <OgreSphere.h>
#include "NPC.h"
#include "Script.h"

namespace Dusk
{

//set TriggerObject as alias for NPC - we only want NPCs to fire triggers
typedef NPC TriggerObject;
const ObjectTypes cTriggerTypeEnum = otNPC;

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

    /* adds an object to the trigger */
    void addToTrigger(TriggerObject* obj);

    /* tries to remove the given object from the trigger and returns true, if
       the object could be removed
    */
    bool removeFromTrigger(TriggerObject* obj);

    /* returns true, if the given object is in the internal list */
    bool isInList(TriggerObject* obj) const;

    /* returns the number of objects that are currently within the trigger */
    unsigned int getNumberOfObjectsWithin() const;

    //calls onWithin() for every object within the trigger
    void processObjects();
  protected:
    bool compLesser(const TriggerObject* a, const TriggerObject* b) const;
    std::set<TriggerObject*> m_ObjectList;

    /*checks if objects within the trigger could be removed, because they moved
      out of the trigger area since last frame
    */
    virtual void checkForRemoval();
}; //class Trigger


/* AABoxTrigger abstract class

   Provides a class for a trigger with an axis aligned box as trigger area.
   It implements methods to get and set the trigger's box, and implements
   isWithin() for a box. However, it does not implement any trigger behaviour.
*/
class AABoxTrigger: public virtual Trigger
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

/* ScriptTrigger abstract class

   Provides a class for a trigger that uses Scripts to implement event handling.
   However, it does not implement any trigger area checks.
*/
class ScriptedTrigger: public virtual Trigger
{
  public:
    /* constructor */
    ScriptedTrigger();

    /* constructor with script initialisation

       parameters:
           enter  - script that is called for entering objects
           within - script that is called for objects within trigger
           exit   - script that is called for leaving objects
    */
    ScriptedTrigger(const Script& enter, const Script& within, const Script& exit);

    /* destructor */
    ~ScriptedTrigger();

    //script access functions
    /* returns the script that is called upon entering the trigger */
    virtual const Script& getEnterScript() const;

    /* returns the script that is called while within the trigger */
    virtual const Script& getWithingScript() const;

    /* returns the script that is called upon leaving the trigger */
    virtual const Script& getExitScript() const;

    //script setters
    /* sets the script that is called upon entering the trigger

       parameters:
           scr - the new script
    */
    virtual void setEnterScript(const Script& scr);

    /* sets the script that is called while within the trigger

       parameters:
           scr - the new script
    */
    virtual void setWithinScript(const Script& scr);

    /* sets the script that is called upon leaving the trigger

       parameters:
           scr - the new script
    */
    virtual void setExitScript(const Script& scr);

    /* is called whenever an object enters the trigger area

       parameters:
           obj - the object that entered the trigger
    */
    virtual void onEnter(TriggerObject* obj);

    /* is called whenever an object leaves the trigger area

       parameters:
           obj - the object that entered the trigger
    */
    virtual void onExit(TriggerObject* obj);

    /* is called once every frame for each object within the trigger area

       parameters:
           obj - the object that is within the trigger area
    */
    virtual void onWithin(TriggerObject* obj);
  protected:
    //the scripts
    Script m_EnterScript;
    Script m_WithinScript;
    Script m_ExitScript;
}; //class ScriptedTrigger

} //namespace

#endif // TRIGGER_H
