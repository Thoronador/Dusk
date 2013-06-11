/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2013  Thoronador

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

#include "CommandSoundVolume.h"
#include "../sound/Sound.h"

namespace Dusk
{
  //constructor
  CommandSoundVolume::CommandSoundVolume(const std::string& NoiseID, const float vol)
  : Command(),
    m_Noise(NoiseID),
    m_Volume(vol)
  {
  }

  //destructor
  CommandSoundVolume::~CommandSoundVolume()
  {
    //empty
  }

  bool CommandSoundVolume::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().setNoiseVolume(m_Noise, m_Volume);
  }
}
