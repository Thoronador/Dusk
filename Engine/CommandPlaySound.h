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
