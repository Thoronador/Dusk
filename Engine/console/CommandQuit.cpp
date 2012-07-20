/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2010 DaSteph, walljumper, thoronador

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

#include "CommandQuit.h"
#include "../Messages.h"
#include "../InputSystemEditor.h"

namespace Dusk
{
  CommandQuit::CommandQuit()
  {
      //ctor
  }

  CommandQuit::~CommandQuit()
  {
      //dtor
  }

  bool CommandQuit::execute(Dusk::Scene* scene, int count)
  {
    DuskLog() << "Dusk: exit\n  Destroying scene...\n";
    Scene::get().destroyScene();
    DuskLog() << "  Send exit signal to ISE...\n";
    InputSystemEditor::get().exit();
    return true;
  }

} //namespace
