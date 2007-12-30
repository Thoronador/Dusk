#ifndef CREATEOBJECT_H
#define CREATEOBJECT_H
#include "Command.h"

namespace Dusk
{
    class Scene;
    namespace Command
    {
        class CreateObject : public Dusk::Command
        {
            public:
            CreateObject();
            virtual ~CreateObject();

            virtual bool execute(Dusk::Scene* scene, int count = 1);

            protected:
            private:
        };
    }
}


#endif // CREATEOBJECT_H
