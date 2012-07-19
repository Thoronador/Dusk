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

#include "CommandZoom.h"
#include "Camera.h"

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
    Camera& cam = Camera::getSingleton();
    if (m_doZoomIn)
    { //zoom in
      cam.setZoom(cam.getZoom()- Camera::cRecommendedZoomStep);
    }
    else
    { //zoom out
      cam.setZoom(cam.getZoom()+ Camera::cRecommendedZoomStep);
    }
    return true;
  }

} //namespace
