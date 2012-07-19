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
 Date:    2010-05-06
 Purpose: CommandZoom class
          Command for zooming the camera in and out via Console

 History:
     - 2009-12-04 (rev 138) - initial version (by thoronador)
     - 2010-03-24 (rev 186) - adjustments to work with new Singleton pattern
                              Camera class
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDZOOM_H
#define COMMANDZOOM_H

#include "Command.h"

namespace Dusk
{
  class CommandZoom : public Dusk::Command
  {
    public:
      /* Constructor.
           parameter:
             ZoomIn - If ZoomIn is true, the command will cause the camera to
                      zoom in, when executed. Otherwise it will zoom out.
      */
      CommandZoom(const bool ZoomIn);

      /* destructor */
      virtual ~CommandZoom();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_doZoomIn;
  };
}

#endif // COMMANDZOOM_H
