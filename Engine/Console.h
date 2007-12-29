#ifndef CONSOLE_H
#define CONSOLE_H

#include <deque>
#include <list>

#include "Script.h"
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
        bool addScript (Dusk::Script p_script);

        /**
         * Processes the scripts on the internal queue
         *
         * @param maxEntries    The number of maximal entries to process. 0 means all
         * @return the number of entries processed.
         */
        int processScripts (int maxEntries = 0);

    protected:
    private:
        /**
         * Standard constructor
         */
        Console();

        /**
         * Standard destructor.
         */
        virtual ~Console();

        int executeCommand (std::string);

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

    };

}

#endif // CONSOLE_H
