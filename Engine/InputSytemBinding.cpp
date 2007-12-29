#include "InputSystemBinding.h"

#include "Console.h"

namespace Dusk{

InputSystemBinding::InputSystemBinding()
{
    myBindListPress[OIS::KC_ESCAPE] = Script("quit");
    myBindListHold[OIS::KC_W] = Script("move_forward");
    myBindListHold[OIS::KC_S] = Script("move_backward");
    myBindListHold[OIS::KC_A] = Script("step_left");
    myBindListHold[OIS::KC_D] = Script("step_right");
    myBindListHold[OIS::KC_SPACE] = Script("jump");
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


} // end namespace
