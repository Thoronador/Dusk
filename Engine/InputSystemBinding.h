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
 Date:    2010-03-27
 Purpose: InputSystemBinding Singleton class
          implements the Interface InputSystem (see InputSystem.h)

 History:
     - 2007-11-26 (rev 7)   - initial version (by DaSteph)
     - 2007-12-29 (rev 15)  - keyPressed() and keyReleased() added
     - 2007-12-29 (rev 16)  - binding list updated, input now generates Scripts
                              and sends them to Console
     - 2008-01-17 (rev 23)  - InputSystemBinding registers itself at API class
                              (by walljumper)
     - 2008-01-26 (rev 30)  - InputSystemBinding is now Singleton
     - 2008-01-26 (rev 31)  - pointer to InputSystemBinding removed from API
     - 2008-03-22 (rev 51)  - bind list updated and adjusted
     - 2008-03-25 (rev 55)  - bindings for left/right movement
     - 2008-03-26 (rev 57)  - mouseMoved(), mousePressed(), mouseReleased() added
     - 2008-04-07 (rev 61)  - some lines for handling mouse input
     - 2009-06-21 (rev 98)  - bindings for turning left/right added (by thoronador)
     - 2009-12-04 (rev 138) - bindings for zooming camera in and out added
     - 2010-01-03 (rev 149) - }  temporary bindings for toggling weather effects
     - 2010-01-19 (rev 157) - }  added (will be removed later)
     - 2010-01-26 (rev 159) - key handling for dialogue menu added
     - 2010-02-01 (rev 162) - InputSystemBinding can now load and save key bindings.
                            - will try to load bindings from keys.conf, if that
                              file is present. Otherwise preset bindings are used.
     - 2010-03-24 (rev 186) - binding for jumping
     - 2010-03-27 (rev 188) - binding for picking up items
     - 2010-06-06 (rev 215) - binding for attacking
     - 2010-08-20 (rev 231) - binding for QuestLog
     - 2010-08-28 (rev 237) - camera will now react on mouse movement
     - 2010-08-31 (rev 239) - naming convention from coding guidelines enforced
     - 2010-11-09 (rev 249) - binding for screenshots
     - 2010-12-04 (rev 268) - use DuskLog/Messages class for logging
     - 2010-12-19 (rev 273) - isBound(), bindKey() and unbindKey() added
     - 2010-12-21 (rev 274) - listKeyBindings() added

 ToDo list:
     - method to load bind list from an external resource
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell me please.
 --------------------------------------------------------------------------*/

#ifndef INPUTSYSTEMBINDING_H_INCLUDED
#define INPUTSYSTEMBINDING_H_INCLUDED

#include "InputSystem.h"
#include "Script.h"

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
        /* singleton access method */
        static InputSystemBinding& get();

        /**
         * Destructor. Declared virtual so that extending classes destructors
         * will also be called.
         */
        virtual ~InputSystemBinding();

        /**
         * Implements the keyPressed event to receive notifications if a key has been pressed.
         *
         * @param arg       The KeyEvent that holds the information which key has been pressed.
         */
        virtual bool keyPressed (const OIS::KeyEvent &arg);

        /**
         * Implements the keyReleased event to receive notifications if a key has been released.
         *
         * @param arg       The KeyEvent that holds the information which key has been released.
         */
        virtual bool keyReleased (const OIS::KeyEvent &arg);

        /**
         * Implements the mousePressed event to receive notifications if the mouse has been moved.
         *
         * @param arg   The MouseEvent that holds the information about the mouse movement.
         */
        virtual bool mouseMoved(const OIS::MouseEvent &arg);

        /**
         * Implements the mousePressed event to receive notifications if a mouse button has been pressed.
         *
         * @param arg   The MouseEvent that holds the information about the mouse.
         * @param id    The MouseButton that has been pressed.
         */
        virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /**
         * Implements the mouseReleased event to receive notifications if a mouse button has been released.
         *
         * @param arg   The MouseEvent that holds the information about the mouse.
         * @param id    The MouseButton that has been released.
         */
        virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

        /* returns true, if the given key is currently bound, i.e. pressing or
           releasing the button will cause a script to be executed

           parameters:
             kc - the OIS code of the key to check
        */
        bool isBound(const OIS::KeyCode kc) const;

        /* binds a certain action to a key and returns true on success.
           This function fails, if the given key is already bound. In that case
           it won't change the key binding and returns false.

           parameters:
               kc - the OIS key code of the key
               key_string - string that represents the function of the key
        */
        bool bindKey(const OIS::KeyCode kc, const std::string& key_string);

        /* removes a key binding and returnes true, if a binding existed

           parameters:
               kc - the OIS key code of the key whose binding will be removed
        */
        bool unbindKey(const OIS::KeyCode kc);

        /* returns the key bindings for display in console */
        std::map<OIS::KeyCode, std::string> listKeyBindings() const;

        /* constant that names the configuration file which holds the key
           bindings
        */
        static const std::string cKeyConfigurationFile;

    protected:
        /* tries to guess the key string by looking at the Script scr.
           If scr is not an acceptable script, this function will return an
           empty string.

           parameters:
               scr - the Script
        */
        static std::string getKeyStringFromScript(const Script& scr);

        /* utility function that returns the Script that should be executed when
           the specified key is pressed. If no Script shall be processed, or if
           ks is not an acceptable value, an empty Script will be returned.

           parameters:
               ks - string that identifies the key
        */
        static Script getPressScriptFromKeyString(const std::string& ks);

        /* utility function that returns the Script that should be executed when
           the specified key is released. If no Script shall be processed, or if
           ks is not an acceptable value, an empty Script will be returned.

           parameters:
               ks - string that identifies the key
        */
        static Script getReleaseScriptFromKeyString(const std::string& ks);

        /* tries to load the key bindings from the file fileName and returns
           true in case of success, false on error

           parameters:
               fileName - name of the file where the key bindings are located
        */
        bool loadKeyConfiguration(const std::string& fileName);

        /* tries to save the key bindings to the file fileName and returns true
           in case of success

           parameters:
               fileName - name of the file that shall be used/created to save
                          the key bindings
        */
        bool saveKeyConfiguration(const std::string& fileName) const;

    private:
        /**
         * Standard constructor
         */
        InputSystemBinding();

        /**
         * Copy constructor
         */
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
    }; //class

}//namespace
#endif // INPUTSYSTEMBINDING_H_INCLUDED
