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
