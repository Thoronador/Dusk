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
#include "LuaEngine.h"
#include "Player.h"
#include "Weather.h"

namespace Dusk
{
//-----------------------------------------------------------------------------------
FrameListener::FrameListener()
{
    m_Continue = true;
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
    InputSystem::captureInput();
    Console::getInstance()->processScripts();
    LuaEngine::getSingleton().processScripts();
    Camera::getSingleton().move(evt);
    InjectionManager::getSingleton().injectAnimationTime(evt.timeSinceLastFrame);
    Player::getSingleton().injectTime(evt.timeSinceLastFrame);
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
    m_Continue = false;
}

} //namespace
