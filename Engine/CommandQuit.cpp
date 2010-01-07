#include "CommandQuit.h"
#include "API.h"
#include "FrameListener.h"
#include "InputSystemEditor.h"
#include <iostream>
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
        std::cout<<"Dusk: exit\n";
        InputSystemEditor::get().exit();
        return true;
    }

}

