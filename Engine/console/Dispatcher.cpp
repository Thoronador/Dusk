/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2010, 2013  ssj5000, walljumper, Thoronador

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

#include "Dispatcher.h"
#include "../API.h"
#include "Command.h"
#include "../Scene.h"
namespace Dusk
{

//ctor
Dispatcher::Dispatcher()
: m_Scene(&(Scene::get()))
{
}

Dispatcher::~Dispatcher()
{
    //dtor
}

bool Dispatcher::executeCommand(Dusk::Command* command)
{
    return command->execute(m_Scene, 0);
}

Dispatcher& Dispatcher::get()
{
    static Dispatcher Instance;
    return Instance;
}

} // namespace
