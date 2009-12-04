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
      virtual ~CommandZoom();
      virtual bool execute(Dusk::Scene* scene, int count = 1);
    private:
      bool m_doZoomIn;
  };
}

#endif // COMMANDZOOM_H
