#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED
namespace Dusk
{
    /**
     * This class represents a script which will be processed
     * by the console to create one or multiple commands.
     */
    class Script {
    public:
        /**
         * Standard konstructor
         */
        Script();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~Script();
    };
}
#endif // SCRIPT_H_INCLUDED
