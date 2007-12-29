#ifndef INPUTSYSTEMEDITOR_H_INCLUDED
#define INPUTSYSTEMEDITOR_H_INCLUDED

#include "InputSystem.h"

#include <string>
#include <deque>
#include <OgreFrameListener.h>
#include <Ogre.h>

#include "Script.h"

namespace Dusk
{
    /**
     * This class implements the Interface InputSystem.
     * It holds the strategy of generating a console script by the
     * keystrokes from the user. It also provides all information for the
     * graphical output.
     */
    class InputSystemEditor : public InputSystem, Ogre::FrameListener {
    public:
        /**
         * Standard konstructor
         *
         * @param root              Pointer to the instance of the ogre root object.
         */
        InputSystemEditor(Ogre::Root *root);
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

        /**
         * Implements the frameStarted event for the FrameListener. Here the rendering is done.
         *
         * @param evt           The event that will be passed.
         * @return              Always true.
         */
        virtual bool frameStarted(const Ogre::FrameEvent &evt);

        /**
         * Implements the frameEnded event for the FrameListener.
         *
         * @param evt           The event that will be passed.
         * @return              Always true.
         */
        virtual bool frameEnded(const Ogre::FrameEvent &evt);

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

        /**
         * Toggles the console for rendering.
         *
         * @return              The visibility state.
         */
        virtual bool toggle();

    private:
        /**
         * Holds the current input line.
         */
        std::string myInputLine;

        /**
         * Holds the script history.
         */
        std::deque<Dusk::Script> myScriptHistory;

        /**
         * Holds the pointer to the currently selected script history item.
         */
        std::deque<Dusk::Script>::iterator myScriptHistoryIterator;

        /**
         * Holds the output lines.
         */
        std::deque<std::string> myOutput;

        /**
         * Holds the page offset vor viewing.
         */
        int myOutputPageOffset;

        /**
         * Holds the lines per page.
         */
        static const int myLinesPerPage = 16;

        /**
         * Holds the visbility state.
         */
        bool visible;

        /**
         * Holds the background overlay for the whole scene.
         */
        Ogre::Rectangle2D *myBackgroundRect;

        /**
         * Holds the scene node for the console.
         */
        Ogre::SceneNode *mySceneNode;

        /**
         * Holds the textbox.
         */
        Ogre::OverlayElement *myTextbox;

        /**
         * Holds the overlay.
         */
        Ogre::Overlay *myOverlay;

        /**
         * Holds the height.
         */
        double height;
    };
}
#endif // INPUTSYSTEMEDITOR_H_INCLUDED
