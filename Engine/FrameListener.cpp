#include "FrameListener.h"
#include<sstream>
#include "Console.h"
#include "InputSystem.h"
#include "InputSystemEditor.h"
#include "InputSystemBinding.h"
#include "InputEvent.h"

namespace Dusk
{

FrameListener::FrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,Ogre::Root* root,bool bufferdInput)
{
    //ctor
    m_Window = win;
    m_Camera = cam;
    m_Root = root;

    m_Continue = true;
    m_UseBufferdInput = bufferdInput;
    m_EditorIsVisible = false;

    OIS::ParamList paramList;
    size_t tmp = 0;
    win->getCustomAttribute("WINDOW",&tmp);
    std::stringstream u;
    u<<tmp;
    paramList.insert(std::make_pair(std::string("WINDOW"),u.str()));

    m_InputManager = OIS::InputManager::createInputSystem(paramList);

    m_Keyboard = static_cast<OIS::Keyboard*>(m_InputManager->createInputObject( OIS::OISKeyboard, m_UseBufferdInput ));
    m_Mouse = static_cast<OIS::Mouse*>(m_InputManager->createInputObject( OIS::OISMouse, m_UseBufferdInput ));
    m_InputSystemBinding = new InputSystemBinding();
    m_InputSystemEditor = new InputSystemEditor(m_Root);
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
    if(m_Keyboard->isKeyDown(OIS::KC_1))
    {
        m_EditorIsVisible = !m_EditorIsVisible;
        m_InputSystemEditor->toggle();

        if(m_InputHandler.getInputSystem() == m_InputSystemBinding)
            m_InputHandler.setInputSystem(m_InputSystemEditor);
        else
            m_InputHandler.setInputSystem(m_InputSystemBinding);
    }

    return true;
}


//-----------------------------------------------------------------------------------
// InputHandler
void FrameListener::InputHandler::setInputSystem(Dusk::InputSystem* inputSystem)
{
    m_InputSystem = inputSystem;
}
Dusk::InputSystem* FrameListener::InputHandler::getInputSystem()
{
    return m_InputSystem;
}

}

