/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2009, 2010 DaSteph, thoronador

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
 Author:  DaSteph, thoronador
 Date:    2010-02-08
 Purpose: Script class
          represents a single script which can be processed by the Console

 History:
     - 2007-11-26 (rev 7)   - initial version (by DaSteph)
     - 2007-12-29 (rev 16)  - explodeCommands(), getStartScript(),
                               getStopScript(), trim() added
     - 2009-06-16 (rev 97)  - improved checks in trim() to avoid exceptions
                              caused by empty string argument (by thoronador)
     - 2009-12-31 (rev 147) - documentation update
     - 2010-02-01 (rev 162) - getStringRepresentation() and isEmpty() added
     - 2010-02-08 (rev 168) - trim() moved to DuskFunctions.h, because this is
                              not dependant on Script class

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include <string>
#include <vector>

namespace Dusk
{
    /**
     * This class represents a script which will be processed
     * by the console to create one or multiple commands.
     */
    class Script {
    public:
        /**
         * Standard constructor.
         *
         * @param p_string  The string representing the script.
         */
        Script(const std::string& p_string = "");

        /**
         * Destructor. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~Script();

        /**
         * Returns the script exploded as an array of single line commands.
         *
         * @return  the script each command in an single line in a array stripped
         *          from leading and trailing spaces.
         */
        std::vector<std::string> explodeCommands() const;

        /**
         * Returns a copy of the Script with a start command
         *
         * @return the modified script;
         */
        Script getStartScript() const;

        /**
         * Returns a copy of the Script with a stop command
         *
         * @return the modified script;
         */
        Script getStopScript() const;

        /* returns the internal string representation of the script */
        std::string getStringRepresentation() const;

        /* returns true, if the script holds no data */
        bool isEmpty() const;

    private:
        /**
         * Holds the string representation of the script.
         */
        std::string m_string;
    };
}
#endif // SCRIPT_H_INCLUDED
