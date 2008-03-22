#ifndef COMMANDMOVE_H
#define COMMANDMOVE_H
#include "API.h"
#include "Command.h"
#include <Ogre.h>
namespace Dusk
{
    class Scene;
    class CommandMove : public Command
    {
        public:
            enum Direction {forward ,backward,left,right};
            CommandMove(){}
            CommandMove(Direction dir);
            virtual ~CommandMove();
            virtual bool execute(Dusk::Scene* scene, int count = 1);

        protected:
        private:
            static int moveConstant;
            Direction m_Direction;

    };
}


#endif // COMMANDMOVE_H
