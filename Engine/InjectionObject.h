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

/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-20
 Purpose: InjectionObject class
          abstract class/ interface for objects that are animated or moving
          and thus need to get time-related data on a regular base

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)
     - 2010-07-31 (rev 220) - GetObjectMesh() added (pure virtual here)
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef INJECTIONOBJECT_H
#define INJECTIONOBJECT_H

#include "DuskObject.h"

namespace Dusk
{

class InjectionObject: virtual public DuskObject
{
  public:
    /* constructor */
    InjectionObject();

    /* interface function to inject time for animation/ movement */
    virtual void injectTime(const float SecondsPassed) = 0;

    /* The following pure virtual function declarations are there in order to
       force derived classes to re-implement their own versions of these
       functions.*/
    /* retrieves the object type as an enumeration value, which is useful for derived classes.*/
    virtual ObjectTypes getDuskType() const = 0;

    virtual bool saveToStream(std::ofstream& OutStream) const = 0;

    virtual bool loadFromStream(std::ifstream& InStream) = 0;
  protected:
    /* returns the name/path of the mesh that is used during enabling this
       object

       remarks:
           Every(!) derived, non-abstract class has to implement their own
           version of that function to ensure the use of the right meshes.
           We declare this pure virtual here to enforce implementation in all
           derived classes.
    */
    virtual std::string getObjectMesh() const = 0;
}; //class

} //namespace

#endif // INJECTIONOBJECT_H
