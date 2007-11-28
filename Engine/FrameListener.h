#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H
#include <Ogre.h>
#include <OIS/OIS.h>
#include "Console.h"
namespace Dusk
{
    class FrameListener : public Ogre::FrameListener
    {
    public:
        FrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,bool bufferdInput = true);
        virtual ~FrameListener();
        virtual bool frameStarted(const Ogre::FrameEvent& evt);
        virtual bool frameEnded(const Ogre::FrameEvent& evt);
        bool handleUnbufferdInput();
    protected:
    private:
        //local variables
        bool m_UseBufferdInput;
        bool m_Continue;

        // OIS input handles
        OIS::InputManager* m_InputManager;
        OIS::Mouse*    m_Mouse;
        OIS::Keyboard* m_Keyboard;

        Ogre::RenderWindow* m_Window;
        Ogre::Camera* m_Camera;

        class InputHandler
        {
        public:
        protected:
        private:
            Dusk::Console* m_Console;
        };
    };
}


#endif // FRAMELISTENER_H
