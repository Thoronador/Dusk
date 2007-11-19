#ifndef DISPATCHER_H
#define DISPATCHER_H
#include "Scene.h"
namespace Dusk
{
    class Dispatcher
    {
    public:
        Dispatcher();
        virtual ~Dispatcher();
    protected:
    private:
        Dusk::Scene* m_Scene;
    };
}


#endif // DISPATCHER_H
