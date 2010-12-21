/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2010 DaSteph, walljumper, thoronador

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
OIS::Mouse* InputSystem::s_mouse = 0;

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
    s_mouse = static_cast<OIS::Mouse*>(inputManager->createInputObject( OIS::OISMouse, true ));

    //Input systems
    s_inputSystemBinding =  &(InputSystemBinding::get());
    s_inputSystemEditor = &(InputSystemEditor::get());

    //current input system is binding input system
    s_keyboard->setEventCallback(s_inputSystemBinding);
    s_mouse->setEventCallback(s_inputSystemBinding);
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
            s_mouse->setEventCallback(s_inputSystemEditor);
            s_inputSystemEditor->toggle();
            break;
        case AIC_EDITOR:
            s_activeInputClass = AIC_BINDING;
            s_keyboard->setEventCallback(s_inputSystemBinding);
            s_mouse->setEventCallback(s_inputSystemBinding);
            s_inputSystemEditor->toggle();
            break;
    }
    return true;
}

bool InputSystem::captureInput()
{
    if (s_keyboard != 0)
        s_keyboard->capture();
    if(s_mouse != 0)
        s_mouse->capture();
    return true;
}

OIS::KeyCode InputSystem::stringToKeyCode(const std::string& key)
{
  if (key=="") return OIS::KC_UNASSIGNED;
  if (key.length()==1)
  {
    switch (key[0])
    { //digits
      case '0': return OIS::KC_0;
      case '1': return OIS::KC_1;
      case '2': return OIS::KC_2;
      case '3': return OIS::KC_3;
      case '4': return OIS::KC_4;
      case '5': return OIS::KC_5;
      case '6': return OIS::KC_6;
      case '7': return OIS::KC_7;
      case '8': return OIS::KC_8;
      case '9': return OIS::KC_9;
      //letters
      case 'a': return OIS::KC_A;
      case 'b': return OIS::KC_B;
      case 'c': return OIS::KC_C;
      case 'd': return OIS::KC_D;
      case 'e': return OIS::KC_E;
      case 'f': return OIS::KC_F;
      case 'g': return OIS::KC_G;
      case 'h': return OIS::KC_H;
      case 'i': return OIS::KC_I;
      case 'j': return OIS::KC_J;
      case 'k': return OIS::KC_K;
      case 'l': return OIS::KC_L;
      case 'm': return OIS::KC_M;
      case 'n': return OIS::KC_N;
      case 'o': return OIS::KC_O;
      case 'p': return OIS::KC_P;
      case 'q': return OIS::KC_Q;
      case 'r': return OIS::KC_R;
      case 's': return OIS::KC_S;
      case 't': return OIS::KC_T;
      case 'u': return OIS::KC_U;
      case 'v': return OIS::KC_V;
      case 'w': return OIS::KC_W;
      case 'x': return OIS::KC_X;
      case 'y': return OIS::KC_Y;
      case 'z': return OIS::KC_Z;
    } //swi
    return OIS::KC_UNASSIGNED;
  } //if length=1

  if (key.length()==2)
  {
    if (key=="f1") return OIS::KC_F1;
    if (key=="f2") return OIS::KC_F2;
    if (key=="f3") return OIS::KC_F3;
    if (key=="f4") return OIS::KC_F4;
    if (key=="f5") return OIS::KC_F5;
    if (key=="f6") return OIS::KC_F6;
    if (key=="f7") return OIS::KC_F7;
    if (key=="f8") return OIS::KC_F8;
    if (key=="f9") return OIS::KC_F9;
    //up arrow key
    if (key=="up") return OIS::KC_UP;
  } //if length==2

  if (key.length()==3)
  {
    if (key=="f10") return OIS::KC_F10;
    if (key=="f11") return OIS::KC_F11;
    if (key=="f12") return OIS::KC_F12;
    //end
    if (key=="end") return OIS::KC_END;
    //tab
    if (key=="tab") return OIS::KC_TAB;
    //+ on numpad
    if (key=="add") return OIS::KC_ADD;
  } //if length==3

  if (key.length()==4)
  {
    if (key=="down") return OIS::KC_DOWN;
    if (key=="home") return OIS::KC_HOME;
    if (key=="left") return OIS::KC_LEFT;
  }

  if (key.length()==5)
  {
    if (key=="enter") return OIS::KC_RETURN;
    if (key=="right") return OIS::KC_RIGHT;
    if (key=="space") return OIS::KC_SPACE;
  } //length==5

  if (key.length()==6)
  {
    if (key=="delete") return OIS::KC_DELETE;
    if (key=="escape") return OIS::KC_ESCAPE;
    if (key=="insert") return OIS::KC_INSERT;
  } //length==6

  if (key=="page_up") return OIS::KC_PGUP;
  if (key=="page_down") return OIS::KC_PGDOWN;
  // + on numpad
  if (key=="numpad_add") return OIS::KC_ADD;
  // - on numpad
  if (key=="numpad_sub") return OIS::KC_SUBTRACT;

  return OIS::KC_UNASSIGNED; //no matching string
}

