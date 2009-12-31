/*---------------------------------------------------------------------------
 Author:  DaSteph, thoronador
 Date:    2009-06-16
 Purpose: Script class
          represents a single script which can be processed by the Console

 History:
     - 2007-11-26 (rev 7)   - initial version (by DaSteph)
     - 2007-12-29 (rev 16)  - explodeCommands(), getStartScript(),
                               getStopScript(), trim() added
     - 2009-06-16 (rev 97)  - improved checks in trim() to avoid exceptions
                              caused by empty string argument (by thoronador)
     - 2009-12-31 (rev 147) - documentation update

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
        Script(std::string p_string = "");

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
        std::vector<std::string> explodeCommands();

        /**
         * Returns a copy of the Script with a start command
         *
         * @return the modified script;
         */
        Script getStartScript();

        /**
         * Returns a copy of the Script with a stop command
         *
         * @return the modified script;
         */
        Script getStopScript();

    private:
        /**
         * Trims the given string from leading and trailing spaces.
         *
         * @param p_string      The string to trim.
         * @return the trimmed string.
         */
        std::string trim(std::string p_string);

        /**
         * Holds the string representation of the script.
         */
        std::string m_string;
    };
}
#endif // SCRIPT_H_INCLUDED
