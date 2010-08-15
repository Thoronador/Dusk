#include "CommandQuit.h"
#include <iostream>
#include "InputSystemEditor.h"
#include "DataLoader.h"
#include "Weather.h"

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
        Scene::get().destroyScene();
        std::cout<<"  Send exit signal to ISE...\n";
        InputSystemEditor::get().exit();
        return true;
    }

} //namespace
