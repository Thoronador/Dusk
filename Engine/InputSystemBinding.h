#ifndef INPUTSYSTEMBINDING_H_INCLUDED
#define INPUTSYSTEMBINDING_H_INCLUDED

#include "InputSystem.h"

#include <map>

namespace Dusk
{
    /**
     * This class implements the Interface InputSystem.
     * It holds the strategy of generating a console script by a
     * lookup on an internal bind list that will be loaded from
     * an external ressource.
     */
    class InputSystemBinding : public InputSystem {
    public:
        /**
         * Standard konstructor
         */
        InputSystemBinding();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemBinding();

        /**
         * Generates a console script by the given input event.
         * A lookup on the bind list will be performed and the corresponding script
         * will be returned.
         *
         * @param inputEvent        InputEvent to lookup for the Script.
         * @return                  The script from the bind list. If none is founf null.
         */
        virtual Dusk::Script* handleEvent(Dusk::InputEvent inputEvent);

    private:
        /**
         * Holds the bind list where an input event is associated with a console script.
         */
        std::map<Dusk::InputEvent, Dusk::Script> myBindList;
    };
}
#endif // INPUTSYSTEMBINDING_H_INCLUDED
