/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 ssj5000, walljumper, DaSteph, thoronador

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

#include "FrameListener.h"
#include "Console.h"
#include "InputSystem.h"
#include "API.h"
#include "Camera.h"
#include "InjectionManager.h"
#include "TriggerManager.h"
#include "Trigger.h"
#include "LuaEngine.h"
#include "Player.h"
#include "Weather.h"

namespace Dusk
{
//-----------------------------------------------------------------------------------
FrameListener::FrameListener()
{
    m_Continue = true;
    m_TriggersActive = true;
    getAPI().setFrameListener(this);
}
//-----------------------------------------------------------------------------------
FrameListener::~FrameListener()
{
    //dtor
}
//-----------------------------------------------------------------------------------
bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    //update input status
    InputSystem::captureInput();
    //process scripts of console and Lua scripts
    Console::getInstance()->processScripts();
    LuaEngine::getSingleton().processScripts();
    //process camera movement for the current frame
    Camera::getSingleton().move(evt);
    //process animations, movement,... of non-static objects
    InjectionManager::getSingleton().injectAnimationTime(evt.timeSinceLastFrame);
    Player::getSingleton().injectTime(evt.timeSinceLastFrame);

    // ---- triggers ----
    if (m_TriggersActive)
    {
      // -- update list of objects in triggers (this can take a long time!)
      unsigned int i;
      const InjectionManager::ConstMapIterator i_end = InjectionManager::getSingleton().getEnd();
      InjectionManager::ConstMapIterator iter = InjectionManager::getSingleton().getBegin();
      while (iter!=i_end)
      {
        for (i=0; i<iter->second.size(); ++i)
        {
          if (iter->second.at(i)->getDuskType()==cTriggerTypeEnum)
          {
            //object should be checked for being within trigger or not
            TriggerManager::ConstIterator trig_it = TriggerManager::getSingleton().getBegin();
            const TriggerManager::ConstIterator trig_end = TriggerManager::getSingleton().getEnd();
            while (trig_it!=trig_end)
            {
              //is the object (NPC) within trigger's area and not already in its list?
              if ((*trig_it)->isWithin(dynamic_cast<TriggerObject*>(iter->second.at(i)))
                  and (!((*trig_it)->isInList(dynamic_cast<TriggerObject*>(iter->second.at(i))))))
              {
                //add object to trigger and fire onEnter() event
                (*trig_it)->addToTrigger(dynamic_cast<TriggerObject*>(iter->second.at(i)));
                (*trig_it)->onEnter(dynamic_cast<TriggerObject*>(iter->second.at(i)));
              }//if

              ++trig_it;
            }//while
          }//if
        }//for
        ++iter;
      }//while

      //now process all triggers
      TriggerManager::ConstIterator trig_it = TriggerManager::getSingleton().getBegin();
      const TriggerManager::ConstIterator trig_end = TriggerManager::getSingleton().getEnd();
      while (trig_it!=trig_end)
      {
        (*trig_it)->processObjects();
        ++trig_it;
      }//while
    } //if triggers are active

    //sun and so on
    const float cSunSpeedFactor = 1200.0f;
    Weather::getSingelton().addDaytime(evt.timeSinceLastFrame*cSunSpeedFactor);
    return m_Continue;
}
//-----------------------------------------------------------------------------------
bool FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return m_Continue;
}
//-----------------------------------------------------------------------------------
void FrameListener::exit()
{
    m_TriggersActive = false;
    m_Continue = false;
}

void FrameListener::setTriggerActivity(const bool active)
{
  m_TriggersActive = active;
}

bool FrameListener::getTriggerActivity() const
{
  return m_TriggersActive;
}

} //namespace
