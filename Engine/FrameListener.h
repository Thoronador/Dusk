/*
 -----------------------------------------------------------------------------
    This file is part of the Dusk Engine.
    Copyright (C) 2007, 2008, 2009, 2010 ssj5000, walljumper, DaSteph, thoronador

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
 Authors: ssj5000, walljumper, DaSteph, thoronador
 Date:    2010-05-06
 Purpose: FrameListener class
          the FrameListener, i.e. derived Ogre::FrameListener which implements
          the frameStarted() and frameEnded() functions as needed by Dusk

 History:
     - 2007-11-19 (rev 3)   - initial version (by ssj5000)
     - 2007-11-28 (rev 9)   - frameStarted() and frameEnded() (by walljumper)
     - 2007-11-28 (rev 10)  - handleUnbufferedInput() added
                            - parameter list added to constructor, local
                              variables to store parameters
     - 2007-11-29 (rev 11)  - InputHandler added
     - 2007-11-29 (rev 12)  - small fix
     - 2007-11-29 (rev 13)  - console now scrolls (by DaSteph)
     - 2007-12-29 (rev 14)  - functions for key and mouse listeners (by walljumper)
     - 2007-12-29 (rev 15)  - input handling code moved to InputSystem (by DaSteph)
     - 2007-12-29 (rev 16)  - Console now processes scripts each frame
     - 2007-12-30 (rev 17)  - exit() added (by walljumper)
     - 2008-01-17 (rev 23)  - FrameListener now registers itself at API class
     - 2008-03-22 (rev 51)  - FrameListener now calls move() from Camera
     - 2009-09-10 (rev 127) - FrameListener now calls InjectAnimationTime()
                              to properly animate AnimatedObjects (by thoronador)
     - 2010-03-04 (rev 177) - LuaEngine now processes Lua Code each frame
     - 2010-03-24 (rev 186) - adjustments to work with new Singleton patterned
                              Camera
     - 2010-05-06 (rev 198) - documentation update (this comment block added)
                            - small improvements
     - 2010-05-21 (rev 206) - adjustments for player movement
     - 2010-11-12 (rev 252) - trigger management added

 ToDo list:
     - ???

 Bugs:
     - No known bugs. If you find one (or more), then tell us please. We'll
       try to fix it as soon as possible.
 --------------------------------------------------------------------------*/

#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H
#include <OgreFrameListener.h>

namespace Dusk
{
    class FrameListener : public Ogre::FrameListener
    {
    public:
        /* constructor */
        FrameListener();
        /* destructor */
        virtual ~FrameListener();

        /* called at the beginning of a frame, i.e. before rendering

           return value:
               True to go on rendering, false to drop out of the rendering loop.
        */
        virtual bool frameStarted(const Ogre::FrameEvent& evt);

        /* called at the end of a frame, i.e. after rendering

           return value:
               True to go on rendering, false to drop out of the rendering loop.
        */
        virtual bool frameEnded(const Ogre::FrameEvent& evt);

        /* function that tells the FrameListener to exit, i.e. the subsequent
           calls of frameStarted() and frameEnded() will return false and thus
           the rendering loop will be stopped.
        */
        void exit();

        /* function to determine whether triggers are checked every frame or not

           parameters:
               active - If set to true, triggers will be processed. If set to
                        false, they won't be processed.
        */
        void setTriggerActivity(const bool active);

        /* returns whether trigger management is currently active */
        bool getTriggerActivity() const;

    protected:
        //local variables
        bool m_Continue;
        bool m_TriggersActive;
    };
}

#endif // FRAMELISTENER_H
