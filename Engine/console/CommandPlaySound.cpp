/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2008, 2009, 2010, 2013  walljumper, thoronador

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

#include "CommandPlaySound.h"
#include "../sound/Sound.h"
#include "../Messages.h"

namespace Dusk
{
    //constructor
    CommandPlaySound::CommandPlaySound(const std::string& NoiseID, const SoundOpCode op_code)
    : Command(),
      m_Noise(NoiseID),
      m_operation(op_code)
    {
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
          return Sound::get().playNoise(m_Noise);
        case sopPause:
          return Sound::get().pauseNoise(m_Noise);
        case sopUnPause:
          return Sound::get().unPauseNoise(m_Noise);
        case sopStop:
          return Sound::get().stopNoise(m_Noise);
        case sopReplay:
          //New version of Sound has no Replay method yet. Instead we are using
          //a combination of Stop and Play to simulate the former Replay method.
          if (Sound::get().stopNoise(m_Noise))
          {
            return Sound::get().playNoise(m_Noise);
          }
          return false;
      }//switch
      DuskLog() << "CommandPlaySound::execute: ERROR: unknown op code parameter.\n";
      return false;
    }
}
