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

    class FrameListener : public Ogre::FrameListener
    {
    public:
        FrameListener(Ogre::RenderWindow* win,Ogre::Camera* cam,Ogre::Root* root,bool bufferdInput = true);
        virtual ~FrameListener();
        virtual bool frameStarted(const Ogre::FrameEvent& evt);
        virtual bool frameEnded(const Ogre::FrameEvent& evt);
        bool handleUnbufferdInput();
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

        Ogre::RenderWindow* m_Window;
        Ogre::Camera* m_Camera;
        Ogre::Root* m_Root;

        Dusk::InputSystemEditor* m_InputSystemEditor;
        Dusk::InputSystemBinding* m_InputSystemBinding;

        //Movement factors
        Real m_MoveSpeed;
        Real m_RotateSpeed;
        Real m_StrafeSpeed;

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
