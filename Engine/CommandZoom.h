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
