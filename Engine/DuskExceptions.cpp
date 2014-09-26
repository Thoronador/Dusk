/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012, 2014  Thoronador

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

#include "DuskExceptions.h"

namespace Dusk
{

IDNotFound::IDNotFound(const std::string& baseClass, const std::string& id)
: m_what("Error in "+baseClass+": No object with ID \""+id+"\" found!")
{
}

IDNotFound::~IDNotFound() throw()
{
  //empty, but we need this because of the throw() specifier
}

const char* IDNotFound::what() const throw()
{
  return m_what.c_str();
}

} //namespace
