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
        static InputSystemEditor& get();

        /**
         * Destructer. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemEditor();

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

        void exit(){m_continue = false;}

        virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

        /* adds a message to the output history */
        void addMessage(const std::string& txt);
    private:
        /**
         * Standard konstructor
         *
         */
        InputSystemEditor();
        InputSystemEditor(const InputSystemEditor& op){}

        /**
         * Holds the current input line.
         */
        std::string myInputLine;

        /**
         * Holds the output history.
         */
        std::deque<std::string> mOutputHistory;

        /**
         * Holds the page offset vor viewing.
         */
        //unsigned int mOutputHistoryOffset;

        /* adds a new line to the output list and removes an old one */
        void addToOutput(const std::string& nl);

        /**
         * Holds the input lines.
         */
        std::deque<std::string> mInputHistory;

        /* adds a new line to the input list and removes an old one */
        void addToInput(const std::string& nl);

        /* holds the index of the line in myOutput which is currently shown */
        unsigned int mInputHistoryIndex;

        /**
         * Holds the lines per page.
         */
        static const unsigned int myLinesPerPage = 16;

        /* holds the character sequence indicating the console promt */
        static const std::string cPrompt;

        /* True, if input shall be processed by LuaEngine and not by Console */
        bool mUseLua;

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
        /**
         *
         */
        bool m_continue;
    };
}
#endif // INPUTSYSTEMEDITOR_H_INCLUDED
