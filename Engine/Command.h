/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008 ssj5000, walljumper

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
 Author:  ssj5000, walljumper
 Date:    2008-02-28
 Purpose: Command class
          basic class for Console commands

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2007-12-30 (rev 17)  - member count and virtual execute() added
                              (by walljumper)
     - 2008-02-28 (rev 48)  - non-virtual execute() added (by ssj5000)
     - 2010-05-06 (rev 197) - documentation update (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs.
 --------------------------------------------------------------------------*/

#ifndef COMMAND_H
#define COMMAND_H

namespace Dusk
{
    class Scene;
    class Command
    {
    public:
        /* constructor */
        Command();
        /* destructor */
        virtual ~Command();
        void execute();

        /* pure virtual function for executing command

           remarks:
               Every derived class has to have its own implementation of that
               function. This one only provides the common interface.
        */
        virtual bool execute(Dusk::Scene* scene, int count = 1) = 0;
    protected:
        int m_Count; // contains the number how many times the Command shall be executed
    };
}

#endif // COMMAND_H
