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
        static InputSystemBinding& get();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemBinding();

        /**
         * Implements the keyPressed event to receive notifications if an key has been pressed.
         *
         * @param arg           The KeyEvent that holds the information which butten has been pressed.
         */
        virtual bool keyPressed (const OIS::KeyEvent &arg);

        /**
         * Implements the keyReleased event to receive notifications if an key has been released.
         *
         * @param arg           The KeyEvent that holds the information which butten has been released.
         */
        virtual bool keyReleased (const OIS::KeyEvent &arg);


        virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    private:
        /**
         * Standard konstructor
         */
        InputSystemBinding();
        InputSystemBinding(const InputSystemBinding& op){}

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all durating scripts, which means that an action is done each frame.
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListHold;

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all scripts that should be executed when the coressponding button is pressed
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListPress;

        /**
         * Holds the bind list where an input event is associated with a console script.
         * This list holds all scripts that should be executed when the coressponding button is pressed
         */
        std::map<OIS::KeyCode, Dusk::Script> myBindListRelease;

    };
}
#endif // INPUTSYSTEMBINDING_H_INCLUDED
