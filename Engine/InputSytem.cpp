#include "InputSystem.h"
#include "InputSystemBinding.h"
#include "InputSystemEditor.h"

#include <sstream>

namespace Dusk
{

eActiveInputClass InputSystem::s_activeInputClass = AIC_NONE;
InputSystemBinding* InputSystem::s_inputSystemBinding = 0;
InputSystemEditor* InputSystem::s_inputSystemEditor = 0;
OIS::Keyboard* InputSystem::s_keyboard = 0;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

bool InputSystem::initializeInput(Ogre::RenderWindow* win, Ogre::Root* root)
{
    OIS::ParamList paramList;
    size_t tmp = 0;
    win->getCustomAttribute("WINDOW",&tmp);
    std::stringstream u;
    u << tmp;
    paramList.insert(std::make_pair(std::string("WINDOW"),u.str()));
    OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(paramList);

    //Input devicesa
    s_keyboard = static_cast<OIS::Keyboard*>(inputManager->createInputObject( OIS::OISKeyboard, true ));

    //Input systems
    s_inputSystemBinding = new InputSystemBinding();
    s_inputSystemEditor = new InputSystemEditor(root);

    //current input system is binding input system
    s_keyboard->setEventCallback(s_inputSystemBinding);
    s_activeInputClass = AIC_BINDING;

    return true;
}

bool InputSystem::toggleInput()
{
    switch (s_activeInputClass)
    {
        case AIC_NONE:
            break;
        case AIC_BINDING:
            s_activeInputClass = AIC_EDITOR;
            s_keyboard->setEventCallback(s_inputSystemEditor);
            s_inputSystemEditor->toggle();
            break;
        case AIC_EDITOR:
            s_activeInputClass = AIC_BINDING;
            s_keyboard->setEventCallback(s_inputSystemBinding);
            s_inputSystemEditor->toggle();
            break;
    }
    return true;
}

bool InputSystem::captureInput()
{
    if (s_keyboard != 0)
        s_keyboard->capture();
    return true;
}

}//end namespace
