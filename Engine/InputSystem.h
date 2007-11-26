#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include "Script.h"
#include "InputEvent.h"

namespace Dusk
{
    /**
     * This is the Interface for different InputSystems. Following the strategy pattern
     * Each extending class will provide another way to generate a console script out of
     * one or more InputEvents.
     */
    class InputSystem {
    public:
        /**
         * Standard konstructor
         */
        InputSystem();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystem();

        /**
         * Interface function for handling InputEvents and transforming them into a Script.
         *
         * @param inputEvent        The InputEvent which should be used to generate the Script.
         * @return                  If a Script is generated it will be returned otherwise null.
         */
        virtual Dusk::Script* handleEvent(Dusk::InputEvent inputEvent) = 0;
    };
}
#endif // INPUTSYSTEM_H_INCLUDED
