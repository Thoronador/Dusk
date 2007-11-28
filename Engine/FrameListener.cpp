#include "FrameListener.h"
#include<sstream>
namespace Dusk
{

FrameListener::FrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,bool bufferdInput)
{
    //ctor
    m_Window = win;
    m_Camera = cam;

    m_Continue = true;
    m_UseBufferdInput = bufferdInput;

    OIS::ParamList paramList;
    size_t tmp = 0;
    win->getCustomAttribute("WINDOW",&tmp);
    std::stringstream u;
    u<<tmp;
    paramList.insert(std::make_pair(std::string("WINDOW"),u.str()));

    m_InputManager = OIS::InputManager::createInputSystem(paramList);

    m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject( OIS::OISKeyboard, m_UseBufferdInput ));
    m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject( OIS::OISMouse, m_UseBufferdInput ));
}

FrameListener::~FrameListener()
{
    //dtor
}

bool FrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    if(!m_UseBufferdInput)
    {
        if(!handleUnbufferdInput())
            {
                //add Errorhandling here
            }
    }

    return m_Continue;
}
bool FrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return m_Continue;
}

bool FrameListener::handleUnbufferdInput()
{
    m_Keyboard->capture();

    if(m_Keyboard->isKeyDown(OIS::KC_ESCAPE)) m_Continue = false;

    return true;
}

}

