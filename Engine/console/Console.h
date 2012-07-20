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

/*---------------------------------------------------------------------------
 Authors: ssj5000, DaSteph, walljumper, thoronador
 Date:    2010-01-19
 Purpose: Console Singleton class
          translates scripts into commands

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2007-12-29 (rev 16)  - Console accepts input (by DaSteph)
     - 2008-01-17 (rev 23)  - Console registers itself at API (by walljumper)
     - 2008-01-17 (rev 24)  - Console sends QuitCommand
     - 2008-02-22 (rev 39)  - parsing of commands improved
     - 2008-02-23 (rev 40)  - CommandPlaySound works
     - 2008-02-25 (rev 45)  - } more sound commands (by thoronador)
     - 2008-02-26 (rev 46)  - }   "    "      "
     - 2008-03-22 (rev 51)  - CommandMove added (by walljumper)
     - 2008-03-24 (rev 52)  - bug fix for CommandMove
     - 2008-03-25 (rev 54)  - another fix in CommandMove (by thoronador)
     - 2008-03-25 (rev 55)  - Camera can now be moved (by walljumper)
     - 2008-03-28 (rev 58)  - commands for sound volume and loop playback (by
                                thoronador)
     - 2008-03-28 (rev 60)  - better handling of volume parameter
     - 2008-04-07 (rev 61)  - mouse handling (by walljumper)
     - 2009-01-03 (rev 67)  - adjustments to work with new version of Sound class
                              (by thoronador)
     - 2009-01-03 (rev 68)  - adjustments to work with merged sound command class
     - 2009-06-21 (rev 98)  - CommandMove: rotation/ turning left and right
     - 2009-12-04 (rev 138) - CommandZoom to zoom camera added
     - 2010-01-03 (rev 149) - CommandWeather added, handles snow and fog
     - 2010-01-19 (rev 157) - CommandWeather updated for rain
     - 2010-03-04 (rev 177) - small fix for some function parameters
     - 2010-03-27 (rev 188) - adjustments for CommandPickUp
     - 2010-06-06 (rev 215) - adjustments for CommandAttack
     - 2010-06-06 (rev 216) - fixed a bug
     - 2010-08-20 (rev 231) - adjustments for CommandQuestLog
     - 2010-11-09 (rev 249) - adjustments for CommandScreenshot
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2010-12-19 (rev 273) - adjustments for CommandBindKey and CommandUnbindKey
     - 2010-12-21 (rev 274) - adjustments for CommandListKeys

 ToDo list:
     - update Console as soon as new commands are available
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef CONSOLE_H
#define CONSOLE_H

#include <deque>
#include <list>

#include "../Script.h"
#include "Dispatcher.h"

namespace Dusk
{
    /**
     * This class represents the console which will translate scripts into
     * commands. These commands are then handled over to the dispatcher.
     * This class follows the singleton pattern.
     */
    class Console
    {
    public:
        /**
         * Returns the singleton instance of this class.
         * If there exists none so far it will create one.
         */
        static Console* getInstance();

        /**
         * Addes a script to the script queue
         *
         * @param p_script      The script to add.
         * @return true for success;
         */
        bool addScript(const Dusk::Script& p_script);

        /**
         * Processes the scripts on the internal queue
         *
         * @param maxEntries    The number of maximal entries to process. 0 means all
         * @return the number of entries processed.
         */
        int processScripts (int maxEntries = 0);

    private:
        /**
         * Standard constructor
         */
        Console();

        /**
         * Empty copy constructor
         */
        Console(const Console& op){}

        /**
         * Standard destructor.
         */
        virtual ~Console();

        int executeCommand(const std::string&);

        /**
         * Holds the instance of the dispatcher.
         */
        Dusk::Dispatcher* m_Dispatcher;

        /**
         * Holds the queue of scripts to process.
         */
        std::deque<Dusk::Script> m_scriptQueue;

        /**
         * Holds the list of repeatively excuted commands.
         */
        std::list<std::string> m_repeatedCommands;

        /**
         * Holds the singleton instance of this class;
         */
        static Console* s_console;

    };//class

} //namespace

#endif // CONSOLE_H
