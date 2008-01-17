#include "CommandQuit.h"
#include "API.h"
#include "FrameListener.h"
namespace Dusk
{
    CommandQuit::CommandQuit()
    {
        //ctor
    }

    CommandQuit::~CommandQuit()
    {
        //dtor
    }

    bool CommandQuit::execute(Dusk::Scene* scene, int count)
    {
        getAPI().getFrameListener()->exit();
    }

}

