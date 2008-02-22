#ifndef INPUTSYSTEM_H_INCLUDED
#define INPUTSYSTEM_H_INCLUDED

#include <Ogre.h>
#include <OIS/OIS.h>

#include "Script.h"
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
    class InputSystem : public OIS::KeyListener{
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
