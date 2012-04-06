/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2012 thoronador

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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2012-04-05
 Purpose: IDNotFound exception class
          an exception that might get thrown, if the engine does not find a
          requested object ID

 History:
     - 2012-04-05 (rev 303) - initial version (by thoronador)

 ToDo list:
     - ???

 Known bugs:
     - None. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DUSKEXCEPTIONS_H
#define DUSKEXCEPTIONS_H

#include <exception>
#include <string>

namespace Dusk
{

class IDNotFound : public std::exception
{
  public:
    /* constructor */
    IDNotFound(const std::string& baseClass, const std::string& id);

    /* destructor */
    virtual ~IDNotFound() throw();

    /* Returns a C-style character string describing the cause of the current
       error.
    */
    virtual const char* what() const throw();
  protected:
    std::string m_base, m_id;
}; //class

} //namespace

#endif // DUSKEXCEPTIONS_H
