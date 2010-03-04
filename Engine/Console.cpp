#include "Console.h"
#include "API.h"
#include "Dispatcher.h"
#include "Command.h"
#include "CommandQuit.h"
#include "CommandPlaySound.h"
#include "CommandMove.h"
#include "CommandLoopSound.h"
#include "CommandSoundVolume.h"
#include "CommandNoiseSound.h"
#include "CommandMediaSound.h"
#include "CommandAssociateSound.h"
#include "CommandWeather.h"
#include "CommandZoom.h"
#include "DuskFunctions.h"
#include "DuskTypes.h"
#include <iostream>
#include <sstream>

namespace Dusk
{


Console* Console::s_console = 0;

Console::Console()
{
    m_Dispatcher = &(Dispatcher::get());
}

Console::~Console()
{
  //empty destructor
}

Console* Console::getInstance()
{
    if (s_console == 0)
        s_console = new Console();
    return s_console;
}

bool Console::addScript(const Dusk::Script& p_script)
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

int Console::executeCommand(const std::string& p_string)
{
    std::vector<std::string> command;
    std::string::const_iterator iter;
    std::string tmp;
    Command* com = NULL;

    //divide string into vector of strings
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
        //---------------------------------------------------------
        //Movement Commands
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
        else if (command[0] == "turn_left")
        {
            com = new CommandMove(TURN_LEFT);
            m_Dispatcher->executeCommand(com);
            std::cout << "Take a look to the left" << std::endl;
        }
        else if (command[0] == "turn_right")
        {
            com = new CommandMove(TURN_RIGHT);
            m_Dispatcher->executeCommand(com);
            std::cout << "Take a look to the right" << std::endl;
        }
        else if (command[0] == "jump")
        {
            com = new CommandMove(UP);
            m_Dispatcher->executeCommand(com);
            std::cout << "up up!" << std::endl;
        }
        else if (command[0] == "MoveMouse")
        {

            std::cout<<"Move Mouse"<<std::endl;
        }
        //-------------------------------------------------------
        // Sound Commands

        else if (command[0] == "CreateSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for CreateSound."<<std::endl;
            }
            else
            {
                com  = new CommandCreateNoise(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "DestroySound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for DestroySound."<<std::endl;
            }
            else
            {
                com  = new CommandDestroyNoise(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "CreateMedia")
        {
            if(command.size()< 3)
            {
                std::cout<<"Console::executeCommand: Error: No media ID parameter "
                         <<"or no file path for CreateMedia."<<std::endl;
            }
            else
            {
                com  = new CommandCreateMedia(command[1], command[2]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "DestroyMedia")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No media ID parameter "
                         <<"for DestroyMedia."<<std::endl;
            }
            else
            {
                com  = new CommandDestroyMedia(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "AssociateSoundMedia")
        {
            if(command.size()< 3)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"or no media ID for AssociateSoundMedia."<<std::endl;
            }
            else
            {
                com  = new CommandAssociateSoundMedia(command[1], command[2]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "DeassociateSoundMedia")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for DeassociateSoundMedia."<<std::endl;
            }
            else
            {
                com  = new CommandDeassociateSoundMedia(command[1]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "PlaySound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for PlaySound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1], sopPlay);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "PauseSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for PauseSound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1], sopPause);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "StopSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID "
                         <<"for StopSound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1], sopStop);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "UnPauseSound" || command[0] == "ResumeSound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID "
                         <<"for UnPauseSound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1], sopUnPause);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "ReplaySound")
        {
            if(command.size()< 2)
            {
                std::cout<<"Console::executeCommand: Error: No noise ID "
                         <<"for ReplaySound."<<std::endl;
            }
            else
            {
                com  = new CommandPlaySound(command[1], sopReplay);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "LoopSound")
        {
            if(command.size()< 3)
            {
                std::cout<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for LoopSound (2 needed)."<<std::endl;
            }
            else
            {
                /*Note on second parameter (bool): all strings except "1" will
                  be interpreted as false. Maybe we should change that later.*/
                com  = new CommandLoopSound(command[1], command[2].compare("1")==0);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "SoundVolume")
        {
            if(command.size()< 3)
            {
                std::cout<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for SoundVolume (2 needed)."<<std::endl;
            }
            else
            {
                //predefined volume value is 1.0, if second param is not a float
                com  = new CommandSoundVolume(command[1], StringToFloat(command[2], 1.0f));
                m_Dispatcher->executeCommand(com);
            }
        }
        // --- Zoom commands ---
        else if (command[0] == "ZoomIn")
        {
            com = new CommandZoom(true);
            m_Dispatcher->executeCommand(com);
            std::cout << "Zoom in" << std::endl;
        }
        else if (command[0] == "ZoomOut")
        {
            com = new CommandZoom(false);
            m_Dispatcher->executeCommand(com);
            std::cout << "Zoom out" << std::endl;
        }
        //fog command
        else if (command[0] == "toggle_fog")
        {
            com = new CommandWeather(CommandWeather::wtFog, true);
            m_Dispatcher->executeCommand(com);
            std::cout << "Fog toggled." << std::endl;
        }
        //rain command
        else if (command[0] == "toggle_rain")
        {
            com = new CommandWeather(CommandWeather::wtRain, true);
            m_Dispatcher->executeCommand(com);
            std::cout << "Rain toggled." << std::endl;
        }
        //snow command
        else if (command[0] == "toggle_snow")
        {
            com = new CommandWeather(CommandWeather::wtSnow, true);
            m_Dispatcher->executeCommand(com);
            std::cout << "Snow toggled." << std::endl;
        }
        // --- no command recognised ---
        else
        {
            std::cout << "Console::executeCommand: Parser error." << std::endl;
        }
    }
    if(com) delete com;

    // we should still return something; here
    return 0;
}

}
