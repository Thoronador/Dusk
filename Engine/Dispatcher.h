#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Scene.h"
namespace Dusk
{
    class Command;
    class Dispatcher
    {
    public:
        Dispatcher();
        virtual ~Dispatcher();
        bool executeCommand(Dusk::Command* command);
    protected:
    private:
        Dusk::Scene* m_Scene;
    };
}


#endif // DISPATCHER_H
