#ifndef COMMANDMOVE_H
#define COMMANDMOVE_H
#include "API.h"
#include "Command.h"
#include "DuskTypes.h"
#include <Ogre.h>
namespace Dusk
{
    class Scene;


    class CommandMove : public Command
    {
        public:

            CommandMove(): m_Direction(FORWARD){}
            CommandMove(DIRECTION dir);
            virtual ~CommandMove();
            virtual bool execute(Dusk::Scene* scene, int count = 1);

        protected:
        private:
            static int moveConstant;
            DIRECTION m_Direction;

    };
}


#endif // COMMANDMOVE_H
