/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 ssj5000, DaSteph, walljumper, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Console.h"
#include "API.h"
#include "Dispatcher.h"
#include "Command.h"
#include "CommandQuit.h"
#include "CommandPlaySound.h"
#include "CommandMove.h"
#include "CommandAttack.h"
#include "CommandPickUp.h"
#include "CommandLoopSound.h"
#include "CommandSoundVolume.h"
#include "CommandNoiseSound.h"
#include "CommandMediaSound.h"
#include "CommandAssociateSound.h"
#include "CommandWeather.h"
#include "CommandZoom.h"
#include "CommandQuestLog.h"
#include "CommandScreenshot.h"
#include "CommandBindKey.h"
#include "DuskFunctions.h"
#include "DuskTypes.h"
#include "Messages.h"
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
            DuskLog() << "Thee wants to quit?\n";
            com = new CommandQuit();
            m_Dispatcher->executeCommand(com); // Object will be destroyed after being executed by the Dispatcher
        }
        else if (command[0] == "bind")
        {
            std::cout << "Bind a key!" << std::endl;
            if(command.size()< 3)
            {
                DuskLog()<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for bind command.\n";
            }
            else
            {
                com  = new CommandBindKey(command[1], command[2]);
                m_Dispatcher->executeCommand(com);
            }
        }
        else if (command[0] == "unbind")
        {
            std::cout << "Delete a key binding!" << std::endl;
            if(command.size()< 2)
            {
                DuskLog()<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for unbind command.\n";
            }
            else
            {
                com  = new CommandUnbindKey(command[1]);
                m_Dispatcher->executeCommand(com);
            }
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
            com = new CommandMove(JUMP_UP);
            m_Dispatcher->executeCommand(com);
            std::cout << "up, up!" << std::endl;
        }
        else if (command[0] == "pick_up")
        {
            com = new CommandPickUp();
            m_Dispatcher->executeCommand(com);
            DuskLog() << "pick something up!\n";
        }
        else if (command[0] == "MoveMouse")
        {

            std::cout<<"Move Mouse"<<std::endl;
        }

        //-------------------------------------------------------
        // attack commands
        else if (command[0] == "StartAttack")
        {
            com = new CommandAttack(true);
            m_Dispatcher->executeCommand(com);
            DuskLog() << "attack started!\n";
        }
        else if (command[0] == "StopAttack")
        {
            com = new CommandAttack(false);
            m_Dispatcher->executeCommand(com);
            DuskLog() << "attack stopped!\n";
        }

        //-------------------------------------------------------
        // Sound Commands

        else if (command[0] == "CreateSound")
        {
            if(command.size()< 2)
            {
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for CreateSound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for DestroySound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No media ID parameter "
                         <<"or no file path for CreateMedia.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No media ID parameter "
                         <<"for DestroyMedia.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"or no media ID for AssociateSoundMedia.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for DeassociateSoundMedia.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for PlaySound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID parameter "
                         <<"for PauseSound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID "
                         <<"for StopSound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID "
                         <<"for UnPauseSound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: No noise ID "
                         <<"for ReplaySound.\n";
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
                DuskLog()<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for LoopSound (2 needed).\n";
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
                DuskLog()<<"Console::executeCommand: Error: Not enough "
                         <<"parameters for SoundVolume (2 needed).\n";
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
            DuskLog() << "Fog toggled.\n";
        }
        //rain command
        else if (command[0] == "toggle_rain")
        {
            com = new CommandWeather(CommandWeather::wtRain, true);
            m_Dispatcher->executeCommand(com);
            DuskLog() << "Rain toggled.\n";
        }
        //snow command
        else if (command[0] == "toggle_snow")
        {
            com = new CommandWeather(CommandWeather::wtSnow, true);
            m_Dispatcher->executeCommand(com);
            DuskLog() << "Snow toggled.\n";
        }
        // --- QuestLog commands ---
        //toggle visibility
        else if (command[0] == "toggle_questlog")
        {
            com = new CommandQuestLog(CommandQuestLog::qloToggle);
            m_Dispatcher->executeCommand(com);
            std::cout << "QuestLog's visibility toggled." << std::endl;
        }
        //next page
        else if (command[0] == "questlog_increase")
        {
            com = new CommandQuestLog(CommandQuestLog::qloNext);
            m_Dispatcher->executeCommand(com);
            std::cout << "Next QuestLog page." << std::endl;
        }
        //previous page
        else if (command[0] == "questlog_decrease")
        {
            com = new CommandQuestLog(CommandQuestLog::qloPrev);
            m_Dispatcher->executeCommand(com);
            std::cout << "Previous QuestLog page." << std::endl;
        }
        // --- Screenshot command ---
        else if (command[0] == "screenshot")
        {
            com = new CommandScreenshot();
            m_Dispatcher->executeCommand(com);
            DuskLog() << "Screenshot command executed.\n";
        }
        // --- no command recognised ---
        else
        {
            DuskLog() << "Console::executeCommand: Parser error.\n";
        }
    }
    if(com) delete com;

    // we should still return something; here
    return 0;
}

}
