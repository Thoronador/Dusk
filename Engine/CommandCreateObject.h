#ifndef COMMANDCREATEOBJECT_H
#define COMMANDCREATEOBJECT_H
#include "Command.h"

namespace Dusk
{
    class Scene;

    class CommandCreateObject : public Dusk::Command
    {
        public:
        CommandCreateObject();
        virtual ~CommandCreateObject();

        virtual bool execute(Dusk::Scene* scene, int count = 1);

        protected:
        private:
    };
}


#endif // CREATEOBJECT_H
