#include "InputSystemBinding.h"

#include <fstream>
#include "Console.h"
#include "API.h"
#include "Menu.h"
#include "DuskFunctions.h"

namespace Dusk{

const std::string InputSystemBinding::cKeyConfigurationFile = "keys.conf";

InputSystemBinding::InputSystemBinding()
{
  bool loadPredefined = false;
  if (FileExists(cKeyConfigurationFile))
  {
    if (LoadKeyConfiguration(cKeyConfigurationFile))
    {
      std::cout << "Info: Key bindings successfully loaded from file \""
                <<cKeyConfigurationFile<<"\".\n";
    }
    else
    {
      std::cout << "Info: Key bindings from file \""<<cKeyConfigurationFile
                <<"\" could not be loaded.\n";
      loadPredefined = true;
    }
  }
  else
  {
    loadPredefined = true;
  }

  if (loadPredefined)
  {
    std::cout << "Info: setting predefined key bindings.\n";
    //predefined bindings
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
}

InputSystemBinding::~InputSystemBinding()
{
  //empty
  SaveKeyConfiguration(cKeyConfigurationFile);
}

bool InputSystemBinding::keyPressed (const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_F12)
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

std::string InputSystemBinding::getKeyStringFromScript(const Script& scr)
{
  const std::string str_rep = scr.getStringRepresentation();

  if (str_rep=="quit") return "quit_key";
  if (str_rep=="move_forward") return "forward_key";
  if (str_rep=="move_backward") return "backward_key";
  if (str_rep=="step_left") return "left_key";
  if (str_rep=="step_right") return "right_key";
  if (str_rep=="turn_left") return "turn_left_key";
  if (str_rep=="turn_right") return "turn_right_key";
  if (str_rep=="toggle_fog") return "debug_fog_key";
  if (str_rep=="toggle_rain") return "debug_rain_key";
  if (str_rep=="toggle_snow") return "debug_snow_key";

  return ""; //no valid or known key script
}

Script InputSystemBinding::getPressScriptFromKeyString(const std::string& ks)
{
  if (ks=="quit_key") return Script("quit");
  if (ks=="forward_key") return Script("move_forward");
  if (ks=="backward_key") return Script("move_backward");
  if (ks=="left_key") return Script("step_left");
  if (ks=="right_key") return Script("step_right");
  if (ks=="turn_left_key") return Script("turn_left");
  if (ks=="turn_right_key") return Script("turn_right");

  if (ks=="debug_fog_key") return Script("toggle_fog");
  if (ks=="debug_rain_key") return Script("toggle_rain");
  if (ks=="debug_snow_key") return Script("toggle_snow");

  return Script(""); //no known key -> empty script
}

Script InputSystemBinding::getReleaseScriptFromKeyString(const std::string& ks)
{
  if (ks=="quit") return Script("");
  if (ks=="forward_key") return Script("move_backward");
  if (ks=="backward_key") return Script("move_forward");
  if (ks=="left_key") return Script("step_right");
  if (ks=="right_key") return Script("step_left");
  if (ks=="turn_left_key") return Script("turn_right");
  if (ks=="turn_right_key") return Script("turn_left");

  if (ks=="debug_fog_key") return Script("");
  if (ks=="debug_rain_key") return Script("");
  if (ks=="debug_snow_key") return Script("");

  return Script(""); //no known key -> empty script
}

bool InputSystemBinding::LoadKeyConfiguration(const std::string& fileName)
{
  std::ifstream inFile;
  inFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!inFile)
  {
    //could not open file
    std::cout << "InputSystemBinding::LoadKeyConfiguration: ERROR: could not "
              << "open file \""<<fileName<<"\" for reading. Aborting.\n";
    return false;
  }
  char buffer[128];
  buffer[0] = '\0';
  std::string line;
  size_t pos;
  unsigned int i = 1;
  while (inFile.getline(buffer, 127))
  {
    line = std::string(buffer);
    if (line!="") //we only want to process non-empty lines...
    {
      if (line[0]!='#')  //...and those not starting with # (=comment)
      {
        pos = line.find(' ');
        if (pos == std::string::npos)
        {
          std::cout << "InputSystemBinding::LoadKeyConfiguration: Hint: line "<<i
                << " of file \""<<fileName<<"\" is not valid.\n";
          inFile.close();
          return false;
        }
        else
        {
          const std::string sLeft = line.substr(0, pos);
          const std::string sRight = line.substr(pos+1);
          const OIS::KeyCode kc = stringToKeyCode(sRight);
          if (kc==OIS::KC_UNASSIGNED)
          {
            std::cout << "InputSystemBinding::LoadKeyConfiguration: Error: line "
                      << i<<" of file \""<<fileName<<"\": string \""<<sRight
                      << "\" does not identify a valid key.\n";
            inFile.close();
            return false;
          }
          const Script pressScript = getPressScriptFromKeyString(sLeft);
          const Script releaseScript = getReleaseScriptFromKeyString(sLeft);
          if (pressScript.isEmpty() and releaseScript.isEmpty())
          {
            //At least one script should contain something. Otherwise the key
            //has not any related actions.
            std::cout << "InputSystemBinding::LoadKeyConfiguration: Error: line "
                      << i<<" of file \""<<fileName<<"\": the string \""<<sLeft
                      << "\" does not identify a valid option.\n";
            inFile.close();
            return false;
          }
          //we got something, add it
          if (pressScript.isEmpty())
          {
            myBindListPress.erase(kc);
          }
          else
          {
            myBindListPress[kc] = pressScript;
          }
          if (releaseScript.isEmpty())
          {
            myBindListRelease.erase(kc);
          }
          else
          {
            myBindListRelease[kc] = releaseScript;
          }
        }//else branch
      }//if (inner, comment check)
    }//if (outer)
    ++i;
  }//while
  inFile.close();
  return true;
}

bool InputSystemBinding::SaveKeyConfiguration(const std::string& fileName) const
{
  std::ofstream outFile;
  outFile.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary
                                  | std::ios_base::trunc);
  if (!outFile)
  {
    //could not open file
    std::cout << "InputSystemBinding::SaveKeyConfiguration: ERROR: could not "
              << "open file \""<<fileName<<"\". Aborting.\n";
    return false;
  }

  std::string key, value;
  std::map<OIS::KeyCode, Dusk::Script>::const_iterator iter;
  iter = myBindListPress.begin();
  while (iter!=myBindListPress.end())
  {
    key = getKeyStringFromScript(iter->second);
    value = keyCodeToString(iter->first);
    if ((key=="") or (value==""))
    {
      outFile.close();
      std::cout << "InputSystemBinding::SaveKeyConfiguration: ERROR: OIS key "
                << "code has no string representation or bind script contains "
                << "unknown command. Aborting.\n";
      return false;
    } //if
    outFile << key << " " <<value <<"\n";
    ++iter;
  }//while
  outFile.close();
  return outFile.good();
}

} // end namespace
