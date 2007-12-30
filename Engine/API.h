#ifndef API_H
#define API_H
#include <Ogre.h>
namespace Dusk
{
    class Application;
    class FrameListener;
    class Console;
    class Dispatcher;
    class InputSystemBinding;
    class InputSystemEditor;
    class Scene;

    // This class contains pointers to every single object
    class API
    {
        public:
            API(Dusk::Application* app);
            virtual ~API();
            void setOgreObjects(Ogre::Root* root,Ogre::Camera* camera,Ogre::RenderWindow* window,Ogre::SceneManager* mgr);



        protected:
        private:
        //Ogre Objects
        Ogre::Root* m_Root;
        Ogre::Camera* m_Camera;
        Ogre::RenderWindow* m_Window;
        Ogre::SceneManager* m_SceneManager;

        //Dusk Objects
        Dusk::Application* m_Application;
        Dusk::FrameListener* m_FrameListener;
        Dusk::Console* m_Console;
        Dusk::Dispatcher* m_Dispatcher;
        Dusk::InputSystemBinding* m_InputSystemBinding;
        Dusk::InputSystemEditor* m_InputSystemEditor;
        Dusk::Scene* m_Scene;
    };
}

#endif // API_H
