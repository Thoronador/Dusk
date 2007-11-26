#ifndef INPUTSYSTEMEDITOR_H_INCLUDED
#define INPUTSYSTEMEDITOR_H_INCLUDED

#include "InputSystem.h"

namespace Dusk
{
    /**
     * This class implements the Interface InputSystem.
     * It holds the strategy of generating a console script by the
     * keystrokes from the user. It also provides all information for the
     * graphical output.
     */
    class InputSystemEditor : public InputSystem {
    public:
        /**
         * Standard konstructor
         */
        InputSystemEditor();
        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemEditor();

        /**
         * Generates a console script by the given input event.
         * The internal line buffer will be filled by the given keystrokes.
         * If an enter event is sent the script is ready and will be returned.
         *
         * @param inputEvent        InputEvent to generate the Script with.
         * @return                  If the given InputEvent is Enter and the buffer
         *                          is not empty it will be returned as a script.
         *                          Otherwise null.
         */
        virtual Dusk::Script* handleEvent(Dusk::InputEvent inputEvent);
    };
}
#endif // INPUTSYSTEMEDITOR_H_INCLUDED