std::string InputSystem::keyCodeToString(const OIS::KeyCode kc)
{
  switch (kc)
  {
    //digits
    case OIS::KC_0: return "0";
    case OIS::KC_1: return "1";
    case OIS::KC_2: return "2";
    case OIS::KC_3: return "3";
    case OIS::KC_4: return "4";
    case OIS::KC_5: return "5";
    case OIS::KC_6: return "6";
    case OIS::KC_7: return "7";
    case OIS::KC_8: return "8";
    case OIS::KC_9: return "9";
    //letters
    case OIS::KC_A: return "a";
    case OIS::KC_B: return "b";
    case OIS::KC_C: return "c";
    case OIS::KC_D: return "d";
    case OIS::KC_E: return "e";
    case OIS::KC_F: return "f";
    case OIS::KC_G: return "g";
    case OIS::KC_H: return "h";
    case OIS::KC_I: return "i";
    case OIS::KC_J: return "j";
    case OIS::KC_K: return "k";
    case OIS::KC_L: return "l";
    case OIS::KC_M: return "m";
    case OIS::KC_N: return "n";
    case OIS::KC_O: return "o";
    case OIS::KC_P: return "p";
    case OIS::KC_Q: return "q";
    case OIS::KC_R: return "r";
    case OIS::KC_S: return "s";
    case OIS::KC_T: return "t";
    case OIS::KC_U: return "u";
    case OIS::KC_V: return "v";
    case OIS::KC_W: return "w";
    case OIS::KC_X: return "x";
    case OIS::KC_Y: return "y";
    case OIS::KC_Z: return "z";
    //Fxx keys
    case OIS::KC_F1: return "f1";
    case OIS::KC_F2: return "f2";
    case OIS::KC_F3: return "f3";
    case OIS::KC_F4: return "f4";
    case OIS::KC_F5: return "f5";
    case OIS::KC_F6: return "f6";
    case OIS::KC_F7: return "f7";
    case OIS::KC_F8: return "f8";
    case OIS::KC_F9: return "f9";
    case OIS::KC_F10: return "f10";
    case OIS::KC_F11: return "f11";
    case OIS::KC_F12: return "f12";
    //arrow keys
    case OIS::KC_LEFT: return "left";
    case OIS::KC_RIGHT: return "right";
    case OIS::KC_UP: return "up";
    case OIS::KC_DOWN: return "down";
    //special keys
    case OIS::KC_END: return "end";
    case OIS::KC_HOME: return "home";
    case OIS::KC_TAB: return "tab";
    case OIS::KC_SPACE: return "space";
    case OIS::KC_RETURN: return "enter";
    case OIS::KC_ESCAPE: return "escape";
    case OIS::KC_DELETE: return "delete";
    case OIS::KC_INSERT: return "insert";
    case OIS::KC_PGUP: return "page_up";
    case OIS::KC_PGDOWN: return "page_down";
    //numpad
    case OIS::KC_ADD: return "numpad_add";
    case OIS::KC_SUBTRACT: return "numpad_sub";
    //no valid/ recognised key
    default: return "";
  } //swi
}

}//end namespace
