/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2010, 2013  thoronador

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

#include "InjectionObject.h"

namespace Dusk
{

InjectionObject::InjectionObject()
: DuskObject()
{
  //empty
}

InjectionObject::InjectionObject(const std::string& _ID, const Ogre::Vector3& pos, const Ogre::Quaternion& rot, const float Scale)
: DuskObject(_ID, pos, rot, Scale)
{
  //empty
}

} //namespace
