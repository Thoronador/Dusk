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
            CommandPlaySound(const std::string& NoiseID, const SoundOpCode op_code);
            virtual ~CommandPlaySound();
            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
        private:
            std::string m_Noise;
            SoundOpCode m_operation;
    };
}

#endif // COMMANDPLAYSOUND_H
