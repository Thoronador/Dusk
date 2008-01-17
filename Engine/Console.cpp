#include "Console.h"
#include "API.h"
#include "Dispatcher.h"
#include "Command.h"
#include "CommandQuit.h"
#include <iostream>

namespace Dusk
{

Console* Console::s_console = 0;

Console::Console()
{
    getAPI().setConsole(this);
    m_Dispatcher = getAPI().getDispatcher();
}

Console::~Console()
{
}

Console* Console::getInstance()
{
    if (s_console == 0)
        s_console = new Console();
    return s_console;
}

bool Console::addScript (Dusk::Script p_script)
{
    m_scriptQueue.push_back(p_script);
    return true;
}

int Console::processScripts (int maxEntries)
{
    int size = m_scriptQueue.size();
    if (maxEntries == 0 || maxEntries > size)
        maxEntries = size;

    size = maxEntries;

    while (maxEntries > 0)
    {
        Script script = m_scriptQueue.front();
        m_scriptQueue.pop_front();

        std::vector<std::string> lines = script.explodeCommands();

        std::vector<std::string>::iterator it;
        for (it = lines.begin(); it != lines.end(); it++)
            executeCommand(*it);
        maxEntries--;
    }

    return size;
}

int Console::executeCommand (std::string p_string)
{
    long posSpace = p_string.find(" ");
    std::string command = p_string;
    if (posSpace != std::string::npos)
        command = p_string.substr(0, posSpace);
    //this is going to get big :/
    if (command == "quit")
    {
        std::cout << "Thee wants to quit?" << std::endl;
        m_Dispatcher->executeCommand(new CommandQuit()); // Object will be destroyed after being executed by the Dispatcher
    }
    else if (command == "bind")
    {
        std::cout << "Bind a key!" << std::endl;
    }
    else if (command == "set")
    {
        std::cout << "A variable to set" << std::endl;
    }
    else if (command == "start")
    {
        std::cout << "Starting something" << std::endl;
    }
    else if (command == "stop")
    {
        std::cout << "Bring it to an end" << std::endl;
    }
    else if (command == "move_forward")
    {
        std::cout << "One step forward" << std::endl;
    }
    else if (command == "move_backward")
    {
        std::cout << "One step back" << std::endl;
    }
    else if (command == "step_left")
    {
        std::cout << "to the left" << std::endl;
    }
    else if (command == "step_right")
    {
        std::cout << "to the right" << std::endl;
    }
    else if (command == "jump")
    {
        std::cout << "up up!" << std::endl;
    }
    else
    {
        std::cout << "Parser error" << std::endl;
    }
}

}
