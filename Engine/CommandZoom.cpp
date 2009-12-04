#include "CommandZoom.h"
#include "Camera.h"
#include "API.h"

namespace Dusk
{
  CommandZoom::CommandZoom(const bool ZoomIn)
  {
    m_doZoomIn = ZoomIn;
  }

  CommandZoom::~CommandZoom()
  {
    //empty destructor
  }

  bool CommandZoom::execute(Dusk::Scene* scene, int count)
  {
    Camera* cam = getAPI().getDuskCamera();
    if (cam==NULL) return true;
    if (m_doZoomIn)
    { //zoom in
      cam->setZoom(cam->getZoom()- Camera::cRecommendedZoomStep);
    }
    else
    { //zoom out
      cam->setZoom(cam->getZoom()+ Camera::cRecommendedZoomStep);
    }
    return true;
  }

} //namespace
