#ifndef COMMANDPLAYSOUND_H
#define COMMANDPLAYSOUND_H

#include "Command.h"
#include <string>
namespace Dusk
{
    class CommandPlaySound : public Command
    {
        public:
            CommandPlaySound(const std::string& NoiseID);
            virtual ~CommandPlaySound();
            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
        private:
        std::string m_Noise;
    };
}


#endif // COMMANDPLAYSOUND_H
