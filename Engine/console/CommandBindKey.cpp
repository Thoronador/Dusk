/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "CommandBindKey.h"
#include "../InputSystemBinding.h"
#include "../InputSystemEditor.h"
#include "../Messages.h"

namespace Dusk
{

/* ---- class CommandBindKey ---- */

CommandBindKey::CommandBindKey(const std::string& key, const std::string& action)
: Command(),
  m_Key(key),
  m_Action(action)
{
}

CommandBindKey::~CommandBindKey()
{
  //empty
}

bool CommandBindKey::execute(Dusk::Scene* scene, int count)
{
  const OIS::KeyCode code = InputSystemBinding::get().stringToKeyCode(m_Key);
  if (code == OIS::KC_UNASSIGNED)
  {
    DuskLog() << "CommandBindKey::execute: ERROR: \""<<m_Key<<"\" does not "
              << "identify a valid key.\n";
    return false;
  }//if
  if (InputSystemBinding::get().isBound(code))
  {
    DuskLog() << "CommandBindKey::execute: ERROR: key is already bound.\n";
    return false;
  }
  return InputSystemBinding::get().bindKey(code, m_Action);
}

/* ---- class CommandUnbindKey ---- */

CommandUnbindKey::CommandUnbindKey(const std::string& key)
: Command(),
  m_Key(key)
{
}

CommandUnbindKey::~CommandUnbindKey()
{
  //empty
}

bool CommandUnbindKey::execute(Dusk::Scene* scene, int count)
{
  const OIS::KeyCode code = InputSystemBinding::get().stringToKeyCode(m_Key);
  if (code == OIS::KC_UNASSIGNED)
  {
    DuskLog() << "CommandUnbindKey::execute: ERROR: \""<<m_Key<<"\" does not "
              << "identify a valid key.\n";
    return false;
  }//if
  return InputSystemBinding::get().unbindKey(code);
}

/* ---- class CommandListKeys ---- */

CommandListKeys::CommandListKeys()
{
  //empty
}

CommandListKeys::~CommandListKeys()
{
  //empty
}

bool CommandListKeys::execute(Dusk::Scene* scene, int count)
{
  std::map<OIS::KeyCode, std::string> key_list = InputSystemBinding::get().listKeyBindings();
  std::map<OIS::KeyCode, std::string>::const_iterator iter = key_list.begin();
  while (iter!=key_list.end())
  {
    InputSystemEditor::get().addMessage(
        InputSystemBinding::get().keyCodeToString(iter->first)+ " " +iter->second);
    ++iter;
  }//while
  return true;
}


} //namespace
