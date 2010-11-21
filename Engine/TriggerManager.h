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
 Date:    2010-11-12
 Purpose: TriggerManager Singleton class
          holds all triggers in the game

 History:
     - 2010-11-12 (rev 252) - initial version (by thoronador)

 ToDo list:
     - load/ save triggers
     - add possibility to create triggers on demand
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell us please.
 --------------------------------------------------------------------------*/

#ifndef TRIGGERMANAGER_H
#define TRIGGERMANAGER_H

#include <set>
#include "Trigger.h"

namespace Dusk
{

class TriggerManager
{
  public:
    /* destructor */
    ~TriggerManager();

    /* singleton access method */
    static TriggerManager& getSingleton();

    /* returns the number of triggers managed by this class */
    unsigned int getNumberOfTriggers() const;

    /* tries to add the given trigger to the list and returns true on success */
    bool addTrigger(Trigger* trig);

    /* tries to remove the given trigger from the list and returns true, if the
       trigger was in the list and could be removed
    */
    bool removeTrigger(Trigger* trig);

    /* iterator type to iterate over complete trigger list */
    typedef std::set<Trigger*>::const_iterator ConstIterator;

    /* returns an iterator to the start of the internal data structure that
       holds all triggers
    */
    ConstIterator getBegin() const;

    /* returns an iterator to the end of the internal data structure that holds
       all triggers
    */
    ConstIterator getEnd() const;
  private:
    /* constructor - private due to singleton pattern */
    TriggerManager();

    /* empty copy constructor (singleton pattern) */
    TriggerManager(const TriggerManager& op) {}

    std::set<Trigger*> m_TriggerList;
}; //class

} //namespace

#endif // TRIGGERMANAGER_H
