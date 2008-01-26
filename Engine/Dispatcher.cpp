#include "Dispatcher.h"
#include "API.h"
#include "Command.h"
#include "Scene.h"
namespace Dusk
{

Dispatcher::Dispatcher()
{
    //ctor
    m_Scene = &(Scene::get());
}

Dispatcher::~Dispatcher()
{
    //dtor
}
bool Dispatcher::executeCommand(Dusk::Command* command)
{
    bool ret = command->execute(m_Scene,0);
    delete command;
    return ret;
}
Dispatcher& Dispatcher::get()
{
    static Dispatcher Instance;
    return Instance;
}
}
