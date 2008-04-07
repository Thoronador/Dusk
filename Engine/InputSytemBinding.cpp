#include "InputSystemBinding.h"

#include "Console.h"
#include "API.h"

namespace Dusk{

InputSystemBinding::InputSystemBinding()
{
    myBindListPress[OIS::KC_ESCAPE] = Script("quit");
    myBindListPress[OIS::KC_W] = Script("move_forward");
    myBindListPress[OIS::KC_S] = Script("move_backward");
    myBindListPress[OIS::KC_A] = Script("step_left");
    myBindListPress[OIS::KC_D] = Script("step_right");
    //myBindListPress[OIS::KC_SPACE] = Script("jump");

    myBindListRelease[OIS::KC_W] = Script("move_backward");
    myBindListRelease[OIS::KC_S] = Script("move_forward");
    myBindListRelease[OIS::KC_A] = Script("step_right");
    myBindListRelease[OIS::KC_D] = Script("step_left");
}

InputSystemBinding::~InputSystemBinding()
{

}

bool InputSystemBinding::keyPressed (const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_GRAVE)
        InputSystem::toggleInput();
    else
    {
        //is it a PressBinding?
        std::map<OIS::KeyCode, Script>::iterator it = myBindListPress.find(arg.key);
        if (it != myBindListPress.end())
            Console::getInstance()->addScript(it->second);
        // is it a hold binding?
        it = myBindListHold.find(arg.key);
        if (it != myBindListHold.end())
            Console::getInstance()->addScript(it->second.getStartScript());
    }

    return true;
}

bool InputSystemBinding::keyReleased (const OIS::KeyEvent &arg)
{
    //is it a ReleaseBinding?
    std::map<OIS::KeyCode, Script>::iterator it = myBindListRelease.find(arg.key);
    if (it != myBindListRelease.end())
        Console::getInstance()->addScript(it->second);
    // is it a HoldBindinginding?
    it = myBindListHold.find(arg.key);
    if (it != myBindListHold.end())
        Console::getInstance()->addScript(it->second.getStopScript());

    return true;
}
InputSystemBinding& InputSystemBinding::get()
{
    static InputSystemBinding Instance;
    return Instance;
}

bool InputSystemBinding::mouseMoved( const OIS::MouseEvent &arg )
{
    const std::string mouseMoveCommand = "MoveMouse ";
    Console::getInstance()->addScript(Script());
    return true;
}

bool InputSystemBinding::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

bool InputSystemBinding::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

} // end namespace
