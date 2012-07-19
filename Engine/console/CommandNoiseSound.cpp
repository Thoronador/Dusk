/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2009, 2010 thoronador

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

#include "CommandNoiseSound.h"
#include "Sound.h"

namespace Dusk
{
  /* **** noise creation command **** */
  //constructor
  CommandCreateNoise::CommandCreateNoise(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandCreateNoise::~CommandCreateNoise()
  {
    //empty
  }

  bool CommandCreateNoise::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().createNoise(m_Noise);
  }


  /* **** noise deletion command **** */
  //constructor
  CommandDestroyNoise::CommandDestroyNoise(const std::string& NoiseID)
  {
    m_Noise = NoiseID;
  }

  //destructor
  CommandDestroyNoise::~CommandDestroyNoise()
  {
    //empty
  }

  bool CommandDestroyNoise::execute(Dusk::Scene* scene, int count)
  {
    return Sound::get().destroyNoise(m_Noise);
  }
}
