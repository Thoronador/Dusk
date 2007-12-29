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
         * Standard konstructor.
         *
         * @param p_string  The string representing the script.
         */
        Script(std::string p_string = "");

        /**
         * Destructer. Declared virtual so that extending classes destructors
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
         * @param p_string      The string to split.
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
