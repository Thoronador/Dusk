#ifndef COMMANDQUIT_H
#define COMMANDQUIT_H
#include "Command.h"

namespace Dusk
{
    class Scene;
    class CommandQuit
    {
        public:
            CommandQuit();
            virtual ~CommandQuit();

            virtual bool execute(Dusk::Scene* scene, int count = 1);
        protected:
        private:
    };
}


#endif // QUIT_H
