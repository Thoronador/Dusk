/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010 thoronador

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

#include "CommandBindKey.h"
#include "InputSystemBinding.h"
#include "Messages.h"

namespace Dusk
{

/* ---- class CommandBindKey ---- */

CommandBindKey::CommandBindKey(const std::string& key, const std::string& action)
{
  m_Key = key;
  m_Action = action;
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
{
  m_Key = key;
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

} //namespace
