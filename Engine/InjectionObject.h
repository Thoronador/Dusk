/*---------------------------------------------------------------------------
 Author:  thoronador
 Date:    2010-05-20
 Purpose: InjectionObject class
          abstract class/ interface for objects that are animated or moving
          and thus need to get time-related data on a regular base

 History:
     - 2010-05-20 (rev 205) - initial version (by thoronador)

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
    virtual ObjectTypes GetType() const = 0;

    virtual bool SaveToStream(std::ofstream& OutStream) const = 0;

    virtual bool LoadFromStream(std::ifstream& InStream) = 0;
}; //class

} //namespace

#endif // INJECTIONOBJECT_H
