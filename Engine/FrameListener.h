#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H
#include <Ogre.h>
#include <OIS/OIS.h>
//#include "Console.h"


namespace Dusk
{
    //Predeclaration of classes
    class Console;
    class InputSystem;
    class InputSystemBinding;
    class InputSystemEditor;
    class InputEvent;

    class FrameListener : public Ogre::FrameListener, OIS::MouseListener, OIS::KeyListener
    {
    public:
        FrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,Ogre::Root* root,bool bufferdInput = true);
        virtual ~FrameListener();
        virtual bool frameStarted(const Ogre::FrameEvent& evt);
        virtual bool frameEnded(const Ogre::FrameEvent& evt);
        bool handleUnbufferdInput();

        //Mouse events
        virtual bool mouseMoved( const OIS::MouseEvent &arg );
		virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

        //Keyboard events
		virtual bool keyPressed( const OIS::KeyEvent &arg );
		virtual bool keyReleased( const OIS::KeyEvent &arg );

    protected:
    private:
        //local variables
        bool m_UseBufferdInput;
        bool m_Continue;
        bool m_EditorIsVisible; // that's the console


        // OIS input handles
        OIS::InputManager* m_InputManager;
        OIS::Mouse*    m_Mouse;
        OIS::Keyboard* m_Keyboard;

        // Ogre Objects
        Ogre::RenderWindow* m_Window;
        Ogre::Camera* m_Camera;
        Ogre::Root* m_Root;

        // Dusk Objects
        Dusk::InputSystemEditor* m_InputSystemEditor;
        Dusk::InputSystemBinding* m_InputSystemBinding;

        //Movement factors
        Ogre::Real m_MoveSpeed;
        Ogre::Real m_RotateSpeed;
        Ogre::Real m_StrafeSpeed;

        class InputHandler
        {
            public:
            void setInputSystem(Dusk::InputSystem* inputSystem);
            Dusk::InputSystem* getInputSystem();
            protected:
            private:
            Dusk::InputSystem* m_InputSystem;
            Dusk::Console* m_Console;
        }m_InputHandler;
    };
}


#endif // FRAMELISTENER_H
