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

/*---------------------------------------------------------------------------
 Authors: walljumper, thoronador
 Date:    2010-05-06
 Purpose: CommandPlaySound class
          Command for changing the playback state of sounds via Console

 History:
     - 2008-02-24 (rev 42)  - initial version (by walljumper)
     - 2009-01-03 (rev 67)  - adjustments to work with new (rev. 66) Sound
                              class (by thoronador)
     - 2009-01-03 (rev 68)  - merge of former classes CommandPlaySound,
                              CommandPauseSound, CommandUnPauseSound,
                              CommandStopSound and CommandReplaySound into
                              CommandPlaySound. OpCode parameter now determines
                              whether sound should be paused, played, stopped,
                              and so on.
     - 2010-05-06 (rev 197) - documentation update

 ToDo list:
     - ???

 Bugs:
     - If you find one (or more), then tell me please. I'll try to fix it as
       soon as possible.
 --------------------------------------------------------------------------*/

#ifndef COMMANDPLAYSOUND_H
#define COMMANDPLAYSOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
    //enumeration type to speciify what to do with the given Noise
    enum SoundOpCode {sopPlay, sopPause, sopUnPause, sopStop, sopReplay};

    class CommandPlaySound : public Command
    {
        public:
            /* constructor

               parameters:
                   NoiseID - ID of sound whose playback mode shall be changed
                   op_code - enumeration type to indicate what shall be done
                             with the specified noise
            */
            CommandPlaySound(const std::string& NoiseID, const SoundOpCode op_code);
            /* destructor */
            virtual ~CommandPlaySound();
            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
            std::string m_Noise;
            SoundOpCode m_operation;
    };
}

#endif // COMMANDPLAYSOUND_H
