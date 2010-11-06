/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 DaSteph, walljumper, thoronador

    The Dusk Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Dusk Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Dusk Engine.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

/*---------------------------------------------------------------------------
 Authors: DaSteph, walljumper, thoronador
 Date:    2010-03-03
 Purpose: InputSystemEditor Singleton class
          ???

 History:
     - 2007-11-26 (rev 7)   - initial version (by DaSteph)
     - 2007-11-27 (rev 8)   - InputSystemEditor is now descendant of Ogre's
                              FrameListener; frameStarted()+frameEnded() added
     - 2007-12-29 (rev 15)  - keyPressed(), keyReleased() added
     - 2007-12-29 (rev 16)  - handleEvent() removed
     - 2008-01-17 (rev 23)  - InputSystemEditor registers itself at API
                              (by walljumper)
     - 2008-01-26 (rev 30)  - InputSystemEditor is now singleton
     - 2008-02-22 (rev 39)  - exit() added
     - 2008-03-26 (rev 57)  - mouse handling added
     - 2009-05-27 (rev 96)  - renamed InputSytem*.cpp to InputSystem*.cpp
                              (by thoronador)
     - 2010-03-04 (rev 177) - ISE can now switch between Console and LuaEngine
                              and has a "browsable" input history
     - 2010-03-12 (rev 181) - "<" and ">" are now allowed input characters, too
     - 2010-03-13 (rev 184) - input and output are now handled separately
     - 2010-03-13 (rev 204) - curly brackets are now allowed input characters, too
     - 2010-08-15 (rev 226) - fixed bugs that could cause a crash/ error on
                              program termination
                            - documentation (history) updated

 ToDo list:
     - ???

 Bugs:
     - Untested. If you find a bug (or more), then tell us please.
 --------------------------------------------------------------------------*/

#ifndef INPUTSYSTEMEDITOR_H_INCLUDED
#define INPUTSYSTEMEDITOR_H_INCLUDED

#include "InputSystem.h"

#include <string>
#include <deque>
#include <OgreFrameListener.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreSceneNode.h>

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
