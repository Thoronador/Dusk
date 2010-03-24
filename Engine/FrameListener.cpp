#include "FrameListener.h"
#include<sstream>
#include "Console.h"
#include "InputSystem.h"
#include "InputSystemEditor.h"
#include "InputSystemBinding.h"
#include "InputSystem.h"
#include "API.h"
#include "Camera.h"
#include "AnimationData.h"
#include "LuaEngine.h"

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
    LuaEngine::GetSingleton().processScripts();
    Camera::getSingleton().move(evt);
    AnimationData::GetSingleton().InjectAnimationTime(evt.timeSinceLastFrame);
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

}

