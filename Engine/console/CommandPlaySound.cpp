/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010 walljumper, thoronador

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

#include "CommandPlaySound.h"
#include "../Sound.h"
#include "../Messages.h"

namespace Dusk
{
    CommandPlaySound::CommandPlaySound(const std::string& NoiseID, const SoundOpCode op_code)
    {
      //ctor
      m_Noise = NoiseID;
      m_operation = op_code;
    }

    CommandPlaySound::~CommandPlaySound()
    {
      //dtor
    }

    bool CommandPlaySound::execute(Dusk::Scene* scene, int count)
    {
      switch(m_operation)
      {
        case sopPlay:
          return Sound::get().playNoise(m_Noise); break;
        case sopPause:
          return Sound::get().pauseNoise(m_Noise); break;
        case sopUnPause:
          return Sound::get().unPauseNoise(m_Noise); break;
        case sopStop:
          return Sound::get().stopNoise(m_Noise); break;
        case sopReplay:
          //New version of Sound has no Replay method yet. Instead we are using
          //a combination of Stop and Play to simulate the former Replay method.
          if (Sound::get().stopNoise(m_Noise))
          {
            return Sound::get().playNoise(m_Noise);
          }
          return false; break;
      }//switch
      DuskLog() << "CommandPlaySound::execute: ERROR: unknown op code parameter.\n";
      return false;
    }
}

