#include "Console.h"
#include "API.h"
#include "Dispatcher.h"
#include "Command.h"
#include "CommandQuit.h"
#include "CommandPauseSound.h"
#include "CommandPlaySound.h"
#include "CommandReplaySound.h"
#include "CommandStopSound.h"
#include "CommandUnPauseSound.h"
#include "CommandMove.h"
#include "DuskTypes.h"
#include <iostream>

namespace Dusk
{


Console* Console::s_console = 0;

Console::Console()
{
    m_Dispatcher = &(Dispatcher::get());
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

int Console::executeCommand(std::string p_string)
{
    std::vector<std::string> command;
    std::string::iterator iter;
    std::string tmp;
    Command* com = NULL;

    if(p_string != "")
    {
        for(iter = p_string.begin();iter != p_string.end();iter++)
        {
            if(*iter != ' ')
                tmp += *iter;
            else
            {
                command.push_back(tmp);
                tmp = "";
            }
        }
        command.push_back(tmp);
    }
    else
    {
        return 1; // Error
    }
    if(command.size())
    {
        //this is going to get big :/
        if (command[0] == "quit")
        {
            std::cout << "Thee wants to quit?" << std::endl;
            com = new CommandQuit();
            m_Dispatcher->executeCommand(com); // Object will be destroyed after being executed by the Dispatcher
        }
        else if (command[0] == "bind")
        {
            std::cout << "Bind a key!" << std::endl;
        }
        else if (command[0] == "set")
        {
            std::cout << "A variable to set" << std::endl;
        }
        else if (command[0] == "start")
        {
            std::cout << "Starting something" << std::endl;
        }
        else if (command[0] == "stop")
        {
            std::cout << "Bring it to an end" << std::endl;
        }
        else if (command[0] == "move_forward")
        {
            com = new CommandMove(FORWARD);
            m_Dispatcher->executeCommand(com);
            std::cout << "One step forward" << std::endl;
        }
        else if (command[0] == "move_backward")
        {
            com = new CommandMove(BACKWARD);
            m_Dispatcher->executeCommand(com);
            std::cout << "One step back" << std::endl;
        }
        else if (command[0] == "step_left")
        {
            com = new CommandMove(LEFT);
            m_Dispatcher->executeCommand(com);
            std::cout << "to the left" << std::endl;
        }
        else if (command[0] == "step_right")
        {
            com = new CommandMove(RIGHT);
            m_Dispatcher->executeCommand(com);
            std::cout << "to the right" << std::endl;
        }
        else if (command[0] == "jump")
        {
            com = new CommandMove(UP);
            m_Dispatcher->executeCommand(com);
            std::cout << "up up!" << std::endl;
        }
        else if (command[0] == "PlaySound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No file parameter "
                         <<"for PlaySound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "PauseSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No file parameter "
                         <<"for PauseSound."<<std::endl;
            }
            else
            {
                com  = new CommandPauseSound(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "StopSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No file parameter "
                         <<"for StopSound."<<std::endl;
            }
            else
            {
                com  = new CommandStopSound(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "UnPauseSound" || command[0] == "ResumeSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No file parameter "
                         <<"for UnPauseSound."<<std::endl;
            }
            else
            {
                com  = new CommandUnPauseSound(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "ReplaySound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No file parameter "
                         <<"for ReplaySound."<<std::endl;
            }
            else
            {
                com  = new CommandReplaySound(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else
        {
            std::cout << "Console::executeCommand: Parser error." << std::endl;
        }
    }
    if(com) delete com;

    // we should still return true; or return false; here
}

}
