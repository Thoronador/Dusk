/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2011, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Trigger.h"
#include "lua/LuaEngine.h"

namespace Dusk
{

Trigger::~Trigger()
{
  //remove all objects from list
  m_ObjectList.clear();
}

void Trigger::addToTrigger(TriggerObject* obj)
{
  m_ObjectList.insert(obj);
}

bool Trigger::removeFromTrigger(TriggerObject* obj)
{
  return m_ObjectList.erase(obj)>0;
}

bool Trigger::isInList(TriggerObject* obj) const
{
  return m_ObjectList.find(obj)!=m_ObjectList.end();
}

unsigned int Trigger::getNumberOfObjectsWithin() const
{
  return m_ObjectList.size();
}

void Trigger::processObjects()
{
  //first remove unwanted objects
  checkForRemoval();
  //now process objects
  std::set<TriggerObject*>::const_iterator iter = m_ObjectList.begin();
  while (iter!=m_ObjectList.end())
  {
    onWithin(*iter);
    ++iter;
  }//while
}

void Trigger::checkForRemoval()
{
  std::set<TriggerObject*>::const_iterator iter = m_ObjectList.begin();
  while (iter!=m_ObjectList.end())
  {
    //Did the object move out of the trigger area?
    if (!isWithin(*iter))
    {
      //call onExit() event handler for object
      onExit(*iter);
      //...and now remove object
      m_ObjectList.erase(iter);
      //Erased iterator is now invalid, that's why we have to set a new value.
      // TO-DO:
      // The begin of the list is valid, but not the optimal value. Optimal
      // would be the object after the erased iterator.
      iter = m_ObjectList.begin();
    }
    else
    {
      ++iter;
    }
  }//while
}

/* ---- AABoxTrigger methods ---- */
AABoxTrigger::AABoxTrigger()
{
  m_Box.setExtents(0, 0, 0, 0, 0, 0);
  m_Box.setNull();
}

AABoxTrigger::AABoxTrigger(const Ogre::AxisAlignedBox& box)
{
  m_Box = box;
}

AABoxTrigger::~AABoxTrigger()
{
  //empty
}

void AABoxTrigger::setBox(const Ogre::AxisAlignedBox& box)
{
  m_Box = box;
}

const Ogre::AxisAlignedBox& AABoxTrigger::getBox() const
{
  return m_Box;
}

bool AABoxTrigger::isWithin(const TriggerObject* obj) const
{
  if (obj==NULL or m_Box.isNull())
  {
    return false;
  }
  return m_Box.intersects(obj->getPosition());
}

/* ---- SphereTrigger methods ---- */

SphereTrigger::SphereTrigger()
{
  //set zero radius sphere, i.e. point, at origin
  m_Sphere = Ogre::Sphere(Ogre::Vector3(0,0,0), 0);
}

SphereTrigger::SphereTrigger(const Ogre::Sphere& sphere)
{
  m_Sphere = sphere;
}

SphereTrigger::~SphereTrigger()
{
  //empty
}

void SphereTrigger::setSphere(const Ogre::Sphere& sphere)
{
  m_Sphere = sphere;
}

bool SphereTrigger::isWithin(const TriggerObject* obj) const
{
  if (obj!=NULL)
  {
    return m_Sphere.intersects(obj->getPosition());
  }
  //NULL objects are not within the sphere
  return false;
}

const Ogre::Sphere& SphereTrigger::getSphere() const
{
  return m_Sphere;
}

/* ---- ScriptedTrigger methods ---- */

ScriptedTrigger::ScriptedTrigger()
{
  m_EnterScript = Script();
  m_WithinScript = Script();
  m_ExitScript = Script();
}

ScriptedTrigger::ScriptedTrigger(const Script& enter, const Script& within, const Script& exit)
{
  m_EnterScript = enter;
  m_WithinScript = within;
  m_ExitScript = exit;
}

ScriptedTrigger::~ScriptedTrigger()
{
  //empty
}

const Script& ScriptedTrigger::getEnterScript() const
{
  return m_EnterScript;
}

const Script& ScriptedTrigger::getWithinScript() const
{
  return m_WithinScript;
}

const Script& ScriptedTrigger::getExitScript() const
{
  return m_ExitScript;
}

void ScriptedTrigger::setEnterScript(const Script& scr)
{
  m_EnterScript = scr;
}

void ScriptedTrigger::setWithinScript(const Script& scr)
{
  m_WithinScript = scr;
}

void ScriptedTrigger::setExitScript(const Script& scr)
{
  m_ExitScript = scr;
}

void ScriptedTrigger::onEnter(TriggerObject* obj)
{
  if ((obj==NULL) or m_EnterScript.isEmpty()) return;
  std::string temp;
  LuaEngine::getSingleton().runString(m_EnterScript.getStringRepresentation(), &temp);
}

void ScriptedTrigger::onExit(TriggerObject* obj)
{
  if ((obj==NULL) or m_ExitScript.isEmpty()) return;
  std::string temp;
  LuaEngine::getSingleton().runString(m_ExitScript.getStringRepresentation(), &temp);
}

void ScriptedTrigger::onWithin(TriggerObject* obj)
{
  if ((obj==NULL) or m_WithinScript.isEmpty()) return;
  std::string temp;
  LuaEngine::getSingleton().runString(m_ExitScript.getStringRepresentation(), &temp);
}

} //namespace
