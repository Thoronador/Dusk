/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2010 ssj5000, walljumper

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

#include "Dispatcher.h"
#include "../API.h"
#include "Command.h"
#include "../Scene.h"
namespace Dusk
{

Dispatcher::Dispatcher()
{
    //ctor
    m_Scene = &(Scene::get());
}

Dispatcher::~Dispatcher()
{
    //dtor
}

bool Dispatcher::executeCommand(Dusk::Command* command)
{
    bool ret = command->execute(m_Scene,0);
    return ret;
}

Dispatcher& Dispatcher::get()
{
    static Dispatcher Instance;
    return Instance;
}

} // namespace
