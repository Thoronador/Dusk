/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010, 2013  Thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "CommandMediaSound.h"
#include "../Sound.h"

namespace Dusk
{
  //media creation command
  //constructor
  CommandCreateMedia::CommandCreateMedia(const std::string& MediaID, const std::string& MediaPath)
  : Command(),
    m_Media(MediaID),
    m_MediaPath(MediaPath)
  {
  }

  //destructor
  CommandCreateMedia::~CommandCreateMedia()
  {
    //empty
  }

  bool CommandCreateMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().createMedia(m_Media, m_MediaPath);
  }


  //media deletion command
  //constructor
  CommandDestroyMedia::CommandDestroyMedia(const std::string& MediaID)
  : Command(),
    m_Media(MediaID)
  {
  }

  //destructor
  CommandDestroyMedia::~CommandDestroyMedia()
  {
    //empty
  }

  bool CommandDestroyMedia::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().destroyMedia(m_Media);
  }
}
