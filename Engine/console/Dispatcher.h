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
 Date:    2008-02-22
 Purpose: Dispatcher Singleton class

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2008-01-17 (rev 23)  - dispatcher now registers itself at API
                              (by walljumper)
     - 2008-01-17 (rev 24)  - executeCommand() added
     - 2008-01-24 (rev 27)  - Dispatcher is now singleton
     - 2008-01-26 (rev 28)  - }
     - 2008-01-26 (rev 29)  - }  fixed some bugs
     - 2008-02-22 (rev 39)  - }
     - 2010-01-01 (rev 148) - documentation update (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Scene.h"
namespace Dusk
{
    class Command;
    class Dispatcher
    {
    public:
        static Dispatcher& get();
        virtual ~Dispatcher();
        bool executeCommand(Dusk::Command* command);
    protected:
    private:
        Dusk::Scene* m_Scene;
        Dispatcher();
        Dispatcher(const Dispatcher& op){}
    };
} // namespace

#endif // DISPATCHER_H
