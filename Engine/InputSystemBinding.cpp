/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 DaSteph, walljumper, thoronador

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

#include "InputSystemBinding.h"
#include <fstream>
#include "Console.h"
#include "Menu.h"
#include "DuskFunctions.h"
#include "Camera.h"
#include "Messages.h"

namespace Dusk{

const std::string InputSystemBinding::cKeyConfigurationFile = "keys.conf";
const float cCameraRotationFactor = 0.5f;

InputSystemBinding::InputSystemBinding()
{
  bool loadPredefined = false;
  if (FileExists(cKeyConfigurationFile))
  {
    if (loadKeyConfiguration(cKeyConfigurationFile))
    {
      DuskLog() << "Info: Key bindings successfully loaded from file \""
                <<cKeyConfigurationFile<<"\".\n";
    }
    else
    {
      DuskLog() << "Info: Key bindings from file \""<<cKeyConfigurationFile
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
    DuskLog() << "Info: setting predefined key bindings.\n";
    //predefined bindings
    myBindListPress.clear();
    myBindListPress[OIS::KC_ESCAPE] = Script("quit");
    myBindListPress[OIS::KC_W] = Script("move_forward");
    myBindListPress[OIS::KC_S] = Script("move_backward");
    myBindListPress[OIS::KC_A] = Script("step_left");
    myBindListPress[OIS::KC_D] = Script("step_right");
    myBindListPress[OIS::KC_Q] = Script("turn_left");
    myBindListPress[OIS::KC_E] = Script("turn_right");
    myBindListPress[OIS::KC_SPACE] = Script("jump");
    myBindListPress[OIS::KC_P] = Script("pick_up");
    // --- Questlog ---
    myBindListPress[OIS::KC_L] = Script("toggle_questlog");
    myBindListPress[OIS::KC_ADD] = Script("questlog_increase");
    myBindListPress[OIS::KC_SUBTRACT] = Script("questlog_decrease");
    // --- screenshot
    myBindListPress[OIS::KC_C] = Script("screenshot");

    myBindListRelease.clear();
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
  // We don't need to save key configuration yet, because the player cannot
  // change it yet ingame.
  //saveKeyConfiguration(cKeyConfigurationFile);
}

bool InputSystemBinding::keyPressed (const OIS::KeyEvent &arg)
{
    if (arg.key == OIS::KC_F12)
        InputSystem::toggleInput();
    else
    {
        //are we in dialogue menu?
        if (Menu::getSingleton().isDialogueActive())
        {
          switch (arg.key)
          {
            case OIS::KC_ESCAPE:
                 Menu::getSingleton().nextDialogueChoice(0);
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
                 Menu::getSingleton().nextDialogueChoice(arg.key+1-OIS::KC_1);
                 return true; break;
            case OIS::KC_0:
                 Menu::getSingleton().nextDialogueChoice(10);
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
    //check left/right movement
    if (arg.state.X.rel!=0)
    {
      Camera::getSingleton().rotateOnceX(-cCameraRotationFactor* arg.state.X.rel);
    }
    //check up/down movement
    if (arg.state.Y.rel!=0)
    {
      Camera::getSingleton().rotateOnceY(-cCameraRotationFactor* arg.state.Y.rel);
    }
    return true;
}

bool InputSystemBinding::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (id==OIS::MB_Left)
    {
      Console::getInstance()->addScript(Script("StartAttack"));
    }
    return true;
}

bool InputSystemBinding::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    if (id==OIS::MB_Left)
    {
      Console::getInstance()->addScript(Script("StopAttack"));
    }
    return true;
}

bool InputSystemBinding::isBound(const OIS::KeyCode kc) const
{
  if (myBindListPress.find(kc)!=myBindListPress.end()) return true;
  if (myBindListRelease.find(kc)!=myBindListRelease.end()) return true;
  return (myBindListHold.find(kc)!=myBindListHold.end());
}

bool InputSystemBinding::bindKey(const OIS::KeyCode kc, const std::string& key_string)
{
  if (kc==OIS::KC_UNASSIGNED) return false;
  if (isBound(kc))
  {
    DuskLog() << "InputSystemBinding::bindKey: Error: the given key is already"
              << " bound.\n";
    return false;
  }
  const Script pressScript = getPressScriptFromKeyString(key_string);
  const Script releaseScript = getReleaseScriptFromKeyString(key_string);
  if (pressScript.isEmpty() and releaseScript.isEmpty())
  {
    //At least one script should contain something. Otherwise the key has not
    //any related actions and/or key_string contains an invalid value.
    DuskLog() << "InputSystemBinding::bindKey: Error: string \""<< key_string
              <<"\" does not identify a valid key.\n";
    return false;
  }
  //press binding
  if (pressScript.isEmpty())
  {
    myBindListPress.erase(kc);
  }
  else
  {
    myBindListPress[kc] = pressScript;
  }
  //release binding
  if (releaseScript.isEmpty())
  {
    myBindListRelease.erase(kc);
  }
  else
  {
    myBindListRelease[kc] = releaseScript;
  }
  //hold binding (currently not used)
  myBindListHold.erase(kc);
  return true;
}

bool InputSystemBinding::unbindKey(const OIS::KeyCode kc)
{
  if (kc==OIS::KC_UNASSIGNED) return false;
  if (isBound(kc))
  {
    myBindListPress.erase(kc);
    myBindListRelease.erase(kc);
    myBindListHold.erase(kc);
    return true;
  }
  return false;
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
  if (str_rep=="jump") return "jump_key";
  if (str_rep=="pick_up") return "pick_up_key";
  if (str_rep=="toggle_fog") return "debug_fog_key";
  if (str_rep=="toggle_rain") return "debug_rain_key";
  if (str_rep=="toggle_snow") return "debug_snow_key";
  if (str_rep=="toggle_questlog") return "questlog_key";
  if (str_rep=="questlog_increase") return "questlog_increase_key";
  if (str_rep=="questlog_decrease") return "questlog_decrease_key";
  if (str_rep=="screenshot") return "screenshot_key";

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
  if (ks=="jump_key") return Script("jump");
  if (ks=="pick_up_key") return Script("pick_up");
  if (ks=="questlog_key") return Script("toggle_questlog");
  if (ks=="questlog_increase_key") return Script("questlog_increase");
  if (ks=="questlog_decrease_key") return Script("questlog_decrease");

  if (ks=="debug_fog_key") return Script("toggle_fog");
  if (ks=="debug_rain_key") return Script("toggle_rain");
  if (ks=="debug_snow_key") return Script("toggle_snow");

  if (ks=="screenshot_key") return Script("screenshot");

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
  if (ks=="jump_key") return Script("");
  if (ks=="pick_up_key") return Script("");
  if (ks=="questlog_key") return Script("");
  if (ks=="questlog_increase_key" or ks=="questlog_decrease_key") return Script("");

  if (ks=="debug_fog_key") return Script("");
  if (ks=="debug_rain_key") return Script("");
  if (ks=="debug_snow_key") return Script("");

  if (ks=="screenshot_key") return Script("");

  return Script(""); //no known key -> empty script
}

bool InputSystemBinding::loadKeyConfiguration(const std::string& fileName)
{
  std::ifstream inFile;
  inFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!inFile)
  {
    //could not open file
    DuskLog() << "InputSystemBinding::loadKeyConfiguration: ERROR: could not "
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
          DuskLog() << "InputSystemBinding::LoadKeyConfiguration: Hint: line "<<i
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
            DuskLog() << "InputSystemBinding::loadKeyConfiguration: Error: line "
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
            DuskLog() << "InputSystemBinding::loadKeyConfiguration: Error: line "
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

bool InputSystemBinding::saveKeyConfiguration(const std::string& fileName) const
{
  std::ofstream outFile;
  outFile.open(fileName.c_str(), std::ios_base::out | std::ios_base::binary
                                  | std::ios_base::trunc);
  if (!outFile)
  {
    //could not open file
    DuskLog() << "InputSystemBinding::saveKeyConfiguration: ERROR: could not "
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
      DuskLog() << "InputSystemBinding::saveKeyConfiguration: ERROR: OIS key "
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
