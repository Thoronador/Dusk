#ifndef QUIT_H
#define QUIT_H
#include "Command.h"

namespace Dusk
{
    class Scene;
    namespace Command
    {
        class Quit
        {
            public:
                Quit();
                virtual ~Quit();

                virtual bool execute(Dusk::Scene* scene, int count = 1);
            protected:
            private:
        };
    }
}


#endif // QUIT_H
