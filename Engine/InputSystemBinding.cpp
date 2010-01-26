#include "InputSystemBinding.h"

#include "Console.h"
#include "API.h"
#include "Menu.h"

namespace Dusk{

InputSystemBinding::InputSystemBinding()
{
    myBindListPress[OIS::KC_ESCAPE] = Script("quit");
    myBindListPress[OIS::KC_W] = Script("move_forward");
    myBindListPress[OIS::KC_S] = Script("move_backward");
    myBindListPress[OIS::KC_A] = Script("step_left");
    myBindListPress[OIS::KC_D] = Script("step_right");
    myBindListPress[OIS::KC_Q] = Script("turn_left");
    myBindListPress[OIS::KC_E] = Script("turn_right");
    //myBindListPress[OIS::KC_SPACE] = Script("jump");

    myBindListRelease[OIS::KC_W] = Script("move_backward");
    myBindListRelease[OIS::KC_S] = Script("move_forward");
    myBindListRelease[OIS::KC_A] = Script("step_right");
    myBindListRelease[OIS::KC_D] = Script("step_left");
    myBindListRelease[OIS::KC_Q] = Script("turn_right");
    myBindListRelease[OIS::KC_E] = Script("turn_left");
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
        //are we in dialogue menu?
        if (Menu::GetSingleton().isDialogueActive())
        {
          switch (arg.key)
          {
            case OIS::KC_ESCAPE:
                 Menu::GetSingleton().nextDialogueChoice(0);
                 return true; break;
            case OIS::KC_1:  //We can handle all numeric keys from 1 to 9 this
            case OIS::KC_2:  // way, because OIS::KC_1 +1 == OIS::KC_2 (and so
            case OIS::KC_3:  // on) allows us to get the number by simple.
            case OIS::KC_4:  // substraction.
            case OIS::KC_5:  // OIS::KC_1 will be translated to 1,
            case OIS::KC_6:  // OIS::KC_2 will be translated to 2, and so on.
            case OIS::KC_7:
            case OIS::KC_8:
            case OIS::KC_9:
                 Menu::GetSingleton().nextDialogueChoice(arg.key+1-OIS::KC_1);
                 return true; break;
            case OIS::KC_0:
                 Menu::GetSingleton().nextDialogueChoice(10);
                 return true; break;
            default: //We have no work here, but include default to avoid the
                     // compiler warning about not handling all enumeration
                     // values in this switch statement.
                 break;
          }//swi
        }//if


        //is it a PressBinding?
        std::map<OIS::KeyCode, Script>::iterator it = myBindListPress.find(arg.key);
        if (it != myBindListPress.end())
            Console::getInstance()->addScript(it->second);
        // is it a hold binding?
        it = myBindListHold.find(arg.key);
        if (it != myBindListHold.end())
            Console::getInstance()->addScript(it->second.getStartScript());
    }

    //temporary bindings to toggle rain, fog and snow
    // for quick enabling/ disabling weather effects; should be removed later!
    switch (arg.key)
    {
      case OIS::KC_R: // R like rain
           Console::getInstance()->addScript(Script("toggle_rain"));
           break;
      case OIS::KC_F:
           Console::getInstance()->addScript(Script("toggle_fog"));
           break;
      case OIS::KC_V: // S like snow is already occupied for movement
           Console::getInstance()->addScript(Script("toggle_snow"));
           break;
      default: break;
    }//swi

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
    //Movement of the mouse wheel is considered movement of the z-axis in OIS.
    if (arg.state.Z.rel>0)
    {
      Console::getInstance()->addScript(Script("ZoomIn"));
    }
    else if (arg.state.Z.rel<0)
    {
      Console::getInstance()->addScript(Script("ZoomOut"));
    }
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
