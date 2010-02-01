/*---------------------------------------------------------------------------
 Authors: DaSteph, walljumper, thoronador
 Date:    2010-02-01
 Purpose: InputSystem class
          interface for different input systems

 History:
     - 2007-11-26 (rev 7)    - initial version (by DaSteph)
     - 2007-12-29 (rev 15)   - initializeInput(), toggleInput(), captureInput()
                               added, class is now descendant of KeyListener
     - 2007-12-29 (rev 16)   - } smaller
     - 2008-01-26 (rev 30)   - }  fixes
     - 2008-03-26 (rev 57)   - mouse support added (by walljumper)
     - 2010-02-01 (rev 162)  - two utility functions added (by thoronador)

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include <Ogre.h>
#include <OIS/OIS.h>

#include "InputEvent.h"

namespace Dusk
{

    class InputSystemBinding;
    class InputSystemEditor;

    /**
     * Flags for the current active input class.
     */
    enum eActiveInputClass
    {
        AIC_NONE,
        AIC_BINDING,
        AIC_EDITOR
    };

    /**
     * This is the Interface for different InputSystems. Following the strategy pattern
     * Each extending class will provide another way to generate a console script out of
     * one or more InputEvents. It extends the OIS KeyListener to receive the KeyEvents
     * and generate the Script.
     */
    class InputSystem : public OIS::KeyListener, OIS::MouseListener{
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
         * Initialises the complete input system. The instance for the binding input and the
         * graphical console will be created. The current Input system will be set to the
         * binding class.
         *
         * @return true for success.
         */
        static bool initializeInput(Ogre::RenderWindow* win, Ogre::Root* root);

        /**
         * Toggles the currently active input system.
         *
         * @return true for success.
         */
        static bool toggleInput();

        /**
         * Forces a capture of all input devices.
         *
         */
        static bool captureInput();

        /**
         * Holds the current active input class flag.
         */
        static eActiveInputClass s_activeInputClass;

        static OIS::Keyboard* s_keyboard;

        static OIS::Mouse* s_mouse;

        /* utility functions to convert string representation into OIS key code.
           Returns OIS::KC_UNASSIGNED, if key does not contain a valid string.
        */
        static OIS::KeyCode stringToKeyCode(const std::string& key);

        /* utility functions to convert OIS key codes into string
           representation. Returns empty string, if kc is "unknown" key code.
        */
        static std::string keyCodeToString(const OIS::KeyCode kc);
    private:
        /**
         * Holds the instance of the binding input class
         */
        static InputSystemBinding* s_inputSystemBinding;

        /**
         * Holds the instance of the editor input class
         */
        static InputSystemEditor* s_inputSystemEditor;

    //    static OIS::Keyboard* s_keyboard;
    };
}
#endif // INPUTSYSTEM_H_INCLUDED
